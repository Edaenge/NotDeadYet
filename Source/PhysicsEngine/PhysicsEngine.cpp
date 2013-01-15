#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{

}

PhysicsEngine::~PhysicsEngine()
{
	while(this->objects.size() != 0)
		delete this->objects.getAndRemove(0);
}

PhysicsObject* PhysicsEngine::CreatePhysicsObject( string path, Vector3 pos )
{
	PhysicsObject* obj = new PhysicsObject(pos);

	if(!obj->LoadFromFile(path))
	{
		delete obj;
		return NULL;
	}

	this->objects.add(obj);

	return obj;
}

void PhysicsEngine::DeletePhysicsObject( PhysicsObject* obj )
{
	for(int i = 0; i < this->objects.size(); i++)
	{
		if(this->objects.get(i) == obj)
		{
			delete this->objects.getAndRemove(i);
			obj = NULL;
			i = this->objects.size();
		}
	}
}
















#include "PhysicsEngine.h"

//////////////////// Overloaded ////////////////////
PhysicsCollisionData PhysicsEngine::GetCollision( Vector3 rayOrigin, Vector3 rayDirection, PhysicsObject* mesh )
{
	return this->GetCollisionRayMesh(rayOrigin, rayDirection, mesh);
}
/*
PhysicsCollisionData PhysicsEngine::GetCollision( Vector3 rayOrigin, Vector3 rayDirection, iTerrain* iTerr )
{
	return this->GetCollisionRayTerrain(rayOrigin, rayDirection, iTerr);
}
*/
PhysicsCollisionData PhysicsEngine::GetCollision( PhysicsObject* mesh1, PhysicsObject* mesh2 )
{
	return this->GetCollisionMeshMesh(mesh1, mesh2);
}
/*
PhysicsCollisionData PhysicsEngine::GetCollision( PhysicsObject* mesh, iTerrain* terr )
{
	return this->GetCollisionMeshTerrain(mesh, terr);
}
*/


PhysicsCollisionData PhysicsEngine::GetCollisionRayMesh( Vector3 rayOrigin, Vector3 rayDirection, PhysicsObject* imesh )
{
	PhysicsCollisionData cd;
	
	this->DoCollisionRayVsMesh(rayOrigin, rayDirection, imesh, cd);

	return cd;
}
/*
PhysicsCollisionData PhysicsEngine::GetCollisionRayTerrain( Vector3 rayOrigin, Vector3 rayDirection, iTerrain* iTerr )
{
	PhysicsCollisionData cd;

	if(Terrain* terrain = dynamic_cast<Terrain*>(iTerr))
	{
		this->DoCollisionRayVsTriangles(rayOrigin, rayDirection, 
			terrain->GetVerticesPointer(), terrain->GetNrOfVertices(), 
			terrain->GetIndicesPointer(), terrain->GetNrOfIndices(), terrain->GetWorldMatrix(), cd);
	}
	else
		MaloW::Debug("Failed to cast iTerrain to Terrain in PhysicsEngine.cpp");

	return cd;
}
*/
PhysicsCollisionData PhysicsEngine::GetCollisionMeshMesh(PhysicsObject* mesh1, PhysicsObject* mesh2)
{
	PhysicsCollisionData cd;

	this->DoCollisionMeshVsMesh(mesh1, mesh2, cd);

	return cd;
}
/*
PhysicsCollisionData PhysicsEngine::GetCollisionMeshTerrain( PhysicsObject* mesh, iTerrain* terr )
{
	PhysicsCollisionData cd;
	// NYI
	return cd;
}
*/


//////////////////////////////////////////////////////////////////////////
/////////////////////////////    Privates      ///////////////////////////
//////////////////////////////////////////////////////////////////////////
void PhysicsEngine::DoCollisionRayVsMesh(Vector3 rayOrigin, Vector3 rayDirection, 
		PhysicsObject* mesh, PhysicsCollisionData& cd)
{
	this->DoCollisionRayVsTriangles(rayOrigin, rayDirection, 
		mesh->getVerts(), mesh->getNrOfVerts(), mesh->getIndicies(), mesh->getNrOfIndicies(), 
		mesh->GetWorldMatrix(), cd);
}

void PhysicsEngine::DoCollisionMeshVsMesh( PhysicsObject* m1, PhysicsObject* m2, PhysicsCollisionData& cd )
{

	this->DoCollisionTrianglesVsTriangles(m1->GetPosition(), m1->getVerts(), m1->getNrOfVerts(),
		m1->getIndicies(), m1->getNrOfIndicies(), m1->GetWorldMatrix(), 
		m2->getVerts(), m2->getNrOfVerts(), m2->getIndicies(),
		m2->getNrOfIndicies(), m2->GetWorldMatrix(), cd);
}

void PhysicsEngine::DoCollisionRayVsTriangles(Vector3 rayOrigin, Vector3 rayDirection, 
		Vertex* vertices, int nrOfVertices, int* indices, int nrOfIndices, Matrix4 worldMat, PhysicsCollisionData& cd)
{
	if(!indices)
	{
		for(int i = 0; i < nrOfVertices; i += 3)
		{
			PhysicsCollisionData tempCD;

			Vertex vert0 = vertices[i];
			Vertex vert1 = vertices[i + 1];
			Vertex vert2 = vertices[i + 2];

			
			Vector4 pos0;
			float x = vert0.pos.x * worldMat.value[0][0] + vert0.pos.y * worldMat.value[1][0] + 
				vert0.pos.z * worldMat.value[2][0] + worldMat.value[3][0];
			float y = vert0.pos.x * worldMat.value[0][1] + vert0.pos.y * worldMat.value[1][1] + 
				vert0.pos.z * worldMat.value[2][1] + worldMat.value[3][1];
			float z = vert0.pos.x * worldMat.value[0][2] + vert0.pos.y * worldMat.value[1][2] + 
				vert0.pos.z * worldMat.value[2][2] + worldMat.value[3][2];
			float w = vert0.pos.x * worldMat.value[0][3] + vert0.pos.y * worldMat.value[1][3] + 
				vert0.pos.z * worldMat.value[2][3] + worldMat.value[3][3];
			pos0.x = x/w;
			pos0.y = y/w;
			pos0.z = z/w;

			Vector4 pos1;
			x = vert1.pos.x * worldMat.value[0][0] + vert1.pos.y * worldMat.value[1][0] + 
				vert1.pos.z * worldMat.value[2][0] + worldMat.value[3][0];
			y = vert1.pos.x * worldMat.value[0][1] + vert1.pos.y * worldMat.value[1][1] + 
				vert1.pos.z * worldMat.value[2][1] + worldMat.value[3][1];
			z = vert1.pos.x * worldMat.value[0][2] + vert1.pos.y * worldMat.value[1][2] + 
				vert1.pos.z * worldMat.value[2][2] + worldMat.value[3][2];
			w = vert1.pos.x * worldMat.value[0][3] + vert1.pos.y * worldMat.value[1][3] + 
				vert1.pos.z * worldMat.value[2][3] + worldMat.value[3][3];
			pos1.x = x/w;
			pos1.y = y/w;
			pos1.z = z/w;

			Vector4 pos2;
			x = vert2.pos.x * worldMat.value[0][0] + vert2.pos.y * worldMat.value[1][0] + 
				vert2.pos.z * worldMat.value[2][0] + worldMat.value[3][0];
			y = vert2.pos.x * worldMat.value[0][1] + vert2.pos.y * worldMat.value[1][1] + 
				vert2.pos.z * worldMat.value[2][1] + worldMat.value[3][1];
			z = vert2.pos.x * worldMat.value[0][2] + vert2.pos.y * worldMat.value[1][2] + 
				vert2.pos.z * worldMat.value[2][2] + worldMat.value[3][2];
			w = vert2.pos.x * worldMat.value[0][3] + vert2.pos.y * worldMat.value[1][3] + 
				vert2.pos.z * worldMat.value[2][3] + worldMat.value[3][3];
			pos2.x = x/w;
			pos2.y = y/w;
			pos2.z = z/w;

			Vector3 v0 = Vector3(pos0.x, pos0.y, pos0.z);
			Vector3 v1 = Vector3(pos1.x, pos1.y, pos1.z);
			Vector3 v2 = Vector3(pos2.x, pos2.y, pos2.z);

			// END OF D3DX STUFF

			if(this->DoCollisionRayVsTriangle(rayOrigin, rayDirection, v0, v1, v2, tempCD))
			{
				if(tempCD.distance < cd.distance)
				{
					cd.distance = tempCD.distance;
					cd.posx = tempCD.posx;
					cd.posy = tempCD.posy;
					cd.posz = tempCD.posz;
					cd.collision = true;
				}
			}
		}
	}
	else
	{
		for(int i = 0; i < nrOfIndices; i += 3)
		{
			PhysicsCollisionData tempCD;

			Vertex vert0 = vertices[indices[i]];
			Vertex vert1 = vertices[indices[i + 1]];
			Vertex vert2 = vertices[indices[i + 2]];

			Vector4 pos0;
			float x = vert0.pos.x * worldMat.value[0][0] + vert0.pos.y * worldMat.value[1][0] + 
				vert0.pos.z * worldMat.value[2][0] + worldMat.value[3][0];
			float y = vert0.pos.x * worldMat.value[0][1] + vert0.pos.y * worldMat.value[1][1] + 
				vert0.pos.z * worldMat.value[2][1] + worldMat.value[3][1];
			float z = vert0.pos.x * worldMat.value[0][2] + vert0.pos.y * worldMat.value[1][2] + 
				vert0.pos.z * worldMat.value[2][2] + worldMat.value[3][2];
			float w = vert0.pos.x * worldMat.value[0][3] + vert0.pos.y * worldMat.value[1][3] + 
				vert0.pos.z * worldMat.value[2][3] + worldMat.value[3][3];
			pos0.x = x/w;
			pos0.y = y/w;
			pos0.z = z/w;

			Vector4 pos1;
			x = vert1.pos.x * worldMat.value[0][0] + vert1.pos.y * worldMat.value[1][0] + 
				vert1.pos.z * worldMat.value[2][0] + worldMat.value[3][0];
			y = vert1.pos.x * worldMat.value[0][1] + vert1.pos.y * worldMat.value[1][1] + 
				vert1.pos.z * worldMat.value[2][1] + worldMat.value[3][1];
			z = vert1.pos.x * worldMat.value[0][2] + vert1.pos.y * worldMat.value[1][2] + 
				vert1.pos.z * worldMat.value[2][2] + worldMat.value[3][2];
			w = vert1.pos.x * worldMat.value[0][3] + vert1.pos.y * worldMat.value[1][3] + 
				vert1.pos.z * worldMat.value[2][3] + worldMat.value[3][3];
			pos1.x = x/w;
			pos1.y = y/w;
			pos1.z = z/w;

			Vector4 pos2;
			x = vert2.pos.x * worldMat.value[0][0] + vert2.pos.y * worldMat.value[1][0] + 
				vert2.pos.z * worldMat.value[2][0] + worldMat.value[3][0];
			y = vert2.pos.x * worldMat.value[0][1] + vert2.pos.y * worldMat.value[1][1] + 
				vert2.pos.z * worldMat.value[2][1] + worldMat.value[3][1];
			z = vert2.pos.x * worldMat.value[0][2] + vert2.pos.y * worldMat.value[1][2] + 
				vert2.pos.z * worldMat.value[2][2] + worldMat.value[3][2];
			w = vert2.pos.x * worldMat.value[0][3] + vert2.pos.y * worldMat.value[1][3] + 
				vert2.pos.z * worldMat.value[2][3] + worldMat.value[3][3];
			pos2.x = x/w;
			pos2.y = y/w;
			pos2.z = z/w;

			Vector3 v0 = Vector3(pos0.x, pos0.y, pos0.z);
			Vector3 v1 = Vector3(pos1.x, pos1.y, pos1.z);
			Vector3 v2 = Vector3(pos2.x, pos2.y, pos2.z);

			// END OF D3DX STUFF

			if(this->DoCollisionRayVsTriangle(rayOrigin, rayDirection, v0, v1, v2, tempCD))
			{
				if(tempCD.distance < cd.distance)
				{
					cd.distance = tempCD.distance;
					cd.posx = tempCD.posx;
					cd.posy = tempCD.posy;
					cd.posz = tempCD.posz;
					cd.collision = true;
				}
			}
		}
	}
}

bool PhysicsEngine::DoCollisionRayVsTriangle(Vector3 rayOrigin, Vector3 rayDirection, 
		Vector3 v0, Vector3 v1, Vector3 v2, PhysicsCollisionData& tempCD)
{
	float eps = 0.000001f;
	Vector3 e1 = v1 - v0;
	Vector3 e2 = v2 - v0;
	Vector3 q = e2.GetCrossProduct(rayDirection);
	float a = e1.GetDotProduct(q);

	if(a > -eps && a < eps)
		return false;
	
	float f = 1.0f / a;
	Vector3 s = rayOrigin - v0;
	float u = f * (s.GetDotProduct(q));

	if(u < 0.0f)
		return false;

	Vector3 r = e1.GetCrossProduct(s);
	float v = f * (rayDirection.GetDotProduct(r));

	if(v < 0.0f || u + v > 1.0f)
		return false;

	float t = f * (e2.GetDotProduct(q));

	Vector3 collPos;
	float w = 1 - (u + v);

	collPos = (v0 * w) + (v1 * u) + (v2 * v);
			
	tempCD.collision = true;
	tempCD.posx = collPos.x;
	tempCD.posy = collPos.y;
	tempCD.posz = collPos.z;
	tempCD.distance = (rayOrigin - Vector3(tempCD.posx, tempCD.posy, tempCD.posz)).GetLength();

	return true;
}

void PhysicsEngine::DoCollisionTrianglesVsTriangles(Vector3 m1Pos, Vertex* vert1, int nrOfVerts1, int* inds1, int nrOfInds1, 
	Matrix4 worldMat1, Vertex* vert2, int nrOfVerts2, int* inds2, int nrOfInds2, 
	Matrix4 worldMat2, PhysicsCollisionData& cd )
{
	if(!inds1)
	{
		// Mesh1 isnt using inds
		if(!inds2)
		{
			// Mesh 2 isnt using inds either
			for(int i = 0; i < nrOfVerts1; i += 3)
			{
				PhysicsCollisionData tempCD;

				Vertex vert00 = vert1[i];
				Vertex vert01 = vert1[i + 1];
				Vertex vert02 = vert1[i + 2];

				// D3DX STUFF

				Vector4 pos00;
				float x = vert00.pos.x * worldMat1.value[0][0] + vert00.pos.y * worldMat1.value[1][0] + 
					vert00.pos.z * worldMat1.value[2][0] + worldMat1.value[3][0];
				float y = vert00.pos.x * worldMat1.value[0][1] + vert00.pos.y * worldMat1.value[1][1] + 
					vert00.pos.z * worldMat1.value[2][1] + worldMat1.value[3][1];
				float z = vert00.pos.x * worldMat1.value[0][2] + vert00.pos.y * worldMat1.value[1][2] + 
					vert00.pos.z * worldMat1.value[2][2] + worldMat1.value[3][2];
				float w = vert00.pos.x * worldMat1.value[0][3] + vert00.pos.y * worldMat1.value[1][3] + 
					vert00.pos.z * worldMat1.value[2][3] + worldMat1.value[3][3];
				pos00.x = x/w;
				pos00.y = y/w;
				pos00.z = z/w;

				Vector4 pos01;
				x = vert01.pos.x * worldMat1.value[0][0] + vert01.pos.y * worldMat1.value[1][0] + 
					vert01.pos.z * worldMat1.value[2][0] + worldMat1.value[3][0];
				y = vert01.pos.x * worldMat1.value[0][1] + vert01.pos.y * worldMat1.value[1][1] + 
					vert01.pos.z * worldMat1.value[2][1] + worldMat1.value[3][1];
				z = vert01.pos.x * worldMat1.value[0][2] + vert01.pos.y * worldMat1.value[1][2] + 
					vert01.pos.z * worldMat1.value[2][2] + worldMat1.value[3][2];
				w = vert01.pos.x * worldMat1.value[0][3] + vert01.pos.y * worldMat1.value[1][3] + 
					vert01.pos.z * worldMat1.value[2][3] + worldMat1.value[3][3];
				pos01.x = x/w;
				pos01.y = y/w;
				pos01.z = z/w;

				Vector4 pos02;
				x = vert02.pos.x * worldMat1.value[0][0] + vert02.pos.y * worldMat1.value[1][0] + 
					vert02.pos.z * worldMat1.value[2][0] + worldMat1.value[3][0];
				y = vert02.pos.x * worldMat1.value[0][1] + vert02.pos.y * worldMat1.value[1][1] + 
					vert02.pos.z * worldMat1.value[2][1] + worldMat1.value[3][1];
				z = vert02.pos.x * worldMat1.value[0][2] + vert02.pos.y * worldMat1.value[1][2] + 
					vert02.pos.z * worldMat1.value[2][2] + worldMat1.value[3][2];
				w = vert02.pos.x * worldMat1.value[0][3] + vert02.pos.y * worldMat1.value[1][3] + 
					vert02.pos.z * worldMat1.value[2][3] + worldMat1.value[3][3];
				pos02.x = x/w;
				pos02.y = y/w;
				pos02.z = z/w;


				Vector3 v00 = Vector3(pos00.x, pos00.y, pos00.z);
				Vector3 v01 = Vector3(pos01.x, pos01.y, pos01.z);
				Vector3 v02 = Vector3(pos02.x, pos02.y, pos02.z);

				// END OF D3DX STUFF

				for(int u = 0; u < nrOfVerts2; u += 3)
				{
					Vertex vert10 = vert2[u];
					Vertex vert11 = vert2[u + 1];
					Vertex vert12 = vert2[u + 2];

					// D3DX STUFF

					Vector4 pos10;
					float x = vert10.pos.x * worldMat2.value[0][0] + vert10.pos.y * worldMat2.value[1][0] + 
						vert10.pos.z * worldMat2.value[2][0] + worldMat2.value[3][0];
					float y = vert10.pos.x * worldMat2.value[0][1] + vert10.pos.y * worldMat2.value[1][1] + 
						vert10.pos.z * worldMat2.value[2][1] + worldMat2.value[3][1];
					float z = vert10.pos.x * worldMat2.value[0][2] + vert10.pos.y * worldMat2.value[1][2] + 
						vert10.pos.z * worldMat2.value[2][2] + worldMat2.value[3][2];
					float w = vert10.pos.x * worldMat2.value[0][3] + vert10.pos.y * worldMat2.value[1][3] + 
						vert10.pos.z * worldMat2.value[2][3] + worldMat2.value[3][3];
					pos10.x = x/w;
					pos10.y = y/w;
					pos10.z = z/w;

					Vector4 pos11;
					x = vert11.pos.x * worldMat2.value[0][0] + vert11.pos.y * worldMat2.value[1][0] + 
						vert11.pos.z * worldMat2.value[2][0] + worldMat2.value[3][0];
					y = vert11.pos.x * worldMat2.value[0][1] + vert11.pos.y * worldMat2.value[1][1] + 
						vert11.pos.z * worldMat2.value[2][1] + worldMat2.value[3][1];
					z = vert11.pos.x * worldMat2.value[0][2] + vert11.pos.y * worldMat2.value[1][2] + 
						vert11.pos.z * worldMat2.value[2][2] + worldMat2.value[3][2];
					w = vert11.pos.x * worldMat2.value[0][3] + vert11.pos.y * worldMat2.value[1][3] + 
						vert11.pos.z * worldMat2.value[2][3] + worldMat2.value[3][3];
					pos11.x = x/w;
					pos11.y = y/w;
					pos11.z = z/w;

					Vector4 pos12;
					x = vert12.pos.x * worldMat2.value[0][0] + vert12.pos.y * worldMat2.value[1][0] + 
						vert12.pos.z * worldMat2.value[2][0] + worldMat2.value[3][0];
					y = vert12.pos.x * worldMat2.value[0][1] + vert12.pos.y * worldMat2.value[1][1] + 
						vert12.pos.z * worldMat2.value[2][1] + worldMat2.value[3][1];
					z = vert12.pos.x * worldMat2.value[0][2] + vert12.pos.y * worldMat2.value[1][2] + 
						vert12.pos.z * worldMat2.value[2][2] + worldMat2.value[3][2];
					w = vert12.pos.x * worldMat2.value[0][3] + vert12.pos.y * worldMat2.value[1][3] + 
						vert12.pos.z * worldMat2.value[2][3] + worldMat2.value[3][3];
					pos12.x = x/w;
					pos12.y = y/w;
					pos12.z = z/w;

					Vector3 v10 = Vector3(pos10.x, pos10.y, pos10.z);
					Vector3 v11 = Vector3(pos11.x, pos11.y, pos11.z);
					Vector3 v12 = Vector3(pos12.x, pos12.y, pos12.z);


					if(this->DoCollisionTriangleVsTriangle(v00, v01, v02, v10, v11, v12, tempCD))
					{
						tempCD.distance = (m1Pos - Vector3(cd.posx, cd.posy, cd.posz)).GetLength();
						if(tempCD.distance < cd.distance)
						{
							cd.distance = tempCD.distance;
							cd.posx = tempCD.posx;
							cd.posy = tempCD.posy;
							cd.posz = tempCD.posz;
							cd.collision = true;
						}
					}
				}

			}

		}
		else
		{
			// Mesh 1 isnt using inds, mesh 2 is
			MaloW::Debug("Collision MeshVSMesh, Mesh1 isnt using inds, mesh2 is, NOT YET IMPLEMENTED");
		}

	}
	else
	{
		
		if(!inds2)
		{
			// Mesh1 is using inds, mesh 2 isnt
			MaloW::Debug("Collision MeshVSMesh, Mesh1 is using inds, mesh2 isnt, NOT YET IMPLEMENTED");
		}
		else
		{
			// Neither is using inds
			MaloW::Debug("Collision MeshVSMesh, Mesh1 and mesh2 is using inds, NOT YET IMPLEMENTED");
		}

		// To be implemented above:
		/*
		for(int i = 0; i < nrOfIndices; i += 3)
		{
			PhysicsCollisionData tempCD;

			Vertex vert0 = vertices[indices[i]];
			Vertex vert1 = vertices[indices[i + 1]];
			Vertex vert2 = vertices[indices[i + 2]];

			// D3DX STUFF

			D3DXVECTOR4 pos0;
			D3DXVec3Transform(&pos0, &vert0.pos, &worldMat);
			D3DXVECTOR4 pos1;
			D3DXVec3Transform(&pos1, &vert1.pos, &worldMat);
			D3DXVECTOR4 pos2;
			D3DXVec3Transform(&pos2, &vert2.pos, &worldMat);

			Vector3 v0 = Vector3(pos0.x, pos0.y, pos0.z);
			Vector3 v1 = Vector3(pos1.x, pos1.y, pos1.z);
			Vector3 v2 = Vector3(pos2.x, pos2.y, pos2.z);

			// END OF D3DX STUFF

			if(this->DoCollisionRayVsTriangle(rayOrigin, rayDirection, v0, v1, v2, tempCD))
			{
				if(tempCD.distance < cd.distance)
				{
					cd.distance = tempCD.distance;
					cd.position = tempCD.position;
					cd.collision = true;
				}
			}
		}
		*/
	}
}














// Stolen and re-worked a bit from:
// http://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/opttritri.txt


#define USE_EPSILON_TEST
#define EPSILON 0.000001f

#define EDGE_EDGE_TEST(V0,U0,U1)                      \
	Bx=U0[i0]-U1[i0];                                   \
	By=U0[i1]-U1[i1];                                   \
	Cx=V0[i0]-U0[i0];                                   \
	Cy=V0[i1]-U0[i1];                                   \
	f=Ay*Bx-Ax*By;                                      \
	d=By*Cx-Bx*Cy;                                      \
	if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
  {                                                   \
  e=Ax*Cy-Ay*Cx;                                    \
  if(f>0)                                           \
	{                                                 \
	if(e>=0 && e<=f) return 1;                      \
}                                                 \
	else                                              \
	{                                                 \
	if(e<=0 && e>=f) return 1;                      \
}                                                 \
}

#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2) \
{                                              \
	float Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
	Ax=V1[i0]-V0[i0];                            \
	Ay=V1[i1]-V0[i1];                            \
	/* test edge U0,U1 against V0,V1 */          \
	EDGE_EDGE_TEST(V0,U0,U1);                    \
	/* test edge U1,U2 against V0,V1 */          \
	EDGE_EDGE_TEST(V0,U1,U2);                    \
	/* test edge U2,U1 against V0,V1 */          \
	EDGE_EDGE_TEST(V0,U2,U0);                    \
}

#define POINT_IN_TRI(V0,U0,U1,U2)           \
{                                           \
	float a,b,c,d0,d1,d2;                     \
	/* is T1 completly inside T2? */          \
	/* check if V0 is inside tri(U0,U1,U2) */ \
	a=U1[i1]-U0[i1];                          \
	b=-(U1[i0]-U0[i0]);                       \
	c=-a*U0[i0]-b*U0[i1];                     \
	d0=a*V0[i0]+b*V0[i1]+c;                   \
	\
	a=U2[i1]-U1[i1];                          \
	b=-(U2[i0]-U1[i0]);                       \
	c=-a*U1[i0]-b*U1[i1];                     \
	d1=a*V0[i0]+b*V0[i1]+c;                   \
	\
	a=U0[i1]-U2[i1];                          \
	b=-(U0[i0]-U2[i0]);                       \
	c=-a*U2[i0]-b*U2[i1];                     \
	d2=a*V0[i0]+b*V0[i1]+c;                   \
	if(d0*d1>0.0)                             \
  {                                         \
  if(d0*d2>0.0) return 1;                 \
}                                         \
}

int coplanar_tri_tri(float N[3],float V0[3],float V1[3],float V2[3],
					 float U0[3],float U1[3],float U2[3])
{
	float A[3];
	short i0,i1;
	/* first project onto an axis-aligned plane, that maximizes the area */
	/* of the triangles, compute indices: i0,i1. */
	A[0]=fabs(N[0]);
	A[1]=fabs(N[1]);
	A[2]=fabs(N[2]);
	if(A[0]>A[1])
	{
		if(A[0]>A[2])
		{
			i0=1;      /* A[0] is greatest */
			i1=2;
		}
		else
		{
			i0=0;      /* A[2] is greatest */
			i1=1;
		}
	}
	else   /* A[0]<=A[1] */
	{
		if(A[2]>A[1])
		{
			i0=0;      /* A[2] is greatest */
			i1=1;
		}
		else
		{
			i0=0;      /* A[1] is greatest */
			i1=2;
		}
	}

	/* test all edges of triangle 1 against the edges of triangle 2 */
	EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2);
	EDGE_AGAINST_TRI_EDGES(V1,V2,U0,U1,U2);
	EDGE_AGAINST_TRI_EDGES(V2,V0,U0,U1,U2);

	/* finally, test if tri1 is totally contained in tri2 or vice versa */
	POINT_IN_TRI(V0,U0,U1,U2);
	POINT_IN_TRI(U0,V0,V1,V2);

	return 0;
}

bool NEWCOMPUTE_INTERVALS(float& VV0, float& VV1, float& VV2, float& D0, float& D1, float& D2, float& D0D1,
						   float& D0D2, float& A, float& B, float& C, float& X0, float& X1)
{ 
	if(D0D1>0.0f) 
	{ 
		A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; 
	}
	else if(D0D2>0.0f)
	{ 
		A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; 
	} 
	else if(D1*D2>0.0f || D0!=0.0f) 
	{ 
		A=VV0; B=(VV1-VV0)*D0; C=(VV2-VV0)*D0; X0=D0-D1; X1=D0-D2; 
	} 
	else if(D1!=0.0f) 
	{ 
		A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; 
	} 
	else if(D2!=0.0f) 
	{ 
		A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; 
	} 
	else 
	{ 
		return true;
	} 
	return false;
}

bool PhysicsEngine::DoCollisionTriangleVsTriangle(Vector3 v00, Vector3 v01, Vector3 v02, 
	Vector3 v10, Vector3 v11, Vector3 v12, PhysicsCollisionData& tempCD)
{
	Vector3 e1 = v01 - v00;
	Vector3 e2 = v02 - v00;
	Vector3 n1 = e1.GetCrossProduct(e2);
	float d1 = -n1.GetDotProduct(v00);

	float du0 = n1.GetDotProduct(v10) + d1;
	float du1 = n1.GetDotProduct(v11) + d1;
	float du2 = n1.GetDotProduct(v12) + d1;

#ifdef USE_EPSILON_TEST
	if(fabs(du0) < EPSILON) du0 = 0.0f;
	if(fabs(du1) < EPSILON) du1 = 0.0f;
	if(fabs(du2) < EPSILON) du2 = 0.0f;
#endif
		
	float du0du1 = du0 * du1;
	float du0du2 = du0 * du2;

	if(du0du1 > 0.0f && du0du2 > 0.0f)
		return false;

	e1 = v11 - v10;
	e2 = v12 - v10;
	Vector3 n2 = e1.GetCrossProduct(e2);
	float d2 = -n2.GetDotProduct(v10);

	float dv0 = n2.GetDotProduct(v00) + d2;
	float dv1 = n2.GetDotProduct(v01) + d2;
	float dv2 = n2.GetDotProduct(v02) + d2;

#ifdef USE_EPSILON_TEST
	if(fabs(dv0) < EPSILON) dv0 = 0.0f;
	if(fabs(dv1) < EPSILON) dv1 = 0.0f;
	if(fabs(dv2) < EPSILON) dv2 = 0.0f;
#endif

	float dv0dv1 = dv0 * dv1;
	float dv0dv2 = dv0 * dv2;

	if(dv0dv1 > 0.0f && dv0dv2 > 0.0f)
		return false;

	Vector3 d = n1.GetCrossProduct(n2);

	float max = fabs(d.x);
	int index = 0;
	float bb = fabs(d.y);
	float cc = fabs(d.z);
	if(bb > max)
	{
		max = bb;
		index = 1;
	}
	if(cc > max)
	{
		max = cc;
		index = 2;
	}

	float vp0 = 0.0f;
	float vp1 = 0.0f;
	float vp2 = 0.0f;
	float up0 = 0.0f;
	float up1 = 0.0f;
	float up2 = 0.0f;

	if(index == 0)
	{
		vp0 = v00.x;
		vp1 = v01.x;
		vp2 = v02.x;

		up0 = v10.x;
		up1 = v11.x;
		up2 = v12.x;
	}
	else if(index == 1)
	{
		vp0 = v00.y;
		vp1 = v01.y;
		vp2 = v02.y;

		up0 = v10.y;
		up1 = v11.y;
		up2 = v12.y;
	}
	else
	{
		vp0 = v00.z;
		vp1 = v01.z;
		vp2 = v02.z;

		up0 = v10.z;
		up1 = v11.z;
		up2 = v12.z;
	}


	float a = 0.0f;
	float b = 0.0f;
	float c= 0.0f;
	float x0 = 0.0f;
	float x1 = 0.0f;

	if(NEWCOMPUTE_INTERVALS(vp0,vp1,vp2,dv0,dv1,dv2,dv0dv1,dv0dv2,a,b,c,x0,x1))
	{
		float N1[3];
		N1[0] = n1.x;
		N1[1] = n1.y;
		N1[2] = n1.z;

		float V00[3];
		V00[0] = v00.x;
		V00[1] = v00.y;
		V00[2] = v00.z;

		float V01[3];
		V01[0] = v01.x;
		V01[1] = v01.y;
		V01[2] = v01.z;

		float V02[3];
		V02[0] = v02.x;
		V02[1] = v02.y;
		V02[2] = v02.z;

		float V10[3];
		V10[0] = v10.x;
		V10[1] = v10.y;
		V10[2] = v10.z;

		float V11[3];
		V11[0] = v11.x;
		V11[1] = v11.y;
		V11[2] = v11.z;

		float V12[3];
		V12[0] = v12.x;
		V12[1] = v12.y;
		V12[2] = v12.z;

		return coplanar_tri_tri(N1, V00, V01, V02, V10, V11, V12);
	}

	float dsec = 0.0f;
	float e = 0.0f;
	float f = 0.0f;
	float y0 = 0.0f;
	float y1 = 0.0f;

	if(NEWCOMPUTE_INTERVALS(up0,up1,up2,du0,du1,du2,du0du1,du0du2,dsec,e,f,y0,y1))
	{
		float N1[3];
		N1[0] = n1.x;
		N1[1] = n1.y;
		N1[2] = n1.z;

		float V00[3];
		V00[0] = v00.x;
		V00[1] = v00.y;
		V00[2] = v00.z;

		float V01[3];
		V01[0] = v01.x;
		V01[1] = v01.y;
		V01[2] = v01.z;

		float V02[3];
		V02[0] = v02.x;
		V02[1] = v02.y;
		V02[2] = v02.z;

		float V10[3];
		V10[0] = v10.x;
		V10[1] = v10.y;
		V10[2] = v10.z;

		float V11[3];
		V11[0] = v11.x;
		V11[1] = v11.y;
		V11[2] = v11.z;

		float V12[3];
		V12[0] = v12.x;
		V12[1] = v12.y;
		V12[2] = v12.z;

		return coplanar_tri_tri(N1, V00, V01, V02, V10, V11, V12);
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	float isect1[2];
	float isect2[2];	

	float xx;
	float yy;
	float xxyy;
	float tmp;

	xx = x0 * x1;
	yy = y0 * y1;
	xxyy = xx * yy;

	tmp = a * xxyy;
	isect1[0] = tmp + b * x1 * yy;
	isect1[1] = tmp + c * x0 * yy;

	tmp = dsec * xxyy;
	isect2[0] = tmp + e * xx * y1;
	isect2[1] = tmp + f * xx * y0;

    
	if(isect1[0] > isect1[1])
	{
		float c; 
		c = isect1[0];     
		isect1[0] = isect1[1];     
		isect1[1] = c;     
	}

	if(isect2[0] > isect2[1])    
	{          
		float c; 
		c = isect2[0];     
		isect2[0] = isect2[1];     
		isect2[1] = c;     
	}

	if(isect1[1] < isect2[0] || isect2[1] < isect1[0]) 
		return false;



	tempCD.collision = true;
	//// ALL BELOW NEEDS TO BE ADDED, not sure if above code has that data somewhere or if additional
	//// calculations are needed to get it. posx, y, z should be the position of collision between
	//// the triangles, and distance should be the distance between Mesh1 (left parameter in above function)
	//// and the intersection point, so distance can't be calculated here is done above.
	tempCD.distance = 0.0f;
	tempCD.posx = 0.0f;
	tempCD.posy = 0.0f;
	tempCD.posz = 0.0f;

	return true;
}













