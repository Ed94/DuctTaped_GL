#pragma once

// DGL
#include "DGL_Buffers.hpp"
#include "DGL_Space.hpp"

// C++
#include "Cpp_Alias.hpp"



namespace DGL
{
	using std::string;

	using UVList     = std    ::vector < Vector2>;
	using Vec3Int    = Generic::Vector3< gUInt  >;
	using Vec2Int    = Generic::Vector2< gUInt  >;
	using VertexList = std    ::vector < Vector3>;
	using VIndexList = std    ::vector < gInt   >;


	
	sfn Offset(gInt _offsetAmount)
	{
		return ptr<void>(_offsetAmount);
	}

	constexpr sfn ZeroOffset() -> ptr<void>
	{
		return 0;
	}


	
	struct VertexGenerator
	{
		using ComponentList = std::vector< gFloat>;



		ComponentList comp;



		// TODO: De-hard-code this generator to do any vector size.
		sfn GetVector() -> Vector3
		{
			return Vector3(comp.at(0), comp.at(1), comp.at(2));
		}

		sfn GetVector2() -> Vector2
		{
			return Vector2(comp.at(0), comp.at(1));
		}

		void Normalize()
		{
			using std::pow ;
			using std::sqrt;

			using Element = ComponentList::iterator;

			gFloat magnitude = 0.0f;

			for (Element element = comp.begin(); element != comp.end(); element++)
			{
				magnitude += pow(*element, 2.0f);
			}

			magnitude = sqrt(magnitude);

			for (Element element = comp.begin(); element != comp.end(); element++)
			{
				*element /= magnitude;
			}
		}
	};



	struct Face
	{
		Vec3Int Vertexes, Normals;
	};

	using FaceList = std::vector<Face>;



	struct FaceGenerator
	{
		using ComponentList = std::vector< gUInt>;

		ComponentList vertIndexes, uvIndexes, normals;

		sfn AddVertexIndex(gUInt _index)
		{
			vertIndexes.push_back(_index);
		}

		sfn AddUVIndex(gUInt _index)
		{
			uvIndexes.push_back(_index);
		}

		sfn AddNormalIndex(gUInt _index)
		{
			normals.push_back(_index);
		}

		sfn GetFace() -> Face
		{
			Face generated;// = { {0,0,0}, {0,0,0}, {0,0,0} };

			for (int index = 0; index < 3; index++)
			{
				generated.Vertexes[index] = vertIndexes[index];

				if (index < 2)
				{
					if (uvIndexes.size() > 0)
					{
						//generated.UVs[index] = uvIndexes[index];
					}
				}

				if (normals.size() > 0)
				{
					generated.Normals[index] = normals[index];
				}
			}

			if (uvIndexes.size() == 0)
			{
				//generated.UVs = { 0, 0 };
			}

			return generated;
		}
	};



	struct Model
	{
		ID<VertexArray  > VAO;
		ID<VertexBuffer > VBO;
		ID<NormalBuffer > NBO;
		ID<TextureBuffer> TBO;
		ID<ElementBuffer> EBO;

		const string FilePath;

		VertexList Verticies  ;
		VertexList VertNormals;

		VertexList RAWVertex     ;
		VertexList VertNormalsRAW;

		UVList     TextureMap ;
		FaceList   Faces      ;
		VIndexList Indicies   ; 



		Model(const Ref(string) _filePath) :
			VAO(-1),
			VBO(-1),
			NBO(-1),
			EBO(-1),
			FilePath   (_filePath   ),
			Verticies  (VertexList()),
			VertNormals(VertexList()),
			TextureMap (UVList    ()),
			Faces      (FaceList  ())
		{}

		// TODO: Add support for textures...
		sfn Load()
		{
			using std::cout        ;
			using std::endl        ;
			using std::get         ;
			using std::getline     ;
			using std::ifstream    ;
			using std::ios         ;
			using std::stringstream;
			using std::ws          ;



			ifstream fileBuffer; fileBuffer.open(FilePath);



			deduce processVertex = [&](Ref(stringstream) _vertexStream)
			{
				VertexGenerator vertex; gFloat componentValue;

				while (not _vertexStream.eof())
				{
					_vertexStream >> componentValue >> ws;

					vertex.comp.push_back(componentValue);
				}

				Verticies.push_back(vertex.GetVector());
			};

			deduce processNormals = [&](Ref(stringstream) _normalStream)
			{
				VertexGenerator normal; gFloat componentValue;

				while (not _normalStream.eof())
				{
					_normalStream >> componentValue >> ws;

					normal.comp.push_back(componentValue);
				}

				normal.Normalize();

				VertNormals.push_back(normal.GetVector());
			};

			deduce processTexture = [&](Ref(stringstream) _normalStream)
			{
				VertexGenerator texture; gFloat componentValue;

				while (not _normalStream.eof())
				{
					_normalStream >> componentValue >> ws;

					texture.comp.push_back(componentValue);
				}

				TextureMap.push_back(texture.GetVector2());
			};

			deduce processFace = [&](Ref(stringstream) _faceStream)
			{
				FaceGenerator faceMade; gUInt vertexIndex, textureIndex, normalIndex;

				while (not _faceStream.eof())
				{
					_faceStream >> vertexIndex >> ws;

					faceMade.AddVertexIndex(vertexIndex - 1);

					Indicies.push_back(vertexIndex - 1);

					if (_faceStream.peek() == '/')
					{
						_faceStream.get();

						if (_faceStream.peek() == '/')
						{
							_faceStream.get();

							_faceStream >> normalIndex >> ws;

							faceMade.AddNormalIndex(normalIndex -1);

							Indicies.push_back(normalIndex - 1);
						}
						else
						{
							_faceStream >> textureIndex >> ws;

							faceMade.AddUVIndex(textureIndex - 1);

							if (_faceStream.peek() == '/')
							{
								_faceStream.get();

								_faceStream >> normalIndex >> ws;

								faceMade.AddNormalIndex(normalIndex - 1);

								Indicies.push_back(normalIndex - 1);
							}
						}
					}
				}

				Faces.push_back(faceMade.GetFace());
			};



			if (fileBuffer.is_open())
			{
				stringstream stringBuffer;

				stringBuffer << fileBuffer.rdbuf();

				string line;

				while (not stringBuffer.eof())
				{
					getline(stringBuffer, line);

					stringstream lineStream(line);

					string lineSig;

					lineStream >> lineSig >> ws;

					if (lineSig == "v")
					{
						processVertex(lineStream);
					}
					else if (lineSig == "vn")
					{
						processNormals(lineStream);
					}
					else if (lineSig == "vt")
					{
						processTexture(lineStream);
					}
					else if (lineSig == "f")
					{
						processFace(lineStream);
					}
				}

				return;
			}
			else
			{
				throw std::runtime_error("Could not open file to load model.");
			}

			fileBuffer.close();
		}



		

		sfn GenerateNormals()
		{
			VertNormals.resize(Verticies.size());

			for (int index = 0; index < Faces.size(); index++)
			{
				int vertexIndex1 = Faces[index].Vertexes[0], 
					vertexIndex2 = Faces[index].Vertexes[1],
					vertexIndex3 = Faces[index].Vertexes[2];

				Vector3 edge1 = Verticies[vertexIndex2] - Verticies[vertexIndex1],
					    edge2 = Verticies[vertexIndex3] - Verticies[vertexIndex1],
					    
					normal = GetDirection(GetCrossNormal(edge1, edge2));

				Faces[index].Normals[0] = vertexIndex1;
				Faces[index].Normals[1] = vertexIndex2;
				Faces[index].Normals[2] = vertexIndex3;

				VertNormals[vertexIndex1] = normal;
				VertNormals[vertexIndex2] = normal;
				VertNormals[vertexIndex3] = normal;
			}
		}


		template<typename Type>
		sfn RoundOff(Type _value, gInt _numDigitsToKeep) -> Type
		{
			uInt64 Rounder = pow(10, _numDigitsToKeep);

			return round(_value * Rounder) / Rounder;
		}



		// Hardcoded to only do the verticies and normals for now...
		sfn Buffer()
		{
			GenerateVertexBuffers(Address(VAO), 1);
			GenerateBuffers      (Address(VBO), 1);
			GenerateBuffers      (Address(NBO), 1);
			//glGenTextures        (1, Address(TBO));
			GenerateBuffers      (Address(EBO), 1);


			if (VertNormals.size() == 0)
			{
				GenerateNormals();
			}

			
			BindVertexArray(VAO);

			BindBuffer(EBufferTarget::VertexAttributes, VBO);

			BufferData(Address(Verticies[0]), Verticies.size() * sizeof(Vector3), EBufferTarget::VertexAttributes, EBufferUsage::StaticDraw);

			FormatVertexAttributes<Vector3>(0, EDataType::Float, ZeroOffset(), 3, EBool::False);

			EnableVertexAttributeArray(0);



			if (VertNormals.size() != 0)
			{
				BindBuffer(EBufferTarget::VertexAttributes, NBO);

				BufferData(Address(VertNormals[0]), VertNormals.size() * sizeof(Vector3), EBufferTarget::VertexAttributes, EBufferUsage::StaticDraw);

				FormatVertexAttributes<Vector3>(1, EDataType::Float, ZeroOffset(), 3, EBool::False);

				EnableVertexAttributeArray(1);
			}



			/*if (TextureMap.size() != 0)
			{
				glBindTexture(TBO, GL_TEXTURE_2D);

				BufferData(Address(TextureMap[0]), TextureMap.size() * sizeof(Vector2), EBufferTarget::TextureData, EBufferUsage::StaticDraw);

				FormatVertexAttributes<Vector2>(2, EDataType::Float, ZeroOffset(), 2, EBool::False);

				EnableVertexAttributeArray(2);
			}*/



			BindBuffer(EBufferTarget::VertexIndices, EBO);

			BufferData(Address(Faces[0]), Faces.size() * sizeof(Face), EBufferTarget::VertexIndices, EBufferUsage::StaticDraw);
			
			BindVertexArray(0);
		}

		sfn Render()
		{
			BindVertexArray(VAO);

			
			gInt Size; GetBufferParameterIV(EBufferTarget::VertexIndices, EBufferParam::Size, Address(Size));

			Size /= sizeof(gUInt);


			DrawElements(EPrimitives::Triangles, Size, EDataType::UnsignedInt, ZeroOffset());

 			BindVertexArray(0);
		}
	};
};
