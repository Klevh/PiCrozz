#include "Errors.hpp"

// glGetError
void Errors::glGetError(std::function<void(void)> func){
    GLenum error = ::glGetError();
    
    if(error){
	std::ostringstream s;
	do{
	    s << "\n\t" << std::string((char*)gluErrorString(error));
	    error = ::glGetError();
	}while(error);

	func();

	throw GL_ERROR(s.str());
    }
}

// GL_ERROR
Errors::GL_ERROR::GL_ERROR(std::string error_log)
    :msg_(error_log)
{}
std::string Errors::GL_ERROR::what(){
    return msg_;
}

// GLEW_INIT_FAILED
std::string Errors::GLEW_INIT_FAILED::what(){
    return "GLEW initialization failed";
}

// ParamError
Errors::ParamError::ParamError(const std::string& message)
    :msg_(message)
{}

std::string Errors::ParamError::what(){
    return "Bad parameter :\n\t" + msg_;
}
