#ifndef _PICROZZ_WINDOW_AUBIN_KLEVH_
#define _PICROZZ_WINDOW_AUBIN_KLEVH_

// STL headers
#include <string>
#include <exception>

// graphical display headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// personal headers
#include "GLErrors.hpp"

class Window{
    // static attributes
    static bool uniq_;
    ///< allow the window to be instanciated only once

    // non-static attributes
    GLFWwindow * window_;
    ///< glfw window object

public:
// constructors destructors
    /**
     * @brief Window constructor
     * @param title : title of the window
     * @param width : width of the window
     * @param height : height of the window
     */
    Window(std::string title,int width,int height);

    /**
     * @brief Window destructor
     */
    ~Window();

// public classes
    class WindowInstancedTwice : public std::exception{
    public:
	/**
	 * @brief getter for the error message
	 * @return The error message as a string
	 */
	std::string what();
    };
    class WindowNotCreated : public std::exception{
    public:
	/**
	 * @brief getter for the error message
	 * @return The error message as a string
	 */
	std::string what();
    };
};

#endif
