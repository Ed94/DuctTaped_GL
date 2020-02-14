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
	sfn BindBuffer(const EBufferTarget _targetType, const ID<Buffer> _buffer)
	{
		glBindBuffer(GLenum(_targetType), _buffer);

		return;
	}

	sfn BindVertexArray(const gUInt _referenceToTrackArray)
	{
		glBindVertexArray(_referenceToTrackArray);

		return;
	}

	template<typename TypeOfData>
	sfn BufferData(ptr<TypeOfData> _data, const EBufferTarget _targetType, const EBufferUsage _usageType)
	{
		glBufferData(GLenum(_targetType), sizeof(TypeOfData), _data, GLenum(_usageType));

		return;
	}

	template<typename... Type, typename = EFrameBuffer>
	sfn ClearBuffer(const Type... _buffersToClear)
	{
		glClear((gBitfield(_buffersToClear) | ...));

		return;
	}

	sfn DisableVertexAttributeArray(const gInt _vertexAttributeArrayIndex)
	{
		glDisableVertexAttribArray(_vertexAttributeArrayIndex);
	}

	sfn EnableVertexAttributeArray(const gInt _vertexAttributeArrayIndex)
	{
		glEnableVertexAttribArray(_vertexAttributeArrayIndex);
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
