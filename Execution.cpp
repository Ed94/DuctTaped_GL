// Cpp STL

// Project
#include "DGL.hpp"
#include "TriangleRaw.hpp"

#include "Cpp_Alias.hpp"


#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Execution
{
	// C++ STL

	using std::thread;


	
	template<typename Type>
	using UPtr = std::unique_ptr<Type>;

	template<typename Type, typename... ParamTypes>
	sfn MakeUPtr(rRef(ParamTypes)... _params) -> UPtr<Type>
	{
		return std::make_unique<Type>(_params...);
	}



	// GL

	using DGL::TimeValDec;
	using DGL::Window    ;
	using DGL::Matrix    ;
	using DGL::Matrix4x4 ;
	using DGL::Vector3   ;
	using DGL::gFloat    ;


	bool Exist = true;

	TimeValDec CycleStart, CycleEnd, DeltaTime, InputPollingInterval = 1.0f / 240.0f, PhysicsInterval = 1.0f / 120.0f, RenderInterval = 1.0f / 60.0f;

	UPtr<thread> BasicLoop, BasicTimedLoop;

	ptr<Window> DefaultWindow;


	gFloat CamMoveSpeed = 3.0f, CamRotationSpeed = 1.0f;



	enum class CameraActions
	{

	};

	enum class EMovement
	{

	};



	template<typename Device>
	struct ActionBind
	{


		Delegate< Func<void>> Action;
	};



	struct Keyboard
	{
		
	};

	struct ActionBinds
	{
		static ActionBind<Keyboard> MoveFoward;
	};



	// Testing

	// Forward Declares

	sfn RenderProcedure     () -> void;
	sfn PrepareRenderObjects() -> void;

	sfn CreateWindow_BasicLoop()
	{
		DGL::InitalizeGLFW();

		deduce windowObj = DGL::CreateWindow(720, 540, "Assignment 1: RawLoop", DGL::WindowedMode(), DGL::NotShared());

		DGL::SetCurrentContext(windowObj);

		DGL::RunBasicWindowLoop(windowObj);
	}

	sfn CreateWindow_TimedRender()
	{
		DGL::InitalizeGLFW();

		deduce windowObj = DGL::CreateWindow(720, 540, "Assignment 1: Timed Render", DGL::WindowedMode(), DGL::NotShared());

		DGL::SetCurrentContext(windowObj);

		DGL::InitalizeGLEW();

		PrepareRenderObjects();

		DGL::RunBasicWindowLoop_Timed(windowObj, 1.0 / 60.0, Address(RenderProcedure));
	}

	sfn PrepareRenderObjects() -> void
	{
		RAW_SetupBuffers();

		RAW_BindAndBufferDataToIDs();

		DGL::LoadDefaultShaders();

		DGL::FormatVertexAttributes<Vertex3>(VertexAttributeIndex, EDataType::Float, ZeroOffset(), Vertex3::ValueCount(), EBool::False);

		DGL::BindBuffer(EBufferTarget::VertexAttributes, 0);   // Dunno. Prob unbinding...

		DGL::BindVertexArray(0);
	}

	using DGL::EFrameBuffer;
	using DGL::LinearColor;
	using DGL::DefaultSpace::WorldCamera;

	using std::cout; using std::endl;

	using DGL::KeyPressed;
	using DGL::EKeyCodes;

	using DGL::DefaultSpace::WorldCamera;
	using DGL::EDirection;


	struct CamShouldMove
	{
		bool Up, Down, Left, Right, Forward, Backward;

		short int numActive;
	};

	CamShouldMove CamMoveRequests = { false, false, false, false, false, false, 0 };

	double CursorX, CursorY;

	using DGL::DefaultSpace::ScreenCenterWidth; using DGL::DefaultSpace::ScreenCenterHeight;


	TimeValDec InputDelta = 0.0, PhysicsDelta = 0.0, RenderDelta = 0.0;

	using DGL::DefaultSpace::Screenspace;

	sfn Cycler(Delegate< Func<void, ptr<Window>> > _inputProcedure, Delegate< Func<void>> _renderProcedure)
	{
		while (Exist)
		{
			CycleStart = DGL::GetTime();

			if (InputDelta >= InputPollingInterval)
			{
				DGL::PollEvents();

				glfwGetCursorPos(DefaultWindow, Address(CursorX), Address(CursorY));

				_inputProcedure(DefaultWindow);

				DGL::ResetCursor(DefaultWindow, ScreenCenterWidth, ScreenCenterHeight);

				InputDelta = 0.0;
			}

			if (PhysicsDelta >= PhysicsInterval)
			{
				if (CamMoveRequests.Up      ) WorldCamera.Move(EDirection::Up      , CamMoveSpeed / CamMoveRequests.numActive, PhysicsDelta);
				if (CamMoveRequests.Down    ) WorldCamera.Move(EDirection::Down    , CamMoveSpeed / CamMoveRequests.numActive, PhysicsDelta);
				if (CamMoveRequests.Left    ) WorldCamera.Move(EDirection::Left    , CamMoveSpeed / CamMoveRequests.numActive, PhysicsDelta);
				if (CamMoveRequests.Right   ) WorldCamera.Move(EDirection::Right   , CamMoveSpeed / CamMoveRequests.numActive, PhysicsDelta);
				if (CamMoveRequests.Forward ) WorldCamera.Move(EDirection::Forward , CamMoveSpeed / CamMoveRequests.numActive, PhysicsDelta);
				if (CamMoveRequests.Backward) WorldCamera.Move(EDirection::Backward, CamMoveSpeed / CamMoveRequests.numActive, PhysicsDelta);

				WorldCamera.UpdateCamera();

				//WorldCamera.Position.y = 0.0;
				//WorldCamera.LookAtPosition.y = 0.0;

				DGL::DefaultSpace::UpdateScreenspace();

				CamMoveRequests = { false, false, false, false, false, false, 0 };

				PhysicsDelta = 0.0;
			}

			if (RenderDelta >= RenderInterval)
			{
				DGL::ClearBuffer(EFrameBuffer::Color, EFrameBuffer::Depth);

				DGL::SetClearColor(LinearColor(0.12f, 0.12f, 0.12f, 1.0f));

				_renderProcedure();

				DGL::SwapBuffers(DefaultWindow);

				RenderDelta = 0.0;
			}

			CycleEnd = DGL::GetTime();

			DeltaTime = CycleEnd - CycleStart;

			InputDelta   += DeltaTime;
			PhysicsDelta += DeltaTime;
			RenderDelta  += DeltaTime;
		}

		return;
	}

	using DGL::EMouseMode; using DGL::ECursorMode;

	using DGL::DefaultSpace::ScreenCenterWidth; using DGL::DefaultSpace::ScreenCenterHeight;

	sfn InputProcedure(ptr<Window> _currentWindowContext)
	{
		if (KeysPressed(_currentWindowContext, EKeyCodes::LeftShift, EKeyCodes::F1))
		{
			ECursorMode cursorMode = ECursorMode(GetMouseInputMode(DefaultWindow, EMouseMode::Cursor));

			if (cursorMode == ECursorMode::Normal || cursorMode == ECursorMode::Hidden)
			{
				SetInputMode(_currentWindowContext, EMouseMode::Cursor, ECursorMode::Disable);
			}
			else
			{
				SetInputMode(_currentWindowContext, EMouseMode::Cursor, ECursorMode::Normal);
			}
		}

		if (CursorX != 0)//!= ScreenCenterWidth)
		{
			WorldCamera.Rotate(DGL::ERotationAxis::Yaw, CursorX * CamMoveSpeed, InputDelta);

			//cout << "Cursor X: " << CursorX << endl;
		}

		if (CursorY != 0)
		{
			WorldCamera.Rotate(DGL::ERotationAxis::Pitch, CursorY * CamMoveSpeed, InputDelta);

			//cout << "Cursor Y: " << CursorX << endl;
		}
		
		if (KeyPressed(_currentWindowContext, EKeyCodes::E))
		{
			WorldCamera.Move(EDirection::Up, CamMoveSpeed, InputDelta);

			CamMoveRequests.Up = true;

			CamMoveRequests.numActive++;
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::Q))
		{
			WorldCamera.Move(EDirection::Down, CamMoveSpeed, InputDelta);

			CamMoveRequests.Down = true;

			CamMoveRequests.numActive++;
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::A))
		{
			WorldCamera.Move(EDirection::Left, CamMoveSpeed, InputDelta);

			CamMoveRequests.Left = true;

			CamMoveRequests.numActive++;
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::D))
		{
			WorldCamera.Move(EDirection::Right, CamMoveSpeed, InputDelta);

			CamMoveRequests.Right = true;

			CamMoveRequests.numActive++;
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::W))
		{
			WorldCamera.Move(EDirection::Forward, CamMoveSpeed, InputDelta);

			CamMoveRequests.Forward = true;

			CamMoveRequests.numActive++;
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::S))
		{
			WorldCamera.Move(EDirection::Backward, CamMoveSpeed, InputDelta);

			CamMoveRequests.Backward = true;

			CamMoveRequests.numActive++;
		}
	}

	sfn RenderProcedure() -> void
	{
		DGL::EnableVertexAttributeArray(VertexAttributeIndex);

		DGL::SetPolygonMode(DGL::EFace::Front_and_Back, DGL::ERenderMode::Line);

		DGL::DefaultSpace::WorldSpace = DGL::Rotate(DGL::DefaultSpace::WorldSpace, 0.0035f, Vector3(0.0f, 0.5f, 0.0f));

		DGL::DefaultSpace::Screenspace = DGL::DefaultSpace::WorldCamera.Perspective * DGL::DefaultSpace::WorldCamera.Viewport * DGL::DefaultSpace::WorldSpace;

		DGL::UseProgramShader(DGL::SimpleShader_Transformed);

		DGL::SetUniformVariable_MatrixVariableArray(DGL::ScreenSpaceVarID, 1, DGL::EBool::False, Address(DGL::DefaultSpace::Screenspace[0][0]));

		DGL::BindVertexArray(VertexArrayObj);

		DGL::DrawArrays(DGL::EPrimitives::Triangles, 0, TriangleRaw::VertexCount());

		DGL::DrawElements(DGL::EPrimitives::Triangles, 6, EDataType::UnsignedInt, ZeroOffset());

		//GL::BindVertexArray(0);

		DGL::DisableVertexAttributeArray(VertexAttributeIndex);
	}

	sfn PrepWorkspace()
	{
		using DGL::DefaultSpace::ScreenWidth ;
		using DGL::DefaultSpace::ScreenHeight;

		DGL::InitalizeGLFW();

		DefaultWindow = DGL::CreateWindow(ScreenWidth, ScreenHeight, "Assignment 1: Cam Movement Test", DGL::WindowedMode(), DGL::NotShared());

		DGL::SetCurrentContext(DefaultWindow);

		DGL::SetInputMode(DefaultWindow, DGL::EMouseMode::Cursor, DGL::ECursorMode::Disable);

		DGL::ResetCursor(DefaultWindow, ScreenCenterWidth, ScreenCenterHeight);

		if (DGL::CanUseRawMouse())
		{
			DGL::SetInputMode(DefaultWindow, DGL::EMouseMode::RawMouse, DGL::EBool::True);
		}

		DGL::InitalizeGLEW();

		PrepareRenderObjects();
	}


	// Runtime Execution: Loop must be specified here to use.

	sfn Execute() -> ExitCode
	{
		PrepWorkspace();

		Cycler(InputProcedure, RenderProcedure);

		DGL::TerminateGLFW();

		return ExitCode::Success;
	}
}



int main(void)
{
	return int(Execution::Execute());
}

