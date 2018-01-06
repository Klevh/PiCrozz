#ifndef _PICROZZ_WINDOW_AUBIN_KLEVH_
#define _PICROZZ_WINDOW_AUBIN_KLEVH_

// STL headers
#include <string>
#include <exception>
#include <vector>
#include <chrono>

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
    Pattern pattern_img_;
    ///< pattern that support image
    Pattern pattern_no_img_;
    ///< pattern that does not support image
    enum STATE_VALUE{MENU, GAME, QUIT, COUNT} state_;
    ///< 'page' to be displayed

public:
// public values available for changing "page"
    static const STATE_VALUE P_MENU;
    ///< state of the window when it is in the menu
    static const STATE_VALUE P_GAME;
    ///< state of the window when it is in the game
    static const STATE_VALUE P_QUIT;
    ///< state of the window when you want it to quit
    
// constructors destructors
    /**
     * @brief Window constructor
     */
    Window();

    //deleted constructors / copy operator
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    /**
     * @brief Window destructor
     */
    ~Window();

// getters
    STATE_VALUE getState() const;

// setters
    STATE_VALUE setState(STATE_VALUE state);

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
    /**
     * @brief Behavior when the window is clicked (same parameters as glfw mouse button callback function)
     * @param win : glfw window on which the event occured
     * @param button : button on which the event occured
     * @param mods : modifier bits
     */
    void click(GLFWwindow * win, int button, int action, int mods);
    /**
     * @brief key event
     * @param key : key used to generate the event
     * @param action : action performed with the key
     */
    void keyEvent(GLFWwindow * window, int key, int action);

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
