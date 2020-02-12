#pragma once

// C++
#include <cstdarg>

//DGL
#include "DGL_FundamentalTypes.hpp"
#include "DGL_MiscTypes.hpp"
#include "DGL_Enum.hpp"



namespace GL
{
	// Some Raw Source Defaults:

	RawString<const char> RawVertextShaderSource =
		"#version 330 core\n"

		"layout (location = 0) in vec3 aPos;\n"

		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	RawString<const char> RawFragmentShaderSource =
		"#version 400\n"

		"out vec4 frag_colour;"

		"void main() {"

		"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"

		"}";


	// Functions

	sfn AttachShader(ID<ShaderProgram> _shaderProgram, ID<Shader> _shaderToAttach)
	{
		glAttachShader(_shaderProgram, _shaderToAttach);

		return;
	}

	sfn BindShaderSource(ID<Shader> _shader, uInt64 _numberOfStringElements, ptr< RawString<const gChar>> _sourceCode, ptr<const gInt> _lengthsOfStrings)
	{
		glShaderSource(_shader, _numberOfStringElements, _sourceCode, _lengthsOfStrings);

		return;
	}

	sfn CompileShader(ID<Shader> _shaderToCompile)
	{
		glCompileShader(_shaderToCompile);

		return;
	}

	sfn CreateShader(EShaderType _typeOfShader) -> ID<Shader>
	{
		return glCreateShader(GLenum(_typeOfShader));
	}

	sfn CreateShaderProgram() -> ID<ShaderProgram>
	{
		return glCreateProgram();
	}

	sfn DeleteShader(ID<Shader> _shaderToDelete)
	{
		glDeleteShader(_shaderToDelete);

		return;
	}
	 
	sfn LinkProgramShader(ID<ShaderProgram> _shaderProgramToLink)
	{
		glLinkProgram(_shaderProgramToLink);

		return;
	}

	sfn UseProgramShader(ID<ShaderProgram> _shaderProgramToUse)
	{
		glUseProgram(_shaderProgramToUse);

		return;
	}


	// Raw Tape

	sfn MakeShader
	(
		Ref(ID<Shader>)             _shaderIDHolder        , 
		EShaderType                 _typeOfShader          , 
		uInt64                      _numberOfStringElements,
		ptr<RawString<const gChar>> _sourceCode            ,
		ptr<          const gInt  > _lengthsOfStrings
	)
	{
		_shaderIDHolder = CreateShader(_typeOfShader);

		BindShaderSource(_shaderIDHolder, _numberOfStringElements, _sourceCode, _lengthsOfStrings);

		CompileShader(_shaderIDHolder);
	}

	sfn MakeShaderProgram(Ref(ID<ShaderProgram>) _shaderProgramIDHolder, ID<Shader> _shadersToAttach...)
	{
		_shaderProgramIDHolder = CreateShaderProgram();

		va_list args;

		va_start(args, _shadersToAttach);

		AttachShader(_shaderProgramIDHolder, va_arg(args, ID<Shader>));

		va_end(args);

		LinkProgramShader(_shaderProgramIDHolder);
	}
}
