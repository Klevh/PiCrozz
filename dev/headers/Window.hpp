#ifndef _PICROZZ_WINDOW_AUBIN_KLEVH_
#define _PICROZZ_WINDOW_AUBIN_KLEVH_

// STL headers
#include <string>
#include <exception>
#include <vector>

// graphical display headers
extern "C"{
#include <GL/glew.h>
#include <GLFW/glfw3.h>
}

// personal headers
#include "Errors.hpp"
#include "Element.hpp"
#include "Pattern.hpp"

class Window{
    // static attributes
    static bool uniq_;
    ///< allow the window to be instanciated only once
    static bool uniq_init_;
    ///< allow the window to be initialized only once

    // non-static attributes
    GLFWwindow * window_;
    ///< glfw window object
    std::vector< std::vector<Element> > elements_;
    ///< collections of elements
    Pattern pattern_img;
    ///< pattern that support image
    Pattern pattern_no_img;
    ///< pattern that does not support image
    enum STATE_VALUE{MENU, GAME, COUNT} state;
    ///< 'page' to be displayed

public:
// constructors destructors
    /**
     * @brief Window constructor
     */
    Window();

    /**
     * @brief Window destructor
     */
    ~Window();

// public methods
    /**
     * @brief Initialize glfw, glew, open gl and create a window
     * @param title : title of the window
     * @param width : width of the window
     * @param height : height of the window
     */
    void init(std::string title,int width,int height);
    /**
     * @brief Open and run the game until the user ask to stop it
     */
    void run();

// private methodes
private:
    /**
     * @brief Initialize game mode and display
     */
    void game_mode();
    /**
     * @brief Initialize menu mode and display
     */
    void menu_mode();

// public classes
public:
    /**
     * @brief Error thrown when trying to instance more than one window
     */
    class WindowInstancedTwice : public std::exception{
    public:
	/**
	 * @brief getter for the error message
	 * @return The error message as a string
	 */
	std::string what();
    };
    /**
     * @brief Error thrown when trying to initialize a window more than once
     */
    class WindowInitTwice : public std::exception{
    public:
	/**
	 * @brief getter for the error message
	 * @return The error message as a string
	 */
	std::string what();
    };
    /**
     * @brief Error thrown when the window creation failed
     */
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
