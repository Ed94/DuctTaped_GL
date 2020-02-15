#pragma once


#include "DGL_FundamentalTypes.hpp"
#include "DGL_MiscTypes.hpp"



#include "Cpp_Alias.hpp"



namespace DGL
{
	using CoordSpace = Matrix4x4;
	using Projection = Matrix4x4;


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

	sfn Translate(const Matrix4x4 _matrix, Vector3 _translationAmount)
	{
		return glm::translate(_matrix, _translationAmount);
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

	sfn GetCrossNormal(Vector3 _subj, Vector3 _ref) -> Vector3
	{
		return glm::cross(_subj, _ref);
	}

	sfn GetDirection(Vector3 _vectorSpecified)
	{
		return glm::normalize(_vectorSpecified);
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

		Vector3 CameraPosition( 0, 0,  2),
			    LookAtPosition( 0, 0,  0),
				RightDirection( 1, 0,  0), 
			    UpDirection   ( 0, 1,  0),
			    FrontDirection( 0, 0,  1) ;

		gFloat ScreenWidth = 720.0f, ScreenHeight = 540.0f, ScreenCenterWidth = ScreenWidth / 2, ScreenCenterHeight = ScreenHeight / 2;
	}

	using std::cout; using std::endl;

	struct Camera
	{
		gFloat AspectRatio, FieldOfView, Yaw, Pitch, Roll;

		ClippingPlanes ClipSpace;

		CoordSpace Viewport;

		Projection Orthographic, Perspective;

		Vector3 Position, LookAtPosition, UpDirection, FrontDirection, RightDirection;


		Camera
		(
			gFloat         _aspectRatio   ,
			gFloat         _fieldOfView   ,
			ClippingPlanes _clippingPlanes,
			Vector3        _position      ,
			Vector3        _lookAtPosition,
			Vector3        _upDirection   ,
			Vector3        _frontDirection
		) :
			AspectRatio   (_aspectRatio   ),
			FieldOfView   (_fieldOfView   ),
			ClipSpace     (_clippingPlanes),
			Position      (_position      ),
			LookAtPosition(_lookAtPosition),
			UpDirection   (_upDirection   ),
			FrontDirection(_frontDirection)
		{
			std::cout << UpDirection.x << ", " << UpDirection.y << ", " << UpDirection.z << std::endl;

			Yaw = -90.0f; Pitch = 0; Roll = 0;

			//Yaw = 0;

			UpdateCamera();

			std::cout << UpDirection.x << ", " << UpDirection.y << ", " << UpDirection.z << std::endl;

			Orthographic = CreateOrthographic(0.0f, DefaultSpace::ScreenWidth, 0.0f, DefaultSpace::ScreenHeight, ClipSpace.Near, ClipSpace.Far);

			Perspective = CreatePerspective<gFloat>(ToRadians(FieldOfView), AspectRatio, ClipSpace.Near, ClipSpace.Far);
		}


		sfn Move(EDirection _direction, gFloat _translationAmount, gFloat _deltaTime)
		{
			switch (_direction)
			{
			case EDirection::Up:
			{
				Position += UpDirection * _translationAmount * _deltaTime;

				LookAtPosition += UpDirection * _translationAmount * _deltaTime;

				break;
			}
			case EDirection::Down:
			{
				Position -= UpDirection * _translationAmount * _deltaTime;

				LookAtPosition -= UpDirection * _translationAmount * _deltaTime;

				break;
			}
			case EDirection::Left:
			{
				Position += GetDirection(GetCrossNormal(FrontDirection, UpDirection)) * _translationAmount * _deltaTime;

				//LookAtPosition += GetDirection(GetCrossNormal(FrontDirection, UpDirection)) * _translationAmount * _deltaTime;

				break;
			}
			case EDirection::Right:
			{
				Position -= GetDirection(GetCrossNormal(FrontDirection, UpDirection)) * _translationAmount * _deltaTime;

				//LookAtPosition -= GetDirection(GetCrossNormal(FrontDirection, UpDirection)) * _translationAmount * _deltaTime;

				break;
			}
			case EDirection::Forward:
			{
				Position -= FrontDirection * _translationAmount * _deltaTime;

				LookAtPosition -= FrontDirection * _translationAmount * _deltaTime;

				break;
			}
			case EDirection::Backward:
			{
				Position += FrontDirection * _translationAmount * _deltaTime;

				LookAtPosition += FrontDirection * _translationAmount * _deltaTime;

				break;
			}
			default:
			{
				throw std::logic_error("Direction move not defined.");
			}
			}
		}

		sfn Move(Vector3 _translationAmount, Ref(gFloat) _deltaTime)
		{
			Position += _translationAmount * _deltaTime;
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

				//std::cout << "Pitch: " << Pitch << std::endl;

				break;
			}
			case ERotationAxis::Roll:
			{
				Roll += _rotationAmount * _deltaTime;

				if (Roll > 89.9f)
				{
					Roll = 89.9f;
				}
				else if (Roll < -89.9f)
				{
					Roll = -89.9f;
				}

				break;
			}
			case ERotationAxis::Yaw:
			{
				Yaw += _rotationAmount * _deltaTime;

				/*if (Yaw > 89.9f)
				{
					Yaw = 89.9f;
				}
				else if (Yaw < -89.9f)
				{
					Pitch = -89.9f;
				}*/

				//std::cout << "Yaw: " << Yaw << std::endl;

				break;
			}
			}


			/*std::cout << "Front Direction: " << FrontDirection.x << ", " << FrontDirection.y << ", " << FrontDirection.z << std::endl;
			std::cout << "Right Direction: " << RightDirection.x << ", " << RightDirection.y << ", " << RightDirection.z << std::endl;
			std::cout << "Up Direction: " << UpDirection.x << ", " << UpDirection.y << ", " << UpDirection.z << std::endl;*/
		}


		sfn UpdateCamera() -> void
		{
			//cout << Pitch << Yaw << Roll << endl;

			//cout << "Cosine: " << Cosine(ToRadians(Yaw)) << endl;

			FrontDirection.x = Cosine(ToRadians(Yaw  )) * Cosine(ToRadians(Pitch));
			FrontDirection.y = Sine  (ToRadians(Pitch))                           ;
			FrontDirection.z = Sine  (ToRadians(Yaw  )) * Cosine(ToRadians(Pitch));

			FrontDirection = GetDirection(FrontDirection                                              );
			RightDirection = GetDirection(GetCrossNormal(FrontDirection, DefaultSpace::UpDirection   ));
			UpDirection    = GetDirection(GetCrossNormal(RightDirection,               FrontDirection));

			//Matrix4x4 mRoll  = glm::rotate(mRoll , Roll , DefaultSpace::FrontDirection);
			//Matrix4x4 mPitch = glm::rotate(mPitch, Pitch, DefaultSpace::RightDirection);
			//Matrix4x4 mYaw   = glm::rotate(mYaw  , Roll , DefaultSpace::UpDirection   );

			//Matrix4x4 rotation = mPitch * mYaw;

			Viewport = CreateLookAtView(Position, LookAtPosition - FrontDirection, UpDirection);

			//glm::mat4 translate = glm::mat4(1.0f);

			//translate = glm::translate(translate, -LookAtPosition);

			//Viewport = rotation * translate;
		}
	};

	namespace DefaultSpace
	{
		Camera WorldCamera
		(
			AspectRatio, 
			FieldOfView, 
			ClippingPlanes(NearClippingPlane, FarClippingPlane), 
			CameraPosition, 
			LookAtPosition, 
			UpDirection, 
			FrontDirection
		);

		CoordSpace WorldSpace(Matrix4x4(1.0f));

		CoordSpace Screenspace = WorldCamera.Perspective * WorldCamera.Viewport * WorldSpace;

		sfn UpdateScreenspace()
		{
			//Screenspace = WorldCamera.Perspective * WorldCamera.Viewport * WorldSpace;
		}
	}


}