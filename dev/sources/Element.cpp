#include "Element.hpp"

#include "Debug.hpp"

// Constructors and Destructors
Element::Element(Pattern * pattern, OnClick onclick)
    :onclick_(onclick)
    ,pattern_(pattern)
    ,uniform_values_(pattern->getSize(),{0,0,0})
    ,surface_(nullptr)
{
    for(unsigned i = 0; i < uniform_values_.size(); ++i){
	uniform_values_[i][0] = 0;
	uniform_values_[i][1] = 0;
	uniform_values_[i][2] = 0;
    }
    glGenTextures(1,&texture_);
    Errors::glGetError("Pattern::Constructor::glGenTextures");
}

// getters
const Vec3& Element::getValue(unsigned i) const{
    return uniform_values_[i];
}

GLuint Element::getId() const{
    return (pattern_ ? pattern_->getId() : 0);
}

// setters
void Element::setValue(unsigned i, GLfloat x, GLfloat y, GLfloat z){
    uniform_values_[i][0] = x;
    uniform_values_[i][1] = y;
    uniform_values_[i][2] = z;
}

void Element::setOnClick(OnClick onclick){
    onclick_ = onclick;
}

void Element::setTextureId(int id){
    surface_id_ = id;
}

void Element::setTexture(SDL_Surface * texture){
    if(surface_){
	SDL_FreeSurface(surface_);
    }
    surface_ = texture;

    glBindTexture(GL_TEXTURE_2D, texture_);
    Errors::glGetError("Pattern::setTexture::glBindTexture");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    Errors::glGetError("Pattern::setTexture::glTexParameteri (1)");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    Errors::glGetError("Pattern::setTexture::glTexParameteri (2)");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface_->w, surface_->h, 0,GL_BGRA, GL_UNSIGNED_BYTE, surface_->pixels);
    Errors::glGetError("Pattern::setTexture::glTexImage2D");
}

// other methods
void Element::draw(){
    if(pattern_){
	pattern_->bindVAO();
	for(unsigned i = 0; i < uniform_values_.size(); ++i){
	    if(i != surface_id_){
		pattern_->setUniform(i,uniform_values_[i][0],uniform_values_[i][1],uniform_values_[i][2]);
	    }
	}
	if(surface_){
	    pattern_->updateTexture(surface_id_, texture_);
	}
	pattern_->draw();
    }
}

void Element::click(Window * window,  int states[GLFW_MOUSE_BUTTON_LAST + 1], int action, int mod, GLfloat x, GLfloat y){
    onclick_(window,this,states,action,mod,x,y);
}
