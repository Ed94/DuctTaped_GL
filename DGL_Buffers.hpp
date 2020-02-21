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
	sfn BindBuffer(EBufferTarget _targetType, ID<VertexBuffer> _buffer)
	{
		glBindBuffer(GLenum(_targetType), _buffer);

		return;
	}

	sfn BindVertexArray(gUInt _referenceToTrackArray)
	{
		glBindVertexArray(_referenceToTrackArray);

		return;
	}

	template<typename TypeOfData>
	sfn BufferData(ro Ref(TypeOfData) _data, EBufferTarget _targetType, EBufferUsage _usageType)
	{
		glBufferData(GLenum(_targetType), sizeof(TypeOfData), Address(_data), GLenum(_usageType));

		return;
	}

	template<typename Type>
	sfn BufferData(ro Ref(Type) _data,  gSize _sizeOfData, EBufferTarget _targetType, EBufferUsage _usageType)
	{
		glBufferData(GLenum(_targetType), _sizeOfData, Address(_data), GLenum(_usageType));
	}

	template<typename... Type, typename = EFrameBuffer>
	sfn ClearBuffer(Type... _buffersToClear)
	{
		glClear( (gBitfield(_buffersToClear) | ...) );

		return;
	}

	sfn DisableVertexAttributeArray(gInt _vertexAttributeArrayIndex)
	{
		glDisableVertexAttribArray(_vertexAttributeArrayIndex);
	}

	sfn DrawArrays(EPrimitives _primitive, gInt _startingIndex, gInt _numToRender)
	{
		glDrawArrays(GLenum(_primitive), _startingIndex, _numToRender);   // Starting from vertex 0; 3 vertices total -> 1 triangle.
	}

	sfn DrawElements(EPrimitives _primitive, gSize _numElements, EDataType _dataType, ro DataPtr _offfsetAddressFromFirstIndex)
	{
		glDrawElements(GLenum(_primitive), _numElements, GLenum(_dataType), _offfsetAddressFromFirstIndex);
	}

	sfn EnableVertexAttributeArray(gInt _vertexAttributeArrayIndex)
	{
		glEnableVertexAttribArray(_vertexAttributeArrayIndex);
	}

	template<typename VertType>
	sfn FormatVertexAttributes
	(
		   gUInt     _attributeIndex              , 
		   EDataType _vertexComponenetType        ,
		ro ptr<void> _firstVertexComponentLocation,
		   gInt      _numberOfVertexComponents    ,
		   gBoolean   _shouldNormalize
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

	sfn GenerateBuffers(Ref(gUInt) _bufferReferencer, gSize _numberOfBuffersToReserve)
	{
		glGenBuffers(_numberOfBuffersToReserve, Address(_bufferReferencer));

		return;
	}

	sfn GenerateVertexBuffers(Ref(gUInt) __referenceRetainerToBuffer, gSize _numOfObjectsToReserveFor)
	{
		glGenVertexArrays(_numOfObjectsToReserveFor, Address(__referenceRetainerToBuffer));

		return;
	}

	sfn GetBufferParameterIV(EBufferTarget _target, EBufferParam _param, Ref(gInt) _dataStore)
	{
		glGetBufferParameteriv(GLenum(_target), GLenum(_param), Address(_dataStore));

		return;
	}

	// Used to get an offset from a specified location.
	sfn Offset(uInt64 _offsetAmount)
	{
		return DataPtr(_offsetAmount);
	}

	sfn UnbindVertexArray()
	{
		BindVertexArray(0);
	}

	// Provides an zero offset pointer specifier.
	constexpr sfn ZeroOffset() -> ptr<void>
	{
		return 0;
	}
}
