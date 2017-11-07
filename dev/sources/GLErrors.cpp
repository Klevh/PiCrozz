#include "GLErrors.hpp"

// glGetError
bool GLErrors::glGetError(){
    GLenum error = ::glGetError();
    bool res = false;
    
    if(error){
	std::ostringstream s;

	do{
	    s << "\n" << std::string((char*)glewGetErrorString(error));
	    error = ::glGetError();
	}while(error);
	GL_ERROR::setErrorMsg(s.str());
	
	res = true;
    }

    return res;
}

// GL_ERROR
void GLErrors::GL_ERROR::setErrorMsg(const std::string& msg){
    error_ += "\n" + msg;
}
std::string GLErrors::GL_ERROR::what(){
    std::string err = error_;

    error_ = "";
    
    return err;
}

// GLEW_INIT_FAILED
std::string GLErrors::GLEW_INIT_FAILED::what(){
    return "GLEW initialization failed";
}
