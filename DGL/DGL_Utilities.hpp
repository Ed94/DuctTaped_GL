/*
Title : Duct Taped GL: Utilities
Author: Edward R. Gonzalez

Description:
The header with the general functionality for using glm/glfw/glew.
*/


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

	 constexpr Window*  NotShared   () { return NULL; }
	 constexpr Monitor* WindowedMode() { return NULL; }



	// Forward Declarations

	void SwapBuffers(Window* const _window);


	// Functionality

	bool CanClose(Window* const _theWindow)
	{
		return glfwWindowShouldClose(_theWindow);
	}

	bool CanUseRawMouse()
	{
		return glfwRawMouseMotionSupported();
	}

	Window* CreateWindow
	(
		      int            _width                     ,
		      int            _height                    ,
		const char*    const _title                     ,
		      Monitor* const _monitorToFullscreen       ,
		      Window*  const _windowToShareResourcesWith
	)
	{
		Windows.push_back(glfwCreateWindow(_width, _height, _title, _monitorToFullscreen, _windowToShareResourcesWith));

		if (Windows.back() == NULL)
		{
			throw std::runtime_error("Failed to create a window");
		}

		return Windows.back();
	}

	void CursorPositionUpdateBind(Window* const _window, void(*_functionToCall)(double, double))
	{
		glfwSetCursorPosCallback(_window, GLFWcursorposfun(_functionToCall));
	}

	void DestoryWindow(Window* const _window)
	{
		using ElementType = decltype(Windows.begin());

		for (ElementType element = Windows.begin(); element != Windows.end(); element++)
		{
			if (*element == _window)
			{
				glfwDestroyWindow(_window);

				Windows.erase(element);

				return;
			}
		}

		return;
	}

	void EnableDepth()
	{
		// TODO: Clean.

		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		return;
	}

	void GetCursorPosition(Window* const _window, double* const _xAxis, double* const _yAxis)
	{
		glfwGetCursorPos(_window, _xAxis, _yAxis);
	}

	gInt GetMouseInputMode(Window* const _contextWindowRef, EMouseMode _mode)
	{
		return glfwGetInputMode(_contextWindowRef, GLenum(_mode));
	}

	TimeValInt GetRawTime()
	{
		return glfwGetTimerValue();
	}

	TimeValDec GetTime()
	{
		return glfwGetTime();
	}

	void InitalizeGLFW()
	{
		std::cout << "Initializing GLFW Version: " << glfwGetVersionString() << std::endl;

		/* Initialize the library */
		if (!glfwInit())
		{
			throw std::runtime_error("Failed to initialize GLFW");
		}

		return;
	}

	void InitalizeGLEW()
	{
		// If using GLEW version 1.13 or earlier
		glewExperimental = true;

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

	bool KeyPressed(Window* const _contextWindowRef, EKeyCodes _keyToCheck)
	{
		return glfwGetKey(_contextWindowRef, int(_keyToCheck));
	}

	template<typename... CodeType, typename = EKeyCodes>
	bool KeysPressed(Window* const _contextWindowRef, CodeType... _otherKeys)
	{
		return (KeyPressed(_contextWindowRef, _otherKeys) && ...) == true;
	}

	void PollEvents()
	{
		glfwPollEvents();

		return;
	}

	void ResetCursor(Window* _window, gUInt _screenCenterWidth, gUInt _screenCenterHeight)
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

	void SetClearColor(const LinearColor& _colorToSet)
	{
		glClearColor(_colorToSet.Red, _colorToSet.Green, _colorToSet.Blue, _colorToSet.Alpha);

		return;
	}

	void SetCurrentContext(Window* const _window)
	{
		glfwMakeContextCurrent(_window);

		const char** ErrorMsg = NULL;

		int code = glfwGetError(ErrorMsg);

		if (code == GLFW_NO_WINDOW_CONTEXT)
		{
			throw std::runtime_error(*ErrorMsg);
		}

		return;
	}

	template<typename ModeParam>
	void SetInputMode(Window* const _window, EMouseMode _mouseMode, ModeParam _modeParam)
	{
		glfwSetInputMode(_window, GLenum(_mouseMode), GLenum(_modeParam));

		return;
	}

	void SetPolygonMode(EFace _desiredFaces, ERenderMode _desiredMode)
	{
		glPolygonMode(GLenum(_desiredFaces), GLenum(_desiredMode));

		return;
	}

	void SetWindowHeader(Window* const _window, string _title)
	{
		glfwSetWindowTitle(_window, _title.c_str());
	}

	void SwapBuffers(Window* const _window)
	{
		glfwSwapBuffers(_window);

		return;
	}

	void TerminateGLFW()
	{
		glfwTerminate();

		return;
	}
}
