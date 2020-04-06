#pragma once

#include "DGL/DGL.hpp"

#include "Cpp_Alias.hpp"



namespace Execution
{
	inline namespace LibraryReferences
	{
		using DGL::HE_Model;
	}


	int Execute_Assignment2()
	{
		cout << "Assignment 2:" << endl << endl;

		HE_Model BlenderTorus("./Models/torus_Blender.obj");

		BlenderTorus.Load();

			


		return EXIT_SUCCESS;
	}
}



