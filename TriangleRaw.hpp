#pragma once

#include "DGL.hpp"

#include "Cpp_Alias.hpp"



using GL::gFloat;
using GL::VertexBuffer;
using GL::EBufferTarget;
using GL::EBufferUsage;
using GL::Buffer;
using GL::ID;
using GL::gInt;
using GL::gSize;



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
	{ -1.0f, -1.0f, 0.0f }, 
	{  1.0f, -1.0f, 0.0f }, 
	{  0.0f,  1.0f, 0.0f } 
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
	{  0.5f,  0.5f, 0.0f },
	{  0.5f, -0.5f, 0.0f },
	{ -0.5f, -0.5f, 0.0f },
	{ -0.5f,  0.5f, 0.0f }
};

RectangleIndices rectIndices =
{
	{ 0, 1, 3 },
	{ 1, 2, 3 }
};

using GL::ElementBuffer;

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
	GL::GenerateVertexBuffers(Address(VertexArrayObj ), 1);
	GL::GenerateBuffers      (Address(VertexBufferObj), 1);
	GL::GenerateBuffers      (Address(ElemBufferObj  ), 1);
}

sfn RAW_SetupTriangleBuffer()
{
	GL::GenerateBuffers(Address(VertexBufferObj), 1);
}

sfn RAW_BindAndBufferDataToIDs()
{
	GL::BindVertexArray(VertexArrayObj);

	GL::BindBuffer(EBufferTarget::VertexAttributes, VertexBufferObj);

	GL::BufferData<TriangleRaw>(Address(EquilateralTriangleVerticies), EBufferTarget::VertexAttributes, EBufferUsage::StaticDraw);

	//GL::BufferData<RectangleCompressed>(Address(rectCompress), EBufferTarget::VertexAttributes, EBufferUsage::StaticDraw);
	
	//GL::BindBuffer(EBufferTarget::VertexIndices, ElemBufferObj);

	//GL::BufferData<RectangleIndices>(Address(rectIndices), EBufferTarget::VertexIndices, EBufferUsage::StaticDraw);
}

GL::gInt VertexAttributeIndex = 0;   // See shader source: (layout = 0).

using GL::EBool    ;
using GL::EDataType;

constexpr sfn ZeroOffset() -> ptr<void>
{
	return 0;
}
