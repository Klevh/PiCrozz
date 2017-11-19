#ifndef _ELEMENT_PICROZZ_AUBIN_KLEVH_
#define _ELEMENT_PICROZZ_AUBIN_KLEVH_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Pattern.hpp"

class Element{
    Pattern * pattern_;
    ///< pattern to be followed by the element
    std::vector<GLfloat[3]> uniform_values_;
    ///< pattern uniform's values
    
public:
// Constructors and Destructors
    /**
     * @brief Element's constructor
     * @param pattern : pattern of the element
     */
    Element(Pattern * pattern);

// getters
    const GLfloat * getValue(unsigned i) const;

// setters
    void setValue(unsigned i, GLfloat x, GLfloat y = 0, GLfloat z = 0);
};

#endif
