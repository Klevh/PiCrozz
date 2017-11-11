#include "Element.hpp"

// Constructors and Destructors
Element::Element(Pattern * pattern)
    :pattern_(pattern)
    ,uniform_values_(pattern->getSize())
{}

// getters
Vec3 Element::getValue(unsigned i) const{
    return uniform_values_[i];
}

// setters
void Element::setValue(unsigned i, Vec3 v){
    uniform_values_[i] = v;
}
