#include "Pattern.hpp"

// Constructors and Destructor
Pattern::Pattern(const std::vector<GLfloat>& triangles,const std::vector<std::string>& uniforms,const char * vertex_path,const char * fragment_path, const char * geometry_path)
    :program_(vertex_path,fragment_path,geometry_path)
    ,uniform_names_(uniforms)
{
    // TODO : buffer
}

// getters
const std::string& Pattern::getName(unsigned i) const{
    return uniform_names_[i];
}

unsigned long Pattern::getSize() const{
    return uniform_names_.size();
}
