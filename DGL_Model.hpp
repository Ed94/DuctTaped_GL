#pragma once

// DGL
#include "DGL_Buffers.hpp"
#include "DGL_Space.hpp"
#include "DGL_Types.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace DGL
{
	struct VertexGenerator
	{
		using ComponentList = std::vector< gFloat>;

	public:

		sfn AddComponent(gFloat _float)
		{
			comp.push_back(_float);
		}

		// TODO: De-hard-code this generator to do any vector size.
		sfn GetVector2() -> Vector2
		{
			return Vector2(comp.at(0), comp.at(1));
		}

		sfn GetVector3() -> Vector3
		{
			return Vector3(comp.at(0), comp.at(1), comp.at(2));
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


	private:
		ComponentList comp;
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


	// TODO: Add support for textures...
	class Model
	{
	private:
		// Not the most efficient method to do normals, but works.
		sfn generateNormals() -> void
		{
			normals.resize(verticies.size(), Vector3(0.0f));

			for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
			{
				int vertexIndex1 = faces[faceIndex].Vertexes[0],
					vertexIndex2 = faces[faceIndex].Vertexes[1],
					vertexIndex3 = faces[faceIndex].Vertexes[2];

				Vector3 edge1 = verticies[vertexIndex2] - verticies[vertexIndex1],
					    edge2 = verticies[vertexIndex3] - verticies[vertexIndex1],

					normal = GetDirection(GetCrossNormal(edge1, edge2));

				faces[faceIndex].Normals[0] = vertexIndex1;
				faces[faceIndex].Normals[1] = vertexIndex2;
				faces[faceIndex].Normals[2] = vertexIndex3;

				normals[vertexIndex1] = normal;
				normals[vertexIndex2] = normal;
				normals[vertexIndex3] = normal;
			}
		}

	public:

		Model(ro Ref(string) _filePath) :
			vertexArrayID  (0           ),    
			vertexBufferID (0           ),
			normalBuffferID(0           ),
			textureBufferID(0           ),
			elementBufferID(0           ),
			filePath       (_filePath   ),
			verticies      (VertexList()),
			normals        (VertexList()),
			textureUVs     (UVList    ()),
			faces          (FaceList  ())
		{}


		// Hardcoded to only do the verticies and normals for now...
		sfn Buffer()
		{
			GenerateVertexBuffers(vertexArrayID  , 1);
			GenerateBuffers      (vertexBufferID , 1);
			GenerateBuffers      (normalBuffferID, 1);
			GenerateBuffers      (elementBufferID, 1);


			if (normals.size() == 0)
			{
				generateNormals();
			}


			BindVertexArray(vertexArrayID);


			// Vertex Position Buffering

			BindBuffer(EBufferTarget::VertexAttributes, vertexBufferID);

			BufferData(verticies[0], gSize(verticies.size() * sizeof(Vector3)), EBufferTarget::VertexAttributes, EBufferUsage::StaticDraw);

			FormatVertexAttributes<Vector3>(0, EDataType::Float, ZeroOffset(), 3, false);

			EnableVertexAttributeArray(0);


			// Normal Buffering

			if (normals.size() != 0)
			{
				BindBuffer(EBufferTarget::VertexAttributes, normalBuffferID);

				BufferData(normals[0], gSize(normals.size() * sizeof(Vector3)), EBufferTarget::VertexAttributes, EBufferUsage::StaticDraw);

				FormatVertexAttributes<Vector3>(1, EDataType::Float, ZeroOffset(), 3, false);

				EnableVertexAttributeArray(1);
			}


			// Texture buffering 
		    // TODO: Fix this.

			if (textureUVs.size() != 0)
			{
				//glBindTexture(TBO, GL_TEXTURE_2D);

				//BufferData(Address(TextureMap[0]), TextureMap.size() * sizeof(Vector2), EBufferTarget::TextureData, EBufferUsage::StaticDraw);

				//FormatVertexAttributes<Vector2>(2, EDataType::Float, ZeroOffset(), 2, EBool::False);

				//EnableVertexAttributeArray(2);
			}



			BindBuffer(EBufferTarget::VertexIndices, elementBufferID);

			BufferData(faces[0], gSize(faces.size() * sizeof(Face)), EBufferTarget::VertexIndices, EBufferUsage::StaticDraw);



			UnbindVertexArray();   // Unbind vertex array.
		}

		sfn Load()
		{
			using std::get         ;
			using std::getline     ;
			using std::ifstream    ;
			using std::ios         ;
			using std::stringstream;
			using std::ws          ;



			ifstream fileBuffer; fileBuffer.open(filePath);



			deduce processVertex = [&](Ref(stringstream) _vertexStream)
			{
				VertexGenerator vertex; gFloat componentValue;

				while (not _vertexStream.eof())
				{
					_vertexStream >> componentValue >> ws;

					vertex.AddComponent(componentValue);
				}

				verticies.push_back(vertex.GetVector3());
			};

			deduce processNormals = [&](Ref(stringstream) _normalStream)
			{
				VertexGenerator normal; gFloat componentValue;

				while (not _normalStream.eof())
				{
					_normalStream >> componentValue >> ws;

					normal.AddComponent(componentValue);
				}

				normal.Normalize();

				normals.push_back(normal.GetVector3());
			};

			deduce processTexture = [&](Ref(stringstream) _normalStream)
			{
				VertexGenerator texture; gFloat componentValue;

				while (not _normalStream.eof())
				{
					_normalStream >> componentValue >> ws;

					texture.AddComponent(componentValue);
				}

				textureUVs.push_back(texture.GetVector2());
			};

			deduce processFace = [&](Ref(stringstream) _faceStream)
			{
				FaceGenerator faceMade; gUInt vertexIndex, textureIndex, normalIndex;

				while (not _faceStream.eof())
				{
					_faceStream >> vertexIndex >> ws;

					faceMade.AddVertexIndex(vertexIndex - 1);

					indicies.push_back(vertexIndex - 1);

					if (_faceStream.peek() == '/')
					{
						_faceStream.get();

						if (_faceStream.peek() == '/')
						{
							_faceStream.get();

							_faceStream >> normalIndex >> ws;

							faceMade.AddNormalIndex(normalIndex -1);

							indicies.push_back(normalIndex - 1);
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

								indicies.push_back(normalIndex - 1);
							}
						}
					}
				}

				faces.push_back(faceMade.GetFace());
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

				fileBuffer.close();

				return;
			}
			else
			{
				throw std::runtime_error("Could not open file to load model.");
			}
		}

		sfn Render()
		{
			BindVertexArray(vertexArrayID);

			
			gInt SizeRef; GetBufferParameterIV(EBufferTarget::VertexIndices, EBufferParam::Size, SizeRef); SizeRef /= sizeof(gUInt);


			DrawElements(EPrimitives::Triangles, SizeRef, EDataType::UnsignedInt, ZeroOffset());


 			UnbindVertexArray();
		}

		sfn operator= (ro Ref(Model) _model) -> Ref(Model)
		{
			vertexArrayID   = _model.vertexArrayID  ;
			vertexBufferID  = _model.vertexBufferID ;
			normalBuffferID = _model.normalBuffferID;
			textureBufferID = _model.textureBufferID;
			elementBufferID = _model.elementBufferID;

			filePath = _model.filePath;

			verticies  = _model.verticies ;
			normals    = _model.normals   ;
			textureUVs = _model.textureUVs;
			faces      = _model.faces     ;
			indicies   = _model.indicies  ;

			return Dref(this);
		}

		private:

			// Components

			ID<VertexArray  > vertexArrayID  ;
			ID<VertexBuffer > vertexBufferID ;
			ID<NormalBuffer > normalBuffferID;
			ID<TextureBuffer> textureBufferID;
			ID<ElementBuffer> elementBufferID;

			string filePath;

			VertexList verticies ;
			VertexList normals   ;
			UVList     textureUVs;
			FaceList   faces     ;
			VIndexList indicies  ; 
	};
};
