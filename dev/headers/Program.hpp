#ifndef _PROGRAM_PICROZZ_AUBIN_KLEVH_
#define _PROGRAM_PICROZZ_AUBIN_KLEVH_

#include <GL/glew.h>

#include <string>

#include "Errors.hpp"

class Program{
    GLuint id_;
    ///< OpenGL id of the program

public:
// Constructors and Destructor
    /**
     * @brief Program constructor
     * @param vertex_path : path of the vertex shader
     * @param fragment_path : path of the fragment shader
     * @param geometry_path : path of the geometry shader
     */
    Program(const char * vertex_path,const char * fragment_path, const char * geometry_path = NULL);
    /**
     * @brief Program destructor
     */
    ~Program();
};

#endif
