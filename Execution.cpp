/*



*/



// DGL
#include "DGL.hpp"

// Utility
#include "Actions.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace Execution
{
	inline namespace LibraryReferences
	{
		// DGL

		using DGL::EBool        ;
		using DGL::ECursorMode  ;
		using DGL::EDirection   ;
		using DGL::EFrameBuffer ;
		using DGL::EKeyCodes    ;
		using DGL::EMouseMode   ;
		using DGL::EPrimitives  ;
		using DGL::ERotationAxis;

		using DGL::Camera        ;
		using DGL::Entity_Basic  ;
		using DGL::gFloat        ;
		using DGL::Light_Basic   ;
		using DGL::LinearColor   ;
		using DGL::Material_Phong;
		using DGL::Matrix        ;
		using DGL::Matrix4x4     ;
		using DGL::Model         ;
		using DGL::TimeValDec    ;
		using DGL::Vector3       ;
		using DGL::Window        ;

		using DGL::BindVertexArray            ;
		using DGL::CanUseRawMouse             ;
		using DGL::ClearBuffer                ;
		using DGL::CreateWindow               ;
		using DGL::DestoryWindow              ;
		using DGL::DisableVertexAttributeArray;
		using DGL::EnableDepth                ;
		using DGL::EnableVertexAttributeArray ;
		using DGL::GetCursorPosition          ;
		using DGL::GetTime                    ;
		using DGL::InitalizeGLEW              ;
		using DGL::InitalizeGLFW              ;
		using DGL::LoadDefaultShaders         ;
		using DGL::KeyPressed                 ;	
		using DGL::NotShared                  ;
		using DGL::PollEvents                 ;
		using DGL::ResetCursor                ;
		using DGL::SetClearColor              ;
		using DGL::SetCurrentContext          ;
		using DGL::SetInputMode               ;
		using DGL::SetPolygonMode             ;
		using DGL::SetUniformVariable_MVA     ;
		using DGL::SwapBuffers                ;
		using DGL::UseProgramShader           ;
		using DGL::TerminateGLFW              ;
		using DGL::WindowedMode               ;

		using DGL::DefaultSpace::ScreenWidth       ;
		using DGL::DefaultSpace::ScreenHeight      ;
		using DGL::DefaultSpace::ScreenCenterHeight;
		using DGL::DefaultSpace::ScreenCenterWidth ; 
		using DGL::DefaultSpace::Screenspace       ;
		using DGL::DefaultSpace::WorldCamera       ;

		using DGL::DefaultSpace::UpdateScreenspace;

		// Actions

		using Actions::ActionQueue;
	}


	// Globals

	bool Exist = true;   // Determines if the the execution should exit cycler.

	TimeValDec CycleStart                     ,    // Snapshot of cycle loop start time. 
		       CycleEnd                       ,    // Snapshot of cycle loop end   time. 
		       DeltaTime                      ,    // Delta between last cycle start and end. 
		       InputInterval   = 1.0f / 480.0f,    // Interval per second to complete the input   process of the cycle.
		       PhysicsInterval = 1.0f / 240.0f,    // Interval per second to complete the physics process of the cycle. 
		       RenderInterval  = 1.0f / 144.0f ;   // Interval per second to complete the render  process of the cycle.

	ptr<Window> DefaultWindow;   // Default window to use for execution.

	double CursorX, CursorY;   // Cursor axis position on the window.

	bool CursorOff = true;

	gFloat CamMoveSpeed     =  8.0f,    // Rate at which the camera should move.
		   CamRotationSpeed = 27.0f ;   // Rate at which the camera should rotate.

	TimeValDec InputDelta   = 0.0,    // Current delta since last input   process. 
		       PhysicsDelta = 0.0,    // Current delta since last physics process. 
		       RenderDelta  = 0.0 ;   // Current delta since last render  process.

	ActionQueue ActionsToComplete;   // Actions queue to run during the physics process of the cycle.

	Model objectModel("torus.obj");   // Hardcoded to specified obj file for now.

	Material_Phong ObjectMaterial;

	Light_Basic  Light       ;   // Hardcoded light. Rotates around object.
	Entity_Basic ObjectToView;   // Object that will be currently in the middle with the light source rotating.




	// Functionality

	
	
	// Input Action common functions...

	sfn RotateCamera(ERotationAxis _rotationAxis, gFloat _rotationAmount, double _delta)
	{
		WorldCamera.Rotate(_rotationAxis, _rotationAmount, gFloat(_delta));
	} 
	
	deduce RotateCamDelegate = Delegate<decltype(RotateCamera)>(RotateCamera);


	sfn MoveCamera(EDirection _direction, gFloat _translationAmount, double _delta)
	{
		WorldCamera.Move(_direction, _translationAmount, gFloat(_delta));
	} 
	
	deduce MoveCamDelegate = Delegate<decltype(MoveCamera)>(MoveCamera);


	// This is here cause its super repetitive..
	sfn ModifyCamSpeed(bool _isPositive, double _delta)
	{
		if (_isPositive)
		{
			CamMoveSpeed += CamMoveSpeed * gFloat(_delta);
		}
		else
		{
			CamMoveSpeed -= CamMoveSpeed * gFloat(_delta);
		}
	}

	deduce ModifyCamSpeedDelegate = Delegate<decltype(ModifyCamSpeed)>(ModifyCamSpeed);
	deduce SetPolyModeDelegate    = Delegate<decltype(SetPolygonMode)>(SetPolygonMode);


	// End of common input functions...



	// Currently Does everything required before entering the cycler.
	sfn PrepWorkspace()
	{
		// Baseline

		InitalizeGLFW();

		DefaultWindow = CreateWindow(ScreenWidth, ScreenHeight, "Assignment 1: Loading Model...", WindowedMode(), NotShared());

		SetCurrentContext(DefaultWindow);

		InitalizeGLEW();   // Glew must initialize only after a context is set.

		EnableDepth();

		SetPolygonMode(DGL::EFace::Front_and_Back, DGL::ERenderMode::Fill);

		// Cursor stuff

		SetInputMode(DefaultWindow, DGL::EMouseMode::Cursor, DGL::ECursorMode::Disable);

		ResetCursor(DefaultWindow, ScreenCenterWidth, ScreenCenterHeight);

		if (CanUseRawMouse())
		{
			SetInputMode(DefaultWindow, DGL::EMouseMode::RawMouse, DGL::EBool::True);
		}

		// End of cursor stuff...

		// Shaders

		LoadDefaultShaders();

		// Entities

		Light.Load();

		objectModel.Load  ();
		objectModel.Buffer();

		ObjectMaterial.Color    = DGL::Colors::GreyColor.Vector();
		ObjectMaterial.Ambience = 0.01f                          ;
		ObjectMaterial.Diffuse  = 1.0f                           ;
		ObjectMaterial.Specular = 0.4f                           ;

		ObjectToView = Entity_Basic(objectModel, ObjectMaterial);
	}


	/*
	Cycles the process of what to do while a window is open.

	The input, physics, and render procedures can be specified with extra functionality by specifying delegates to those procedures.

	Cycler is hardcoded to exit if escape key is pressed.
	*/
	sfn Cycler(ro Ref(Delegate< Func<void, ptr<Window>> >) _inputProcedure, ro Ref(Delegate< Func<void>>) _physicsProcedure, ro Ref(Delegate< Func<void>>) _renderProcedure)
	{
		while (Exist)
		{
			CycleStart = GetTime();

			if (InputDelta >= InputInterval)
			{
				PollEvents();

				if (KeyPressed(DefaultWindow, EKeyCodes::Escape))
				{
					Exist = false;
				}

				GetCursorPosition(DefaultWindow, Address(CursorX), Address(CursorY));

				_inputProcedure(DefaultWindow);

				if (CursorOff)
				{
					ResetCursor(DefaultWindow, ScreenCenterWidth, ScreenCenterHeight);
				}
			}

			if (PhysicsDelta >= PhysicsInterval)
			{
				while (ActionsToComplete.HasAction())
				{
					ActionsToComplete.DoNextAction();
				}

				_physicsProcedure();

				PhysicsDelta = 0.0;
			}

			if (RenderDelta >= RenderInterval)
			{
				ClearBuffer(EFrameBuffer::Color, EFrameBuffer::Depth);

				SetClearColor(LinearColor(0.12f, 0.12f, 0.12f, 1.0f));

				_renderProcedure();

				SwapBuffers(DefaultWindow);

				RenderDelta = 0.0;
			}

			if (InputDelta >= InputInterval)
			{
				InputDelta = 0.0;
			}

			CycleEnd = GetTime();

			DeltaTime = CycleEnd - CycleStart;

			InputDelta   += DeltaTime;
			PhysicsDelta += DeltaTime;
			RenderDelta  += DeltaTime;
		}

		return;
	}

	
	sfn InputProcedure(ptr<Window> _currentWindowContext)
	{
		if (KeyPressed(_currentWindowContext, EKeyCodes::F1))
		{
			ECursorMode cursorMode = ECursorMode(GetMouseInputMode(DefaultWindow, EMouseMode::Cursor));

			deduce delegate    = Delegate<decltype(SetInputMode<ECursorMode>)>(SetInputMode<ECursorMode>);
			deduce delegateRaw = Delegate<decltype(SetInputMode<EBool      >)>(SetInputMode<EBool      >);

			if (cursorMode == ECursorMode::Normal || cursorMode == ECursorMode::Hidden)
			{
				ActionsToComplete.AddToQueue(delegate   , _currentWindowContext, EMouseMode::Cursor  , ECursorMode::Disable);
				ActionsToComplete.AddToQueue(delegateRaw, _currentWindowContext, EMouseMode::RawMouse, EBool      ::True   );

				CursorOff = true;
			}
			else
			{
				ActionsToComplete.AddToQueue(delegate   , _currentWindowContext, EMouseMode::Cursor  , ECursorMode::Normal);
				ActionsToComplete.AddToQueue(delegateRaw, _currentWindowContext, EMouseMode::RawMouse, EBool      ::False );

				CursorOff = false;
			}
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::UpArrow))
		{
			ActionsToComplete.AddToQueue(ModifyCamSpeedDelegate, true, PhysicsDelta);
		}

		if (KeysPressed(_currentWindowContext, EKeyCodes::DnArrow))
		{
			ActionsToComplete.AddToQueue(ModifyCamSpeedDelegate, false, PhysicsDelta);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::F2))
		{
			ActionsToComplete.AddToQueue(SetPolyModeDelegate, DGL::EFace::Front_and_Back, DGL::ERenderMode::Line);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::F3))
		{
			ActionsToComplete.AddToQueue(SetPolyModeDelegate, DGL::EFace::Front_and_Back, DGL::ERenderMode::Fill);
		}

		if (CursorOff)
		{
			if (CursorX != 0)
			{
				ActionsToComplete.AddToQueue(RotateCamDelegate, ERotationAxis::Yaw, gFloat(CursorX) * CamRotationSpeed, PhysicsDelta);
			}

			if (CursorY != 0)
			{
				ActionsToComplete.AddToQueue(RotateCamDelegate, ERotationAxis::Pitch, gFloat(CursorY) * CamRotationSpeed, PhysicsDelta);
			}
		}
		
		if (KeyPressed(_currentWindowContext, EKeyCodes::E))
		{
			ActionsToComplete.AddToQueue(MoveCamDelegate, EDirection::Up, CamMoveSpeed, PhysicsDelta);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::Q))
		{
			ActionsToComplete.AddToQueue(MoveCamDelegate, EDirection::Down, CamMoveSpeed, PhysicsDelta);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::A))
		{
			ActionsToComplete.AddToQueue(MoveCamDelegate, EDirection::Left, CamMoveSpeed, PhysicsDelta);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::D))
		{
			ActionsToComplete.AddToQueue(MoveCamDelegate, EDirection::Right, CamMoveSpeed, PhysicsDelta);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::W))
		{
			ActionsToComplete.AddToQueue(MoveCamDelegate, EDirection::Forward, CamMoveSpeed, PhysicsDelta);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::S))
		{
			ActionsToComplete.AddToQueue(MoveCamDelegate, EDirection::Backward, CamMoveSpeed, PhysicsDelta);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::I))
		{
			ActionsToComplete.AddToQueue(RotateCamDelegate, ERotationAxis::Pitch, -6.0f * CamRotationSpeed, PhysicsDelta);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::K))
		{
			ActionsToComplete.AddToQueue(RotateCamDelegate, ERotationAxis::Pitch, 6.0f * CamRotationSpeed, PhysicsDelta);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::J))
		{
			ActionsToComplete.AddToQueue(RotateCamDelegate, ERotationAxis::Yaw, -6.0f * CamRotationSpeed, PhysicsDelta);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::L))
		{
			ActionsToComplete.AddToQueue(RotateCamDelegate, ERotationAxis::Yaw, 6.0f * CamRotationSpeed, PhysicsDelta);
		}
	}


	std::string 
		windowTitle     = "Assignment 1"    , 
		deltaStr        = "Delta: "         , 
		inputDeltaStr   = "Input Delta: "   , 
		physicsDeltaStr = "Physics Delta: " , 
		renderDeltaStr  = "RenderDeltaStr: " ;

	std::stringstream somethingtoupdate;

	sfn UpdateWindowDeltaTitle()
	{
		somethingtoupdate.str("");

		somethingtoupdate 
			<< windowTitle                     << "  " 
			<< deltaStr        << DeltaTime    << "  " 
			<< inputDeltaStr   << InputDelta   << "  " 
			<< physicsDeltaStr << PhysicsDelta << "  " 
			<< renderDeltaStr  << RenderDelta         ;
	}

	sfn PhysicsProcedure()
	{
		WorldCamera.UpdateCamera();

		UpdateScreenspace();

		Light.Update();

		ObjectToView.Update();

		UpdateWindowDeltaTitle();
	}
	
	sfn RenderProcedure() -> void
	{
		glfwSetWindowTitle(DefaultWindow, somethingtoupdate.str().c_str());

		Light.Render(WorldCamera.Perspective, WorldCamera.Viewport);

		ObjectToView.Render(WorldCamera.Perspective, WorldCamera.Viewport, Light.GetPosition(), Light.GetColor());
	}
	


	// Runtime Execution: Default Execution

	sfn Execute() -> ExitCode
	{
		PrepWorkspace();

		Cycler(InputProcedure, PhysicsProcedure, RenderProcedure);

		// TODO: There should be more to properly close...

		DestoryWindow(DefaultWindow);

		TerminateGLFW();

		return ExitCode::Success;
	}
}


// Currently only can do one execution route.
int main(void)
{
	return int(Execution::Execute());
}

