#pragma once

// GLFW, GLEW, GLM
#include <glew.h                      >
#include <glfw3.h                     >
#include <glm/glm.hpp                 >
#include <glm/gtc/matrix_transform.hpp>

// DGL
#include "DGL_Types.hpp"
#include "DGL_Enum.hpp"
#include "DGL_Shader.hpp"
#include "DGL_Buffers.hpp"
#include "DGL_Space.hpp"
#include "DGL_Model.hpp"

// Non-Standard C+
#include "Cpp_Alias.hpp"


namespace DGL
{
	// Object Instances

	WindowRefList Windows;


	// Constants

	sfn constexpr NotShared   () -> ptr<Window > { return NULL; }
	sfn constexpr WindowedMode() -> ptr<Monitor> { return NULL; }



	// Forward Declarations

	sfn SwapBuffers(ro ptr<Window> _window) -> void;


	// Functionality

	sfn CanClose(ro ptr<Window> _theWindow)
	{
		return glfwWindowShouldClose(_theWindow);
	}

	sfn CanUseRawMouse()
	{
		return glfwRawMouseMotionSupported();
	}

	sfn CreateWindow
	(
		   int                _width                     ,
		   int                _height                    ,
		ro RawString<ro char> _title                     ,
		ro ptr      <Monitor> _monitorToFullscreen       ,
		ro ptr      <Window > _windowToShareResourcesWith
	)
		-> ptr<Window>
	{
		Windows.push_back(glfwCreateWindow(_width, _height, _title, _monitorToFullscreen, _windowToShareResourcesWith));

		if (Windows.back() == NULL)
		{
			throw std::runtime_error("Failed to create a window");
		}

		return Windows.back();
	}

	sfn CursorPositionUpdateBind(ro ptr<Window> _window, ro FnPtr<void, double, double> _functionToCall)
	{
		glfwSetCursorPosCallback(_window, GLFWcursorposfun(_functionToCall));
	}

	sfn DestoryWindow(ro ptr<Window> _window)
	{
		using ElementType = decltype(Windows.begin());

		for (ElementType element = Windows.begin(); element != Windows.end(); element++)
		{
			if (Dref(element._Ptr) == _window)
			{
				glfwDestroyWindow(_window);

				Windows.erase(element);

				return;
			}
		}

		return;
	}

	sfn EnableDepth()
	{
		// TODO: Clean.

		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
	}

	sfn GetCursorPosition(ro ptr<Window> _window, ro ptr<double> _xAxis, ro ptr<double> _yAxis)
	{
		glfwGetCursorPos(_window, _xAxis, _yAxis);
	}

	sfn GetMouseInputMode(ro ptr<Window> _contextWindowRef, EMouseMode _mode)
	{
		return glfwGetInputMode(_contextWindowRef, GLenum(_mode));
	}

	sfn GetRawTime() -> TimeValInt
	{
		return glfwGetTimerValue();
	}

	sfn GetTime() -> TimeValDec
	{
		return glfwGetTime();
	}

	sfn InitalizeGLFW()
	{
		std::cout << "Initializing GLFW Version: " << glfwGetVersionString() << std::endl;

		/* Initialize the library */
		if (!glfwInit())
		{
			throw std::runtime_error("Failed to initialize GLFW");
		}

		return;
	}

	sfn InitalizeGLEW()
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

	sfn KeyPressed(ro ptr<Window> _contextWindowRef, EKeyCodes _keyToCheck) -> bool
	{
		return glfwGetKey(_contextWindowRef, int(_keyToCheck));
	}

	template<typename... CodeType, typename = EKeyCodes>
	sfn KeysPressed(ro ptr<Window> _contextWindowRef, CodeType... _otherKeys) -> bool
	{
		return (KeyPressed(_contextWindowRef, _otherKeys) && ...) == true;
	}

	sfn PollEvents()
	{
		glfwPollEvents();

		return;
	}

	sfn ResetCursor(ro ptr<Window> _window, gUInt _screenCenterWidth, gUInt _screenCenterHeight)
	{
		//glfwSetCursorPos(_window, _screenCenterWidth, _screenCenterHeight);

		ECursorMode cursorMode = ECursorMode(GetMouseInputMode(_window, EMouseMode::Cursor));

		if (cursorMode == ECursorMode::Normal || cursorMode == ECursorMode::Hidden)   // Normal Cursor Mode
		{
			glfwSetCursorPos(_window, double(_screenCenterWidth), double(_screenCenterHeight));
		}
		else   // Raw Cursor mode
		{
			glfwSetCursorPos(_window, 0, 0);
		}

		return;
	}

	sfn SetClearColor(ro Ref(LinearColor) _colorToSet)
	{
		glClearColor(_colorToSet.Red, _colorToSet.Green, _colorToSet.Blue, _colorToSet.Alpha);

		return;
	}

	sfn SetCurrentContext(ro ptr<Window> _window)
	{
		glfwMakeContextCurrent(_window);

		ptr< RawString<const char> > ErrorMsg = NULL;

		int code = glfwGetError(ErrorMsg);

		if (code == GLFW_NO_WINDOW_CONTEXT)
		{
			throw std::runtime_error(Dref(ErrorMsg));
		}

		return;
	}

	template<typename ModeParam>
	sfn SetInputMode(ro ptr<Window> _window, EMouseMode _mouseMode, ModeParam _modeParam)
	{
		glfwSetInputMode(_window, GLenum(_mouseMode), GLenum(_modeParam));

		return;
	}

	sfn SetPolygonMode(EFace _desiredFaces, ERenderMode _desiredMode)
	{
		glPolygonMode(GLenum(_desiredFaces), GLenum(_desiredMode));

		return;
	}

	sfn SwapBuffers(ro ptr<Window> _window) -> void
	{
		glfwSwapBuffers(_window);

		return;
	}

	sfn TerminateGLFW()
	{
		glfwTerminate();

		return;
	}


	// Old Tape

	[[deprecated]]
	sfn RunBasicWindowLoop(ro ptr<Window> _window)
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

	[[deprecated]]
	sfn RunBasicWindowLoop_Timed(ro ptr<Window> _window, TimeValDec _interval, ro Delegate< Func<void>> _renderProcedure)
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
}
