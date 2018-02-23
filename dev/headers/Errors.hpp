#ifndef _ERRORS_PICROZZ_AUBIN_KLEVH_
#define _ERRORS_PICROZZ_AUBIN_KLEVH_

#include <fstream>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <functional>

extern "C"{
#include <GL/glew.h>
#include <GL/gl.h>
}

/**
 * @brief namespace containing all the general error managment
 */
namespace Errors{
    /**
     * @brief testify that there is no openGL errors or throw a GL_ERROR
     * @param func : function to be called if an error occure
     */
    void glGetError(const char * t = "",std::function<void(void)> func = [](){});
    
    /**
     * @brief OpenGL error
     */
    class GL_ERROR : public std::exception{
	std::string msg_;
    public:
	/**
	 * @brief GL_ERROR constructor
	 * @param error_log : error log of openGL
	 */
	GL_ERROR(std::string error_log);
	/**
	 * @brief getter for the error message
	 * @return The error message as a string
	 */
	std::string what();
    };

    /**
     * @brief Error of glew initialization
     */
    class GLEW_INIT_FAILED : public std::exception{
    public:
	/**
	 * @brief getter for the error message
	 * @return The error message as a string
	 */
	std::string what();
    };

    /**
     * @brief Bad parameter
     */
    class ParamError : public std::exception{
	std::string msg_;
	///< Personnalized message for the error
    public:
	/**
	 * @brief Constructor
	 * @param message : personnalized message for the error
	 */
	ParamError(const std::string& message);
	/**
	 * @brief getter for the error message
	 * @return The error message as a string
	 */
	std::string what();
    };

    /**
     * @brief TTF initialisation failed
     */
    class TTF_INIT_FAILED : public std::exception{
    public:
	/**
	 * @brief getter for the error message
	 * @return The error message as a string
	 */
	std::string what();
    };

    /**
     * @brief Font openning failed
     */
    class FontNotOpened : public std::exception{
	std::string path_;
	///< Personnalized message for the error
    public:
	/**
	 * @brief Constructor
	 * @param path : path of the font
	 */
	FontNotOpened(const std::string& message);
	/**
	 * @brief getter for the error message
	 * @return The error message as a string
	 */
	std::string what();
    };

    /**
     * @brief Failed to initialize a surface from a Font
     */
    class FontToSurface : public std::exception{
    public:
	/**
	 * @brief getter for the error message
	 * @return The error message as a string
	 */
	std::string what();
    };
}

#endif
