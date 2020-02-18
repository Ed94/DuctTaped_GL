#pragma once

#include "DGL_Buffers.hpp"
#include "DGL_Space.hpp"

#include "Cpp_Alias.hpp"



namespace DGL
{
	sfn Offset(gInt _offsetAmount)
	{
		return ptr<void>(_offsetAmount);
	}

	constexpr sfn ZeroOffset() -> ptr<void>
	{
		return 0;
	}

	using std::string;

	using VertexList = std    ::vector < Vector3>;
	using UVList     = std    ::vector < Vector2>;
	using VecInt     = Generic::Vector3< gUInt  >;
	using VIndexList = std    ::vector < gInt   >;

	


	struct VertexGenerator
	{
		using ComponentList = std::vector< gFloat>;

		ComponentList comp;

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


		// TODO: De-hard-code this generator to do any vector size.
		sfn GetVector() -> Vector3
		{
			return Vector3(comp.at(0), comp.at(1), comp.at(2));
		}
	};

	struct Face
	{
		VecInt Vertexes,
			Normals;
		    	//UVs[3];
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

				if (uvIndexes.size() > 0)
				{
					//generated.UVs[index] = uvIndexes[index];
				}

				if (normals.size() > 0)
				{
					generated.Normals[index] = normals[index];
				}
			}

			return generated;
		}
	};

	struct Model
	{
		
		struct VN
		{
			Vector3 Vertex, Normal;

			VN(Vector3 _v, Vector3 _n) : Vertex(_v), Normal(_n) {};
		};

		using VNList = std::vector<VN>;

		ID<VertexArray  > VAO;
		ID<VertexBuffer > VBO;
		ID<NormalBuffer > NBO;
		ID<ElementBuffer> EBO;

		const string FilePath;

		VertexList Verticies        ;
		VertexList VertNormals      ;

		VertexList RAWVertex  ;
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
			using std::ifstream    ;
			using std::ios         ;
			using std::stringstream;
			using std::getline;
			using std::ws;
			using std::get;


			ifstream fileBuffer;

			fileBuffer.open(FilePath);

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

		template<typename Type>
		sfn RoundOff(Type _value, gInt _numDigitsToKeep) -> Type
		{
			uInt64 Rounder = pow(10, _numDigitsToKeep);

			return round(_value * Rounder) / Rounder;
		}

		sfn GenerateNormals()
		{
			for (int index = 0; index < Faces.size(); index++)
			{
				int vertexIndex1 = Faces[index].Vertexes[0], 
					vertexIndex2 = Faces[index].Vertexes[1],
					vertexIndex3 = Faces[index].Vertexes[2];

				Vector3 edge1 = Verticies[vertexIndex2] - Verticies[vertexIndex1],
					    edge2 = Verticies[vertexIndex3] - Verticies[vertexIndex2],
					    
					normal = GetDirection(GetCrossNormal(edge2, edge1));


				normal[0] = normal[0];  // RoundOff(normal[0], 7);
				normal[1] = normal[1];  // RoundOff(normal[1], 7);
				normal[2] = normal[1];  // RoundOff(normal[2], 7);


				bool normalExists = false;

				for (int normIndex = 0; normIndex < VertNormals.size(); normIndex++)
				{ 
					if (normal == VertNormals[normIndex])
					{
						normalExists = true;
					}
				}


				if (!normalExists)
				{
					VertNormals.push_back(normal);
				}
			}
		}

		// Hardcoded to only do the verticies and normals for now...
		sfn Buffer()
		{
			GenerateVertexBuffers(Address(VAO), 1);
			GenerateBuffers      (Address(VBO), 1);
			GenerateBuffers      (Address(NBO), 1);
			GenerateBuffers      (Address(EBO), 1);


			/*if (VertNormals.size() == 0)
			{
				GenerateNormals();
			}*/

			/*for (int index = 0; index < Faces.size(); index++)
			{
				cout << "FaceID: " << index << endl << endl;

				for (int vertIndex = 0; vertIndex < 3; vertIndex++)
				{
					cout << "Vert Index: " << Faces[index].Vertexes[vertIndex] << endl;
					cout << "Face Index: " << Faces[index].Normals[vertIndex] << endl;

					RAWVertex.push_back
					(
						Verticies[Faces[index].Vertexes[vertIndex]]
					);

					RAWVertex.push_back
					(
						VertNormals[Faces[index].Normals[vertIndex]]
					);
				}
			}
			

			BindVertexArray(VAO);

			BindBuffer(EBufferTarget::VertexAttributes, VBO);

			BufferData(Address(RAWVertex[0]), RAWVertex.size() * sizeof(Vector3), EBufferTarget::VertexAttributes, EBufferUsage::StaticDraw);

			FormatVertexAttributes<Vector3>(0, EDataType::Float, ZeroOffset(), 3, EBool::False);

			EnableVertexAttributeArray(0);

			FormatVertexAttributes<Vector3>(1, EDataType::Float, Offset(sizeof(Vector3)), 3, EBool::False);

			EnableVertexAttributeArray(1);*/
			
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


			BindBuffer(EBufferTarget::VertexIndices, EBO);

			BufferData(Address(Faces[0]), Faces.size() * sizeof(Face), EBufferTarget::VertexIndices, EBufferUsage::StaticDraw);
			
			BindVertexArray(0);
		}

		sfn Render()
		{
			BindVertexArray(VAO);

			BindBuffer(EBufferTarget::VertexAttributes, VBO);

			gInt Size; GetBufferParameterIV(EBufferTarget::VertexIndices, EBufferParam::Size, Address(Size));

			Size /= sizeof(gInt);

			//DrawArrays(EPrimitives::Triangles, 0, RAWVertex.size());

			DrawElements(EPrimitives::Triangles, Size, EDataType::UnsignedInt, ZeroOffset());

 			BindVertexArray(0);
		}
	};
};
