#pragma once

#include "DGL.hpp"

#include "Cpp_Alias.hpp"



using DGL::gFloat;
using DGL::VertexBuffer;
using DGL::EBufferTarget;
using DGL::EBufferUsage;
using DGL::Buffer;
using DGL::ID;
using DGL::gInt;
using DGL::gSize;



//DGL::DefaultSpace::WorldSpace = DGL::Rotate(DGL::DefaultSpace::WorldSpace, 0.015f, Vector3(0.0f, 1.0f, 0.0f));

// This will identify our vertex buffer
ID<Buffer> VertexBufferObj;

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

ID<VertexBuffer> VertexArrayObj;

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

constexpr sfn ZeroOffset() -> ptr<void>
{
	return 0;
}

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

	PrepareRenderObjects();

	//DGL::RunBasicWindowLoop_Timed(windowObj, 1.0 / 60.0, Address(RenderProcedure));
}

sfn PrepareRenderObjects() -> void
{
	RAW_SetupBuffers();

	RAW_BindAndBufferDataToIDs();

	DGL::LoadDefaultShaders();

	DGL::FormatVertexAttributes<Vertex3>(VertexAttributeIndex, EDataType::Float, ZeroOffset(), Vertex3::ValueCount(), EBool::False);

	DGL::BindBuffer(EBufferTarget::VertexAttributes, 0);   // Dunno. Prob unbinding...

	DGL::BindVertexArray(0);
}


