#include "Window.hpp"

// static attributes
bool Window::uniq_ = true;
bool Window::uniq_init_ = true;

// constructors destructors
Window::Window()
    :window_(nullptr)
    ,elements_()
    ,pattern_no_img({0.,1.,0.,0.,1.,0.,1.,0.,1.,1.,0.,1.},{"myPlan","myOffset","mySize","myColor"})
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
	glClearColor(1,1,1,0);

	// creating elements
	elements_.push_back(Element(&pattern_no_img));
	elements_.push_back(Element(&pattern_no_img));
	for(unsigned i = 0; i < elements_.size(); ++i){
	    elements_[i].setValue(0,0.5); // set plan
	    elements_[i].setValue(1,0.1 + i*0.3,0.1); // set offset
	    elements_[i].setValue(2,0.2,0.2); // set size
	    elements_[i].setValue(3,0,1.,0); // set color
	}
	
	Errors::glGetError("Window::Window",glfwTerminate);
    }else
	throw WindowInitTwice();
}

void Window::run(){
    GLuint curr_prog;
//    int w,h;
//    glfwGetFramebufferSize(window_,&w,&h);
//    glViewport(0,0,w,h);
    
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
