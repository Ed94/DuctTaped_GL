#pragma once

// DGL
#include "DGL/DGL_Types.hpp"

// OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

// C++
#include "Cpp_Alias.hpp"


namespace OMeshInterface
{
	inline namespace LibraryReferences
	{
		// DGL

		using FaceGL     = DGL::Face      ;
		using FaceList   = DGL::FaceList  ;
		using Vector3    = DGL::Vector3   ;
		using VertexList = DGL::VertexList;

		// OpenMesh

		using EdgeHandle     = OpenMesh::EdgeHandle             ;
		using HE_Mesh        = OpenMesh::TriMesh_ArrayKernelT<> ;
		using HE_MeshPoly    = OpenMesh::PolyMesh_ArrayKernelT<>;
		using FaceHandle     = OpenMesh::FaceHandle             ;
		using HalfEdgeHandle = OpenMesh::HalfedgeHandle         ;
		using IO_Options     = OpenMesh::IO::Options            ;
		using VertexHandle   = OpenMesh::VertexHandle           ;
		using Vertex         = OpenMesh::ArrayKernel::Vertex    ;

		using OpenMesh::IO::read_mesh;

		// Second Order Aliases
		using Point = HE_Mesh::Point;
		using Edge  = HE_Mesh::Edge ;

		using EdgeList = vector<Edge>;

		using EdgeHandles     = vector<EdgeHandle    >;
		using FaceHandles     = vector<FaceHandle    >;
		using HalfEdgeHandles = vector<HalfEdgeHandle>;
		using VertHandles     = vector<VertexHandle  >;
	}

	
	

	class OMesh_HE
	{
	public:

		OMesh_HE() {}

		void Load(string _filePath)
		{
			IO_Options loadOptions;

			bool loadResult, hasFaceNormals, hasVertexNormals;

			oMeshObj.request_vertex_normals();
			oMeshObj.request_face_normals  ();

			loadResult = read_mesh(oMeshObj, _filePath, loadOptions);

			if (loadResult == false)
			{
				throw std::runtime_error("OMesh_HE: Failed to load the mesh.");
			}

			hasFaceNormals   = loadOptions.check(IO_Options::FaceNormal  );
			hasVertexNormals = loadOptions.check(IO_Options::VertexNormal);

			if (!hasVertexNormals)
			{
				oMeshObj.update_vertex_normals ();
			}
			if (!hasFaceNormals)
			{
				oMeshObj.update_face_normals ();
			}

			GenerateVertexList      ();
			GenerateVertexNormalList();
			GenerateFaceNormalList  ();
			GenerateVertexEdgeList  ();
			GenerateFaceList        ();

			return;
		}

		const VertexList& GetVerticies() const
		{
			return verticies;
		}

		const VertexList& GetVertNormals() const 
		{
			return vertNormals;
		}

		const EdgeList& GetVertEdges() const 
		{
			return edges;
		}

		const FaceList& GetFaces() const
		{
			return faces;
		}

		const VertexList& GetFaceNormals() const 
		{
			return faceNormals;
		}

	protected:

		void GenerateVertexList()
		{
			using VertIter = HE_Mesh::VertexIter;

			for (VertIter element = oMeshObj.vertices_begin(); element != oMeshObj.vertices_end(); element++)
			{
				auto vertex = oMeshObj.point(*element);

				verticies.push_back(Vector3(vertex[0], vertex[1], vertex[2]));
			}

			return;
		}

		void GenerateVertexNormalList()
		{
			using VertIter = HE_Mesh::VertexIter;

			for (VertIter element = oMeshObj.vertices_begin(); element != oMeshObj.vertices_end(); element++)
			{
				auto normal = oMeshObj.normal(*element);

				vertNormals.push_back(Vector3(normal[0], normal[1], normal[2]));
			}

			return;
		}

		void GenerateFaceNormalList()
		{
			using FaceIter = HE_Mesh::FaceIter;

			for (FaceIter faceElem = oMeshObj.faces_begin(); faceElem != oMeshObj.faces_end(); faceElem++)
			{
				auto normal = oMeshObj.normal(*faceElem);

				faceNormals.push_back(Vector3(normal[0], normal[1], normal[2]));
			}
		}

		void GenerateVertexEdgeList()
		{
			using EdgeIter = HE_Mesh::EdgeIter;

			for (EdgeIter element = oMeshObj.edges_begin(); element != oMeshObj.edges_begin(); element++)
			{
				using OEdge = decltype(oMeshObj.edge(*element));

				OEdge edge = oMeshObj.edge(*element);

				edges.push_back(edge);
			}
		}

		void GenerateFaceList()
		{
			using FaceIterElem = decltype(oMeshObj.faces_begin());

			for (FaceIterElem faceElement = oMeshObj.faces_begin(); faceElement != oMeshObj.faces_end(); faceElement++)
			{
				FaceGL face;

				int index = 0;

				using FaceVertexIter = HE_Mesh::FaceVertexIter;

				for (FaceVertexIter vertElement = oMeshObj.fv_begin(*faceElement); vertElement != oMeshObj.fv_end(*faceElement); vertElement++, index++)
				{
					face.vec[index] = vertElement->idx();
				}

				faces.push_back(face);
			}

			return;
		}

		HE_Mesh oMeshObj;

		VertexList verticies  ;
		VertexList vertNormals;
		VertexList faceNormals;
		EdgeList   edges      ;
		FaceList   faces      ;
	};
}
