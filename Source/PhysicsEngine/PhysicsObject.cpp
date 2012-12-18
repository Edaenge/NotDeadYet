#include "PhysicsObject.h"

Vector4 QuatMult(Vector4 quat1, Vector4 quat2)
{	
	Vector4 newQuat = Vector4(0, 0, 0, 1);
	newQuat.x = (quat1.x * quat2.x - quat1.y * quat2.y - quat1.z * quat2.z - 
		quat1.w * quat2.w);
	newQuat.y = (quat1.x * quat2.y + quat1.y * quat2.x - quat1.z * quat2.w + 
		quat1.w * quat2.z);
	newQuat.z = (quat1.x * quat2.z + quat1.y * quat2.w + quat1.z * quat2.x - 
		quat1.w * quat2.y);
	newQuat.w = (quat1.x * quat2.w - quat1.y * quat2.z + quat1.z * quat2.y + 
		quat1.w * quat2.x);
	return newQuat;
};

PhysicsObject::PhysicsObject(Vector3 position)
{
	this->pos = position;
	this->indicies = NULL;
	this->mesh = NULL;
	this->nrOfIndicies = 0;
	this->nrOfVerts = 0;
}

PhysicsObject::~PhysicsObject()
{
	if(this->mesh)
		delete [] this->mesh;
	if(this->indicies)
		delete [] this->indicies;
}

void PhysicsObject::SetPosition( const Vector3& pos )
{
	this->pos = pos;
	this->RecreateWorldMatrix();
}

void PhysicsObject::SetQuaternion( const Vector4& quat )
{
	this->rotQuat = quat;
	this->rotQuat.normalize();
	this->RecreateWorldMatrix();
}

void PhysicsObject::MoveBy( const Vector3& moveby )
{
	this->pos += moveby;
	this->RecreateWorldMatrix();
}


void PhysicsObject::Rotate( const Vector4& quat )
{
	this->rotQuat = QuatMult(this->rotQuat, quat);
	this->rotQuat.normalize();
	this->RecreateWorldMatrix();
}

void PhysicsObject::RotateAxis( const Vector3& around, float angle )
{
	Vector4 quaternion = Vector4(0, 0, 0, 1);
	quaternion.x = cos(angle/2);
	quaternion.y = sin(angle/2) * cos(around.x);
	quaternion.z = sin(angle/2) * cos(around.y);
	quaternion.w = sin(angle/2) * cos(around.z);

	this->rotQuat = QuatMult(this->rotQuat, quaternion);
	this->rotQuat.normalize();
	this->RecreateWorldMatrix();
}

void PhysicsObject::Scale( const Vector3& scale )
{
	this->scale.x *= scale.x;
	this->scale.y *= scale.y;
	this->scale.z *= scale.z;
	this->RecreateWorldMatrix();
}

void PhysicsObject::Scale( float scale )
{
	this->scale.x *= scale;
	this->scale.y *= scale;
	this->scale.z *= scale;
	this->RecreateWorldMatrix();
}

Vector3 PhysicsObject::GetPosition() const
{
	return this->pos;
}

Vector4 PhysicsObject::GetRotationQuaternion() const
{
	return this->rotQuat;
}

Vector3 PhysicsObject::GetScaling() const
{
	return this->scale;
}

void PhysicsObject::RecreateWorldMatrix()
{
	Matrix4 translate;
	translate.value[0][3] = this->pos.x;
	translate.value[1][3] = this->pos.y;
	translate.value[2][3] = this->pos.z;

	Matrix4 scaling;
	scaling.value[0][0] = this->scale.x;
	scaling.value[1][1] = this->scale.y;
	scaling.value[2][2] = this->scale.z;


	Matrix4 QuatMat;
	QuatMat.value[0][0] = 1 - 2 * pow(this->rotQuat.y, 2) - 2 * pow(this->rotQuat.z, 2);
	QuatMat.value[0][1] = 2 * this->rotQuat.x * this->rotQuat.y + 2 * this->rotQuat.w * this->rotQuat.z;
	QuatMat.value[0][2] = 2 * this->rotQuat.x * this->rotQuat.z - 2 * this->rotQuat.w * this->rotQuat.y;

	QuatMat.value[1][0] = 2 * this->rotQuat.x * this->rotQuat.y - 2 * this->rotQuat.w * this->rotQuat.z;
	QuatMat.value[1][1] = 1 - 2 * pow(this->rotQuat.x, 2) - 2 * pow(this->rotQuat.z, 2);
	QuatMat.value[1][2] = 2 * this->rotQuat.y * this->rotQuat.z + 2 * this->rotQuat.w * this->rotQuat.x;

	QuatMat.value[2][0] = 2 * this->rotQuat.x * this->rotQuat.z + 2 * this->rotQuat.w * this->rotQuat.y;
	QuatMat.value[2][1] = 2 * this->rotQuat.y * this->rotQuat.z - 2 * this->rotQuat.w * this->rotQuat.x;
	QuatMat.value[2][2] = 1 - 2 * pow(this->rotQuat.x, 2) - 2 * pow(this->rotQuat.y, 2);

	Matrix4 world = scaling*QuatMat*translate;

	this->worldMatrix = world;
}

bool PhysicsObject::LoadFromFile( string file )
{
	// if substr of the last 4 = .obj do this:    - else load other format / print error

	ObjLoader oj;
	ObjData* od = oj.LoadObjFile(file);

	if(od)
	{
		int nrOfVerts = 0;

		Vertex* tempverts = new Vertex[od->faces->size()*3];

		for(int i = 0;  i < od->faces->size(); i++)
		{
			int vertpos = od->faces->get(i).data[0][0] - 1;
			int textcoord = od->faces->get(i).data[0][1] - 1;
			int norm = od->faces->get(i).data[0][2] - 1;
			tempverts[nrOfVerts] = Vertex(od->vertspos->get(vertpos), od->vertsnorms->get(norm));
			nrOfVerts++;

			vertpos = od->faces->get(i).data[2][0] - 1;
			textcoord = od->faces->get(i).data[2][1] - 1;
			norm = od->faces->get(i).data[2][2] - 1;
			tempverts[nrOfVerts] = Vertex(od->vertspos->get(vertpos), od->vertsnorms->get(norm));
			nrOfVerts++;

			vertpos = od->faces->get(i).data[1][0] - 1;
			textcoord = od->faces->get(i).data[1][1] - 1;
			norm = od->faces->get(i).data[1][2] - 1;
			tempverts[nrOfVerts] = Vertex(od->vertspos->get(vertpos), od->vertsnorms->get(norm));
			nrOfVerts++;
		}

		this->setNrOfVerts(nrOfVerts);
		Vertex* verts = new Vertex[nrOfVerts];
		for(int z = 0; z < nrOfVerts; z++)
		{
			verts[z] = tempverts[z];
		}
		delete tempverts;
		this->SetVerts(verts);
		
		delete od;

		return true;
	}

	return false;
}

Matrix4 PhysicsObject::GetWorldMatrix() const
{
	return this->worldMatrix;
}
