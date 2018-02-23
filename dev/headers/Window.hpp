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
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
}

// personal headers
#include "Errors.hpp"
#include "Element.hpp"
#include "Pattern.hpp"
#include "Picross.hpp"

class Window{
    // static attributes
    static bool uniq_;
    ///< allow the window to be instanciated only once
    static bool uniq_init_;
    ///< allow the window to be initialized only once

    // inner classes
    /**
     * @brief datas about the current grid (to simplify click detection)
     */
    struct GridCase{
	Element * e[2];
	///< elements composing the shape (2 for cross, 1 for block)
	int state;
        ///< 0 for block, 1 for cross
	int id[2];
	///< ids of the element in grid_

	/**
	 * @brief GridCase constructor
	 */
	GridCase():e{nullptr,nullptr},state(0),id{0,0}{}
    };
    // non-static attributes
    GLFWwindow * window_;
    ///< glfw window object
    std::vector< std::vector<Element *> > elements_;
    ///< collections of elements
    std::vector< unsigned > ids_;
    ///< current id in the vector (+1)
    Pattern pattern_img_;
    ///< pattern that support image
    Pattern pattern_no_img_;
    ///< pattern that does not support image
    enum STATE_VALUE{MENU, GAME, QUIT, CHOICE, COUNT} state_;
    ///< 'page' to be displayed
    TTF_Font * font_;
    ///< font of the text
    Picross grid_;
    ///< picross grid of the game
    std::vector< std::vector< GridCase > > ihm_grid_;
    ///< graphical representation of the grid
    size_t cursor;
    ///< cursor in choice mode
    bool finished;
    ///< tells whether the grid is finished or not

public:
// public values available for changing "page"
    static const STATE_VALUE P_MENU;
    ///< state of the window when it is in the menu
    static const STATE_VALUE P_GAME;
    ///< state of the window when it is in the game
    static const STATE_VALUE P_QUIT;
    ///< state of the window when you want it to quit
    static const STATE_VALUE P_CHOICE;
    ///< state of the window when you chose the grid
    
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
    /**
     * @brief Initialize choice mode and display
     */
    void choice_mode();
    /**
     * @brief load a grid from 
     */
    void load_grid(const std::string& path);

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
