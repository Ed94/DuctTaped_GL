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

	enum class ECompareMode
	{
		RefToTexture = GL_COMPARE_REF_TO_TEXTURE,
		None         = GL_NONE
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

	enum class EFace
	{
		Front = GL_FRONT,
		Back = GL_BACK,
		Front_and_Back = GL_FRONT_AND_BACK
	};

	enum class EFrameBuffer
	{
		Accumulation = GL_ACCUM_BUFFER_BIT  ,
		Color        = GL_COLOR_BUFFER_BIT  ,
		Depth        = GL_DEPTH_BUFFER_BIT  ,
		Stencil      = GL_STENCIL_BUFFER_BIT
	};

	enum class ELODBias
	{
		Max = GL_MAX_TEXTURE_LOD_BIAS
	};

	enum class EMaxFilter
	{
		Nearest = GL_NEAREST,
		Linear  = GL_LINEAR
	};

	enum class EMinFilter
	{
		Nearest          = GL_NEAREST               ,
		Linear           = GL_LINEAR                ,
		NearestToNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearToNearest  = GL_NEAREST_MIPMAP_LINEAR ,
		NearestToLinear  = GL_NEAREST_MIPMAP_LINEAR ,
		LinearToLinear   = GL_LINEAR_MIPMAP_LINEAR
	};

	enum class EMode
	{
		Point = GL_POINT,
		Line  = GL_LINE ,
		Fill  = GL_FILL
	};

	enum class EPrimitives
	{
		Points = GL_POINTS,

		Lines     = GL_LINES     ,
		LineStrip = GL_LINE_STRIP,
		LineLoop  = GL_LINE_LOOP ,

		Triangles     = GL_TRIANGLES     ,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan   = GL_TRIANGLE_FAN  ,
		
		Quads      = GL_QUADS     ,
		QuadsStrip = GL_QUAD_STRIP,

		Patches = GL_PATCHES,

		Polygon = GL_POLYGON
	};

	enum class EShaderInfo
	{
		Type               = GL_SHADER_TYPE         ,
		DeleteStatus       = GL_DELETE_STATUS       ,
		CompileStatus      = GL_COMPILE_STATUS      ,
		InfoLogLength      = GL_INFO_LOG_LENGTH     ,
		ShaderSourceLength = GL_SHADER_SOURCE_LENGTH
	};

	enum class EShaderProgramInfo
	{
		DeleteStatus                 = GL_DELETE_STATUS                        ,
		LinkStatus                   = GL_LINK_STATUS                          ,
		ValidateStatus               = GL_VALIDATE_STATUS                      ,
		InfoLogLength                = GL_INFO_LOG_LENGTH                      ,
		AttachedShaders              = GL_ATTACHED_SHADERS                     ,
		ActiveAttributes             = GL_ACTIVE_ATTRIBUTES                    ,
		ActiveAttributesMaxLength    = GL_ACTIVE_ATTRIBUTE_MAX_LENGTH          ,
		ActiveUniforms               = GL_ACTIVE_UNIFORMS                      ,
		TransformFeedbackActive      = GL_TRANSFORM_FEEDBACK_BUFFER_MODE       ,
		TransformFeedbackVaryings    = GL_TRANSFORM_FEEDBACK_VARYING           ,
		TransformFeedbackVaryingsMax = GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH,
		ShaderVerticiesMax           = GL_GEOMETRY_VERTICES_OUT                ,
		GeometryInputType            = GL_GEOMETRY_INPUT_TYPE                  ,
		GeometryOutputType           = GL_GEOMETRY_OUTPUT_TYPE
	};

	enum class EShaderType
	{
		Vertex   = GL_VERTEX_SHADER  ,
		Fragment = GL_FRAGMENT_SHADER
	};

	enum class EStencilMode
	{
		DepthComponent = GL_DEPTH_COMPONENT,
		StencilIndex   = GL_STENCIL_INDEX  
	};

	enum class ESWIZZLE
	{
		Red   = GL_RED  ,
		Green = GL_GREEN,
		Blue  = GL_BLUE ,
		Alpha = GL_ALPHA,
		Zero  = GL_ZERO ,
		One   = GL_ONE
	};

	enum class ETexCompareFuncs
	{
		LessOrEqual    = GL_LEQUAL  ,
		GreaterOrEqual = GL_GEQUAL  ,
		Less           = GL_LESS    ,
		Greater        = GL_GREATER ,
		Equal          = GL_EQUAL   ,
		NotEqual       = GL_NOTEQUAL,
		Always         = GL_ALWAYS  ,
		Never          = GL_NEVER
	};

	enum class ETextureMode
	{
		DepthStencil        = GL_DEPTH_STENCIL_TEXTURE_MODE, 
		MipmapLowest        = GL_TEXTURE_BASE_LEVEL        , 
		ComparisonOperator  = GL_TEXTURE_COMPARE_FUNC      , 
		Comparision         = GL_TEXTURE_COMPARE_MODE      , 
		LevelOfDetailBias   = GL_TEXTURE_LOD_BIAS          , 
		MinimizingFilter    = GL_TEXTURE_MIN_FILTER        , 
		MagnificationFilter = GL_TEXTURE_MAG_FILTER        , 
		MinimumLOD          = GL_TEXTURE_MIN_LOD           , 
		MaximumLOD          = GL_TEXTURE_MAX_LOD           , 
		MipmapMax           = GL_TEXTURE_MAX_LEVEL         , 
		Swizzle_R           = GL_TEXTURE_SWIZZLE_R         , 
		Swizzle_G           = GL_TEXTURE_SWIZZLE_G         , 
		Swizzle_B           = GL_TEXTURE_SWIZZLE_B         , 
		Swizzle_A           = GL_TEXTURE_SWIZZLE_A         , 
		Swizzle_RGBA        = GL_TEXTURE_SWIZZLE_RGBA      ,
		Wrap_S              = GL_TEXTURE_WRAP_S            , 
		Wrap_T              = GL_TEXTURE_WRAP_T            , 
		Wrap_R              = GL_TEXTURE_WRAP_R
	};

	enum class ETextureType
	{
		_1D = GL_TEXTURE_1D,
		_2D = GL_TEXTURE_2D,
		_3D = GL_TEXTURE_3D,

		Rectangle = GL_TEXTURE_RECTANGLE,
		Buffer    = GL_TEXTURE_BUFFER   ,
		
		CubeMap      = GL_TEXTURE_CUBE_MAP      ,	 
		CubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
		
		Array1D = GL_TEXTURE_1D_ARRAY,
		Array2D = GL_TEXTURE_2D_ARRAY,
		
		Multisample   = GL_TEXTURE_2D_MULTISAMPLE      ,
		Multisample2D = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
	};

	enum class EWrap
	{
		ClampToEdge       = GL_CLAMP_TO_EDGE       ,
		ClampToBorder     = GL_CLAMP_TO_BORDER     ,
		MirroredRepeat    = GL_MIRRORED_REPEAT     ,
		Repeat            = GL_REPEAT              ,
		MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
	};
}
