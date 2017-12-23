#ifndef VEC3_PICROZZ_AUBIN_KLEVH_H
#define VEC3_PICROZZ_AUBIN_KLEVH_H

extern "C"{
#include <GL/glew.h>
}

class Vec3{
    GLfloat tab[3];

public:
    Vec3(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);

    GLfloat& operator[](unsigned i);
    GLfloat operator[](unsigned i) const;
};

#endif
