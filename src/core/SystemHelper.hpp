

#ifndef SYSTEMHELPER_HPP
#include <SDL2/SDL.h>

#define SYSTEMHELPER_HPP

bool openGLChecker()
{
	if(SDL_GL_LoadLibrary(NULL) != 0){
		return 1;
	}

	return 0;
}

#endif