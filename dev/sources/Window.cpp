#include "Window.hpp"

#include "Debug.hpp"

#include <sstream>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

// static attributes
const Window::STATE_VALUE Window::P_MENU = MENU;
const Window::STATE_VALUE Window::P_GAME = GAME;
const Window::STATE_VALUE Window::P_QUIT = QUIT;
const Window::STATE_VALUE Window::P_CHOICE = CHOICE;
static const char * MENU_TEXT[2] = {"START", "QUIT"};

bool Window::uniq_ = true;
bool Window::uniq_init_ = true;

// constructors destructors
Window::Window()
    :window_(nullptr)
    ,elements_(COUNT)
    ,pattern_no_img_({-1,1,0,0,-1,-1,0,0,1,-1,0,0,1,-1,0,0,1,1,0,0,-1,1,0,0},{"myPlan","myOffset","myRatio","myColor","myRotation"})
    ,pattern_img_({-1,1,0,0,-1,-1,0,1,1,-1,1,1,1,-1,1,1,1,1,1,0,-1,1,0,0},{"myPlan","myOffset","myRatio","myRotation","sampler"})
    ,state_(MENU)
    ,font_(nullptr)
    ,figures_({nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr})
    ,grid_("ressources/30511.xml")
    ,ihm_grid_()
{
    if(uniq_){
	uniq_ = false;
    }else
	throw WindowInstancedTwice();
}

Window::~Window(){
    if(font_){
	TTF_CloseFont(font_);
    }
    for(SDL_Surface * s : figures_){
	if(s){
	    SDL_FreeSurface(s);
	}
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

	// SDL2_ttf init
	if(TTF_Init()){
	    throw Errors::TTF_INIT_FAILED();
	}

	// Font creation
	font_ = TTF_OpenFont("ressources/arial.ttf",30);
	if(!font_){
	    throw Errors::FontNotOpened("ressources/arial.ttf");
	}

	// Instanciating figures
	std::ostringstream oss;
	
	for(unsigned i = 0; i < 10; ++i){
	    oss << i;
	    figures_[i] = TTF_RenderText_Blended(font_, oss.str().c_str(), {0, 0, 0, 0});
	    oss.str("");
	    if(!figures_[i]){
		throw Errors::FontToSurface();
	    }
	}

	// compiling programs
	pattern_no_img_.init("ressources/vertex_no_img.glsl","ressources/fragment_no_img.glsl");
	pattern_img_.init("ressources/vertex_img.glsl","ressources/fragment_img.glsl");
	
	// background color
	glClearColor(1,1,.5,0);

	// generating all pages
	game_mode();
	menu_mode();
	//choice_mode();

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

	Errors::glGetError("Window::Window",glfwTerminate);
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
	if(elements_[state_].size()){
	    curr_prog = elements_[state_][0].getId();
	    glUseProgram(curr_prog);
	    Errors::glGetError("Window::run::glUseProgram");
	    for(const Element& e : elements_[state_]){
		if(curr_prog != e.getId()){
		    curr_prog = e.getId();
		    glUseProgram(curr_prog);
		    Errors::glGetError("Window::run::glUseProgram");
		}
		e.draw();
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
    unsigned i = 0;

    state_ = GAME;
    // TODO : when fusion with physic part made, transform this to be generic

    if(elements_.size() && grid_.getGrille().size()){
	// emptying the current game placement
	elements_[GAME].clear();

	// initializing the ihm_grid_
	ihm_grid_.clear();
	ihm_grid_.resize(grid_.getGrille().size());
	for(std::vector<GridCase>& v : ihm_grid_){
	    v.resize(grid_.getGrille()[0].size());
	}
	
	// white background
	elements_[GAME].push_back(Element(&pattern_no_img_));
	elements_[GAME][0].setValue(0,0.5); // set plan
	elements_[GAME][0].setValue(1,0.2,0); // set offset
	elements_[GAME][0].setValue(2,0.8,0.8); // set size
	elements_[GAME][0].setValue(3,1,1,1); // set color
	++i;

	for(unsigned j = 0; j < 2 * grid_.getGrille().size() + 2; ++j){
	    elements_[GAME].push_back(Element(&pattern_no_img_));
	}
	
	// vertical lines
	for(unsigned j = i; j < i + grid_.getGrille().size(); ++j){
	    //elements_[GAME].push_back(Element(&pattern_no_img_));
	    elements_[GAME][j].setValue(0,0.5); // set plan
	    elements_[GAME][j].setValue(1,0.198 + (j - i)*0.08 / (grid_.getGrille().size() / 10.),0); // set offset
	    elements_[GAME][j].setValue(2,0.002,0.8); // set size
	    elements_[GAME][j].setValue(3,0,0,0); // set color
	}
	i += 1 + grid_.getGrille().size();
	
	// horizontal lines
	for(unsigned j = i; j < i + grid_.getGrille().size() + 1; ++j){
	    //elements_[GAME].push_back(Element(&pattern_no_img_));
	    elements_[GAME][j].setValue(0,0.5); // set plan
	    elements_[GAME][j].setValue(1,0.198,(j - i)*0.08 / (grid_.getGrille().size() / 10.)); // set offset
	    elements_[GAME][j].setValue(2,0.8,0.002); // set size
	    elements_[GAME][j].setValue(3,0,0,0); // set color
	}
	i += 1 + grid_.getGrille().size();

	// interaction of the white background
	elements_[GAME][0].setOnClick(
	    [&](Window * w, Element * e, int buttons[GLFW_MOUSE_BUTTON_LAST + 1], int action, int mode, GLfloat x_, GLfloat y_){
		size_t i = 1;
		size_t x = x_ * grid_.getGrille().size();
		size_t y = (1 - y_) * grid_.getGrille().size();
		x = std::min((size_t)19,x);
		y = std::min((size_t)19,y);

		LOG_DEBUG(x << " - " << y);

		if(!ihm_grid_[x][y].e[0]){
		    GLfloat square_size = 0.08 / (1.0 * grid_.getGrille().size() / 10);
		    GLfloat block_size = 0.07 / (1.0 * grid_.getGrille().size() / 10);

		    if(buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS){
			// adding a block
			elements_[GAME].push_back(Element(&pattern_no_img_));
			
			ihm_grid_[x][y].id[0] = elements_[GAME].size() - 1;
			ihm_grid_[x][y].e[0] = &(elements_[GAME][ihm_grid_[x][y].id[0]]);
			ihm_grid_[x][y].state = 0;
			
			elements_[GAME][ihm_grid_[x][y].id[0]].setValue(0, 0.5); // set plan
			elements_[GAME][ihm_grid_[x][y].id[0]].setValue(1, 0.204 + x * square_size , 0.805 - (y + 1) * square_size); // set offset
			elements_[GAME][ihm_grid_[x][y].id[0]].setValue(2, block_size, block_size); // set size
			elements_[GAME][ihm_grid_[x][y].id[0]].setValue(3, 0, 0, 0); // set color
		    }else if(buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS){
			// adding a cross
		    }
		}else{
		    if(buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS && !ihm_grid_[x][y].state){
			// removing a block
			for(int i = ihm_grid_[x][y].id[0]; i < elements_[GAME].size() - 1; ++i){
			    elements_[GAME][i] = elements_[GAME][i + 1];
			}

			elements_[GAME].pop_back();

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

			// setting the removed block
			ihm_grid_[x][y].e[0] = nullptr;
		    }else if(buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS && ihm_grid_[x][y].state == 1){
			// removing a cross
		    }
		}
	    });

	// example of blocks
	/*
	{
	    unsigned coords[3][2] = {{0, 3}, {2, 9}, {5,5}};
	    for (unsigned i = 0; i < 3; ++i)
		elements_[GAME].push_back(Element(&pattern_no_img_));
	    for(unsigned j = i; j < i + 3; ++j){
		elements_[GAME][j].setValue(0,0.5); // set plan
		elements_[GAME][j].setValue(1,0.204 + coords[j - i][0] * 0.08,0.805 - coords[j - i][1] * 0.08); // set offset
		elements_[GAME][j].setValue(2,0.07,0.07); // set size
		elements_[GAME][j].setValue(3,0,0,0); // set color
	    }
	    i += 3;
	}

	// example of crosses
	{
	    unsigned coords[3][2] = {{1,2}, {6,5}, {7,2}};
	    for(unsigned i = 0; i < 6; ++i)
		elements_[GAME].push_back(Element(&pattern_no_img_));
	    for(unsigned j = i; j < i + 3; ++j){
		elements_[GAME][j].setValue(0,0.5); // set plan
		elements_[GAME][j].setValue(1,0.204 + coords[j - i][0] * 0.08,0.83 - coords[j - i][1] * 0.08); // set offset
		elements_[GAME][j].setValue(2,0.07,0.02); // set size
		elements_[GAME][j].setValue(3,0,0,0); // set color
		elements_[GAME][j].setValue(4,45); // set rotation
	    }
	    i += 3;
	    for(unsigned j = i; j < i + 3; ++j){
		elements_[GAME][j].setValue(0,0.5); // set plan
		elements_[GAME][j].setValue(1,0.204 + coords[j - i][0] * 0.08,0.83 - coords[j - i][1] * 0.08); // set offset
		elements_[GAME][j].setValue(2,0.07,0.02); // set size
		elements_[GAME][j].setValue(3,0,0,0); // set color
		elements_[GAME][j].setValue(4,-45); // set rotation
	    }
	    i += 3;
	}
	*/
    }
}

void Window::menu_mode(){
    state_ = MENU;
    // buttons
    for(unsigned i = 0; i < 2; ++i){
	// block
	elements_[MENU].push_back(Element(&pattern_no_img_));
	elements_[MENU][i * 2].setValue(0,0.5); // set plan
	elements_[MENU][i * 2].setValue(1,.3,.2 + .2 * i); // set offset
	elements_[MENU][i * 2].setValue(2,.4,.1); // set size
	elements_[MENU][i * 2].setValue(3,.3,.3,.3); // set color

	// text
	elements_[MENU].push_back(Element(&pattern_img_));
	elements_[MENU][i * 2 + 1].setValue(0,0.5); // set plan
	elements_[MENU][i * 2 + 1].setValue(1,.3,.2 + .2 * i); // set offset
	elements_[MENU][i * 2 + 1].setValue(2,.4,.1); // set size
	
	SDL_Surface * s = TTF_RenderText_Blended(font_, MENU_TEXT[i], {0,0,0,0});
	if(!s){
	    throw Errors::FontToSurface();
	}

	elements_[MENU][i * 2 + 1].setTexture(s);
	elements_[MENU][i * 2 + 1].setTextureId(4);
    }

    elements_[MENU][0].setOnClick(
	[&](Window * w, Element * e, int states[GLFW_MOUSE_BUTTON_LAST + 1], int action, int mods, GLfloat, GLfloat){
	    if(states[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS)
		state_ = QUIT;
	});
    
    elements_[MENU][2].setOnClick(
	[&](Window * w, Element * e, int states[GLFW_MOUSE_BUTTON_LAST + 1], int action, int mods, GLfloat, GLfloat){
	    if(states[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS)
		state_ = GAME;
	});
}

void Window::choice_mode(){
    grid_ = Picross("ressources/5090.xml");
    game_mode();
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
	for(unsigned i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
	    states[i] = glfwGetMouseButton(win, i);
	
	// looking for clicked buttons
	double x, y;
	int w, h;
	//getting cursor position and window size
	glfwGetCursorPos(win, &x, &y);
	glfwGetWindowSize(win, &w, &h);
	
	for(unsigned i = 0; i < elements_[state_].size(); ++i){
	    const Vec3 coords = elements_[state_][i].getValue(1);
	    const Vec3 dimension = elements_[state_][i].getValue(2);
	    
	    if(x >= coords[0] * w && x < (coords[0] + dimension[0]) * w
	       && y < (1 - coords[1]) * h && y >= ((1 - coords[1]) - dimension[1]) * h){
		elements_[state_][i].click(this, states, action, mods, (x * 1. / w - coords[0]) / dimension[0], ((h - y) * 1. / h - coords[1]) / dimension[1]);
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
