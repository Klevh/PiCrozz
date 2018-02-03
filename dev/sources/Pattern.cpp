#include "Pattern.hpp"

#include "Debug.hpp"

// Constructors and Destructor
Pattern::Pattern(const std::vector<GLfloat>& triangles,const std::vector<std::string>& uniforms, const std::string& texture)
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
    Errors::glGetError("Pattern::init::glEnableVertexAtrribArray(0)");
    glVertexAttribPointer(0,
			  2,
			  GL_FLOAT,
			  GL_FALSE,
			  4*sizeof(GLfloat),
			  (GLvoid*)0);
    Errors::glGetError("Pattern::init::glVertexAtrribArray(0)");
    
    glEnableVertexAttribArray(1);
    Errors::glGetError("Pattern::init::glEnableVertexAtrribArray(1)");
    glVertexAttribPointer(1,
			  2,
			  GL_FLOAT,
			  GL_FALSE,
			  4*sizeof(GLfloat),
			  (GLvoid*) (2 * sizeof(GLfloat)));
    Errors::glGetError("Pattern::init::glVertexAtrribArray(1)");
  
    glBindVertexArray(0);
    Errors::glGetError("Pattern::init::glBindVertexArray");
    
    for(unsigned i = 0; i < uniform_names_.size(); ++i){
	uniform_id_[i] = glGetUniformLocation(program_.getId(), uniform_names_[i].c_str());
	Errors::glGetError((std::string("Pattern::init::glGetUniformLocation  -  ") + uniform_names_[i]).c_str());
    }

    glGenTextures(1,&texture_);
    LOG_DEBUG("texture : " << texture_);
    
    Errors::glGetError("Pattern::init::glGenTextures");
}

void Pattern::bindVAO() const{
    glBindVertexArray(VAO);
    Errors::glGetError("Pattern::bindVAO::glBindVertexArray");
}

void Pattern::setUniform(unsigned i, GLfloat x, GLfloat y, GLfloat z) const{
    static unsigned b = 0;
    glUniform3f(uniform_id_[i],x,y,z);
    Errors::glGetError("Pattern::setUniform::glUniform3f");
}

void Pattern::draw() const{
    glDrawArrays(GL_TRIANGLES, 0, triangles_.size() / 4);
    Errors::glGetError("Pattern::draw::glDrawArrays");
}

void Pattern::setTexture(int width, int height, void * pixels){
    glBindTexture(GL_TEXTURE_2D, texture_);
    Errors::glGetError("Pattern::setTexture::glBindTexture");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    Errors::glGetError("Pattern::setTexture::glTexParameteri (1)");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    Errors::glGetError("Pattern::setTexture::glTexParameteri (2)");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,GL_BGRA, GL_UNSIGNED_BYTE, pixels);
    Errors::glGetError("Pattern::setTexture::glTexImage2D");
}

void Pattern::updateTexture(int id){
    glBindTexture(GL_TEXTURE_2D, texture_);
    Errors::glGetError("Pattern::updateTexture::glBindTexture");
    glActiveTexture(GL_TEXTURE0);
    Errors::glGetError("Pattern::updateTexture::glActiveTexture");
    glUniform1i(uniform_id_[id], 0);
    Errors::glGetError("Pattern::updateTexture::glUniform1i");
}
