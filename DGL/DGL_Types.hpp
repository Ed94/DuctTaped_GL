/*
Title : Ducktaped GL: Types
Author: Edward R. Gonzalez

Description:
Provides type definitions and aliases for the various types used in DGL library.
*/


#pragma once

// GLEW
#include <glew.h>

// GLFW
#include <glfw3.h>

// GLM
#include <glm/glm.hpp>

// C++
#include "Cpp_Alias.hpp"



namespace DGL
{
	// OpenGL

	// Fundamental Types

	using gBitfield     = GLbitfield;
	using gBoolean      = GLboolean ;
	using gChar         = GLchar    ;
	using gFloat        = GLfloat   ;
	using gFloatClamped = GLclampf  ;
	using gInt          = GLint     ;
	using gUInt         = GLuint    ;
	using gSize         = GLsizei   ;
	using gVoid         = GLvoid    ;

	using DataPtr = ptr<gVoid>;

	// Type used to indicate the context of an integer used as an ID for an object managed by OpenGL.
	template<typename ReferenceType>
	using ID = gUInt;

	// ID Reference Types

	class ElementBuffer;
	class Matrix       ;
	class NormalBuffer ;
	class Shader       ;
	class ShaderProgram;
	class TextureBuffer;
	class Vec3         ;
	class VertexArray  ;
	class VertexBuffer ;



	// GLM

	namespace Generic
	{
		template<typename Type>
		using Vector3 = glm::tvec3<Type>;

		template<typename Type>
		using Vector2 = glm::tvec2<Type>;
	}

	using Matrix4x4 = glm::mat4;

	using CoordSpace = Matrix4x4;
	using Projection = Matrix4x4;


	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;

	using UVList = std::vector < Vector2>;

	using Vec2Int    = Generic::Vector2< gUInt  >;
	using Vec3Int    = Generic::Vector3< gUInt  >;
	using VertexList = std    ::vector < Vector3>;


	// GLFW

	using Monitor       = GLFWmonitor               ;
	using TimeValInt    = uint64_t                  ;
	using TimeValDec    = double                    ;
	using Window        = GLFWwindow                ;
	using WindowRefList = std::vector< ptr<Window> >;


	// DGL

	using VecColor = Vector3;

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

	using VIndexList = std    ::vector < gInt   >;
}
