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

	// ID Reference Types

	class VertexBuffer ;
	class NormalBuffer;
	class Vec3       ;
	class Matrix       ;
	class Shader       ;
	class ShaderProgram;
	class VertexArray ;
	class ElementBuffer;



	using Matrix4x4 = glm::mat4;

	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;

	struct LinearColor
	{
		gFloatClamped Red, Green, Blue, Alpha;

		LinearColor(gFloatClamped _red, gFloatClamped _green, gFloatClamped _blue, gFloatClamped _alpha) :
			Red(_red), Green(_green), Blue(_blue), Alpha(_alpha) {};

		sfn Vector() -> Vector3
		{
			return Vector3(Red, Green, Blue);
		}
	};
}
