#ifndef _ELEMENT_PICROZZ_AUBIN_KLEVH_
#define _ELEMENT_PICROZZ_AUBIN_KLEVH_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Pattern.hpp"

class Vec3{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

class Element{
    Pattern * pattern_;
    ///< pattern to be followed by the element
    std::vector<Vec3> uniform_values_;
    ///< pattern uniform's values
    
public:
// Constructors and Destructors
    /**
     * @brief Element's constructor
     * @param pattern : pattern of the element
     */
    Element(Pattern * pattern);

// getters
    Vec3 getValue(unsigned i) const;

// setters
    void setValue(unsigned i);
};

#endif
