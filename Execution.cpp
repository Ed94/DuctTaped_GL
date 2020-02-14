// Cpp STL
#include <chrono>
#include <thread>
#include <memory>



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

	using GL::TimeValDec;
	using GL::Window    ;
	using GL::Matrix    ;
	using GL::Matrix4x4 ;
	using GL::Vector3   ;
	using GL::gFloat    ;


	bool Exist = true;

	TimeValDec CycleStart, CycleEnd, DeltaTime, InputPollingInterval = 1.0f / 240.0f, RenderInterval = 1.0f / 60.0f;

	UPtr<thread> BasicLoop, BasicTimedLoop;



	// Testing

	// Foward Declares

	sfn RenderProcedure     () -> void;
	sfn PrepareRenderObjects() -> void;

	sfn CreateWindow_BasicLoop()
	{
		GL::InitalizeGLFW();

		deduce windowObj = GL::CreateWindow(720, 450, "Assignment 1: RawLoop", GL::WindowedMode(), GL::NotShared());

		GL::SetCurrentContext(windowObj);

		GL::RunBasicWindowLoop(windowObj);
	}

	sfn CreateWindow_TimedRender()
	{
		GL::InitalizeGLFW();

		deduce windowObj = GL::CreateWindow(720, 540, "Assignment 1: Timed Render", GL::WindowedMode(), GL::NotShared());

		GL::SetCurrentContext(windowObj);

		GL::InitalizeGLEW();

		PrepareRenderObjects();

		GL::RunBasicWindowLoop_Timed(windowObj, 1.0 / 60.0, Address(RenderProcedure));
	}

	sfn PrepareRenderObjects() -> void
	{
		RAW_SetupBuffers();

		RAW_BindAndBufferDataToIDs();

		GL::LoadDefaultShaders();

		GL::FormatVertexAttributes<Vertex3>(VertexAttributeIndex, EDataType::Float, ZeroOffset(), Vertex3::ValueCount(), EBool::True);

		GL::BindBuffer(EBufferTarget::VertexAttributes, 0);   // Dunno. Prob unbinding...

		GL::BindVertexArray(0);
	}

	sfn Cycler(FnPtr<void> _inputProcedure, FnPtr<void> _renderProcedure)
	{
		while (Exist)
		{
			CycleStart = GL::GetTime();

			if (DeltaTime >= InputPollingInterval)
			{
				GL::PollEvents();

				_inputProcedure();
			}

			if (DeltaTime >= RenderInterval)
			{
				_renderProcedure();
			}

			CycleEnd = GL::GetTime();

			DeltaTime = DeltaTime + CycleEnd - CycleStart;
		}

		return;
	}

	sfn InputProcedure()
	{

	}

	sfn RenderProcedure() -> void
	{
		GL::EnableVertexAttributeArray(VertexAttributeIndex);

		GL::SetPolygonMode(GL::EFace::Front_and_Back, GL::EMode::Fill);

		GL::DefaultSpace::WorldSpace = GL::Rotate(GL::DefaultSpace::WorldSpace, 0.0035f, Vector3(0.0f, 0.5f, 0.0f));

		GL::DefaultSpace::Screenspace = GL::DefaultSpace::WorldCamera.Perspective * GL::DefaultSpace::WorldCamera.ViewPort * GL::DefaultSpace::WorldSpace;

		GL::UseProgramShader(GL::SimpleShader_Transformed);

		GL::SetUniformVariable_MatrixVariableArray(GL::ScreenSpaceVarID, 1, GL::EBool::False, Address(GL::DefaultSpace::Screenspace[0][0]));

		GL::BindVertexArray(VertexArrayObj);

		GL::DrawArrays(GL::EPrimitives::Triangles, 0, TriangleRaw::VertexCount());

		GL::DisableVertexAttributeArray(VertexAttributeIndex);
	}

	sfn PrepWorkspace()
	{

	}


	// Runtime Execution: Loop must be specified here to use.

	sfn Execute() -> ExitCode
	{
		/*PrepWorkspace();*/

		//Cycler(Address(InputProcedure), Address(RenderProcedure));

		CreateWindow_TimedRender();

		GL::TerminateGLFW();

		return ExitCode::Success;
	}
}



int main(void)
{
	return int(Execution::Execute());
}

