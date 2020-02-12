#pragma once

// C++ STL
#include <iostream>
#include <stdexcept>
#include <vector>

// GL
#include <glew.h>
#include <glfw3.h>

#include "DGL_FundamentalTypes.hpp"
#include "DGL_MiscTypes.hpp"
#include "DGL_Enum.hpp"
#include "DGL_Shader.hpp"
#include "DGL_Buffers.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace GL
{
	// Aliases

	// C++ STL

	using std::cout;
	using std::endl;


	// GLFW

	using Monitor       = GLFWmonitor               ;
	using TimeValInt    = uint64_t                  ;
	using TimeValDec    = double                    ;
	using Window        = GLFWwindow                ;
	using WindowRefList = std::vector< ptr<Window> >;



	// Object Instances

	WindowRefList Windows;



	// Error Stuff

	sfn ErrorRuntime(Ref(std::runtime_error) _error)
	{
		cout << "Runtime error occurred: " << _error.what() << endl;

		return;
	}



	// Constants

	sfn constexpr NotShared   () -> ptr<Window > { return NULL; }
	sfn constexpr WindowedMode() -> ptr<Monitor> { return NULL; }



	// Functionality

	sfn SwapBuffers(const ptr<Window> _window) -> void
	{
		glfwSwapBuffers(_window);

		return;
	}

	sfn CanClose(const ptr<Window> _theWindow)
	{
		return glfwWindowShouldClose(_theWindow);
	}

	sfn CreateWindow
	(
		int                   _width                     , 
		int                   _height                    , 
		RawString<const char> _title                     , 
		ptr      <Monitor   > _monitorToFullscreen       , 
		ptr      <Window    > _windowToShareResourcesWith
	) 
		-> ptr<Window>
	{
		Windows.push_back(glfwCreateWindow(_width, _height, _title, _monitorToFullscreen, _windowToShareResourcesWith));

		try
		{
			if (Windows.back() == NULL)
			{
				throw std::runtime_error("Failed to create a window");
			}
		}
		catch (std::runtime_error _error)
		{
			ErrorRuntime(_error);

			Exit(ExitCode::Failed);
		}

		return Windows.back();
	}

	sfn DestoryWindow(const ptr<Window> _window)
	{
		using ElementType = decltype(Windows.begin());

		for (ElementType element = Windows.begin(); element != Windows.end(); element++)
		{
			if (*element == _window)
			{
				glfwDestroyWindow(_window);

				Windows.erase(element);
			}
		}

		return;
	}

	sfn GetTime() -> TimeValDec
	{
		return glfwGetTime();
	}

	sfn GetRawTime() -> TimeValInt
	{
		return glfwGetTimerValue();
	}

	sfn InitalizeGLFW()
	{
		try
		{
			std::cout << "Initializing GLFW Version: " << glfwGetVersionString() << std::endl;

			/* Initialize the library */
			if (!glfwInit())
			{
				throw std::runtime_error("Failed to initialize GLFW");
			}
		}
		catch (std::runtime_error _error)
		{
			ErrorRuntime(_error);

			Exit(ExitCode::Failed);
		}

		return;
	}

	sfn InitalizeGLEW()
	{
		try
		{
			// If using GLEW version 1.13 or earlier
					//glewExperimental = true;

			std::cout << "Initializing Glew Version: " << glewGetString(GLEW_VERSION) << std::endl;

			GLenum err = glewInit();

			if (err != GLEW_OK)
			{
				// Problem: glewInit failed, something is seriously wrong.
				std::cout << "glewInit failed: " << glewGetErrorString(err) << std::endl;

				throw std::runtime_error("Failed to initialize GLFW");
			}

			cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
		}
		catch (std::runtime_error _error)
		{
			ErrorRuntime(_error);

			Exit(ExitCode::Failed);
		}
	}

	sfn PollEvents()
	{
		glfwPollEvents();

		return;
	}

	sfn RunBasicWindowLoop(const ptr<Window> _window)
	{
		/* Loop until the user closes the window */
		while (not CanClose(_window))
		{
			ClearBuffer();

			SwapBuffers(_window);

			PollEvents();
		}

		return;
	}

	sfn RunBasicWindowLoop_Timed(const ptr<Window> _window, TimeValDec _interval, FnPtr<void> _renderProcedure)
	{
		TimeValDec start, end, deltaSinceClear = 0.0;

		while (not CanClose(_window))
		{
			start = GetTime();

			if (deltaSinceClear > _interval)
			{
				ClearBuffer();

				_renderProcedure();

				SwapBuffers(_window);

				PollEvents();
			}

			end = GetTime();

			deltaSinceClear = deltaSinceClear + end - start;
		}

		return;
	}

	sfn RunTimingLoop()
	{
		return;
	}

	sfn SetCurrentContext(const ptr<Window> _window)
	{
		try
		{
			glfwMakeContextCurrent(_window);

			ptr< RawString<const char> > ErrorMsg = NULL;

			int code = glfwGetError(ErrorMsg);
			
			if (code == GLFW_NO_WINDOW_CONTEXT)
			{
				throw std::runtime_error( Dref(ErrorMsg) );
			}
		}
		catch (std::runtime_error _error)
		{
			ErrorRuntime(_error);

			Exit(ExitCode::Failed);
		}

		return;
	}

	sfn TerminateGLFW()
	{
		glfwTerminate();

		return;
	}
}
