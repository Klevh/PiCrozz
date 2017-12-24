#include "Window.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// static none-class functions
static void resize_framebuffer(GLFWwindow * win, int width, int height){
    glViewport(0,0,width,height);
}

// static attributes
bool Window::uniq_ = true;
bool Window::uniq_init_ = true;

// constructors destructors
Window::Window()
    :window_(nullptr)
    ,elements_()
    ,pattern_no_img({0.,1.,0.,0.,1.,0.,1.,0.,1.,1.,0.,1.},{"myPlan","myOffset","myRatio","myColor","myRotation"})
    ,pattern_img({},{})
{
    if(uniq_)
	uniq_ = false;
    else
	throw WindowInstancedTwice();
}

Window::~Window(){
    glfwTerminate();
}

// public methods
void Window::init(std::string title,int width,int height){
    if(uniq_init_){
	uniq_init_ = false;
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #if defined(__APPLE__) || defined(__MACH__)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

	window_ = glfwCreateWindow(width,height,title.c_str(),NULL,NULL);
	int w,h;
	glfwGetFramebufferSize(window_,&w,&h);
	glViewport(0,0,w,h);
	if(!window_){
	    glfwTerminate();
	    throw WindowNotCreated();
	}
	glfwMakeContextCurrent(window_);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
	    glfwTerminate();
	    throw Errors::GLEW_INIT_FAILED();
	}
	glGetError();

	// compiling programs
	pattern_no_img.init("ressources/vertex_no_img.glsl","ressources/fragment_no_img.glsl");
	//pattern_img.init("ressources/vertex_img.glsl","ressources/fragment_img.glsl");
	
	// background color
	glClearColor(1,1,.5,0);

	// size changing buffer
	glfwSetFramebufferSizeCallback(window_,
				       resize_framebuffer);
	
	Errors::glGetError("Window::Window",glfwTerminate);
    }else
	throw WindowInitTwice();
}

void Window::run(){
    GLuint curr_prog;
    
    menu_mode();
    
    while(!glfwWindowShouldClose(window_)){
	// capture d'evenement 
	glfwPollEvents();
    
	// definition de l'affichage 
	glClear(GL_COLOR_BUFFER_BIT); 

        // TODO : dessin des elements
	if(elements_.size()){
	    curr_prog = elements_[0].getId();
	    glUseProgram(curr_prog);
	    Errors::glGetError("Window::run::glUseProgram");
	    for(const Element& e : elements_){
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

void Window::menu_mode(){
    unsigned i = 0;
    
    // grid 10 x 10
    elements_.push_back(Element(&pattern_no_img));
    elements_[0].setValue(0,0.5); // set plan
    elements_[0].setValue(1,0.2,0); // set offset
    elements_[0].setValue(2,0.8,0.8); // set size
    elements_[0].setValue(3,1,1,1); // set color
    ++i;
    for(unsigned i = 0; i < 22; ++i)
	elements_.push_back(Element(&pattern_no_img));
    for(unsigned j = i; j < i + 11; ++j){
	elements_[j].setValue(0,0.5); // set plan
	elements_[j].setValue(1,0.198 + (j - i)*0.08,0.002); // set offset
	elements_[j].setValue(2,0.002,0.8); // set size
	elements_[j].setValue(3,0,0,0); // set color
    }
    i += 11;
    for(unsigned j = i; j < i + 11; ++j){
	elements_[j].setValue(0,0.5); // set plan
	elements_[j].setValue(1,0.198,0 + (j - i)*0.08); // set offset
	elements_[j].setValue(2,0.8,0.002); // set size
	elements_[j].setValue(3,0,0,0); // set color
    }
    i += 11;

    // example of blocks
    {
	unsigned coords[3][2] = {{0, 3}, {2, 9}, {5,5}};
	for (unsigned i = 0; i < 3; ++i)
	    elements_.push_back(Element(&pattern_no_img));
	for(unsigned j = i; j < i + 3; ++j){
	    elements_[j].setValue(0,0.5); // set plan
	    elements_[j].setValue(1,0.205 + coords[j - i][0] * 0.08,1 - (0.205 + coords[j - i][1] * 0.08) + 0.01); // set offset
	    elements_[j].setValue(2,0.07,0.07); // set size
	    elements_[j].setValue(3,0,0,0); // set color
	}
	i += 3;
    }

    // example of crosses
    {
	unsigned coords[3][2] = {{1,2}, {6,5}, {7,2}};
	for(unsigned i = 0; i < 6; ++i)
	    elements_.push_back(Element(&pattern_no_img));
	for(unsigned j = i; j < i + 3; ++j){
	    elements_[j].setValue(0,0.5); // set plan
	    elements_[j].setValue(1,0.205 + coords[j - i][0] * 0.08,1 - (0.205 + coords[j - i][1] * 0.08) + 0.01 + 0.025); // set offset
	    elements_[j].setValue(2,0.07,0.02); // set size
	    elements_[j].setValue(3,0,0,0); // set color
	    elements_[j].setValue(4,0); // set rotation
	}
	i += 3;
	for(unsigned j = i; j < i + 3; ++j){
	    elements_[j].setValue(0,0.5); // set plan
	    elements_[j].setValue(1,0.205 + coords[j - i][0] * 0.08, 1 - (0.205 + coords[j - i][1] * 0.08) + 0.01 + 0.025); // set offset
	    elements_[j].setValue(2,0.07,0.02); // set size
	    elements_[j].setValue(3,0,0,0); // set color
	    elements_[j].setValue(4,0); // set rotation
	}
	i += 3;
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
