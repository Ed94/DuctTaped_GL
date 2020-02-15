#pragma once

// GL
//#include <gl/GLU.h>
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "DGL_FundamentalTypes.hpp"
#include "DGL_MiscTypes.hpp"
#include "DGL_Enum.hpp"
#include "DGL_Shader.hpp"
#include "DGL_Buffers.hpp"
#include "DGL_Space.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace DGL
{
	// Aliases

	// C++ STL

	using std::cout;
	using std::endl;


	// GLFW

	using Monitor = GLFWmonitor;
	using TimeValInt = uint64_t;
	using TimeValDec = double;
	using Window = GLFWwindow;
	using WindowRefList = std::vector< ptr<Window> >;



	// Object Instances

	WindowRefList Windows;


	// Constants

	sfn constexpr NotShared   () -> ptr<Window > { return NULL; }
	sfn constexpr WindowedMode() -> ptr<Monitor> { return NULL; }



	// Forward Declares

	sfn SwapBuffers(const ptr<Window> _window) -> void;


	// Functionality

	sfn CanClose(const ptr<Window> _theWindow)
	{
		return glfwWindowShouldClose(_theWindow);
	}

	sfn CreateWindow
	(
		int                   _width,
		int                   _height,
		RawString<const char> _title,
		ptr      <Monitor   > _monitorToFullscreen,
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

	sfn CanUseRawMouse()
	{
		return glfwRawMouseMotionSupported();
	}

	sfn CursorPositionUpdateBind(ptr<Window> _window, FnPtr<void, double, double> _functionToCall)
	{
		glfwSetCursorPosCallback(_window, GLFWcursorposfun(_functionToCall));
	}

	sfn DrawArrays(EPrimitives _primitive, gInt _startingIndex, gInt _numToRender)
	{
		glDrawArrays(GLenum(_primitive), _startingIndex, _numToRender);   // Starting from vertex 0; 3 vertices total -> 1 triangle.
	}

	sfn DrawElements(EPrimitives _primitive, gSize _numElements, EDataType _dataType, DataPtr _offfsetAddressFromFirstIndex)
	{
		glDrawElements(GLenum(_primitive), _numElements, GLenum(_dataType), _offfsetAddressFromFirstIndex);
	}

	sfn KeyPressed(ptr<Window> _contextWindowRef, EKeyCodes _keyToCheck) -> bool
	{
		return glfwGetKey(_contextWindowRef, int(_keyToCheck)) ;
	}

	template<typename... CodeType, typename = EKeyCodes>
	sfn KeysPressed(ptr<Window> _contextWindowRef, CodeType... _otherKeys) -> bool
	{
		return ( KeyPressed(_contextWindowRef, _otherKeys) && ... ) == true;
	}

	sfn GetMouseInputMode(ptr<Window> _contextWindowRef, EMouseMode _mode)
	{
		return glfwGetInputMode(_contextWindowRef, GLenum(_mode));
	}

	sfn GetTime() -> TimeValDec
	{
		return glfwGetTime();
	}

	sfn GetRawTime() -> TimeValInt
	{
		return glfwGetTimerValue();
	}

	sfn GetCursorPosition(ptr<Window> _window, ptr<double> _xAxis, ptr<double> _yAxis)
	{
		glfwGetCursorPos(_window, _xAxis, _yAxis);
	}

	sfn ResetCursor(ptr<Window> _window, gFloat _screenCenterWidth, gFloat _screenCenterHeight)
	{
		glfwSetCursorPos(_window, _screenCenterWidth, _screenCenterHeight);


		glfwSetCursorPos(_window, 0, 0);
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
		catch (const std::runtime_error _error)
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
		catch (const std::runtime_error _error)
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
			ClearBuffer(EFrameBuffer::Color);

			SwapBuffers(_window);

			PollEvents();
		}

		return;
	}

	sfn RunBasicWindowLoop_Timed(const ptr<Window> _window, TimeValDec _interval, Delegate< Func<void>> _renderProcedure)
	{
		TimeValDec start, end, deltaSinceClear = 0.0;

		while (not CanClose(_window))
		{
			start = GetTime();

			if (deltaSinceClear > _interval)
			{

				ClearBuffer(EFrameBuffer::Color, EFrameBuffer::Depth);
				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

				//glMatrixMode(GL_MODELVIEW);

				//glLoadIdentity();

				_renderProcedure();

				SwapBuffers(_window);

				PollEvents();
			}

			end = GetTime();

			deltaSinceClear = deltaSinceClear + end - start;
		}

		return;
	}

	sfn SetClearColor(LinearColor _colorToSet)
	{
		glClearColor(_colorToSet.Red, _colorToSet.Green, _colorToSet.Blue, _colorToSet.Alpha);
	}
	
	template<typename ModeParam>
	sfn SetInputMode(ptr<Window> _window, EMouseMode _mouseMode, ModeParam _modeParam)
	{
		glfwSetInputMode(_window, GLenum(_mouseMode), GLenum(_modeParam));
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
		catch (const std::runtime_error _error)
		{
			ErrorRuntime(_error);

			Exit(ExitCode::Failed);
		}

		return;
	}

	sfn SetPolygonMode(EFace _desiredFaces, ERenderMode _desiredMode)
	{
		glPolygonMode(GLenum(_desiredFaces), GLenum(_desiredMode));
	}

	sfn SwapBuffers(const ptr<Window> _window) -> void
	{
		glfwSwapBuffers(_window);

		return;
	}

	sfn TerminateGLFW()
	{
		glfwTerminate();

		return;
	}
}
