#pragma once

// GLEW
#include <glew.h>



namespace GL
{
	enum class EBool
	{
		True  = GL_TRUE ,
		False = GL_FALSE
	};

	enum class EBufferTarget
	{
		VertexAttributes                = GL_ARRAY_BUFFER             ,
		AtomicCounter                   = GL_ATOMIC_COUNTER_BUFFER    ,
		CopySource                      = GL_COPY_READ_BUFFER         ,
		CopyDestination                 = GL_COPY_WRITE_BUFFER        ,
		IndirectComputeDispatchCommands = GL_DISPATCH_INDIRECT_BUFFER ,
		IndirectCommandArguments        = GL_DRAW_INDIRECT_BUFFER     ,
		VertexIndices                   = GL_ELEMENT_ARRAY_BUFFER     ,
		PixelReadTarget                 = GL_PIXEL_PACK_BUFFER        ,
		TextureDataSource               = GL_PIXEL_UNPACK_BUFFER      , 
		QueryResult                     = GL_QUERY_BUFFER             ,
		ReadWriteShaderStorage          = GL_SHADER_STORAGE_BUFFER    ,
		TextureData                     = GL_TEXTURE_BUFFER           ,
		TransformFeedback               = GL_TRANSFORM_FEEDBACK_BUFFER,
		UniformBlockStorage             = GL_UNIFORM_BUFFER
	};

	enum class EBufferUsage
	{
		DynamicCopy = GL_DYNAMIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ,

		StreamCopy = GL_STREAM_COPY,
		StreamDraw = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,

		StaticCopy = GL_STATIC_COPY,
		StaticDraw = GL_STATIC_DRAW,
		StaticRead = GL_STATIC_READ
	};

	enum class EDataType
	{
		Byte          = GL_BYTE          ,
		UnsignedByte  = GL_UNSIGNED_BYTE ,
		Short         = GL_SHORT         ,
		UnsignedShort = GL_UNSIGNED_SHORT,
		Int           = GL_INT           ,
		UnsignedInt   = GL_UNSIGNED_INT  ,
		Fixed         = GL_FIXED         ,
		Half          = GL_HALF_FLOAT    ,
		Float         = GL_FLOAT         ,
		Double        = GL_DOUBLE
	};

	enum class EShaderType
	{
		Vertex   = GL_VERTEX_SHADER  ,
		Fragment = GL_FRAGMENT_SHADER
	};

	enum class EPrimitives
	{
		Points        = GL_POINTS,

		Lines         = GL_LINES     ,
		LineStrip     = GL_LINE_STRIP,
		LineLoop      = GL_LINE_LOOP ,

		Triangles     = GL_TRIANGLES     ,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan   = GL_TRIANGLE_FAN  ,
		
		Quads      = GL_QUADS     ,
		QuadsStrip = GL_QUAD_STRIP,

		Patches = GL_PATCHES
	};
}
