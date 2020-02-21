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

	gFloat Cosine(gFloat _angleInRadians)
	{
		return glm::cos(_angleInRadians);
	}

	gFloat Sine(gFloat _angleInRadians)
	{
		return glm::sin(_angleInRadians);
	}

	template<typename Type>
	Matrix4x4 CreateLookAtView(const Generic::Vector3<Type>& _viewPosition, const Generic::Vector3<Type>& _lookAtPosition, const Generic::Vector3<Type>& _upDirection)
	{
		return glm::lookAt(_viewPosition, _lookAtPosition, _upDirection);
	}

	Matrix4x4 CreateOrthographic(gFloat _leftScreenCoord, gFloat _rightScreenCoord, gFloat _bottomScreenCoord, gFloat _topScreenCoord, gFloat _nearPlane, gFloat _farPlane)
	{
		return glm::ortho(_leftScreenCoord, _rightScreenCoord, _bottomScreenCoord, _topScreenCoord, _nearPlane, _farPlane);
	}

	template<typename FloatType>
	Matrix4x4 CreatePerspective(FloatType _fieldOfView, FloatType _aspectRatio, FloatType _nearPlane, FloatType _farPlane)
	{
		return glm::perspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);
	}

	Vector3 GetCrossNormal(const Vector3& _subj, const Vector3& _ref)
	{
		return glm::cross(_subj, _ref);
	}

	Vector3 GetDirection(const Vector3& _vectorSpecified)
	{
		return glm::normalize(_vectorSpecified);
	}

	Matrix4x4 Inverse(const Matrix4x4& _matrix)
	{
		return glm::inverse(_matrix);
	}

	Matrix4x4 Rotate(const Matrix4x4& _matrix, gFloat _rotationAngleAmount, const Vector3& _axis)
	{
		return glm::rotate(_matrix, _rotationAngleAmount, _axis);
	}

	Matrix4x4 Scale(const Matrix4x4& _matrix, const Vector3& _scaleBy)
	{
		return glm::scale(_matrix, _scaleBy);
	}

	template<typename Type>
	Type ToRadians(Type _degrees)
	{
		return glm::radians(_degrees);
	}

	Matrix4x4 Translate(const Matrix4x4& _matrix, const Vector3& _translationAmount)
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

		gInt ScreenWidth = 1280, ScreenHeight = 800, ScreenCenterWidth = ScreenWidth / 2, ScreenCenterHeight = ScreenHeight / 2;
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
			const ClippingPlanes& _clippingPlanes,
			const Vector3&        _position      ,
			const Vector3&        _lookAtPosition,
			const Vector3&        _upDirection   ,
			const Vector3&        _frontDirection
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


		void Move(EDirection _direction, gFloat _translationAmount, gFloat _deltaTime)
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

		void Move(const Vector3& _translationAmount, gFloat _deltaTime)
		{
			Position += _translationAmount * _deltaTime;

			return;
		}

		void Rotate(ERotationAxis _pivot, gFloat _rotationAmount, gFloat _deltaTime)
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


		void UpdateCamera()
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

		void UpdateScreenspace()
		{
			Screenspace = WorldCamera.Perspective * WorldCamera.Viewport * WorldSpace;

			return;
		}
	}
}