#pragma once

// GLEW
#include <glew.h>

// DGL
#include "DGL_FundamentalTypes.hpp"
#include "DGL_MiscTypes.hpp"
#include "DGL_Enum.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace DGL
{
	sfn BindBuffer(const EBufferTarget _targetType, const ID<VertexBuffer> _buffer)
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

	sfn BufferData(DataPtr _data,  gSize _sizeOfData, const EBufferTarget _targetType, const EBufferUsage _usageType)
	{
		glBufferData(GLenum(_targetType), _sizeOfData, _data, GLenum(_usageType));
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

	sfn DrawArrays(EPrimitives _primitive, gInt _startingIndex, gInt _numToRender)
	{
		glDrawArrays(GLenum(_primitive), _startingIndex, _numToRender);   // Starting from vertex 0; 3 vertices total -> 1 triangle.
	}

	sfn DrawElements(EPrimitives _primitive, gSize _numElements, EDataType _dataType, DataPtr _offfsetAddressFromFirstIndex)
	{
		glDrawElements(GLenum(_primitive), _numElements, GLenum(_dataType), _offfsetAddressFromFirstIndex);
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

	sfn GetBufferParameterIV(EBufferTarget _target, EBufferParam _param, ptr<gInt> _data)
	{
		glGetBufferParameteriv(GLenum(_target), GLenum(_param), _data);

		return;
	}
}
