#pragma once

//DGL
#include "DGL_Enum.hpp"
#include "DGL_Types.hpp"
#include "DGL_Buffers.hpp"
#include "DGL_Space.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace DGL
{
	// Forward Declarations

	void GetShaderInfo    (ID<Shader       >  _shader               , gSize       _logLength        , gSize*      _infoLengthRef          ,       char*        _infoLogRef                               );
	void QueryShader      (ID<Shader       >  _shaderToQuery        , EShaderInfo _shaderInfoDesired, gInt* const _requestedInfoObject                                                                   );
	void MakeShader       (ID<Shader       >& _shaderIDHolder       , EShaderType _typeOfShader     , uInt64      _numberOfStringElements , const char** const _sourceCode, gInt* const _lengthsOfStrings);
	void MakeShaderProgram(ID<ShaderProgram>& _shaderProgramIDHolder, ID<Shader>  _vertexShader     , ID<Shader>  _fragShader                                                                            );



	// Functions

	GLint ActiveUniforms(ID<Shader> _shaderToQueryForUniforms)
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

	void AttachShader(ID<ShaderProgram> _shaderProgram, ID<Shader> _shaderToAttach)
	{
		glAttachShader(_shaderProgram, _shaderToAttach);

		return;
	}

	void BindShaderSource(ID<Shader> _shader, gSize _numberOfStringElements, const char** _sourceCode, gInt* const _lengthsOfStrings)
	{
		glShaderSource(_shader, _numberOfStringElements, _sourceCode, _lengthsOfStrings); //Address(_sourceCode), Address(_lengthsOfStrings));

		return;
	}

	void CompileShader(ID<Shader> _shaderToCompile)
	{
		glCompileShader(_shaderToCompile);

		gInt Result = gInt(EBool::False);

		gSize InfoLogLength;

		QueryShader(_shaderToCompile, EShaderInfo::CompileStatus, &Result);

		if (!Result)
		{
			QueryShader(_shaderToCompile, EShaderInfo::InfoLogLength, &InfoLogLength);

			if (InfoLogLength > 0)
			{
				std::vector<char> ErrorMessage(InfoLogLength + 1);

				GetShaderInfo(_shaderToCompile, InfoLogLength, nullptr, &ErrorMessage.at(0));

				throw std::runtime_error(&ErrorMessage.at(0));
			}
			else
			{
				throw std::runtime_error("Shader compilation failed and did not get a proper info log.");
			}
		}

		return;
	}

	ID<Shader> CreateShader(EShaderType _typeOfShader)
	{
		return glCreateShader(GLenum(_typeOfShader));
	}

	ID<ShaderProgram> CreateShaderProgram()
	{
		return glCreateProgram();
	}

	void DeleteShader(ID<Shader> _shaderToDelete)
	{
		glDeleteShader(_shaderToDelete);

		return;
	}

	void DetachShader(ID<ShaderProgram> _shaderProgram, ID<Shader> _shaderToDetach)
	{
		glDetachShader(_shaderProgram, _shaderToDetach);

		return;
	}

	void GetShaderInfo(ID<Shader> _shader, gSize _logLength, gSize* const _infoLengthRef, char* _infoLogRef)
	{
		glGetShaderInfoLog(_shader, _logLength, _infoLengthRef, _infoLogRef);

		return;
	}

	void GetShaderProgramInfo(ID<ShaderProgram> _shaderProgram, gSize _logLength, gSize* const _infoLengthRef, char* _infoLogRef)
	{
		glGetProgramInfoLog(_shaderProgram, _logLength, _infoLengthRef, _infoLogRef);

		return;
	}

	ID<Matrix> GetUniformVariable(const ID<ShaderProgram> _programID, const char* _nameOfVariable)
	{
		return glGetUniformLocation(_programID, _nameOfVariable);
	}

	void LinkProgramShader(ID<ShaderProgram> _shaderProgramToLink)
	{
		glLinkProgram(_shaderProgramToLink);

		return;
	}

	void QueryShader(ID<Shader> _shaderToQuery, EShaderInfo _shaderInfoDesired, gInt* const _requestedInfoObject)
	{
		glGetShaderiv(_shaderToQuery, GLenum(_shaderInfoDesired), _requestedInfoObject);

		return;
	}

	void QueryShaderProgram(ID<ShaderProgram> _shaderToQuery, EShaderProgramInfo _shaderProgramInfoDesired, gInt* _requestedInfoObject)
	{
		glGetProgramiv(_shaderToQuery, GLenum(_shaderProgramInfoDesired), _requestedInfoObject);

		return;
	}

	ID<ShaderProgram> LoadShaders(const char* const _vertexShaderFilePath, const char* const _fragmentShaderFilePath)
	{
		using std::vector;

		string   vertexShaderCode        ;
		string   fragmentShaderCode      ;

		ifstream vertexShaderFileStream  ;
		ifstream fragmentShaderFileStream;

		vertexShaderFileStream  .open(_vertexShaderFilePath);
		fragmentShaderFileStream.open(_fragmentShaderFilePath);

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


		const char* vertexSourcePtr   = vertexShaderCode  .c_str();
		const char* fragmentSourcePtr = fragmentShaderCode.c_str();

		cout << "Compiling shader: " << _vertexShaderFilePath << endl;

		ID<Shader> vertexShader = CreateShader(EShaderType::Vertex);

		BindShaderSource(vertexShader, 1, &vertexSourcePtr, NULL);
		CompileShader   (vertexShader                          );

		cout << "Compiling shader: " << _fragmentShaderFilePath << endl;

		ID<Shader> fragmentShader = CreateShader(EShaderType::Fragment);

		BindShaderSource(fragmentShader, 1, &fragmentSourcePtr, NULL);
		CompileShader   (fragmentShader                            );

		
		cout << "Making Shader Program and Linking..." << endl;

		ID<ShaderProgram> generatedProgram;

		MakeShaderProgram(generatedProgram, vertexShader, fragmentShader);

		DeleteShader(vertexShader  );
		DeleteShader(fragmentShader);

		return generatedProgram;
	}

	void MakeShader
	(
		      ID<Shader>&  _shaderIDHolder        , 
		      EShaderType  _typeOfShader          , 
		      gSize        _numberOfStringElements,
	    const char** const _sourceCode            ,
		      gInt*  const _lengthsOfStrings
	)
	{
		_shaderIDHolder = CreateShader(_typeOfShader);

		BindShaderSource(_shaderIDHolder, _numberOfStringElements, _sourceCode, _lengthsOfStrings);

		CompileShader(_shaderIDHolder);

		return;
	}

	void MakeShaderProgram(ID<ShaderProgram>& _shaderProgramIDHolder, ID<Shader> _vertexShader, ID<Shader> _fragShader)
	{
		_shaderProgramIDHolder = CreateShaderProgram();

		AttachShader(_shaderProgramIDHolder, _vertexShader);
		AttachShader(_shaderProgramIDHolder, _fragShader  );

		LinkProgramShader(_shaderProgramIDHolder);

		gInt Result = false;

		QueryShaderProgram(_shaderProgramIDHolder, EShaderProgramInfo::LinkStatus, &Result);

		if (!Result)
		{
			gInt infoLogLength;

			QueryShaderProgram(_shaderProgramIDHolder, EShaderProgramInfo::InfoLogLength, &infoLogLength);

			if (infoLogLength > 0)
			{
				std::vector<char> ErrorMessage(infoLogLength + 1);

				GetShaderProgramInfo(_shaderProgramIDHolder, infoLogLength, NULL, &ErrorMessage.at(0));

				cout << (char*)&ErrorMessage[0] << endl;

				throw std::runtime_error(&ErrorMessage.at(0));
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
	void SetUniformVariable_MVA(ID<Matrix> _matrixID, gSize _numMatricies, gBoolean _shouldTransposeValues, const gFloat& _dataPtr)
	{
		glUniformMatrix4fv(_matrixID, _numMatricies, _shouldTransposeValues, &_dataPtr);

		return;
	}

	void SetUniformVariable_Vector3(ID<Vec3> _ID, gSize _num, const gFloat& _dataPtr)
	{
		glUniform3fv(_ID, _num, &_dataPtr);

		return;
	}

	void SetUniformVariable_Float(ID<gFloat> _ID, gFloat _data)
	{
		glUniform1f(_ID, _data);

		return;
	}

	void UseProgramShader(ID<ShaderProgram> _shaderProgramToUse)
	{
		glUseProgram(_shaderProgramToUse);

		return;
	}

	namespace Basic_LightShader
	{
		ID<ShaderProgram> Shader;

		ID<CoordSpace> ScreenSpaceVarID;

		void LoadShader()
		{
			Shader = LoadShaders("./Shaders/BasicLight.vert", "./Shaders/BasicLight.frag");

			ScreenSpaceVarID = GetUniformVariable(Shader, "ScreenSpaceTransform");

			return;
		}

		void Use(const CoordSpace& _lampTransform)
		{
			UseProgramShader(Shader);

			SetUniformVariable_MVA(ScreenSpaceVarID, 1, false, _lampTransform[0][0]);	

			EnableVertexAttributeArray(0);
			EnableVertexAttributeArray(1);

			return;
		}

		void Stop()
		{
			DisableVertexAttributeArray(0);
			DisableVertexAttributeArray(1);

			return;
		}
	}

	struct Material_Phong
	{
		VecColor Color   ;
		gFloat   Ambience;
		gFloat   Diffuse ;
		gFloat   Specular;
	};

	namespace PhongShader
	{
		ID<ShaderProgram> ShaderID;

		ID<CoordSpace> ModelSpaceID, InverseModelSpaceID, ViewportID, ProjectionID;

		ID<Vec3> LightPositionID, ObjectColorID, LightColorID;

		ID<gFloat> AmbientStrengthID, DiffuseStrengthID, SpecularStrengthID;

		gInt VertexIndexID, NormalIndexID;

		void LoadShader()
		{
			ShaderID = LoadShaders("./Shaders/PhongShader.vert", "./Shaders/PhongShader.frag");

			InverseModelSpaceID = GetUniformVariable(ShaderID, "InverseModelSpace");
			ModelSpaceID        = GetUniformVariable(ShaderID, "ModelSpace"       );
			ProjectionID        = GetUniformVariable(ShaderID, "Projection"       );
			ViewportID          = GetUniformVariable(ShaderID, "Viewport"         );

			ObjectColorID   = GetUniformVariable(ShaderID, "ObjectColor"  );
			LightColorID    = GetUniformVariable(ShaderID, "LightColor"   );
			LightPositionID = GetUniformVariable(ShaderID, "LightPosition");

			AmbientStrengthID  = GetUniformVariable(ShaderID, "AmbientStrength" );
			DiffuseStrengthID  = GetUniformVariable(ShaderID, "DiffuseStrength" );
			SpecularStrengthID = GetUniformVariable(ShaderID, "SpecularStrength");

			return;
		}

		void Use
		(
			const CoordSpace&     _projection     ,
			const CoordSpace&     _viewport       ,
			const CoordSpace&     _objectTransform, 
			const Vector3&        _lightPosition  , 
			const Vector3&        _lightColor     ,
			const Material_Phong& _material
		)
		{
			CoordSpace inverseTransform = Inverse(_viewport * _objectTransform);

			UseProgramShader(ShaderID);

			SetUniformVariable_MVA(InverseModelSpaceID, 1, false, inverseTransform[0][0]);
			SetUniformVariable_MVA(ModelSpaceID       , 1, false, _objectTransform[0][0]);
			SetUniformVariable_MVA(ProjectionID       , 1, false, _projection     [0][0]);
			SetUniformVariable_MVA(ViewportID         , 1, false, _viewport       [0][0]);

			SetUniformVariable_Vector3(LightPositionID, 1, _lightPosition[0]);

			SetUniformVariable_Vector3(ObjectColorID , 1, _material.Color[0]);
			SetUniformVariable_Vector3(LightColorID  , 1, _lightColor    [0]);

			SetUniformVariable_Float(AmbientStrengthID , _material.Ambience);
			SetUniformVariable_Float(DiffuseStrengthID , _material.Diffuse );
			SetUniformVariable_Float(SpecularStrengthID, _material.Specular);

			EnableVertexAttributeArray(0);
			EnableVertexAttributeArray(1);

			return;
		}

		void Stop()
		{
			DisableVertexAttributeArray(1);
			DisableVertexAttributeArray(0);

			return;
		}
	}

	void LoadDefaultShaders()
	{
		Basic_LightShader::LoadShader();
		PhongShader      ::LoadShader();

		return;
	}
}
