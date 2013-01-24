#pragma once

#include "PhysicsObject.h"

struct PhysicsCollisionData
{
	PhysicsCollisionData()
	{
		this->collision = false;
		this->BoundingSphereCollision = false;
		this->distance = 999999.0f;
		this->posx = 0.0f;
		this->posy = 0.0f;
		this->posz = 0.0f;
	}
	bool collision;
	bool BoundingSphereCollision;
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
	bool DoCollisionSphereVsRay(BoundingSphere bs, Matrix4 world, float scale, Vector3 rayOrigin, Vector3 rayDirection);
	bool DoCollisionSphereVsSphere(BoundingSphere bs1, Matrix4 world1, float scale1, BoundingSphere bs2, Matrix4 world2, float scale2);
	PhysicsCollisionData DoCollisionSphereVsRayDetailed(BoundingSphere bs, Matrix4 world, float scale, Vector3 rayOrigin, Vector3 rayDirection);
	PhysicsCollisionData DoCollisionSphereVsSphereDetailed(BoundingSphere bs1, Matrix4 world1, float scale1, BoundingSphere bs2, Matrix4 world2, float scale2);


public:
	PhysicsEngine();
	virtual ~PhysicsEngine();

	PhysicsObject* CreatePhysicsObject(string path, Vector3 pos);
	void DeletePhysicsObject(PhysicsObject* obj);

	// Functions
	virtual PhysicsCollisionData GetCollisionRayMesh(Vector3 rayOrigin, Vector3 rayDirection, PhysicsObject* imesh);
	/*virtual PhysicsCollisionData GetCollisionRayTerrain(Vector3 rayOrigin, Vector3 rayDirection, Vector3 position, 
		float heightMap[], float distanceXZBetweenVerts, int nrOfVertsPerRow);*/
	virtual PhysicsCollisionData GetCollisionMeshMesh(PhysicsObject* mesh1, PhysicsObject* mesh2);
	//virtual PhysicsCollisionData GetCollisionMeshTerrain(PhysicsObject* mesh, iTerrain* terr);

	// Only BoundingSphere functions
	virtual PhysicsCollisionData GetCollisionRayMeshBoundingOnly(Vector3 rayOrigin, Vector3 rayDirection, PhysicsObject* imesh);
	//virtual PhysicsCollisionData GetCollisionRayTerrainBoundingOnly(Vector3 rayOrigin, Vector3 rayDirection, iTerrain* iterr);
	virtual PhysicsCollisionData GetCollisionMeshMeshBoundingOnly(PhysicsObject* mesh1, PhysicsObject* mesh2);
	//virtual PhysicsCollisionData GetCollisionMeshTerrainBoundingOnly(iMesh* mesh, iTerrain* terr);

	// Overloaded
	virtual PhysicsCollisionData GetCollision(Vector3 rayOrigin, Vector3 rayDirection, PhysicsObject* mesh);
	//virtual PhysicsCollisionData GetCollision(Vector3 rayOrigin, Vector3 rayDirection, Vector3 position, 
		//float heightMap[], float distanceXZBetweenVerts, int nrOfVertsPerRow);
	virtual PhysicsCollisionData GetCollision(PhysicsObject* mesh1, PhysicsObject* mesh2);
	//virtual PhysicsCollisionData GetCollision(PhysicsObject* mesh, iTerrain* terr);

	// Only BoundingSphere functions
	virtual PhysicsCollisionData GetCollisionBoundingOnly(Vector3 rayOrigin, Vector3 rayDirection, PhysicsObject* imesh);
	//virtual PhysicsCollisionData GetCollisionBoundingOnly(Vector3 rayOrigin, Vector3 rayDirection, iTerrain* iterr);
	virtual PhysicsCollisionData GetCollisionBoundingOnly(PhysicsObject* mesh1, PhysicsObject* mesh2);
	//virtual PhysicsCollisionData GetCollisionBoundingOnly(iMesh* mesh, iTerrain* terr);
};