#pragma once

#include "DGL.hpp"

#include "Cpp_Alias.hpp"



using DGL::gFloat;
using DGL::VertexArray;
using DGL::EBufferTarget;
using DGL::EBufferUsage;
using DGL::VertexBuffer;
using DGL::ID;
using DGL::gInt;
using DGL::gSize;
using DGL::LinearColor;
using DGL::Vector3;
using DGL::ZeroOffset;


float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};




//DGL::DefaultSpace::WorldSpace = DGL::Rotate(DGL::DefaultSpace::WorldSpace, 0.015f, Vector3(0.0f, 1.0f, 0.0f));

// This will identify our vertex buffer
ID<VertexBuffer> VertexBufferObj;



struct Vertex3
{
	gFloat x, y, z;

	static constexpr sfn ValueCount() -> gSize { return 3; }
};

struct TriangleRaw
{
	Vertex3 a, b, c;

	static constexpr sfn VertexCount() -> gSize { return 3; }
};


TriangleRaw EquilateralTriangleVerticies = 
{ 
	{ -1.0f, -1.0f, 0.0f },   // Vert1
	{  1.0f, -1.0f, 0.0f },   // Vert2
	{  0.0f,  1.0f, 0.0f }    // Vert 3 
};


struct RectangleRaw
{
	TriangleRaw first, second;

	static constexpr sfn VertexCount() -> gSize { return 6;  }
};


RectangleRaw SquareVerticies =
{
	{
		{ -0.5f, -0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f },
		{  0.0f,  0.5f, 0.0f }
	},
	{
		{   0.5f, -0.5f, 0.0f },
		{  -0.5f, -0.5f, 0.0f },
		{  -0.5f,  0.5f, 0.0f }
	}
};

ID<VertexArray> VertexArrayObj;

struct RectangleCompressed
{
	Vertex3 a, b, c, d;

	static constexpr sfn VertexCount() -> gSize { return 4; }
};

struct TriIndex
{
	gInt a, b, c;
};

struct RectangleIndices
{
	TriIndex first, second;
};


RectangleCompressed rectCompress =
{
	{  1.0f,  1.0f, 0.0f },
	{  1.0f, -1.0f, 0.0f },
	{ -1.0f, -1.0f, 0.0f },
	{ -1.0f,  1.0f, 0.0f }
};

RectangleIndices rectIndices =
{
	{ 0, 1, 3 },
	{ 1, 2, 3 }
};

using DGL::ElementBuffer;

ID<ElementBuffer> ElemBufferObj;

struct Vertex2
{
	gFloat x, y;
};

struct TriTexCoords
{
	Vertex2 a, b, c;
};


TriTexCoords textureCoords =
{
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 0.5f, 1.0f }
};


sfn RAW_SetupBuffers()
{
	DGL::GenerateVertexBuffers(Address(VertexArrayObj ), 1);
	DGL::GenerateBuffers      (Address(VertexBufferObj), 1);
	DGL::GenerateBuffers      (Address(ElemBufferObj  ), 1);
}

sfn RAW_SetupTriangleBuffer()
{
	DGL::GenerateBuffers(Address(VertexBufferObj), 1);
}

sfn RAW_BindAndBufferDataToIDs()
{
	DGL::BindVertexArray(VertexArrayObj);

	DGL::BindBuffer(EBufferTarget::VertexAttributes, VertexBufferObj);

	//GL::BufferData<TriangleRaw>(Address(EquilateralTriangleVerticies), EBufferTarget::VertexAttributes, EBufferUsage::StaticDraw);

	DGL::BufferData<RectangleCompressed>(Address(rectCompress), EBufferTarget::VertexAttributes, EBufferUsage::StaticDraw);
	
	DGL::BindBuffer(EBufferTarget::VertexIndices, ElemBufferObj);

	DGL::BufferData<RectangleIndices>(Address(rectIndices), EBufferTarget::VertexIndices, EBufferUsage::StaticDraw);
}

DGL::gInt VertexAttributeIndex = 0;   // See shader source: (layout = 0).

using DGL::EBool;
using DGL::EDataType;



// Testing

	// Forward Declares

sfn RenderProcedure     () -> void;
sfn PrepareRenderObjects() -> void;

sfn CreateWindow_BasicLoop()
{
	DGL::InitalizeGLFW();

	deduce windowObj = DGL::CreateWindow(720, 540, "Assignment 1: RawLoop", DGL::WindowedMode(), DGL::NotShared());

	DGL::SetCurrentContext(windowObj);

	DGL::RunBasicWindowLoop(windowObj);
}

sfn CreateWindow_TimedRender()
{
	DGL::InitalizeGLFW();

	deduce windowObj = DGL::CreateWindow(720, 540, "Assignment 1: Timed Render", DGL::WindowedMode(), DGL::NotShared());

	DGL::SetCurrentContext(windowObj);

	DGL::InitalizeGLEW();

	//PrepareRenderObjects();

	//DGL::RunBasicWindowLoop_Timed(windowObj, 1.0 / 60.0, Address(RenderProcedure));
}



struct Edge3
{
	TriIndex a, b;
};

struct VertPhong
{
	Vertex3 locationPad,
		    normalPad   ;
};

struct CubeVerts
{
	Vertex3 
		f1, f2, f3, f4,   // Front
		b1, b2, b3, b4;   // Back
};

struct CubeElements
{
	Edge3 front, right, back, left, bottom, top;
};

CubeVerts DefaultCube =
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

CubeElements DefaultCubeElements =
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


using DGL::CoordSpace;
using DGL::Matrix4x4;
using DGL::Model;

ID<VertexArray> CubeVAO;
ID<VertexBuffer > CubeModelBuffer  ;
ID<ElementBuffer> CubeModelElements;






sfn RAW_MakeCube()
{
	DGL::GenerateVertexBuffers(Address(CubeVAO          ), 1);
	DGL::GenerateBuffers      (Address(CubeModelBuffer  ), 1);
	DGL::GenerateBuffers      (Address(CubeModelElements), 1);

	DGL::BindVertexArray(CubeVAO);


	DGL::BindBuffer(EBufferTarget::VertexAttributes, CubeModelBuffer);

	DGL::BufferData<CubeVerts>(Address(DefaultCube), EBufferTarget::VertexAttributes, EBufferUsage::StaticDraw);


	DGL::BindBuffer(EBufferTarget::VertexIndices, CubeModelElements);

	DGL::BufferData<CubeElements>(Address(DefaultCubeElements), EBufferTarget::VertexIndices, EBufferUsage::StaticDraw);



	DGL::FormatVertexAttributes<Vertex3>(0, EDataType::Float, ZeroOffset(), Vertex3::ValueCount(), EBool::False);

	DGL::EnableVertexAttributeArray(0);

	//DGL::FormatVertexAttributes<VertPhong>(1, EDataType::Float, Offset(Vertex3::ValueCount()), Vertex3::ValueCount(), EBool::False);

	//DGL::EnableVertexAttributeArray(1);

	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);*/

	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/


	//DGL::BindBuffer(EBufferTarget::VertexAttributes, 0);   // Dunno. Prob unbinding...
}

sfn RAW_RenderCube()
{
	DGL::BindBuffer(EBufferTarget::VertexIndices, CubeModelElements);

	gInt Size; GetBufferParameterIV(EBufferTarget::VertexIndices, DGL::EBufferParam::Size, Address(Size));

	Size /= sizeof(unsigned int);

	DGL::DrawElements(DGL::EPrimitives::Triangles, Size, EDataType::UnsignedInt, ZeroOffset());
}


LinearColor CoralColor(1.0f, 0.5f, 0.31f, 1.0f);
LinearColor LightColor(1.0f, 1.0f, 1.0f , 1.0f);

Vector3 LightPosition(1.2f, 1.0f, 2.0f);

Vector3 LightScale = Vector3(0.2f);

Vector3 result = LightColor.Vector() * CoralColor.Vector();

CoordSpace LightTransform = Matrix4x4(1.0f);

ID<VertexArray> LightVAO;

sfn RAW_MakeLightVAO()
{
	DGL::GenerateVertexBuffers(Address(LightVAO), 1);

	DGL::BindVertexArray(LightVAO);

	DGL::BindBuffer(EBufferTarget::VertexAttributes, CubeModelBuffer  );
	DGL::BindBuffer(EBufferTarget::VertexIndices   , CubeModelElements);

	DGL::FormatVertexAttributes<Vertex3>(0, EDataType::Float, ZeroOffset(), Vertex3::ValueCount(), EBool::False);

	DGL::EnableVertexAttributeArray(0);

	LightTransform = DGL::Translate(LightTransform, LightPosition);
	LightTransform = DGL::Scale    (LightTransform, LightScale   );
}


using DGL::GetBufferParameterIV;

sfn RAW_RenderLight(CoordSpace _projection, CoordSpace _viewport)
{
	deduce screenspaceTransform = _projection * _viewport * LightTransform;

	DGL::Basic_LampShader::SetupLampRender(screenspaceTransform);

	DGL::BindVertexArray(LightVAO);

	RAW_RenderCube();
}



Vector3 LitCubePosition = Vector3(0.0f);

Vector3 CubeColor = CoralColor.Vector();

CoordSpace LitCubeTransform = Matrix4x4(1.0f);

gFloat RotationRate = 0.015f;

ID<VertexArray> LitCubeVAO;

sfn RAW_MakeLitCube()
{
	DGL::GenerateVertexBuffers(Address(LitCubeVAO), 1);

	DGL::BindVertexArray(LitCubeVAO);

	DGL::BindBuffer(EBufferTarget::VertexAttributes, CubeModelBuffer  );
	DGL::BindBuffer(EBufferTarget::VertexIndices   , CubeModelElements);

	DGL::FormatVertexAttributes<Vertex3>(0, EDataType::Float, ZeroOffset(), Vertex3::ValueCount(), EBool::False);

	DGL::EnableVertexAttributeArray(0);
}

sfn RAW_RotateLitCube(gFloat _delta)
{
	LitCubeTransform = DGL::Rotate(LitCubeTransform, RotationRate * _delta, Vector3(0.0f, 1.0f, 0.0f));
}

sfn RAW_RenderLitCube(CoordSpace _projection, CoordSpace _viewport)
{
	CoordSpace screenspaceTransform = _projection * _viewport * LitCubeTransform;

	Vector3 lightColor = LightColor.Vector();

	DGL::PhongShader::SetupRender(screenspaceTransform, LitCubeTransform, CubeColor, LightPosition, lightColor);

	DGL::BindVertexArray(LitCubeVAO);

	RAW_RenderCube();
}



namespace ProperCube
{
	Model model("cube.obj");

	Vector3 position = Vector3(0.0f);

	Vector3 color = CoralColor.Vector();

	CoordSpace transform = Matrix4x4(1.0f);

	sfn Rotate(gFloat _delta)
	{
		transform = DGL::Rotate(transform, 3.5f * _delta, Vector3(0, 1, 0));
	}

	sfn Render(Ref(CoordSpace) _projection, Ref(CoordSpace) _viewport)
	{
		CoordSpace screenspaceTransform = _projection * _viewport * transform;

		Vector3 lightColor = LightColor.Vector();

		//DGL::PhongShader::SetupRender(screenspaceTransform, transform, color, LightPosition, lightColor);

		DGL::Basic_LightingShader::SetupRender(screenspaceTransform, color, lightColor);

		model.Render();
	}

	sfn Setup()
	{
		model.Load();

		//model.GenVN();

		model.Buffer();

		//transform = DGL::Scale(transform, Vector3(0.01));
	}
}






