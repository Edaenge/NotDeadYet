#pragma once

#include "PhysicsObject.h"

struct PhysicsCollisionData
{
	PhysicsCollisionData()
	{
		this->collision = false;
		this->distance = 999999.0f;
		this->posx = 0.0f;
		this->posy = 0.0f;
		this->posz = 0.0f;
	}
	bool collision;
	float distance;
	float posx;
	float posy;
	float posz;
};


class PhysicsEngine
{
private:
	MaloW::Array<PhysicsObject*> objects;

	void DoCollisionRayVsMesh(Vector3 rayOrigin, Vector3 rayDirection, 
		PhysicsObject* mesh, PhysicsCollisionData& cd);
	void DoCollisionRayVsTriangles(Vector3 rayOrigin, Vector3 rayDirection, 
		Vertex* vertices, int nrOfVertices, int* indices, int nrOfIndices, Matrix4 worldMat, PhysicsCollisionData& cd);
	bool DoCollisionRayVsTriangle(Vector3 rayOrigin, Vector3 rayDirection, 
		Vector3 v0, Vector3 v1, Vector3 v2, PhysicsCollisionData& tempCD);

	void DoCollisionMeshVsMesh(PhysicsObject* m1, PhysicsObject* m2, PhysicsCollisionData& cd);
	void DoCollisionTrianglesVsTriangles(Vector3 m1Pos, Vertex* vert1, int nrOfVerts1, int* inds1, int nrOfInds1, Matrix4 worldMat1,
		Vertex* vert2, int nrOfVerts2, int* inds2, int nrOfInds2, Matrix4 worldMat2, PhysicsCollisionData& cd);
	bool DoCollisionTriangleVsTriangle(Vector3 v00, Vector3 v01, Vector3 v02, Vector3 v10, Vector3 v11, Vector3 v12,
		PhysicsCollisionData& tempCD);

public:
	PhysicsEngine();
	virtual ~PhysicsEngine();

	PhysicsObject* CreatePhysicsObject(string path, Vector3 pos);
	void DeletePhysicsObject(PhysicsObject* obj);

	virtual PhysicsCollisionData GetCollisionRayMesh(Vector3 rayOrigin, Vector3 rayDirection, PhysicsObject* imesh);
	//virtual PhysicsCollisionData GetCollisionRayTerrain(Vector3 rayOrigin, Vector3 rayDirection, iTerrain* iTerr);
	virtual PhysicsCollisionData GetCollisionMeshMesh(PhysicsObject* mesh1, PhysicsObject* mesh2);
	//virtual PhysicsCollisionData GetCollisionMeshTerrain(PhysicsObject* mesh, iTerrain* terr);

	// Overloaded
	virtual PhysicsCollisionData GetCollision(Vector3 rayOrigin, Vector3 rayDirection, PhysicsObject* mesh);
	//virtual PhysicsCollisionData GetCollision(Vector3 rayOrigin, Vector3 rayDirection, iTerrain* iTerr);
	virtual PhysicsCollisionData GetCollision(PhysicsObject* mesh1, PhysicsObject* mesh2);
	//virtual PhysicsCollisionData GetCollision(PhysicsObject* mesh, iTerrain* terr);
};