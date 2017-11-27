#include "Program.hpp"

#include <fstream>

// Constructors and Destructor
Program::Program()
    :id_(0)
{}

Program::~Program(){
    if(id_)
	glDeleteProgram(id_);
}

// getters
GLuint Program::getId() const{
    return id_;
}

// other methods
static std::string get_string_from_file(const char * path){
    std::ifstream is(path);
    std::string s;

    if(is.is_open()){
	std::getline(is,s,(char)EOF);
	is.close();
    }else{
	s = "Program::Constructor : could not open the file ";
	s += std::string(path);
	throw Errors::ParamError(s);
    }
    
    return s;
}

static GLuint compileShader(GLuint prog,const std::string& text,GLenum type){
    GLuint s = glCreateShader(type);
    const char * txt = text.c_str();
    char infoLog[512] = {0};
    GLint success;
    
    glShaderSource(s, 1, &txt, NULL);
    glCompileShader(s);

    glGetShaderiv(s, GL_COMPILE_STATUS, &success);
    if(success != GL_TRUE){
	glGetShaderInfoLog(s,512,NULL,infoLog);
	throw Errors::GL_ERROR((std::string("Program::compile::compileShader::glCompileShader\n") + std::string(infoLog)).c_str());
    }else
	glAttachShader(prog,s);
    
    Errors::glGetError("Program::compile::compileShader");
    return s;
}

void Program::compile(const char * vertex_path,const char * fragment_path, const char * geometry_path){
    if(vertex_path || fragment_path || geometry_path){
	std::string s;
	GLuint vs, fs, gs;
	char infoLog[512] = {0};
	GLint success;

	id_ = glCreateProgram();
	Errors::glGetError("Program::compile::glCreateProgram");

	if(vertex_path){
	    s = get_string_from_file(vertex_path);
	    vs = compileShader(id_,s,GL_VERTEX_SHADER);
	}
	if(fragment_path){
	    s = get_string_from_file(fragment_path);
	    fs = compileShader(id_,s,GL_FRAGMENT_SHADER);
	}
	if(geometry_path){
	    s = get_string_from_file(geometry_path);
	    gs = compileShader(id_,s,GL_GEOMETRY_SHADER);
	}

	glLinkProgram(id_);
	Errors::glGetError("Program::compile::glLinkProgram");

	glGetProgramiv(id_, GL_LINK_STATUS, &success);
	if(!success) {
	    glGetProgramInfoLog(id_, 512, NULL, infoLog);
	    Errors::glGetError((std::string("Program::compile::glLinkProgram\n\t") + std::string(infoLog)).c_str());
	}
  
	if(vertex_path){
	    glDetachShader(id_,vs);
	    glDeleteShader(vs);
	}
	if(fragment_path){
	    glDetachShader(id_,fs);
	    glDeleteShader(fs);
	}
	if(geometry_path){
	    glDetachShader(id_,gs);
	    glDeleteShader(gs);
	}
	
	Errors::glGetError("Program::compile");
    }else
	throw Errors::ParamError("Program::compile : all parameters are NULL pointers");
}
