#pragma once

// GLEW
#include <glew.h>

// DGL
#include "DGL_FundamentalTypes.hpp"
#include "DGL_MiscTypes.hpp"
#include "DGL_Enum.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace GL
{
	sfn ClearBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		return;
	}

	sfn BindBuffer(EBufferTarget _targetType, ID<Buffer> _buffer)
	{
		glBindBuffer(GLenum(_targetType), _buffer);

		return;
	}

	sfn BindVertexArray(ptr<gUInt> _referenceToTrackArray)
	{
		glBindVertexArray(Dref(_referenceToTrackArray));

		return;
	}

	template<typename TypeOfData>
	sfn BufferData(ptr<TypeOfData> _data, EBufferTarget _targetType, EBufferUsage _usageType)
	{
		glBufferData(GLenum(_targetType), sizeof(TypeOfData), _data, GLenum(_usageType));

		return;
	}

	template<typename VertType>
	sfn FormatVertexAttributes
	(
		gUInt     _attributeIndex              , 
		EDataType _vertexComponenetType        ,
		ptr<void> _firstVertexComponentLocation,
		gInt      _numberOfVertexComponents    ,
		EBool     _shouldNormalize
	)
	{
		glVertexAttribPointer
		(
			_attributeIndex              ,
			_numberOfVertexComponents    , 
			GLenum(_vertexComponenetType),
			GLenum(_shouldNormalize     ),
			sizeof(VertType             ),
			_firstVertexComponentLocation
		);
	}

	sfn GenerateBuffers(ptr<gUInt> _bufferReferencer, uInt64 _numberOfBuffersToReserve)
	{
		glGenBuffers(_numberOfBuffersToReserve, _bufferReferencer);

		return;
	}

	sfn GenerateVertexBuffers(ptr<gUInt> __referenceRetainerToBuffer, uInt64 _numOfObjectsToReserveFor)
	{
		glGenVertexArrays(_numOfObjectsToReserveFor, __referenceRetainerToBuffer);

		return;
	}
}
