#include <iostream>

#define LOG_OUTPUT std::cout
#include "Debug.hpp"
#include "Window.hpp"

int main(int, char**){
    Window window;
    
    try{
	window.init("PiCrozz - a ZZ game", 800, 800);
	window.run();	
    }catch(Errors::GLEW_INIT_FAILED& gif){
	SHOW_WHERE(Errors::GLEW_INIT_FAILED,gif);
    }catch(Errors::GL_ERROR& ge){
	SHOW_WHERE(Errors::GL_ERROR,ge);
    }catch(Window::WindowInitTwice& wit){
	SHOW_WHERE(Window::WindowInitTwice,wit);
    }catch(Window::WindowInstancedTwice& wit){
	SHOW_WHERE(Window::WindowInstancedTwice,wit);
    }catch(Window::WindowNotCreated& wnc){
	SHOW_WHERE(Window::WindowNotCreated,wnc);
    }catch(Errors::TTF_INIT_FAILED& tif){
	SHOW_WHERE(Errors::TTF_INIT_FAILED, tif);
    }catch(std::exception& e){
	SHOW_WHERE(std::exception,e);
    }catch(...){
	LOG_OUTPUT << "Unknown error" << std::endl;
    }
    
    return 0;
}
