#include "Element.hpp"

// Constructors and Destructors
Element::Element(Pattern * pattern)
    :pattern_(pattern)
    ,uniform_values_(pattern->getSize(),{0,0,0})
{
    for(unsigned i = 0; i < uniform_values_.size(); ++i){
	uniform_values_[i][0] = 0;
	uniform_values_[i][1] = 0;
	uniform_values_[i][2] = 0;
    }
}

// getters
const Vec3 Element::getValue(unsigned i) const{
    return uniform_values_[i];
}

const GLuint Element::getId() const{
    return (pattern_ ? pattern_->getId() : 0);
}

// setters
void Element::setValue(unsigned i, GLfloat x, GLfloat y, GLfloat z){
    uniform_values_[i][0] = x;
    uniform_values_[i][1] = y;
    uniform_values_[i][2] = z;
}

// other methods
void Element::draw() const{
    if(pattern_){
	pattern_->bindVAO();
	for(unsigned i = 0; i < uniform_values_.size(); ++i)
	    pattern_->setUniform(i,uniform_values_[i][0],uniform_values_[i][1],uniform_values_[i][2]);
	pattern_->draw();
    }
}
