#include <SDL3/SDL.h>
#include <iostream>
#include <glad/glad.h>
#include <cstring>
#include "./core/SystemHelper.hpp"
#include "./core/swf/parser.hpp"

int main(int argc, char *argv[])
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		std::cerr << "SDL_Init Error Nya!: " << SDL_GetError() << std::endl;
		return 1;
	}

	bool openGL = true;

	if(argc >= 1){
		
		if(strcmp(argv[1], "--software") == 0 || strcmp(argv[1], "-S") == 0){
			std::cout << "Selected Acceleration: Software" << std::endl;
			openGL = false;
		} else if(strcmp(argv[1], "--hardware") == 0 || strcmp(argv[1], "-H") == 0) {
			std::cout << "Selected Acceleration: Hardware" << std::endl;
		}
	}
	// OpenGL version setting
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// OpenGL Context Profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// OpenGL option settings
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Setting Stencil Size
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// SDL Window Creation
	SDL_Window *wind = SDL_CreateWindow(
		"Test Window (OpenGL)",
		800, 600,
		SDL_WINDOW_OPENGL);

	if (!wind)
	{
		std::cerr << "Window Error Nya!: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_GLContext glCtx = nullptr;
	SDL_Renderer *renderer = nullptr;
	bool gladStatus = false;

	if(openGL)
	{
		glCtx = SDL_GL_CreateContext(wind);
	}
	
	if(glCtx && openGL){
		gladStatus = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	}
	
	if (!glCtx || !gladStatus)
	{

		if (!glCtx && openGL)
		{
			std::cerr << "OpenGL failed: " << SDL_GetError() << std::endl;
		}
		else if (!gladStatus && openGL)
		{
			std::cerr << "GLAD failed...\n";
		} else if (!gladStatus && !openGL || !glCtx && !openGL){
			std::cout << "Using Sofware Renderer..." << std::endl;
		} else {
			std::cerr << "Falling Back to Software Renderer...\n";
		}

		openGL = false;

		if (glCtx)
		{
			SDL_GL_DestroyContext(glCtx);
			glCtx = nullptr;
		}

		SDL_DestroyWindow(wind);

		wind = SDL_CreateWindow(
			"Test Window (Software Rendering)",
			800, 600, 0);

		if (!wind)
		{
			std::cerr << "Fallback window failed: " << SDL_GetError() << std::endl;
			SDL_Quit();
			return 1;
		}

		renderer = SDL_CreateRenderer(
			wind, "software");

		if (!renderer)
		{
			std::cerr << "Renderer Error: " << SDL_GetError() << std::endl;
			SDL_Quit();
			return 1;
		}
	}

	std::cout << "Current Acceleration: " << (openGL ? "Hardware (OpenGL)" : "Software (SDL)") << std::endl;

	if (openGL)
	{
		glViewport(0, 0, 800, 600);
		glEnable(GL_DEPTH_TEST);
		glClearColor(128.0f/255.0f, 200.0f/255.0f, 128.0f/255.0f, 1.0f);
		SDL_GL_SetSwapInterval(1);
		const GLubyte* ver = glGetString(GL_VERSION);
		std::cout << "GL Version: " << (ver ? (const char*)ver : "Unknown") << std::endl;
	}

	bool running = true;
	SDL_Event evt;
	SWFFile swf;

	std::string testFile = "/home/vikkialets/Documents/Projects/KittySWF/assets/test.swf";
	if(swf.loadFile(testFile))
	{
		std::cout << "--- SWF FILE INFO ---\n";
		std::cout << "Version: SWF " << (int)swf.Header.version << std::endl;
		std::cout << "File Length: " << swf.Header.FileLenght << " bytes\n";
		std::cout << "Dimensions: " << swf.Header.FrameSize.getHeight() << "x" << swf.Header.FrameSize.getWidth() << "px\n";
		std::cout << "FPS: " << swf.Header.getFPS() << std::endl;
		std::cout << "Frames: " << swf.Header.FrameCount << std::endl;
	} else {
		std::cerr << "Failed to load: " << testFile << std::endl;
	}
	

	while (running)
	{
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_EVENT_QUIT)
			{
				running = false;
			}
		}

		if (openGL)
		{
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			SDL_GL_SwapWindow(wind);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 128, 200, 128, 255);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_FRect r{100, 100, 200, 150};
			SDL_RenderFillRect(renderer, &r);

			SDL_RenderPresent(renderer);
		}
	}

	if (openGL)
	{
		SDL_GL_DestroyContext(glCtx);
	}
	else
	{
		SDL_DestroyRenderer(renderer);
	}

	SDL_DestroyWindow(wind);
	SDL_Quit();
	std::cout << "Exiting, Bye!" << std::endl;
	return 0;
}