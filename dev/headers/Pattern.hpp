#ifndef _PATTERN_PICROZZ_AUBIN_KLEVH_
#define _PATTERN_PICROZZ_AUBIN_KLEVH_

#include <GL/glew.h>

#include <string>
#include <vector>

#include "Program.hpp"

class Pattern{
    Program program_;
    ///< program of the pattern
    std::vector<std::string> uniform_names_;
    ///< name of all uniforms
    
public:
// Constructors and Destructor
    /**
     * @brief Pattern constructor
     * @param uniforms : list of uniforms name
     * @param vertex_path : path of the vertex shader
     * @param fragment_path : path of the fragment shader
     * @param geometry_path : path of the geometry shader
     */
    Pattern(const std::vector<GLfloat>& triangles,const std::vector<std::string>& uniforms,const char * vertex_path,const char * fragment_path, const char * geometry_path = NULL);

// getters
    const std::string& getName(unsigned i) const;
    unsigned long getSize() const;
};

#endif
