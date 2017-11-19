#include "Element.hpp"

// Constructors and Destructors
Element::Element(Pattern * pattern)
    :pattern_(pattern)
    ,uniform_values_(pattern->getSize())
{}

// getters
const GLfloat * Element::getValue(unsigned i) const{
    return uniform_values_[i];
}

// setters
void Element::setValue(unsigned i, GLfloat x, GLfloat y, GLfloat z){
    uniform_values_[i][0] = x;
    uniform_values_[i][1] = y;
    uniform_values_[i][2] = z;
}
