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

	bool Float64_ApproxEqual(double _first, double _second)
	{
		//Implementation influenced by: https://floating-point-gui.de/errors/comparison/, https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/

		double firstAbs  = fabsl(_first          ),
			   secondAbs = fabsl(_second         ),
			   diffAbs   = fabsl(_first - _second) ;

		bool ExactlyEqual,
			 CloseToZero  ;

		ExactlyEqual = (_first == _second                                      );
		CloseToZero  = (_first == 0       || _second == 0 || diffAbs < DBL_MIN);

		if (ExactlyEqual)       //Handles infinites
		{
			return true;
		}
		else if (CloseToZero)   //Close to zero
		{
			return (diffAbs < (DBL_EPSILON * DBL_EPSILON));
		}
		else                    //Relative Error
		{
			return (diffAbs / fminl(firstAbs + secondAbs, DBL_MAX) < DBL_EPSILON);
		}
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

		cout << "Calculating Discrete Gauss Curvatures..." << endl;

		double curvatureByVertex = mesh.GetGuassianCurvature_Discretely();

		cout << "Curvature by Vertex: " << curvatureByVertex << endl;

		double curvatureByEuler = 2 * OMeshInterface::PI() * eulerNumRight;

		cout << "Curvature by Euler : " << curvatureByEuler << endl << endl;

		if (eulerNumRight == eulerNumLeft && Float64_ApproxEqual(curvatureByVertex, curvatureByEuler))
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

		VerifyMesh("./Models/hand.obj", 1);

		VerifyMesh("./Models/horse.obj", 0);

		VerifyMesh("./Models/sculpture.obj", 3);

		VerifyMesh("./Models/topology.obj", 13);

		VerifyMesh("./Models/torus.obj", 1);
		
		return EXIT_SUCCESS;
	}
}



