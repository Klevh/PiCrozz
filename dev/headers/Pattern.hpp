#ifndef _PATTERN_PICROZZ_AUBIN_KLEVH_
#define _PATTERN_PICROZZ_AUBIN_KLEVH_

extern "C"{
#include <GL/glew.h>
}
    
#include <string>
#include <vector>

#include "Program.hpp"

class Pattern{
    Program program_;
    ///< program of the pattern
    std::vector<GLint> uniform_id_;
    ///< id of all uniforms
    std::vector<std::string> uniform_names_;
    ///< name of all uniforms
    std::vector<GLfloat> triangles_;
    ///< vertices
    GLuint VAO;
    ///< VAO of the pattern
    GLuint VBO;
    ///< VBO of the pattern
    
public:
// Constructors and Destructor
    /**
     * @brief Pattern constructor
     * @param triangle : list of triangles
     * @param uniforms : list of uniforms name
     */
    Pattern(const std::vector<GLfloat>& triangles,const std::vector<std::string>& uniforms);

// getters
    /**
     * @brief getter for the number of uniforms in this pattern
     * @return the number of uniforms of this pattern
     */
    unsigned long getSize() const;
    /**
     * @brief getter for the program's ID
     * @return the pattern's program ID
     */
    GLuint getId() const;

// other methods
    /**
     * @brief compile the pattern's program
     * @param vertex_path : path of the vertex shader
     * @param fragment_path : path of the fragment shader
     * @param geometry_path : path of the geometry shader
     */
    void init(const char * vertex_path,const char * fragment_path, const char * geometry_path = NULL);
    /**
     * @brief bind the pattern's VAO
     */
    void bindVAO();
    /**
     * @brief load the value of a uniform to the GPU
     * @param i : index of the uniform to be loaded
     * @param x : first value of the uniform
     * @param y : second value of the uniform
     * @param z : third value of the uniform
     */
    void setUniform(unsigned i, GLfloat x, GLfloat y, GLfloat z) const;
    /**
     * @brief draw the pattern
     */
    void draw() const;
};

#endif
