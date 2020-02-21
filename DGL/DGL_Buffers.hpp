/*
Title : Ducktaped GL: Buffers
Author: Edward R. Gonzalez

Description:
Contains wrappers to buffer related functionality.
*/

#pragma once

// GLEW
#include <glew.h>

// DGL
#include "DGL_Enum.hpp"
#include "DGL_Types.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace DGL
{
	void BindBuffer(EBufferTarget _targetType, ID<VertexBuffer> _buffer)
	{
		glBindBuffer(GLenum(_targetType), _buffer);

		return;
	}

	void BindVertexArray(gUInt _referenceToTrackArray)
	{
		glBindVertexArray(_referenceToTrackArray);

		return;
	}

	template<typename TypeOfData>
	void BufferData(const TypeOfData& _data, EBufferTarget _targetType, EBufferUsage _usageType)
	{
		glBufferData(GLenum(_targetType), sizeof(TypeOfData), &_data, GLenum(_usageType));

		return;
	}

	template<typename Type>
	void BufferData(const Type& _data,  gSize _sizeOfData, EBufferTarget _targetType, EBufferUsage _usageType)
	{
		glBufferData(GLenum(_targetType), _sizeOfData, &_data, GLenum(_usageType));
	}

	template<typename... Type, typename = EFrameBuffer>
	void ClearBuffer(Type... _buffersToClear)
	{
		glClear( (gBitfield(_buffersToClear) | ...) );

		return;
	}

	void DisableVertexAttributeArray(gInt _vertexAttributeArrayIndex)
	{
		glDisableVertexAttribArray(_vertexAttributeArrayIndex);
	}

	void DrawArrays(EPrimitives _primitive, gInt _startingIndex, gInt _numToRender)
	{
		glDrawArrays(GLenum(_primitive), _startingIndex, _numToRender);   // Starting from vertex 0; 3 vertices total -> 1 triangle.
	}

	void DrawElements(EPrimitives _primitive, gSize _numElements, EDataType _dataType, const DataPtr _offfsetAddressFromFirstIndex)
	{
		glDrawElements(GLenum(_primitive), _numElements, GLenum(_dataType), _offfsetAddressFromFirstIndex);
	}

	void EnableVertexAttributeArray(gInt _vertexAttributeArrayIndex)
	{
		glEnableVertexAttribArray(_vertexAttributeArrayIndex);
	}

	template<typename VertType>
	void FormatVertexAttributes
	(
		   gUInt       _attributeIndex              , 
		   EDataType   _vertexComponenetType        ,
		   void* const _firstVertexComponentLocation,
		   gInt        _numberOfVertexComponents    ,
		   gBoolean    _shouldNormalize
	)
	{
		glVertexAttribPointer
		(
			_attributeIndex              ,
			_numberOfVertexComponents    , 
			GLenum(_vertexComponenetType),
			_shouldNormalize             ,
			sizeof(VertType             ),
			_firstVertexComponentLocation
		);
	}

	void GenerateBuffers(gUInt& _bufferReferencer, gSize _numberOfBuffersToReserve)
	{
		glGenBuffers(_numberOfBuffersToReserve, &_bufferReferencer);

		return;
	}

	void GenerateVertexBuffers(gUInt& __referenceRetainerToBuffer, gSize _numOfObjectsToReserveFor)
	{
		glGenVertexArrays(_numOfObjectsToReserveFor, &__referenceRetainerToBuffer);

		return;
	}

	void GetBufferParameterIV(EBufferTarget _target, EBufferParam _param, gInt& _dataStore)
	{
		glGetBufferParameteriv(GLenum(_target), GLenum(_param), &_dataStore);

		return;
	}

	// Used to get an offset from a specified location.
	DataPtr Offset(uInt64 _offsetAmount)
	{
		return DataPtr(_offsetAmount);
	}

	void UnbindVertexArray()
	{
		BindVertexArray(0);
	}

	// Provides an zero offset pointer specifier.
	constexpr void* ZeroOffset()
	{
		return 0;
	}
}
