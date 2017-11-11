#include "Window.hpp"

/* TODO
   Creating elements
*/

// static attributes
bool Window::uniq_ = true;
bool Window::uniq_init_ = true;

// constructors destructors
Window::Window()
    :window_(nullptr)
    ,elements_()
    ,pattern_no_img({-1.,1.,-1.,-1.,1.,-1.,1.,-1.,1.,1.,-1.,1.},{"plan","offset","size"},"ressources/vertex_no_img.glsl","ressources/fragment_no_img.glsl")
    ,pattern_img({-1.,1.,-1.,-1.,1.,-1.,1.,-1.,1.,1.,-1.,1.},{"plan","offset","size"},"ressources/vertex_no_img.glsl","ressources/fragment_no_img.glsl")
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

	if(!window_){
	    glfwTerminate();
	    throw WindowNotCreated();
	}

	glfwMakeContextCurrent(window_);

	int w,h;
	glfwGetFramebufferSize(window_,&w,&h);
	glViewport(0,0,w,h);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
	    glfwTerminate();
	    throw Errors::GLEW_INIT_FAILED();
	}
	glGetError();
	
	// background color
	glClearColor(0,0,0,0);

	// TODO : creating elements
	
	Errors::glGetError(glfwTerminate);
    }else
	throw WindowInitTwice();
}

void Window::run(){
    
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
