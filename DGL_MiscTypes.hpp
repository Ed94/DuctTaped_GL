#pragma once

// OpenGL
#include <glew.h                  >
#include <glm/detail/type_vec3.hpp>


// Duck Tape
#include "DGL_FundamentalTypes.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace DGL
{
	namespace Generic
	{
		template<typename Type>
		using Vector3 = glm::tvec3<Type>;
	}

	using DataPtr = ptr<GLvoid>;

	template<typename ReferenceType>
	using ID = gUInt;


	struct LinearColor
	{
		gFloatClamped Red, Green, Blue, Alpha;

		LinearColor(gFloatClamped _red, gFloatClamped _green, gFloatClamped _blue, gFloatClamped _alpha) :
			Red(_red), Green(_green), Blue(_blue), Alpha(_alpha) {};
	};

	// ID Reference Types

	class Buffer       ;
	class Matrix       ;
	class Shader       ;
	class ShaderProgram;
	class VertexBuffer ;
	class ElementBuffer;

	using Matrix4x4 = glm::mat4;

	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
}
