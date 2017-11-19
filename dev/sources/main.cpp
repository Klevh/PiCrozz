#include <iostream>

#define LOG_OUTPUT std::cout
#include "Window.hpp"

int main(int, char**){
    Window window;
    
    try{
	window.init("PiCrozz - a ZZ game", 500, 500);	
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
    }catch(std::exception& e){
	SHOW_WHERE(std::exception,e);
    }catch(...){
	LOG_OUTPUT << "Unknown error" << std::endl;
    }

    window.run();
    
    return 0;
}
