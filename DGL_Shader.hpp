#pragma once

//DGL
#include "DGL_FundamentalTypes.hpp"
#include "DGL_MiscTypes.hpp"
#include "DGL_Enum.hpp"
#include "DGL_Space.hpp"



namespace DGL
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


	// Default Shaders

	ID<ShaderProgram> RawShader   ,
		              SimpleShader ;

	

	// Forward Declarations

	sfn GetShaderInfo    (    ID<Shader       >  _shader               ,       gSize       _logLength        ,       ptr<gSize> _infoLengthRef      , RawString<gChar> _infoLogRef                                              ) -> void;
	sfn QueryShader      (    ID<Shader       >  _shaderToQuery        ,       EShaderInfo _shaderInfoDesired,       ptr<gInt > _requestedInfoObject                                                                            ) -> void;
	sfn MakeShader       (Ref(ID<Shader       >) _shaderIDHolder       ,       EShaderType _typeOfShader     ,       uInt64 _numberOfStringElements , ptr<RawString<const gChar>> _sourceCode, ptr<const gInt> _lengthsOfStrings) -> void;
	sfn MakeShaderProgram(Ref(ID<ShaderProgram>) _shaderProgramIDHolder, const ID<Shader>  _vertexShader     , const ID<Shader> _fragShader                                                                                     ) -> void;



	// Functions

	sfn ActiveUniforms(ID<Shader> _shaderToQueryForUniforms) -> GLint
	{
		GLint uniforms;

		glGetProgramiv(_shaderToQueryForUniforms, GL_ACTIVE_UNIFORMS, &uniforms);

		for (int uniformIndex = 0; uniformIndex < uniforms; uniformIndex++)
		{
			int name_len = -1, num = -1;

			GLenum type = GL_ZERO;

			char name[100];

			glGetActiveUniform(_shaderToQueryForUniforms, GLuint(uniformIndex), sizeof(name) - 1, &name_len, &num, &type, name);

			name[name_len] = 0;
		}

		return uniforms;
	}

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

	sfn CompileShader(const ID<Shader> _shaderToCompile)
	{
		glCompileShader(_shaderToCompile);

		gInt Result = gInt(EBool::False);

		gSize InfoLogLength;

		QueryShader(_shaderToCompile, EShaderInfo::CompileStatus, Address(Result));

		if (!Result)
		{
			QueryShader(_shaderToCompile, EShaderInfo::InfoLogLength, Address(InfoLogLength));

			if (InfoLogLength > 0)
			{
				std::vector<char> ErrorMessage(InfoLogLength + 1);

				GetShaderInfo(_shaderToCompile, InfoLogLength, nullptr, Address(ErrorMessage.at(0)));

				throw std::runtime_error(Address(ErrorMessage.at(0)));
			}
			else
			{
				throw std::runtime_error("Shader compilation failed and did not get a proper info log.");
			}
		}

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

	sfn DetachShader(ID<ShaderProgram> _shaderProgram, ID<Shader> _shaderToDetach)
	{
		glDetachShader(_shaderProgram, _shaderToDetach);

		return;
	}

	sfn GetShaderInfo(ID<Shader> _shader, gSize _logLength, ptr<gSize> _infoLengthRef, RawString<gChar> _infoLogRef) -> void
	{
		glGetShaderInfoLog(_shader, _logLength, _infoLengthRef, _infoLogRef);

		return;
	}

	sfn GetShaderProgramInfo(ID<ShaderProgram> _shaderProgram, gSize _logLength, ptr<gSize> _infoLengthRef, RawString<gChar> _infoLogRef) -> void
	{
		glGetProgramInfoLog(_shaderProgram, _logLength, _infoLengthRef, _infoLogRef);

		return;
	}

	sfn GetUniformVariable(const ID<ShaderProgram> _programID, RawString<const char> _nameOfVariable) -> ID<Matrix>
	{
		return glGetUniformLocation(_programID, _nameOfVariable);
	}

	sfn LinkProgramShader(ID<ShaderProgram> _shaderProgramToLink)
	{
		glLinkProgram(_shaderProgramToLink);

		return;
	}

	sfn QueryShader(ID<Shader> _shaderToQuery, EShaderInfo _shaderInfoDesired, ptr<gInt> _requestedInfoObject) -> void
	{
		glGetShaderiv(_shaderToQuery, GLenum(_shaderInfoDesired), _requestedInfoObject);

		return;
	}

	sfn QueryShaderProgram(ID<ShaderProgram> _shaderToQuery, EShaderProgramInfo _shaderProgramInfoDesired, ptr<gInt> _requestedInfoObject) -> void
	{
		glGetProgramiv(_shaderToQuery, GLenum(_shaderProgramInfoDesired), _requestedInfoObject);

		return;
	}

	sfn LoadShaders(RawString<const char> _vertexShaderFilePath, RawString<const char> _fragmentShaderFilePath) -> ID<ShaderProgram>
	{
		using std::cout        ;
		using std::endl        ;
		using std::ifstream    ;
		using std::ios         ;
		using std::string      ;
		using std::stringstream;
		using std::vector      ;


		string   vertexShaderCode        ;
		string   fragmentShaderCode      ;

		ifstream vertexShaderFileStream  ;
		ifstream fragmentShaderFileStream;

		vertexShaderFileStream  .open(_vertexShaderFilePath);
		fragmentShaderFileStream.open(_fragmentShaderFilePath);

		try
		{
			if (vertexShaderFileStream.is_open() and fragmentShaderFileStream.is_open())
			{
				stringstream vertSourceStrStream;
				stringstream fragSourceStrStream;

				vertSourceStrStream << vertexShaderFileStream  .rdbuf();
				fragSourceStrStream << fragmentShaderFileStream.rdbuf();

				vertexShaderFileStream  .close();
				fragmentShaderFileStream.close();

				vertexShaderCode   = vertSourceStrStream.str();
				fragmentShaderCode = fragSourceStrStream.str();
			}
			else
			{
				throw std::runtime_error("Impossible to open% s.Are you in the right directory ? Don't forget to read the FAQ !");
			}


			RawString<const char> vertexSourcePtr   = vertexShaderCode  .c_str();
			RawString<const char> fragmentSourcePtr = fragmentShaderCode.c_str();

			cout << "Compiling shader: " << _vertexShaderFilePath << endl;

			ID<Shader> vertexShader = CreateShader(EShaderType::Vertex);

			BindShaderSource(vertexShader, 1, Address(vertexSourcePtr), NULL);
			CompileShader   (vertexShader                                   );

			cout << "Compiling shader: " << _fragmentShaderFilePath << endl;

			ID<Shader> fragmentShader = CreateShader(EShaderType::Fragment);

			BindShaderSource(fragmentShader, 1, Address(fragmentSourcePtr), NULL);
			CompileShader   (fragmentShader                                     );

			
			cout << "Making Shader Program and Linking..." << endl;

			ID<ShaderProgram> generatedProgram;

			MakeShaderProgram(generatedProgram, vertexShader, fragmentShader);

			DeleteShader(vertexShader  );
			DeleteShader(fragmentShader);

			return generatedProgram;
		}
		catch (const std::runtime_error _error)
		{
			ErrorRuntime(_error);

			Exit(ExitCode::Failed);
		}
	}

	sfn MakeShader
	(
		Ref(ID<Shader>)             _shaderIDHolder        , 
		EShaderType                 _typeOfShader          , 
		uInt64                      _numberOfStringElements,
		ptr<RawString<const gChar>> _sourceCode            ,
		ptr<          const gInt  > _lengthsOfStrings
	)
		-> void
	{
		_shaderIDHolder = CreateShader(_typeOfShader);

		BindShaderSource(_shaderIDHolder, _numberOfStringElements, _sourceCode, _lengthsOfStrings);

		CompileShader(_shaderIDHolder);

		return;
	}

	sfn MakeShaderProgram(Ref(ID<ShaderProgram>) _shaderProgramIDHolder, const ID<Shader> _vertexShader, const ID<Shader> _fragShader) -> void
	{
		_shaderProgramIDHolder = CreateShaderProgram();

		AttachShader(_shaderProgramIDHolder, _vertexShader);
		AttachShader(_shaderProgramIDHolder, _fragShader  );

		LinkProgramShader(_shaderProgramIDHolder);

		gInt Result = false;

		QueryShaderProgram(_shaderProgramIDHolder, EShaderProgramInfo::LinkStatus, Address(Result));

		if (!Result)
		{
			gInt infoLogLength;

			QueryShaderProgram(_shaderProgramIDHolder, EShaderProgramInfo::InfoLogLength, Address(infoLogLength));

			if (infoLogLength > 0)
			{
				std::vector<char> ErrorMessage(infoLogLength + 1);

				GetShaderProgramInfo(_shaderProgramIDHolder, infoLogLength, NULL, Address(ErrorMessage.at(0)));

				throw std::runtime_error(Address(ErrorMessage.at(0)));
			}
			else
			{
				throw std::runtime_error("ShaderProgram compilation failed and did not get a proper info log.");
			}
		}

		DetachShader(_shaderProgramIDHolder, _vertexShader);
		DetachShader(_shaderProgramIDHolder, _fragShader  );

		return;
	}

	// MVA: MatrixVariableArray
	sfn SetUniformVariable_MVA(const ID<Matrix> _matrixID, const gSize _numMatricies, const EBool _shouldTransposeValues, ptr<const float> _dataPtr)
	{
		glUniformMatrix4fv(_matrixID, _numMatricies, GLenum(_shouldTransposeValues), _dataPtr);

		return;
	}

	sfn UseProgramShader(ID<ShaderProgram> _shaderProgramToUse)
	{
		glUseProgram(_shaderProgramToUse);

		return;
	}


	namespace SS_Transformed
	{
		ID<ShaderProgram> Shader;

		ID<CoordSpace> ScreenSpaceVarID;

		sfn UpdateShader(Ref(CoordSpace) _screenSpace)
		{
			SetUniformVariable_MVA(ScreenSpaceVarID, 1, DGL::EBool::False, Address(_screenSpace[0][0]));

			return;
		}

		sfn LoadShader()
		{
			Shader = LoadShaders("SimpleTransform.vert", "SingleColor.frag");

			ScreenSpaceVarID = DGL::GetUniformVariable(Shader, "ScreenSpaceTransform");

			return;
		}
	}

	sfn LoadRawShader()
	{
		ID<Shader> VertexShader;
		ID<Shader> FragmentShader;

		MakeShader(VertexShader, EShaderType::Vertex, 1, Address(DGL::RawVertextShaderSource), NULL);
		MakeShader(FragmentShader, EShaderType::Fragment, 1, Address(DGL::RawFragmentShaderSource), NULL);

		MakeShaderProgram(RawShader, VertexShader, FragmentShader);

		DeleteShader(VertexShader);
		DeleteShader(FragmentShader);

		return;
	}

	sfn LoadSimpleShader()
	{
		SimpleShader = LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");

		return;
	}

	sfn LoadDefaultShaders()
	{
		                LoadRawShader   ();
		                LoadSimpleShader();
		SS_Transformed::LoadShader      ();

		return;
	}
}
