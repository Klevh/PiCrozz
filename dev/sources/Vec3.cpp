#include "Vec3.hpp"

Vec3::Vec3(GLfloat x, GLfloat y, GLfloat z){
    tab[0] = x;
    tab[1] = y;
    tab[2] = z;
}

GLfloat& Vec3::operator[](unsigned i){
    return tab[i];
}

GLfloat Vec3::operator[](unsigned i) const{
    return tab[i];
}
