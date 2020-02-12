#pragma once

// OpenGL
#include <glew.h>

// Duck Tape
#include "DGL_FundamentalTypes.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace GL
{
	// Made up types.

	template<typename ReferenceType>
	using ID = gUInt;

	// ID Reference Types

	class Buffer       ;
	class Shader       ;
	class ShaderProgram;

	using DataPtr      = ptr<GLvoid>;
	using VertexBuffer = gUInt      ;
}
