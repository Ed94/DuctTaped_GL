#pragma once

// GLM
#include <glm/glm.hpp                  >
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp >

// DGL
#include "DGL_Types.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace DGL
{
	


	// Function

	sfn Cosine(gFloat _angleInRadians)
	{
		return glm::cos(_angleInRadians);
	}

	sfn Sine(gFloat _angleInRadians)
	{
		return glm::sin(_angleInRadians);
	}

	template<typename Type>
	sfn CreateLookAtView(ro Ref(Generic::Vector3<Type>) _viewPosition, ro Ref(Generic::Vector3<Type>) _lookAtPosition, ro Ref(Generic::Vector3<Type>) _upDirection) -> Matrix4x4
	{
		return glm::lookAt(_viewPosition, _lookAtPosition, _upDirection);
	}

	sfn CreateOrthographic(gFloat _leftScreenCoord, gFloat _rightScreenCoord, gFloat _bottomScreenCoord, gFloat _topScreenCoord, gFloat _nearPlane, gFloat _farPlane)
	{
		return glm::ortho(_leftScreenCoord, _rightScreenCoord, _bottomScreenCoord, _topScreenCoord, _nearPlane, _farPlane);
	}

	template<typename FloatType>
	sfn CreatePerspective(FloatType _fieldOfView, FloatType _aspectRatio, FloatType _nearPlane, FloatType _farPlane)
	{
		return glm::perspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);
	}

	sfn GetCrossNormal(ro Ref(Vector3) _subj, ro Ref(Vector3) _ref) -> Vector3
	{
		return glm::cross(_subj, _ref);
	}

	sfn GetDirection(ro Ref(Vector3) _vectorSpecified)
	{
		return glm::normalize(_vectorSpecified);
	}

	sfn Inverse(ro Ref(Matrix4x4) _matrix)
	{
		return glm::inverse(_matrix);
	}

	sfn Rotate(ro Ref(Matrix4x4) _matrix, gFloat _rotationAngleAmount, ro Ref(Vector3) _axis) -> Matrix4x4
	{
		return glm::rotate(_matrix, _rotationAngleAmount, _axis);
	}

	sfn Scale(ro Ref(Matrix4x4) _matrix, ro Ref(Vector3) _scaleBy)
	{
		return glm::scale(_matrix, _scaleBy);
	}

	template<typename Type>
	sfn ToRadians(Type _degrees) -> Type
	{
		return glm::radians(_degrees);
	}

	sfn Translate(ro Ref(Matrix4x4) _matrix, ro Ref(Vector3) _translationAmount)
	{
		return glm::translate(_matrix, _translationAmount);
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
			   NearClippingPlane = 0.01f        ,
			   FarClippingPlane  = 1000.0f       ;

		Vector3 CameraPosition( 0, 0,  2),
			    LookAtPosition( 0, 0,  0),
				RightDirection( 1, 0,  0), 
			    UpDirection   ( 0, 1,  0),
			    FrontDirection( 0, 0,  1) ;

		gInt ScreenWidth = 720, ScreenHeight = 540, ScreenCenterWidth = ScreenWidth / 2, ScreenCenterHeight = ScreenHeight / 2;
	}

	struct Camera
	{
		gFloat AspectRatio, FieldOfView, Yaw, Pitch, Roll;

		ClippingPlanes ClipSpace;

		CoordSpace Viewport;

		Projection Orthographic, Perspective;

		Vector3 Position, LookAtPosition, UpDirection, FrontDirection, RightDirection;


		Camera
		(
			       gFloat          _aspectRatio   ,
			       gFloat          _fieldOfView   ,
			ro Ref(ClippingPlanes) _clippingPlanes,
			ro Ref(Vector3       ) _position      ,
			ro Ref(Vector3       ) _lookAtPosition,
			ro Ref(Vector3       ) _upDirection   ,
			ro Ref(Vector3       ) _frontDirection
		) :
			AspectRatio   (_aspectRatio   ),
			FieldOfView   (_fieldOfView   ),
			ClipSpace     (_clippingPlanes),
			Position      (_position      ),
			LookAtPosition(_lookAtPosition),
			UpDirection   (_upDirection   ),
			FrontDirection(_frontDirection)
		{
			Yaw = -90.0f; Pitch = 0.0f; Roll = 0.0f;

			UpdateCamera();

			Orthographic = CreateOrthographic(0.0f, gFloat(DefaultSpace::ScreenWidth), 0.0f, gFloat(DefaultSpace::ScreenHeight), ClipSpace.Near, ClipSpace.Far);

			Perspective = CreatePerspective<gFloat>(ToRadians(FieldOfView), AspectRatio, ClipSpace.Near, ClipSpace.Far);
		}


		sfn Move(EDirection _direction, gFloat _translationAmount, gFloat _deltaTime)
		{
			switch (_direction)
			{
			case EDirection::Up:
			{
				Position -= UpDirection * _translationAmount * _deltaTime;

				break;
			}
			case EDirection::Down:
			{
				Position += UpDirection * _translationAmount * _deltaTime;

				break;
			}
			case EDirection::Left:
			{
				Position -= GetDirection(GetCrossNormal(FrontDirection, UpDirection)) * _translationAmount * _deltaTime;

				break;
			}
			case EDirection::Right:
			{
				Position += GetDirection(GetCrossNormal(FrontDirection, UpDirection)) * _translationAmount * _deltaTime;

				break;
			}
			case EDirection::Forward:
			{
				Position += FrontDirection * _translationAmount * _deltaTime;

				break;
			}
			case EDirection::Backward:
			{
				Position -= FrontDirection * _translationAmount * _deltaTime;

				break;
			}
			default:
			{
				throw std::logic_error("Direction move not defined.");
			}
			}

			return;
		}

		sfn Move(ro Ref(Vector3) _translationAmount, gFloat _deltaTime)
		{
			Position += _translationAmount * _deltaTime;

			return;
		}

		sfn Rotate(ERotationAxis _pivot, gFloat _rotationAmount, gFloat _deltaTime)
		{
			switch (_pivot)
			{
			case ERotationAxis::Pitch:
			{
				Pitch -= _rotationAmount * _deltaTime;

				if (Pitch > 89.9f)
				{
					Pitch = 89.9f;
				}
				else if (Pitch < -89.9f)
				{
					Pitch = -89.9f;
				}

				return;
			}
			case ERotationAxis::Roll:
			{
				Roll += _rotationAmount * _deltaTime;

				return;
			}
			case ERotationAxis::Yaw:
			{
				Yaw += _rotationAmount * _deltaTime;

				return;
			}
			}
		}


		sfn UpdateCamera() -> void
		{
			FrontDirection.x = Cosine(ToRadians(Yaw  )) * Cosine(ToRadians(Pitch));
			FrontDirection.y = Sine  (ToRadians(Pitch))                           ;
			FrontDirection.z = Sine  (ToRadians(Yaw  )) * Cosine(ToRadians(Pitch));

			FrontDirection = GetDirection(FrontDirection                                              );
			RightDirection = GetDirection(GetCrossNormal(FrontDirection, DefaultSpace::UpDirection   ));
			UpDirection    = GetDirection(GetCrossNormal(RightDirection,               FrontDirection));

			LookAtPosition = Position + FrontDirection;

			Viewport = CreateLookAtView(Position, LookAtPosition, UpDirection);

			return;
		}
	};

	namespace DefaultSpace
	{
		Camera WorldCamera
		(
			AspectRatio                                        , 
			FieldOfView                                        , 
			ClippingPlanes(NearClippingPlane, FarClippingPlane), 
			CameraPosition                                     , 
			LookAtPosition                                     , 
			UpDirection                                        , 
			FrontDirection
		);	

		CoordSpace WorldSpace(Matrix4x4(1.0f));

		CoordSpace Screenspace = WorldCamera.Perspective * WorldCamera.Viewport * WorldSpace;

		sfn UpdateScreenspace()
		{
			Screenspace = WorldCamera.Perspective * WorldCamera.Viewport * WorldSpace;

			return;
		}
	}
}