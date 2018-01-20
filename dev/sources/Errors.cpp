#include "Errors.hpp"

// glGetError
void Errors::glGetError(const char * t,std::function<void(void)> func){
    GLenum error = ::glGetError();
    
    if(error){
	std::ostringstream s;

	if(t)
	    s << "\n\tError message : " << t;
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

// TTF_INIT_FAILED
std::string Errors::TTF_INIT_FAILED::what(){
    return "SDL2_ttf initialization failed";
}

// FontNotOpened
Errors::FontNotOpened::FontNotOpened(const std::string& path)
    :path_(path)
{}
std::string Errors::FontNotOpened::what(){
    return "Font could not be opened\n\tLocated at : " + path_;
}

// FontToSurface
std::string Errors::FontToSurface::what(){
    return "Surface could not be created from font";
}
