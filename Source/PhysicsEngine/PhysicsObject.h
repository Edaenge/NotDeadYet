#pragma once

#include "../../External/NDYGFX/Include/Matrix.h"
#include "../../External/NDYGFX/Include/Vector.h"
#include "../MaloWLib/Array.h"
#include "../MaloWLib/MaloW.h"
#include "../MaloWLib/MaloWFileDebug.h"
#include "ObjLoader.h"
#include "BoundingSphere.h"

struct Vertex
{
	Vector3 pos;
	Vector3 normal;

	Vertex(Vector3 _pos, Vector3 _norm)
	{
		pos = _pos;
		normal = _norm;
	}
	Vertex()
	{
		pos = Vector3(0, 0, 0);
		normal = Vector3(0, 0, 0);
	}
	Vertex(const Vertex* origObj)
	{
		pos = origObj->pos;
		normal = origObj->normal;
	}
};

class PhysicsObject
{
private:
	Vector3 pos;
	Vector4 rotQuat;
	Vector3 scale;
	Matrix4 worldMatrix;

	Vertex* mesh;
	int nrOfVerts;
	int nrOfIndicies;
	int* indicies;

	BoundingSphere bs;

	void RecreateWorldMatrix();

public:
	PhysicsObject(Vector3 position = Vector3(0, 0, 0));
	virtual ~PhysicsObject();

	inline void SetPosition(const Vector3& pos){this->pos = pos; this->RecreateWorldMatrix();}
	void SetQuaternion(const Vector4& quat);
	void MoveBy(const Vector3& moveby);
	void Rotate(const Vector4& quat);
	void RotateAxis(const Vector3& around, float angle);
	void Scale(const Vector3& scale);
	void Scale(float scale);
	void SetScaling(const Vector3& scale);
	void SetScaling(const float scale);
	Vector3 GetPosition() const;
	Vector4 GetRotationQuaternion() const;
	Vector3 GetScaling() const;

	Matrix4 GetWorldMatrix() const;

	virtual bool LoadFromFile(string file);

	Vertex* getVerts() const { return this->mesh; }
	void SetVerts(Vertex* verts) { this->mesh = verts; }
	int getNrOfVerts() const { return this->nrOfVerts; }
	void setNrOfVerts(int vertno) { this->nrOfVerts = vertno; }
	int* getIndicies() const { return this->indicies; }
	void SetIndicies(int* inds) { this->indicies = inds; }
	int getNrOfIndicies() const { return this->nrOfIndicies; }
	void setNrOfIndicies(int indcount) { this->nrOfIndicies = indcount; }

	BoundingSphere GetBoundingSphere() const { return this->bs; }
	void SetBoundingSphere(BoundingSphere bs) { this->bs = bs; }
};