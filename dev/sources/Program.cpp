#include "Program.hpp"

#include <fstream>

static std::string get_string_from_file(const char * path){
    std::ifstream is(path);
    std::string s;

    if(is.is_open()){
	std::getline(is,s,EOF);
	is.close();
    }else{
	s = "Program::Constructor : could not open the file ";
	s += std::string(path);
	throw Errors::ParamError(s);
    }
    
    return s;
}

static GLuint compileShader(GLuint prog,std::string text,GLenum type){
    GLuint s = glCreateShader(type);

    glShaderSource(s, &(s.c_str()), NULL);
    glCompileShader(s);
    glAttachShader(prog,s);

    return s;
}

Program::Program(const char * vertex_path,const char * fragment_path, const char * geometry_path = NULL){
    if(vertex_path || fragment_path || geometry_path){
	std::string s;
	GLuint vs, fs, gs;

	if(vertex_path){
	    s = get_string_from_file(vertex_path);
	    vs = compileShader(id_,s,GL_VERTEX_SHADER);
	}
	if(fragment_path){
	    s = get_string_from_file(vertex_path);
	    fs = compileShader(id_,s,GL_FRAGMENT_SHADER);
	}
	if(geometry_path){
	    s = get_string_from_file(vertex_path);
	    gs = compileShader(id_,s,GL_GEOMETRY_SHADER);
	}

	glLinkProgram(id_);

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
	
	Errors::glGetError(NULL);
    }else
	throw Errors::ParamError("Program::Constructor : all parameters are NULL");
}

Program::~Program(){
    glDeleteProgram(id_);
}
