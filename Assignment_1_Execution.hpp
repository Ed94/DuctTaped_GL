/*
Title : Execution
Author: Edward R. Gonzalez

Description:
This brings together the functionality I made in DGL and the Actions libraries to produce an OpenGL Workspace.

Currently the workspace is heavily hardcoded and has one light rotating around the specified object. The material for the object is set during prep as well.

All exposed library references are inside the inline namespace right after declaring the namespace execution.

After I have the global objects used, followed by the functionality implementations, and at the very end the main function running the default execution implementation.
*/

#pragma once


// DGL
#include "DGL/DGL.hpp"

// Utility
#include "Actions.hpp"

// Non-Standard C++
#include "Cpp_Alias.hpp"



namespace Execution
{
	inline namespace LibraryReferences
	{
		// DGL

		using DGL::EAxis        ;
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
		using DGL::SetWindowHeader            ;
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

	enum class EModels
	{
		Bunny    ,
		Eight    ,
		Gargoyle ,
		Hand     ,
		Horse    ,
		Sculpture,
		Topology ,
		Torus
	};


	// Globals

	// Assignment 1

	bool Exist = true;   // Determines if the the execution should exit cycler.

	TimeValDec CycleStart                    ,    // Snapshot of cycle loop start time. 
		       CycleEnd                      ,    // Snapshot of cycle loop end   time. 
		       DeltaTime                     ,    // Delta between last cycle start and end. 
		       InputInterval   = 1.0f / 60.0f,    // Interval per second to complete the input   process of the cycle.
		       PhysicsInterval = 1.0f / 60.0f,    // Interval per second to complete the physics process of the cycle. 
		       RenderInterval  = 1.0f / 60.0f ;   // Interval per second to complete the render  process of the cycle.

	Window* DefaultWindow;   // Default window to use for execution.

	double CursorX, CursorY;   // Cursor axis position on the window.

	bool CursorOff  = true,
		 ShowLight  = true,
		 RotateObj  = true,
		 OrbitLight = true ;

	gFloat CamMoveSpeed     =  7.0f,    // Rate at which the camera should move.
		   CamRotationSpeed = 27.0f ;   // Rate at which the camera should rotate.

	TimeValDec InputDelta   = 0.0,    // Current delta since last input   process. 
		       PhysicsDelta = 0.0,    // Current delta since last physics process. 
		       RenderDelta  = 0.0 ;   // Current delta since last render  process.

	ActionQueue ActionsToComplete;   // Actions queue to run during the physics process of the cycle.

	EModels CurrentModel = EModels::Bunny;

	Model Bunny    ("./Models/bunny.obj"       );
	Model Eight    ("./Models/eight.obj"       );
	Model Gargoyle ("./Models/gargoyle.obj"    );
	Model Hand     ("./Models/hand.obj"        );
	Model Horse    ("./Models/horse.obj"       );
	Model Sculpture("./Models/sculpture.obj"   );
	Model Topology ("./Models/topology.obj"    );
	Model Torus    ("./Models/Torus.obj"       );

	DGL::HE_Model BlenderTorus("./Models/torus_Blender.obj");

	Material_Phong ObjectMaterial;   // Material to use on the model.

	Light_Basic  Light       ;   // Hardcoded light. Rotates around object.
	Entity_Basic ObjectToView;   // Object that will be currently in the middle with the light source rotating.

	string 
		windowTitle     = "Assignment 1"    , 
		deltaStr        = "Delta: "         , 
		inputDeltaStr   = "Input Delta: "   , 
		physicsDeltaStr = "Physics Delta: " , 
		renderDeltaStr  = "RenderDeltaStr: " ;

	stringstream WindowTitle;

	// Functionality

	// Input Action functions...

	void ChangeModel()
	{
		if (CurrentModel == EModels::Torus)
		{
			CurrentModel = EModels::Bunny;
		}
		else
		{
			CurrentModel = EModels(int(CurrentModel) + 1);
		}

		switch (CurrentModel)
		{
		case EModels::Bunny:
		{
			if (not Bunny.Ready())
			{
				SetWindowHeader(DefaultWindow, "Assignment 1: Loading Bunny...");

				Bunny.Load();
			}

			ObjectToView.SetModel(Bunny);

			ObjectToView.SetScale(4.0f);

			ObjectToView.SetPosition(Vector3(-0.05, -4.4f, 0));

			ObjectMaterial.Color    = DGL::Colors::White.Vector();
			ObjectMaterial.Ambience = 0.300f                     ;
			ObjectMaterial.Diffuse  = 1.000f                     ;
			ObjectMaterial.Specular = 0.910f                     ;	

			ObjectToView.SetMaterial(ObjectMaterial);

			return;
		}
		case EModels::Eight:
		{
			if (not Eight.Ready())
			{
				SetWindowHeader(DefaultWindow, "Assignment 1: Loading Eight...");

				Eight.Load();
			}

			ObjectToView.SetModel(Eight);

			ObjectToView.SetScale(2.0f);

			ObjectToView.SetPosition(Vector3(0, -1.0, 0));

			ObjectMaterial.Color    = DGL::Colors::Blue.Vector();
			ObjectMaterial.Ambience = 0.112f                    ;
			ObjectMaterial.Diffuse  = 0.828f                    ;
			ObjectMaterial.Specular = 0.421f                    ;

			ObjectToView.SetMaterial(ObjectMaterial);

			return;
		}
		case EModels::Gargoyle:
		{
			if (not Gargoyle.Ready())
			{
				SetWindowHeader(DefaultWindow, "Assignment 1: Loading Gargoyle...");

				Gargoyle.Load();
			}

			ObjectToView.SetModel(Gargoyle);

			ObjectToView.SetPosition(Vector3(-1, -5.4f, 0));

			ObjectToView.SetScale(6.0f);

			ObjectMaterial.Color    = DGL::Colors::Red.Vector();
			ObjectMaterial.Ambience = 0.001f                   ;
			ObjectMaterial.Diffuse  = 0.658f                   ;
			ObjectMaterial.Specular = 0.821f                   ;

			ObjectToView.SetMaterial(ObjectMaterial);

			return;
		}
		case EModels::Hand:
		{
			if (not Hand.Ready())
			{
				SetWindowHeader(DefaultWindow, "Assignment 1: Loading Hand...");

				Hand.Load();
			}

			ObjectToView.SetModel(Hand);

			ObjectToView.SetScale(0.03f);

			ObjectToView.SetPosition(Vector3(0, -1.1f, 0));

			ObjectMaterial.Color    = DGL::Colors::DarkTone.Vector();
			ObjectMaterial.Ambience = 0.000f                        ;
			ObjectMaterial.Diffuse  = 1.000f                        ;
			ObjectMaterial.Specular = 0.640f                        ;

			ObjectToView.SetMaterial(ObjectMaterial);

			SetClearColor(LinearColor(0.53f, 0.53f, 0.53f, 1.0f));

			return;
		}
		case EModels::Horse:
		{
			if (not Horse.Ready())
			{
				SetWindowHeader(DefaultWindow, "Assignment 1: Loading Horse...");

				Horse.Load();
			}

			ObjectToView.SetModel(Horse);

			ObjectToView.SetScale(20.0f);

			ObjectToView.Rotate(90.0f, EAxis::X);

			ObjectToView.SetPosition(Vector3(0, 0.0f, 0));

			ObjectMaterial.Color    = DGL::Colors::Green.Vector();
			ObjectMaterial.Ambience = 0.000f                     ;
			ObjectMaterial.Diffuse  = 1.000f                     ;
			ObjectMaterial.Specular = 0.640f                     ;

			ObjectToView.SetMaterial(ObjectMaterial);

			SetClearColor(LinearColor(0.02f, 0.02f, 0.02f, 1.0f));

			return;
		}
		case EModels::Sculpture:
		{
			if (not Sculpture.Ready())
			{
				SetWindowHeader(DefaultWindow, "Assignment 1: Loading Sculpture...");

				Sculpture.Load();
			}

			ObjectToView.SetModel(Sculpture);

			ObjectToView.SetScale(0.01f);

			ObjectToView.Rotate(-90.0f, EAxis::X);

			ObjectMaterial.Color    = DGL::Colors::WarmSphia.Vector();
			ObjectMaterial.Ambience = 0.112f                         ;
			ObjectMaterial.Diffuse  = 0.928f                         ;
			ObjectMaterial.Specular = 0.21f                          ;

			ObjectToView.SetMaterial(ObjectMaterial);

			return;
		}
		case EModels::Topology:
		{
			if (not Topology.Ready())
			{
				SetWindowHeader(DefaultWindow, "Assignment 1: Loading Topology...");

				Topology.Load();
			}

			ObjectToView.SetModel(Topology);

			ObjectToView.SetScale(0.2f);

			ObjectMaterial.Color    = DGL::Colors::Coral.Vector();
			ObjectMaterial.Ambience = 0.212f                     ;
			ObjectMaterial.Diffuse  = 0.728f                     ;
			ObjectMaterial.Specular = 0.41f                      ;

			ObjectToView.SetMaterial(ObjectMaterial);

			return;
		}
		case EModels::Torus:
		{
			if (not Torus.Ready())
			{
				SetWindowHeader(DefaultWindow, "Assignment 1: Loading Torus...");

				Torus.Load();
			}

			ObjectToView.SetModel(Torus);

			ObjectToView.SetScale(1.0f);

			ObjectMaterial.Color    = DGL::Colors::Grey.Vector();
			ObjectMaterial.Ambience = 0.170f                    ;
			ObjectMaterial.Diffuse  = 0.720f                    ;
			ObjectMaterial.Specular = 0.100f                    ;

			ObjectToView.SetMaterial(ObjectMaterial);

			return;
		}
		}
	}

	void ToggleLight()
	{
		ShowLight ? ShowLight = false : ShowLight = true;

		return;
	}

	void ToggleModelRotation()
	{
		RotateObj ? RotateObj = false : RotateObj = true;

		return;
	}

	void ToogleLightOrbit()
	{
		OrbitLight ? OrbitLight = false : OrbitLight = true;

		return;
	}

	void RotateCamera(ERotationAxis _rotationAxis, gFloat _rotationAmount, double _delta)
	{
		WorldCamera.Rotate(_rotationAxis, _rotationAmount, gFloat(_delta));

		return;
	} 
	
	void MoveCamera(EDirection _direction, gFloat _translationAmount, double _delta)
	{
		WorldCamera.Move(_direction, _translationAmount, gFloat(_delta));

		return;
	} 
	
	void ModifyCamSpeed(bool _isPositive, double _delta)
	{
		if (_isPositive)
		{
			CamMoveSpeed += CamMoveSpeed * gFloat(_delta);

			return;
		}
		else
		{
			CamMoveSpeed -= CamMoveSpeed * gFloat(_delta);

			return;
		}
	}
	
	// End of common input functions...


	void UpdateWindowDeltaTitle()
	{
		WindowTitle.str("");

		WindowTitle 
			<< windowTitle                     << "  " 
			<< deltaStr        << DeltaTime    << "  " 
			<< inputDeltaStr   << InputDelta   << "  " 
			<< physicsDeltaStr << PhysicsDelta << "  " 
			<< renderDeltaStr  << RenderDelta         ;
	}


	// Currently Does everything required before entering the cycler.
	void PrepWorkspace()
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
	}

	void SetupEntities()
	{
		Light.Load();

		Bunny.Load();

		ObjectMaterial.Color = DGL::Colors::White.Vector();
		ObjectMaterial.Ambience = 0.300f;
		ObjectMaterial.Diffuse = 1.000f;
		ObjectMaterial.Specular = 0.910f;

		ObjectToView = Entity_Basic(Bunny, ObjectMaterial);

		ObjectToView.SetScale(4.0f);

		ObjectToView.SetPosition(Vector3(-0.05, -4.4f, 0));
	}

	/*
	Cycles the process of what to do while a window is open.

	The input, physics, and render procedures can be specified with extra functionality by specifying delegates to those procedures.

	Cycler is hardcoded to exit if escape key is pressed.
	*/
	void Cycler(const function< void(Window*)>& _inputProcedure, const function< void()>& _physicsProcedure, const function< void()>& _renderProcedure)
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

				GetCursorPosition(DefaultWindow, &CursorX, &CursorY);

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



	// Input functionality delegates (These are made due to a limitation with the actions implementation).

	auto ChangeModelDelegate         = function<decltype(ChangeModel        )>(ChangeModel        );
	auto ToogleLightDelegate         = function<decltype(ToggleLight        )>(ToggleLight        );
	auto ToggleModelRotationDelegate = function<decltype(ToggleModelRotation)>(ToggleModelRotation);
	auto ToggleLightOrbitDelegate    = function<decltype(ToogleLightOrbit   )>(ToogleLightOrbit   );
	auto RotateCamDelegate           = function<decltype(RotateCamera       )>(RotateCamera       );
	auto MoveCamDelegate             = function<decltype(MoveCamera         )>(MoveCamera         );
	auto ModifyCamSpeedDelegate      = function<decltype(ModifyCamSpeed     )>(ModifyCamSpeed     );
	auto SetPolyModeDelegate         = function<decltype(SetPolygonMode     )>(SetPolygonMode     );

	void InputProcedure(Window* _currentWindowContext)
	{
		static bool F1_Held = false, 
			        F4_Held = false, 
			        F5_Held = false, 
			        H_Held  = false, 
			        M_Held  = false ;

		if (!KeyPressed(_currentWindowContext, EKeyCodes::F1)) F1_Held = false;
		if (!KeyPressed(_currentWindowContext, EKeyCodes::F4)) F4_Held = false;
		if (!KeyPressed(_currentWindowContext, EKeyCodes::F5)) F5_Held = false;
		if (!KeyPressed(_currentWindowContext, EKeyCodes::H )) H_Held  = false;
		if (!KeyPressed(_currentWindowContext, EKeyCodes::M )) M_Held  = false;

		// TODO: Not necessary for now and throws memory error.
		/*if (KeyPressed(_currentWindowContext, EKeyCodes::F1) && not F1_Held)
		{
			ECursorMode cursorMode = ECursorMode(GetMouseInputMode(DefaultWindow, EMouseMode::Cursor));

			auto delegate    = function<decltype(SetInputMode<ECursorMode>)>(SetInputMode<ECursorMode>);
			auto delegateRaw = function<decltype(SetInputMode<EBool      >)>(SetInputMode<EBool      >);

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

			F1_Held = true;
		}*/

		if (KeyPressed(_currentWindowContext, EKeyCodes::H) && not H_Held)
		{
			H_Held = true;

			ActionsToComplete.AddToQueue(ToogleLightDelegate);
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::M) && not M_Held)
		{
			M_Held = true;

			ActionsToComplete.AddToQueue(ChangeModelDelegate);
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
		
		if (KeyPressed(_currentWindowContext, EKeyCodes::F4) && not F4_Held)
		{
			ActionsToComplete.AddToQueue(ToggleModelRotationDelegate);

			F4_Held = true;
		}

		if (KeyPressed(_currentWindowContext, EKeyCodes::F5) && not F5_Held)
		{
			ActionsToComplete.AddToQueue(ToggleLightOrbitDelegate);

			F5_Held = true;
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

		return;
	}

	void PhysicsProcedure()
	{
		WorldCamera.UpdateCamera();

		UpdateScreenspace();

		if (OrbitLight)
		{
			Light.Update(gFloat(PhysicsDelta));
		}

		if (RotateObj)
		{
			ObjectToView.Rotate(-1.0f * gFloat(PhysicsDelta), EAxis::Y);
		}

		ObjectToView.Update();

		UpdateWindowDeltaTitle();

		return;
	}
	
	void RenderProcedure()
	{
		SetWindowHeader(DefaultWindow, WindowTitle.str());

		if (ShowLight)
		{
			Light.Render(WorldCamera.Perspective, WorldCamera.Viewport);
		}

		ObjectToView.Render(WorldCamera.Perspective, WorldCamera.Viewport, Light.GetPosition(), Light.GetColor());

		return;
	}
	

	// Runtime Execution: Default Execution

	int Execute_Assignment1()
	{
		PrepWorkspace();

		SetupEntities();

		Cycler(InputProcedure, PhysicsProcedure, RenderProcedure);

		// TODO: There should be more to properly close...

		DestoryWindow(DefaultWindow);

		TerminateGLFW();

		return EXIT_SUCCESS;
	}
}
