#include "Pattern.hpp"

// Constructors and Destructor
Pattern::Pattern(const std::vector<GLfloat>& triangles,const std::vector<std::string>& uniforms)
    :program_()
    ,uniform_id_(uniforms.size())
    ,uniform_names_(uniforms)
    ,triangles_(triangles)
{}

// getters
unsigned long Pattern::getSize() const{
    return uniform_names_.size();
}
GLuint Pattern::getId() const{
    return program_.getId();
}

// other methods
void Pattern::init(const char * vertex_path,const char * fragment_path, const char * geometry_path){    
    program_.compile(vertex_path, fragment_path, geometry_path);
    
    glGenVertexArrays(1,&VAO);
    Errors::glGetError("Pmaattern::init::glGenVertexArrays");
    glBindVertexArray(VAO);
    Errors::glGetError("Pattern::init::glBindVertexArray");

    glGenBuffers(1,&VBO);
    Errors::glGetError("Pattern::init::glGenBuffers");
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    Errors::glGetError("Pattern::init::glBindBuffer");
    glBufferData(GL_ARRAY_BUFFER, triangles_.size() * sizeof(GLfloat), triangles_.data(), GL_STATIC_DRAW);
    Errors::glGetError("Pattern::init::glBufferData");
    
    glEnableVertexAttribArray(0);
    Errors::glGetError("Pattern::init::glEnableVertexAtrribArray");
    glVertexAttribPointer(0,
			  2,
			  GL_FLOAT,
			  GL_FALSE,
			  2*sizeof(GLfloat),
			  (GLvoid*)0);
    Errors::glGetError("Pattern::init::glVertexAtrribArray");
  
    glBindVertexArray(0);
    Errors::glGetError("Pattern::init::glBindVertexArray");
    
    for(unsigned i = 0; i < uniform_names_.size(); ++i){
	uniform_id_[i] = glGetUniformLocation(program_.getId(), uniform_names_[i].c_str());
	Errors::glGetError((std::string("Pattern::init::glGetUniformLocation  -  ") + uniform_names_[i]).c_str());
    }
}

void Pattern::bindVAO(){
    glBindVertexArray(VAO);
    Errors::glGetError("Pattern::bindVAO::glBindVertexArray");
}

void Pattern::setUniform(unsigned i, GLfloat x, GLfloat y, GLfloat z) const{
    glUniform3f(uniform_id_[i],x,y,z);
    Errors::glGetError("Pattern::setUniform::glUniform3f");
}

void Pattern::draw() const{
    glDrawArrays(GL_TRIANGLES,0,triangles_.size());
    Errors::glGetError("Pattern::draw::glDrawArrays");
}
