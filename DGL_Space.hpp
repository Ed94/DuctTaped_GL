#pragma once


#include "DGL_FundamentalTypes.hpp"
#include "DGL_MiscTypes.hpp"



#include "Cpp_Alias.hpp"



namespace GL
{
	using CoordSpace = Matrix4x4;
	using Projection = Matrix4x4;


	// Function

	template<typename Type>
	sfn CreateLookAtView(const Generic::Vector3<Type> _viewPosition, const Generic::Vector3<Type> _lookAtPosition, const Generic::Vector3<Type> _upDirection) -> Matrix4x4
	{
		return glm::lookAt(_viewPosition, _lookAtPosition, _upDirection);
	}

	sfn CreateOrthographic(gFloat _leftScreenCoord, gFloat _rightScreenCoord, gFloat _bottomScreenCoord, gFloat _topScreenCoord, gFloat _nearPlane, gFloat _farPlane)
	{
		return glm::ortho(_leftScreenCoord, _rightScreenCoord, _bottomScreenCoord, _topScreenCoord, _nearPlane, _farPlane);
	}

	template<typename FloatType>
	sfn CreatePerspective(const FloatType _fieldOfView, const FloatType _aspectRatio, const FloatType _nearPlane, const FloatType _farPlane)
	{
		return glm::perspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);
	}

	sfn Rotate(const Matrix4x4 _matrix, gFloat _rotationAngleAmount, Vector3 _axis) -> Matrix4x4
	{
		return glm::rotate(_matrix, _rotationAngleAmount, _axis);
	}

	template<typename Type>
	sfn ToRadians(const Ref(Type) _degrees) -> Type
	{
		return glm::radians(_degrees);
	}

	struct ClippingPlanes
	{
		gFloat Near, Far;

		ClippingPlanes(gFloat _near, gFloat _far) : Near(_near), Far(_far) {};
	};

	namespace DefaultSpace
	{
		gFloat 
			   AspectRatio       = 16.0f / 10.0f,
			   FieldOfView       = 90.0f        ,
			   NearClippingPlane = 0.1f         ,
			   FarClippingPlane  = 100.0f        ;

		Vector3 CameraPosition(-3, 0, 0),
			    LookAtPosition( 0, 0, 0),
			    UpDirection   ( 0, 1, 0) ;

		gFloat ScreenWidth = 720.0f, ScreenHeight = 450.0f;
	}

	struct Camera
	{
		gFloat AspectRatio, FieldOfView;

		ClippingPlanes ClipSpace;

		CoordSpace ViewPort;

		Projection Orthographic, Perspective;

		Vector3 Position, LookAtPosition, UpDirection;


		Camera
		(
			gFloat         _aspectRatio   ,
			gFloat         _fieldOfView   ,
			ClippingPlanes _clippingPlanes,
			Vector3        _position      ,
			Vector3        _lookAtPosition,
			Vector3        _upDirection
		) :
			AspectRatio   (_aspectRatio   ),
			FieldOfView   (_fieldOfView   ),
			ClipSpace     (_clippingPlanes),
			Position      (_position      ),
			LookAtPosition(_lookAtPosition),
			UpDirection   (_upDirection   )
		{
			ViewPort = CreateLookAtView(Position, LookAtPosition, UpDirection);

			Orthographic = CreateOrthographic(0.0f, DefaultSpace::ScreenWidth, 0.0f, DefaultSpace::ScreenHeight, ClipSpace.Near, ClipSpace.Far);

			Perspective = CreatePerspective<gFloat>(ToRadians(FieldOfView), AspectRatio, ClipSpace.Near, ClipSpace.Far);
		}
	};

	namespace DefaultSpace
	{
		Camera WorldCamera(AspectRatio, FieldOfView, ClippingPlanes(NearClippingPlane, FarClippingPlane), CameraPosition, LookAtPosition, UpDirection);

		CoordSpace WorldSpace(Matrix4x4(1.0f));

		CoordSpace Screenspace = WorldCamera.Perspective * WorldCamera.ViewPort * WorldSpace;
	}
}