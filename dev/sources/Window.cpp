#include "Window.hpp"

#include "Debug.hpp"

#include <sstream>
#include <algorithm>
#include <boost/filesystem.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define ROW_CHOICE 4
#define COL_CHOICE 4

// static none-class functions
static void resize_framebuffer(GLFWwindow * win, int width, int height){
    glViewport(0,0,width,height);
}
static void mouse_button(GLFWwindow * win, int button, int action, int mods){
    if(win){
	Window * window = (Window *)glfwGetWindowUserPointer(win);

	if(window)
	    window->click(win,button,action,mods);
    }
}
static void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
    if(win){
	Window * window = (Window *)glfwGetWindowUserPointer(win);

	if(window)
	    window->keyEvent(win,key,action);
    }
}
static std::vector<std::string> all_file_in_dir(const std::string& path){
    std::vector<std::string> res;
    boost::filesystem::path p(path);
    boost::filesystem::directory_iterator end;
    
    for(boost::filesystem::directory_iterator start(p); start != end; ++start){
	std::string s = start->path().leaf().string();
	size_t id = s.find(".xml");
	
	if(id != std::string::npos && id == s.size() - 4){
	    res.push_back(s.substr(0, s.size() - 4));
	}
    }

    return res;
}

// static attributes
const Window::STATE_VALUE Window::P_MENU = MENU;
const Window::STATE_VALUE Window::P_GAME = GAME;
const Window::STATE_VALUE Window::P_QUIT = QUIT;
const Window::STATE_VALUE Window::P_CHOICE = CHOICE;
static const char * MENU_TEXT[] = {"PICROZZ", "QUIT", "START"};

bool Window::uniq_ = true;
bool Window::uniq_init_ = true;

// constructors destructors
Window::Window()
    :window_(nullptr)
    ,elements_(COUNT)
    ,ids_(COUNT, 0)
    ,pattern_no_img_({-1,1,0,0,-1,-1,0,0,1,-1,0,0,1,-1,0,0,1,1,0,0,-1,1,0,0},{"myPlan","myOffset","myRatio","myColor","myRotation"})
    ,pattern_img_({-1,1,0,0,-1,-1,0,1,1,-1,1,1,1,-1,1,1,1,1,1,0,-1,1,0,0},{"myPlan","myOffset","myRatio","myRotation","sampler_img"})
    ,state_(MENU)
    ,font_(nullptr)
    ,grid_("ressources/5435.xml")
    ,ihm_grid_()
{
    if(uniq_){
	uniq_ = false;
    }else
	throw WindowInstancedTwice();
}

Window::~Window(){
    for(int i = 0; i < COUNT; ++i){
	for(int j = 0; j < elements_[i].size(); ++j){
	    delete elements_[i][j];
	}
    }
    
    if(font_){
	TTF_CloseFont(font_);
    }
    TTF_Quit();
    glfwTerminate();
}

// getters
Window::STATE_VALUE Window::getState() const{
    return state_;
}

// setters
Window::STATE_VALUE Window::setState(Window::STATE_VALUE state){
    state_ = state;
}

// public methods
void Window::init(std::string title,int width,int height){
    if(uniq_init_){
	uniq_init_ = false;

	// GLFW init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #if defined(__APPLE__) || defined(__MACH__)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

	// GL Context init
	window_ = glfwCreateWindow(width,height,title.c_str(),NULL,NULL);
	int w,h;
	glfwGetFramebufferSize(window_,&w,&h);
	glViewport(0,0,w,h);
	if(!window_){
	    glfwTerminate();
	    throw WindowNotCreated();
	}
	glfwMakeContextCurrent(window_);

	// GLEW init
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
	    glfwTerminate();
	    throw Errors::GLEW_INIT_FAILED();
	}
	glGetError();

	// enabling blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// SDL2_ttf init
	if(TTF_Init()){
	    throw Errors::TTF_INIT_FAILED();
	}

	// Font creation
	font_ = TTF_OpenFont("ressources/arial.ttf",100);
	if(!font_){
	    throw Errors::FontNotOpened("ressources/arial.ttf");
	}

	// compiling programs
	pattern_no_img_.init("ressources/vertex_no_img.glsl","ressources/fragment_no_img.glsl");
	pattern_img_.init("ressources/vertex_img.glsl","ressources/fragment_img.glsl");
	
	// background color
	glClearColor(1,1,.5,1);

	// generating all pages
	menu_mode();

	// set user pointer of window_ with this
	glfwSetWindowUserPointer(window_,this);

	// setting event as sticky (remains until catch)
	glfwSetInputMode(window_, GLFW_STICKY_MOUSE_BUTTONS, 1);
	
	// bind events
	// --- size changing buffer
	glfwSetFramebufferSizeCallback(window_,resize_framebuffer);
	// --- mouse button click
	glfwSetMouseButtonCallback(window_,mouse_button);
	// --- key event
	glfwSetKeyCallback(window_,key_callback);

	Errors::glGetError("Window::Window");
    }else
	throw WindowInitTwice();
}

void Window::run(){
    GLuint curr_prog;
    
    while(!glfwWindowShouldClose(window_) && state_ != QUIT){
	// capture d'evenement 
	glfwPollEvents();
    
	// definition de l'affichage 
	glClear(GL_COLOR_BUFFER_BIT); 

        // dessin des elements
	if(ids_[state_] && state_ != QUIT){
	    curr_prog = elements_[state_][0]->getId();
	    glUseProgram(curr_prog);
	    Errors::glGetError("Window::run::glUseProgram");
	    for(Element * e : elements_[state_]){
		if(e && state_ != QUIT){
		    if(curr_prog != e->getId()){
			curr_prog = e->getId();
			glUseProgram(curr_prog);
			Errors::glGetError("Window::run::glUseProgram");
		    }
		    e->draw();
		}
	    }
	    glBindVertexArray(0);
	    Errors::glGetError("Window::run::glBindVertexArray");
	}
    
	// validation de l'affichage
	glfwSwapBuffers(window_);
	Errors::glGetError("Window::glfwSwapBuffers");
    }
}

void Window::game_mode(){
    SDL_Surface * surface;

    if(elements_.size() && grid_.getGrille().size()){
	for(int i = 0; i < ids_[GAME]; ++i){
	    delete elements_[GAME][i];
	}
	
	// calculating number of elements for indications
	int max_row = (*std::max_element(
	    grid_.getIndicationsLignes().begin(),
	    grid_.getIndicationsLignes().end(),
	    [](const std::vector<InfoCase>& v1, const std::vector<InfoCase>& v2){
		return v1.size() < v2.size();
	    })).size();
	int max_column = (*std::max_element(
	    grid_.getIndicationsColonnes().begin(),
	    grid_.getIndicationsColonnes().end(),
	    [](const std::vector<InfoCase>& v1, const std::vector<InfoCase>& v2){
		return v1.size() < v2.size();
	    })).size();
	
	// emptying the current game placement
	elements_[GAME].clear();
	elements_[GAME].resize(grid_.getGrille().size() + 1 // number of horizontal lines
			       + grid_.getGrille()[0].size() + 1 // number of vertical lines
			       + 1 // background
			       + grid_.getGrille().size() * grid_.getGrille()[0].size() * 2
			       + max_column * grid_.getGrille().size()
			       + max_row * grid_.getGrille()[0].size()
	    ); // number of blocks * 2 (for the cross)
	std::generate(elements_[GAME].begin(),elements_[GAME].end(),
		      [](){return nullptr;});
	ids_[GAME] = 0;
	
	// white background
	elements_[GAME][ids_[GAME]] = new Element(&pattern_no_img_);
	elements_[GAME][ids_[GAME]]->setValue(0,0.5); // set plan
	elements_[GAME][ids_[GAME]]->setValue(1,0.2,0); // set offset
	elements_[GAME][ids_[GAME]]->setValue(2,0.8,0.8); // set size
	elements_[GAME][ids_[GAME]]->setValue(3,1,1,1); // set color
	++ids_[GAME];

	// initializing the ihm_grid_
	ihm_grid_.clear();
	ihm_grid_.resize(grid_.getGrille().size());
	for(std::vector<GridCase>& v : ihm_grid_){
	    v.resize(grid_.getGrille()[0].size());
	}

	// rows indications
	double size_row = 0.2 / max_row;
	double size_col = 0.8 / grid_.getGrille().size();
	for(int i = 0; i < grid_.getIndicationsLignes().size(); ++i){
	    double offset = max_row - grid_.getIndicationsLignes()[i].size();
	    for(int j = 0; j < grid_.getIndicationsLignes()[i].size(); j++){
		elements_[GAME][ids_[GAME]] = new Element(&pattern_img_);
		elements_[GAME][ids_[GAME]]->setValue(0,0.5); // set plan
		elements_[GAME][ids_[GAME]]->setValue(
		    1,
		    size_row * (j + offset),
		    0.8 - size_col * (i + 1)); // set offset
		elements_[GAME][ids_[GAME]]->setValue(2,size_row,size_col); // set size

		// ajout du text
		std::ostringstream oss;
		oss << grid_.getIndicationsLignes()[i][j].getType();
		SDL_Surface * s = TTF_RenderText_Blended(font_, oss.str().c_str(), {0,0,0,255});
		if(!s){
		    throw Errors::FontToSurface();
		}

		elements_[GAME][ids_[GAME]]->setTexture(s);
		elements_[GAME][ids_[GAME]]->setTextureId(4);

		++ids_[GAME];
	    }
	}

	// column indications
	size_row = 0.8 / grid_.getGrille()[0].size();
	size_col = 0.2 / max_column;
	for(int i = 0; i < grid_.getIndicationsColonnes().size(); ++i){
	    double offset = max_column - grid_.getIndicationsColonnes()[i].size();
	    for(int j = 0; j < grid_.getIndicationsColonnes()[i].size(); j++){
		elements_[GAME][ids_[GAME]] = new Element(&pattern_img_);
		elements_[GAME][ids_[GAME]]->setValue(0,0.5); // set plan
		elements_[GAME][ids_[GAME]]->setValue(
		    1,
		    0.2 + size_row * i,
		    1 - size_col * (j + offset + 1)); // set offset
		elements_[GAME][ids_[GAME]]->setValue(2,size_row,size_col); // set size

		// ajout du text
		std::ostringstream oss;
		oss << grid_.getIndicationsColonnes()[i][j].getType();
		SDL_Surface * s = TTF_RenderText_Blended(font_, oss.str().c_str(), {0,0,0,255});
		if(!s){
		    throw Errors::FontToSurface();
		}

		elements_[GAME][ids_[GAME]]->setTexture(s);
		elements_[GAME][ids_[GAME]]->setTextureId(4);

		++ids_[GAME];
	    }
	}

	// creating vertical and horizontal lines
	for(unsigned j = 0; j < 2 * grid_.getGrille().size() + 2; ++j){
	    elements_[GAME][j + ids_[GAME]] = new Element(&pattern_no_img_);
	}
	
	// vertical lines
	for(unsigned j = ids_[GAME]; j < ids_[GAME] + grid_.getGrille().size() + 1; ++j){
	    elements_[GAME][j]->setValue(0,0.5); // set plan
	    elements_[GAME][j]->setValue(1,0.198 + (j - ids_[GAME])*0.08 / (grid_.getGrille().size() / 10.),0); // set offset
	    elements_[GAME][j]->setValue(2,0.002,0.802); // set size
	    elements_[GAME][j]->setValue(3,0,0,0); // set color
	}
	ids_[GAME] += 1 + grid_.getGrille().size();
	
	// horizontal lines
	for(unsigned j = ids_[GAME]; j < ids_[GAME] + grid_.getGrille().size() + 1; ++j){
	    elements_[GAME][j]->setValue(0,0.5); // set plan
	    elements_[GAME][j]->setValue(1,0.198,(j - ids_[GAME])*0.08 / (grid_.getGrille().size() / 10.)); // set offset
	    elements_[GAME][j]->setValue(2,0.8,0.002); // set size
	    elements_[GAME][j]->setValue(3,0,0,0); // set color
	}
	ids_[GAME] += 1 + grid_.getGrille().size();

	// interaction of the white background
	elements_[GAME][0]->setOnClick(
	    [&](Window * w, Element * e, int buttons[GLFW_MOUSE_BUTTON_LAST + 1], int action, int mode, GLfloat x_, GLfloat y_){
		const int BLACK = grid_.getColors().getColorFromName("black");
		
		size_t i = 1;
		size_t x = x_ * grid_.getGrille()[0].size();
		size_t y = (1 - y_) * grid_.getGrille().size();
		x = std::min((size_t)(grid_.getGrille()[0].size()),x);
		y = std::min((size_t)(grid_.getGrille().size()),y);

		LOG_DEBUG(x << " - " << y);
		LOG_DEBUG(ihm_grid_[x][y].e[0]);
		
		if(!ihm_grid_[x][y].e[0]){
		    GLfloat square_size = 0.08 / (1.0 * grid_.getGrille().size() / 10);
		    GLfloat block_size = 0.07 / (1.0 * grid_.getGrille().size() / 10);

		    if(buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS && buttons[GLFW_MOUSE_BUTTON_RIGHT] != GLFW_PRESS){
			// adding a block
			elements_[GAME][ids_[GAME]] = new Element(&pattern_no_img_);
			++ids_[GAME];
			
			ihm_grid_[x][y].id[0] = ids_[GAME] - 1;
			ihm_grid_[x][y].e[0] = elements_[GAME][ihm_grid_[x][y].id[0]];
			ihm_grid_[x][y].state = 0;
			
			LOG_DEBUG(ihm_grid_[x][y].id[0] << " " << ids_[GAME] - 1);
			elements_[GAME][ihm_grid_[x][y].id[0]]->setValue(0, 0.5); // set plan
			elements_[GAME][ihm_grid_[x][y].id[0]]->setValue(1,
									 0.204 + 0.0003 * (10 - (int)grid_.getGrille()[0].size()) + x * square_size,
									 0.805 + 0.0001 * (10 - (int)grid_.getGrille().size()) - (y + 1) * square_size); // set offset
			elements_[GAME][ihm_grid_[x][y].id[0]]->setValue(2, block_size, block_size); // set size
			elements_[GAME][ihm_grid_[x][y].id[0]]->setValue(3, 0, 0, 0); // set color

			// setting the grid_
			grid_.setGrilleIJ(x,y,1,BLACK);
		    }else if(buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS && buttons[GLFW_MOUSE_BUTTON_LEFT] != GLFW_PRESS){
			// adding a cross
			elements_[GAME][ids_[GAME]] = new Element(&pattern_no_img_);
			++ids_[GAME];
			elements_[GAME][ids_[GAME]] = new Element(&pattern_no_img_);
			++ids_[GAME];
			
			ihm_grid_[x][y].id[0] = ids_[GAME] - 2;
			ihm_grid_[x][y].e[0] = elements_[GAME][ihm_grid_[x][y].id[0]];
			ihm_grid_[x][y].id[1] = ids_[GAME] - 1;
			ihm_grid_[x][y].e[1] = elements_[GAME][ihm_grid_[x][y].id[1]];
			ihm_grid_[x][y].state = 1;

			for(int i = 0; i < 2; ++i){
			    elements_[GAME][ihm_grid_[x][y].id[i]]->setValue(0,0.5); // set plan
			    elements_[GAME][ihm_grid_[x][y].id[i]]->setValue(1,0.201 + x * square_size,0.678 + grid_.getGrille().size() * 0.005 - y * square_size); // set offset
			    elements_[GAME][ihm_grid_[x][y].id[i]]->setValue(2,block_size,square_size / 8); // set size
			    elements_[GAME][ihm_grid_[x][y].id[i]]->setValue(3,0,0,0); // set color
			    elements_[GAME][ihm_grid_[x][y].id[i]]->setValue(4,-45 + 90 * i); // set rotation
			}
		    }

		    // setting the grid_
		    grid_.setGrilleIJ(x,y,0,BLACK);
		}else{
		    if(buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS && !ihm_grid_[x][y].state){
			LOG_DEBUG("id in v : " << ihm_grid_[x][y].id[0]);
			// removing a block
			for(int i = ihm_grid_[x][y].id[0]; i < ids_[GAME] - 1; ++i){
			    elements_[GAME][i] = elements_[GAME][i + 1];
			}
			LOG_DEBUG("1");

			delete ihm_grid_[x][y].e[0];
			elements_[GAME][ids_[GAME] - 1] = nullptr;
			--ids_[GAME];
			// setting the removed block
			ihm_grid_[x][y].e[0] = nullptr;
			LOG_DEBUG("2");

			// setting new id for other blocks
			for(int i = 0; i < ihm_grid_.size(); ++i){
			    for(int j = 0; j < ihm_grid_[i].size(); ++j){
				for(int k = 0; k < 2; ++k){
				    if(ihm_grid_[i][j].e[k] && ihm_grid_[i][j].id[k] > ihm_grid_[x][y].id[0]){
					--ihm_grid_[i][j].id[k];
				    }
				}
			    }
			}
			LOG_DEBUG("3");

			// setting the grid_
			grid_.setGrilleIJ(x,y,-1,BLACK);
		    }else if(buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS && ihm_grid_[x][y].state == 1){
			// removing a cross
			for(int j = 1; j >= 0; --j){
			    for(int i = ihm_grid_[x][y].id[j]; i < ids_[GAME] - 1; ++i){
				elements_[GAME][i] = elements_[GAME][i + 1];
			    }
			    
			    delete ihm_grid_[x][y].e[j];
			    elements_[GAME][ids_[GAME] - 1] = nullptr;
			    --ids_[GAME];
			    // setting the removed block
			    ihm_grid_[x][y].e[j] = nullptr;
			    
			    // setting new id for other blocks
			    for(int i = 0; i < ihm_grid_.size(); ++i){
				for(int l = 0; l < ihm_grid_[i].size(); ++l){
				    for(int k = 0; k < 2; ++k){
					if(ihm_grid_[i][l].e[k] && ihm_grid_[i][l].id[k] >= ihm_grid_[x][y].id[j]){
					    --ihm_grid_[i][l].id[k];
					}
				    }
				}
			    }
			}

			// setting the grid_
			grid_.setGrilleIJ(x,y,-1,BLACK);
		    }
		}
	    });
    }
}

void Window::menu_mode(){
    state_ = MENU;
    // buttons
    for(unsigned i = 0; i < 2; ++i, ids_[MENU] += 2){
	// block
	elements_[MENU].push_back(new Element(&pattern_no_img_));
	elements_[MENU][i * 2]->setValue(0,0.5); // set plan
	elements_[MENU][i * 2]->setValue(1,.3,.2 + .2 * i); // set offset
	elements_[MENU][i * 2]->setValue(2,.4,.1); // set size
	elements_[MENU][i * 2]->setValue(3,.8,.8,.8); // set color

	// text
	elements_[MENU].push_back(new Element(&pattern_img_));
	elements_[MENU][i * 2 + 1]->setValue(0,0.5); // set plan
	elements_[MENU][i * 2 + 1]->setValue(1,.3 + .05,.2 + .2 * i); // set offset
	elements_[MENU][i * 2 + 1]->setValue(2,.3,.1); // set size
	
	SDL_Surface * s = TTF_RenderText_Blended(font_, MENU_TEXT[i + 1], {0,0,0, 255});
	if(!s){
	    throw Errors::FontToSurface();
	}

	elements_[MENU][i * 2 + 1]->setTexture(s);
	elements_[MENU][i * 2 + 1]->setTextureId(4);
    }

    elements_[MENU][0]->setOnClick(
	[&](Window * w, Element * e, int states[GLFW_MOUSE_BUTTON_LAST + 1], int action, int mods, GLfloat, GLfloat){
	    if(states[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS)
		state_ = QUIT;
	});
    
    elements_[MENU][2]->setOnClick(
	[&](Window * w, Element * e, int states[GLFW_MOUSE_BUTTON_LAST + 1], int action, int mods, GLfloat, GLfloat){
	    if(states[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS){
		choice_mode();
		state_ = CHOICE;
	    }
	});

    elements_[MENU].push_back(new Element(&pattern_img_));
    elements_[MENU][4]->setValue(0, 0.5); // set plan
    elements_[MENU][4]->setValue(1, .2, .6); // set offset
    elements_[MENU][4]->setValue(2, .6, .15); // set size
	
    SDL_Surface * s = TTF_RenderText_Blended(font_, MENU_TEXT[0],{0,0,0, 255});
    if(!s){
	throw Errors::FontToSurface();
    }

    elements_[MENU][4]->setTexture(s);
    elements_[MENU][4]->setTextureId(4);
}

void Window::choice_mode(){
    SDL_Surface * surface;
    std::vector<std::string> files = all_file_in_dir("ressources/");

    for(int i = 0; i < ids_[CHOICE]; ++i){
	delete elements_[CHOICE][i];
	elements_[CHOICE][i] = nullptr;
    }
    
    // initializing attributes
    cursor = 0;
    ids_[CHOICE] = 5;
    elements_[CHOICE].resize(files.size() * 2 + 5);
    std::generate(elements_[CHOICE].begin(), elements_[CHOICE].end(),
		  [](){return nullptr;});

    // creating next and back behavior
    auto next = [&](Window*,Element*,int[GLFW_MOUSE_BUTTON_LAST + 1],int,int,GLfloat,GLfloat){
	if((cursor + 1) * ROW_CHOICE * COL_CHOICE * 2 < ids_[CHOICE]){
	    // changing back button color
	    if(!cursor){ 
		elements_[CHOICE][1]->setValue(3, 0.8, 0.8, 0.8);
	    }

	    // shifting all choice buttons to the left
	    for(int i = 5; i < ids_[CHOICE]; ++i){
		const Vec3& offset = elements_[CHOICE][i]->getValue(1);

		elements_[CHOICE][i]->setValue(1, offset[0] - 1, offset[1]);
	    }

	    ++cursor;
	    
	    // changing next button color
	    if((cursor + 1) * ROW_CHOICE * COL_CHOICE * 2 >= ids_[CHOICE]){ 
		elements_[CHOICE][3]->setValue(3, 0.4, 0.4, 0.4);
	    }
	}
    };
    auto back = [&](Window*,Element*,int[GLFW_MOUSE_BUTTON_LAST + 1],int,int,GLfloat,GLfloat){
	if(cursor){
	    // changing next button color
	    if((cursor + 1) * ROW_CHOICE * COL_CHOICE * 2 >= ids_[CHOICE]){ 
		elements_[CHOICE][3]->setValue(3, 0.8, 0.8, 0.8);
	    }

	    // shifting all choice buttons to the right
	    for(int i = 5; i < ids_[CHOICE]; ++i){
		const Vec3& offset = elements_[CHOICE][i]->getValue(1);

		elements_[CHOICE][i]->setValue(1, offset[0] + 1, offset[1]);
	    }

	    --cursor;
	    
	    // changing next button color
	    if(!cursor){ 
		elements_[CHOICE][1]->setValue(3, 0.4, 0.4, 0.4);
	    }
	}
    };

    // creating choice button behavior
    auto button = [&, files](Window*,Element * e,int[GLFW_MOUSE_BUTTON_LAST + 1],int,int,GLfloat,GLfloat){
	state_ = GAME;

	const Vec3& offset = e->getValue(1);
	
	// open the good file
	double off_col = (0.8 - offset[1]) / (0.7 / ROW_CHOICE) - 0.1;
	double off_row = offset[0] / (1.0 / COL_CHOICE) - 0.1;
	LOG_DEBUG(files[cursor * ROW_CHOICE * COL_CHOICE
			+ ROW_CHOICE * off_col + off_row]);
        grid_ = Picross(files[cursor * ROW_CHOICE * COL_CHOICE
			      + ROW_CHOICE * off_col + off_row]);
	    
	game_mode();
    };
    
    // creating next/back button
    char btn_txt[][5] = {"BACK","NEXT"};
    for(int i = 0; i < 2; ++i){
	// the block of the button
	elements_[CHOICE][i*2 + 1] = new Element(&pattern_no_img_);
	elements_[CHOICE][i*2 + 1]->setValue(0, 0.5); // set plan
	elements_[CHOICE][i*2 + 1]->setValue(1,
					     0.01 + 0.68 * i,
					     0.01); // set offset
	elements_[CHOICE][i*2 + 1]->setValue(2, 0.3, 0.08); // set size
	elements_[CHOICE][i*2 + 1]->setValue(3, 0.4 * (i + 1), 0.4 * (i + 1), 0.4 * (i + 1)); // set color
	
	// the text of the button
	elements_[CHOICE][i*2 + 2] = new Element(&pattern_img_);
	elements_[CHOICE][i*2 + 2]->setValue(0, 0.5); // set plan
	elements_[CHOICE][i*2 + 2]->setValue(1,
					     0.015 + 0.68 * i,
					     0.015); // set offset
	elements_[CHOICE][i*2 + 2]->setValue(2, 0.29, 0.07); // set size

	surface = TTF_RenderText_Blended(font_, btn_txt[i], {0,0,0,255});
	if(!surface){
	    throw Errors::FontToSurface();
	}
	elements_[CHOICE][i*2 + 2]->setTexture(surface);
	elements_[CHOICE][i*2 + 2]->setTextureId(4);
    }
    if(files.size() <= ROW_CHOICE * COL_CHOICE){
	elements_[CHOICE][3]->setValue(3, 0.4, 0.4, 0.4);
    }
    elements_[CHOICE][1]->setOnClick(back);
    elements_[CHOICE][1]->setOnClick(next);

    // adding title
    elements_[CHOICE][0] = new Element(&pattern_img_);
    elements_[CHOICE][0]->setValue(0, 0.5); // set plan
    elements_[CHOICE][0]->setValue(1, 0.2, 0.805); // set offset
    elements_[CHOICE][0]->setValue(2, 0.6, 0.19); // set size
    surface = TTF_RenderText_Blended(font_, "Choose your grid", {0,0,0,255});
    if(!surface){
	throw Errors::FontToSurface();
    }
    elements_[CHOICE][0]->setTexture(surface);
    elements_[CHOICE][0]->setTextureId(4);

    // creating each "pages"
    double size_col = 1.0 / COL_CHOICE;
    double size_row = 0.7 / ROW_CHOICE;
    while((ids_[CHOICE] - 5) / 2 < files.size()){
	for(int i = 0; i < ROW_CHOICE && (ids_[CHOICE] - 5) / 2 < files.size(); ++i){
	    for(int j = 0; j < COL_CHOICE && (ids_[CHOICE] - 5) / 2 < files.size(); ++j){
		// adding block
		elements_[CHOICE][ids_[CHOICE]] = new Element(&pattern_no_img_);
		elements_[CHOICE][ids_[CHOICE]]->setValue(0, 0.5); // plan
		elements_[CHOICE][ids_[CHOICE]]->setValue(1,
							  size_col * (j + 0.1) + cursor,
							  0.8 - size_row * (i + 0.1 + 1)); // offset
		elements_[CHOICE][ids_[CHOICE]]->setValue(2,
							  size_col * 0.8,
							  size_row * 0.8); // size
		elements_[CHOICE][ids_[CHOICE]]->setValue(3, 0.8, 0.8, 0.8); // color
		elements_[CHOICE][ids_[CHOICE]]->setOnClick(button);
		++ids_[CHOICE];
		
		// adding text
		elements_[CHOICE][ids_[CHOICE]] = new Element(&pattern_img_);
		elements_[CHOICE][ids_[CHOICE]]->setValue(0, 0.5); // plan
		elements_[CHOICE][ids_[CHOICE]]->setValue(1,
							  size_col * (j + 0.15),
							  0.8 - size_row * (i + 0.15 + 1)); // offset
		elements_[CHOICE][ids_[CHOICE]]->setValue(2,
							  size_col * 0.7,
							  size_row * 0.7); // size

		LOG_DEBUG(files[(ids_[CHOICE] - 5) / 2]);
		surface = TTF_RenderText_Blended(font_, files[(ids_[CHOICE] - 5) / 2].c_str(), {0, 0, 0, 255});
		if(!surface){
		    throw Errors::FontToSurface();
		}
		elements_[CHOICE][ids_[CHOICE]]->setTexture(surface);
		elements_[CHOICE][ids_[CHOICE]]->setTextureId(4);
		ids_[CHOICE]++;
	    }
	}
	++cursor;
    }

    LOG_DEBUG("id : " << ids_[CHOICE]);

    cursor = 0;
}

void Window::load_grid(const std::string& path){
    grid_ = Picross(path);

    elements_[GAME].clear();
}

void Window::click(GLFWwindow * win, int button, int action, int mods){
    static unsigned last_click = 0;
    int states[GLFW_MOUSE_BUTTON_LAST + 1];

    unsigned clicked = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
	
    if(clicked >= last_click + 200){
	// reading
	for(unsigned i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i){
	    states[i] = 0;
	}
	states[button] = action;
	
	// looking for clicked buttons
	double x, y;
	int w, h;
	//getting cursor position and window size
	glfwGetCursorPos(win, &x, &y);
	glfwGetWindowSize(win, &w, &h);
	
	for(unsigned i = 0; i < ids_[state_]; ++i){
	    const Vec3 coords = elements_[state_][i]->getValue(1);
	    const Vec3 dimension = elements_[state_][i]->getValue(2);
	    
	    if(elements_[state_][i]
	       && x >= coords[0] * w && x < (coords[0] + dimension[0]) * w
	       && y < (1 - coords[1]) * h && y >= ((1 - coords[1]) - dimension[1]) * h){
		elements_[state_][i]->click(this, states, action, mods, (x * 1. / w - coords[0]) / dimension[0], ((h - y) * 1. / h - coords[1]) / dimension[1]);
	    }
	}
	last_click = clicked;
    }
}

void Window::keyEvent(GLFWwindow * window, int key, int action){
    static unsigned last_click_escape = 0;
    
    if(key == GLFW_KEY_ESCAPE){
	unsigned click_escape = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
	
	if(click_escape >= last_click_escape + 200){
	    switch(state_){
	    case MENU:
		state_ = QUIT;
		break;
	    case GAME:
	    case CHOICE:
		state_ = MENU;
		break;
	    }
	    last_click_escape = click_escape;
	}
    }
}

// public classes
// --- WindowInstancedTwice
std::string Window::WindowInstancedTwice::what(){
    return "You tried to instance more than one window";
}
// --- WindowInitTwice
std::string Window::WindowInitTwice::what(){
    return "You tried to initialize a window more than once";
}
// --- WindowNotCreated
std::string Window::WindowNotCreated::what(){
    return "GLFW3 window could not be created";
}
