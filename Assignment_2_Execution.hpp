#pragma once




#include "DGL/DGL.hpp"

#include "Cpp_Alias.hpp"



namespace Execution
{
	inline namespace LibraryReferences
	{
		using DGL::HE_Model;

		using OMeshInterface::OMesh_HE;
	}

	void VerifyMesh(string _filePathForMesh, int genus)
	{
		cout << "Verifying: " << _filePathForMesh << endl;

		OMesh_HE mesh;

		mesh.Load(_filePathForMesh);


		// Euler Number:

		int eulerNumLeft = mesh.GetVerticies().size() + mesh.GetFaces().size() - mesh.GetEdges().size();

		cout << "Left  Side Value: " << eulerNumLeft << endl;

		int eulerNumRight = 2 - 2 * genus;

		cout << "Right Side Value: " << eulerNumRight << endl << endl;


		// Gauss Curvatures:

		cout << "Calculating Discrete Gauss Curvatures... (Accuracy is up to 0.0001 epsilon on the floats)" << endl;

		double curvatureByVertex = mesh.GetGuassianCurvature_Discretely();

		cout << "Curvature by Vertex: " << curvatureByVertex << endl;

		double curvatureByEuler = 2 * OMeshInterface::PI() * eulerNumRight;

		cout << "Curvature by Euler : " << curvatureByEuler << endl << endl;

		double fabsResult = fabs(curvatureByEuler - curvatureByVertex);

		if (eulerNumRight == eulerNumLeft && fabs(curvatureByEuler - curvatureByVertex) < 0.0001)
		{
			cout << _filePathForMesh << " is valid." << endl << endl;;
		}
		else
		{
			cout << _filePathForMesh << " is invalid." << endl << endl;;
		}
	}


	int Execute_Assignment2()
	{
		cout << "Assignment 2: Mesh operations" << endl << endl;

		VerifyMesh("./Models/bunny.obj", 0);

		VerifyMesh("./Models/eight.obj", 2);

		VerifyMesh("./Models/gargoyle.obj", 0);

		VerifyMesh("./Models/horse.obj", 0);

		VerifyMesh("./Models/torus.obj", 1);
		
		return EXIT_SUCCESS;
	}
}



