#include "Window.hpp"

// static attributes
bool Window::uniq_ = true;

// constructors destructors
Window::Window(std::string title,int width,int height){
    if(uniq_){
	uniq_ = false;
	
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
	    throw GLErrors::GLEW_INIT_FAILED();
	}

	if(GLErrors::glGetError()){
	    glfwTerminate();
	    throw GLErrors::GL_ERROR();
	}
    }else
	throw WindowInstancedTwice();
}
Window::~Window(){
    glfwTerminate();
}

// public classes
// --- WindowInstancedTwice
std::string Window::WindowInstancedTwice::what(){
    return "You tried to instance more than one window";
}
// --- WindowNotCreated
std::string Window::WindowNotCreated::what(){
    return "GLFW3 window could not be created";
}
