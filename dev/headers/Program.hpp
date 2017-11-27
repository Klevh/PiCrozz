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
     */
    Program();
    /**
     * @brief Program destructor
     */
    ~Program();
    
// getters
    GLuint getId() const;
    
// other methods
    /**
     * @brief compile the program
     * @param vertex_path : path of the vertex shader
     * @param fragment_path : path of the fragment shader
     * @param geometry_path : path of the geometry shader
     */
    void compile(const char * vertex_path,const char * fragment_path, const char * geometry_path = NULL);
};

#endif
