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
    GLuint texture_;
    ///< texture
    
public:
// Constructors and Destructor
    /**
     * @brief Pattern constructor
     * @param triangle : list of triangles
     * @param uniforms : list of uniforms name
     * @param texture : path of the texture to be loaded, if any
     */
    Pattern(const std::vector<GLfloat>& triangles,const std::vector<std::string>& uniforms, const std::string& texture = "");

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
    void bindVAO() const;
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
    /**
     * @brief set the texture of the pattern
     * @param width : width of the texture
     * @param height : height of the texture
     * @param pixels : RGBA structure of the texture
     */
    void setTexture(int width, int height, void * pixels);
    /**
     * @brief update the texture's uniform
     * @param id : id of the texture's uniform
     */
    void updateTexture(int id);
};

#endif
