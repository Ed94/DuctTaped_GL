#pragma once


// DGL
#include "DGL_Types.hpp"
#include "DGL_Model.hpp"
#include "DGL_Shader.hpp"
#include "DGL_Utilities.hpp"


// Non-Standard C++
#include "Cpp_Alias.hpp"


namespace DGL
{
	namespace Colors
	{
		LinearColor CoralColor(1.0f , 0.5f , 0.31f, 1.0f);
		LinearColor GreyColor (0.60f, 0.60f, 0.60f, 1.0f);
		LinearColor LightColor(1.0f , 1.0f , 1.0f , 1.0f);
	}

	
	class Light_Basic
	{
	public:
		Light_Basic() :
			color            (Colors::LightColor.Vector()),
			position         (Vector3   (0.0f)           ),
			scale            (Vector3   (0.2f)           ),
			transform        (CoordSpace(1.0f)           ),
			translationRadius(4.0f                       )

		{}

		sfn GetColor() -> VecColor
		{
			return color;
		}

		sfn GetPosition() -> Vector3
		{
			return position;
		}

		sfn Load()
		{
			GenerateVertexBuffers(modelVAO, 1);
			GenerateBuffers      (modelVBO, 1);
			GenerateBuffers      (modelEBO, 1);

			BindVertexArray(modelVAO);

			BindBuffer(EBufferTarget::VertexAttributes, modelVBO);

			BufferData<CubeVerts>(DefaultCube, EBufferTarget::VertexAttributes, EBufferUsage::StaticDraw);


			BindBuffer(EBufferTarget::VertexIndices, modelEBO);

			BufferData<CubeElements>(DefaultCubeElements, EBufferTarget::VertexIndices, EBufferUsage::StaticDraw);


			FormatVertexAttributes<CubeVerts::Vertex3>(0, EDataType::Float, ZeroOffset(), CubeVerts::Vertex3::ValueCount(), false);

			UnbindVertexArray();
		}

		sfn Update()
		{
			transform = CoordSpace(1.0f);

			position.x = translationRadius * sin(GetTime());
			position.z = translationRadius * cos(GetTime());

			transform = Translate(transform, position);
			transform = Scale    (transform, scale   );
		}

		sfn Render(ro Ref(CoordSpace) _projection, ro Ref(CoordSpace) _viewport)
		{
			deduce screenspaceTransform = _projection * _viewport * transform;

			Basic_LampShader::Use(screenspaceTransform);

			BindVertexArray(modelVAO);

			BindBuffer(EBufferTarget::VertexIndices, modelEBO);

			gInt SizeRef; GetBufferParameterIV(EBufferTarget::VertexIndices, DGL::EBufferParam::Size, SizeRef); SizeRef /= sizeof(unsigned int);

			DrawElements(EPrimitives::Triangles, SizeRef, EDataType::UnsignedInt, ZeroOffset());

			Basic_LampShader::Stop();
		}

	private:

		VecColor   color            ;
		Vector3    position         ;
		Vector3    scale            ;
		CoordSpace transform        ;
		gFloat     translationRadius;


		// Hardcoded Model

		struct CubeVerts
		{
			struct Vertex3
			{
				gFloat x, y, z;

				static constexpr sfn ValueCount() -> gSize { return 3; }
			};

			Vertex3
				f1, f2, f3, f4,   // Front
				b1, b2, b3, b4;   // Back
		} 
		
		DefaultCube =
		{
			// Front
			{-1.0f, -1.0f,  1.0f},
			{ 1.0f, -1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f},

			// Back
			{-1.0f, -1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f}
		};

		struct CubeElements
		{
			struct Edge3
			{
				struct TriIndex
				{
					gInt a, b, c;
				};

				TriIndex a, b;
			};

			Edge3 front, right, back, left, bottom, top;
		}
		
		DefaultCubeElements =
		{
			// Front
			{ { 0, 1, 2 }, { 2, 3, 0 } },

			// Right
			{ { 1, 5, 6 }, { 6, 2, 1 } },

			// Back
			{ { 7, 6, 5 }, { 5, 4, 7 } },

			// Left
			{ { 4, 0, 3 }, { 3, 7, 4 } },

			// Bottom
			{ { 4, 5, 1 }, { 1, 0, 4 } },

			// Top
			{ { 3, 2, 6 }, { 6, 7, 3 } }
		};

		ID<VertexArray  > modelVAO;
		ID<VertexBuffer > modelVBO;
		ID<ElementBuffer> modelEBO;
	};


	class Entity_Basic
	{

	public:
		Entity_Basic() : 
			position (Vector3(0.0f)   ),
			model    (""              ),
			transform(CoordSpace(1.0f))
		{};

		Entity_Basic(Ref(Model) _model, Ref(Material_Phong) _material) :
			position (Vector3(0.0f)   ),
			model    (_model          ),
			transform(CoordSpace(1.0f)),
			material (_material       )
			//type     (_type        )
		{};

		sfn Update()
		{
			
		}

		sfn Render(ro Ref(CoordSpace) _projection, ro Ref(CoordSpace) _viewport, ro Ref(Vector3) _lightPosition, ro Ref(VecColor) _lightColor)
		{
			PhongShader::Use(_projection, _viewport, transform, _lightPosition,_lightColor, material);

			model.Render();

			PhongShader::Stop();

			return;
		}

		sfn operator= (ro Ref(Entity_Basic) _entity)->Ref(Entity_Basic)
		{
			position  = _entity.position ;
			model     = _entity.model    ;
			transform = _entity.transform;
			material  = _entity.material ;

			return Dref(this);
		}

	private:

		//EEntityType type;

		Vector3        position     ;
		Model          model        ;
		CoordSpace     transform    ;
		Material_Phong material     ;
	};
}
