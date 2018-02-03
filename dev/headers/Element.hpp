#ifndef _ELEMENT_PICROZZ_AUBIN_KLEVH_
#define _ELEMENT_PICROZZ_AUBIN_KLEVH_

extern "C"{
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
}

#include <vector>
#include <functional>

#include "Pattern.hpp"
#include "Vec3.hpp"

class Window;

class Element{
    /**
     * @brief Element's onclick behavior type, it is a function/lambda/foncter having the same parameters as mouse button callback of GLFW but there is an Element * between the GLFWwindow * and the first int. The two last GLfloat are the location of the click in the element (both from 0 to 1)
     */
    typedef std::function <void(Window*,Element*,int[GLFW_MOUSE_BUTTON_LAST + 1],int,int,GLfloat,GLfloat)> OnClick_window;
    
    OnClick_window onclick_;
    ///< on click element's behavior
    Pattern * pattern_;
    ///< pattern to be followed by the element
    std::vector< Vec3 > uniform_values_;
    ///< pattern uniform's values
    SDL_Surface * surface_;
    ///< texture of the element, if any
    int surface_id_;
    ///< id of the texture in uniform_values_
    bool surface_changed;
    ///< tells when we have to send back the surface to the GPU
    
public:
    typedef OnClick_window OnClick;
// Constructors and Destructors
    /**
     * @brief Element's constructor
     * @param pattern : pattern of the element
     * @param onclick : behavior of the element
     */
    Element(Pattern * pattern, OnClick onclick = [](Window*,Element*,int[GLFW_MOUSE_BUTTON_LAST + 1],int,int,GLfloat,GLfloat){});
    /**
     * @brief Element's destructor
     */
    virtual ~Element(){}

// getters
    /**
     * @brief Getter for the value of a uniform
     * @param i : index of the value to be gotten
     * @return the GLfloat[3] of the uniform
     */
    const Vec3& getValue(unsigned i) const;
    /**
     * @brief getter for the Program ID
     * @return ID of the program
     */
    GLuint getId() const;

// setters
    /**
     * @brief Setter for the value of a uniform
     * @param i : index of the uniform to be modified
     * @param x : first value of the uniform
     * @param y : second value of the uniform
     * @param z : third value of the uniform
     */
    void setValue(unsigned i, GLfloat x, GLfloat y = 0, GLfloat z = 0);
    /**
     * @brief set the element's behavior on click
     * @param onclick : new behavior
     */
    void setOnClick(OnClick onclick);
    /**
     * @brief set the texture ID in the uniform vector
     * @param id : id of the texture
     */
    void setTextureId(int id);
    /**
     * @brief set the element's texture
     * @param texture : new texture of the element
     */
    void setTexture(SDL_Surface * texture);

// other methods
    /**
     * @brief Draw the element
     */
    virtual void draw();
    /**
     * @brief Click on the element
     * @param window : window that received the event
     * @param states : GLFW statue of all buttons
     * @param action : GLFW_PRESS or GLFW_RELEASE
     * @param mods : modifier bit (see http://www.glfw.org/docs/latest/group__mods.html)
     * @param x : x coordinate of the click in the element (from 0 to 1)
     * @param y : y coordinate of the click in the element (from 0 to 1)
     */
    void click(Window * window, int states[GLFW_MOUSE_BUTTON_LAST + 1], int action, int mod, GLfloat x, GLfloat y);
};

#endif
