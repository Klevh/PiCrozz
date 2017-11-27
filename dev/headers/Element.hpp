#ifndef _ELEMENT_PICROZZ_AUBIN_KLEVH_
#define _ELEMENT_PICROZZ_AUBIN_KLEVH_

extern "C"{
#include <GL/glew.h>
#include <GLFW/glfw3.h>
}

#include <vector>

#include "Pattern.hpp"

class Element{
    Pattern * pattern_;
    ///< pattern to be followed by the element
    std::vector< GLfloat[3] > uniform_values_;
    ///< pattern uniform's values
    
public:
// Constructors and Destructors
    /**
     * @brief Element's constructor
     * @param pattern : pattern of the element
     */
    Element(Pattern * pattern);

// getters
    /**
     * @brief Getter for the value of a uniform
     * @param i : index of the value to be gotten
     * @return the GLfloat[3] of the uniform
     */
    const GLfloat * getValue(unsigned i) const;
    /**
     * @brief getter for the Program ID
     * @return ID of the program
     */
    const GLuint getId() const;

// setters
    /**
     * @brief Setter for the value of a uniform
     * @param i : index of the uniform to be modified
     * @param x : first value of the uniform
     * @param y : second value of the uniform
     * @param z : third value of the uniform
     */
    void setValue(unsigned i, GLfloat x, GLfloat y = 0, GLfloat z = 0);

// other methods
    /**
     * @brief Draw the element
     */
    void draw() const;
};

#endif
