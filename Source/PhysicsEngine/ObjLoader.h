#ifndef OBJLOADER
#define OBJLOADER

#include <string>
#include <fstream>
#include <iostream>
#include "../MaloWLib/Array.h"
#include "../../External/NDYGFX/Include/Vector.h"

struct MaterialData
{
	std::string name;
	std::string texture;
	int illum;
	Vector3 kd;
	Vector3 ka;
	Vector3 tf;
	Vector3 ks;
	
	float ni;

	MaterialData()
	{
		name = "";
		illum = 0;
		kd = Vector3(0, 0, 0);
		ka = Vector3(0, 0, 0);
		tf = Vector3(0, 0, 0);
		texture = "";
		ks = Vector3(0, 0, 0);
		ni = 0.0f;
	}
};

struct FaceData
{
	int data[3][3];
	// NOTE: string in material is slow?
	std::string material;
};

class ObjData
{
public:
	MaloW::Array<Vector3>* vertspos;
	MaloW::Array<Vector2>* textcoords;
	MaloW::Array<Vector3>* vertsnorms;
	MaloW::Array<FaceData>* faces;
	MaloW::Array<MaterialData>* mats;

	ObjData() 
	{ 
		vertspos = new MaloW::Array<Vector3>();
		textcoords = new MaloW::Array<Vector2>();
		vertsnorms = new MaloW::Array<Vector3>();
		faces = new MaloW::Array<FaceData>();
		mats = new MaloW::Array<MaterialData>();
	}

	virtual ~ObjData()
	{
		
		if(this->vertspos)
			delete this->vertspos;

		if(this->textcoords)
			delete this->textcoords;

		if(this->vertsnorms)
			delete this->vertsnorms;

		if(this->faces)
			delete this->faces;

		if(this->mats)
			delete this->mats;
			
	}
};

class ObjLoader
{
private:
	Vector3 GetVertPosData(std::string line);
	Vector2 GetTextCoordsData(std::string line);
	Vector3 GetVertsNormsData(std::string line);
	FaceData GetFaceData(std::string line);
	void trianglulate(std::string& filename);

public:
	ObjLoader() { }
	virtual ~ObjLoader() { }

	ObjData* LoadObjFile(std::string filepath);
	void ReadFromBinaryFile(ObjData* returndata, ifstream& binfile);
	void CreateBinaryFile(std::string filename, ObjData* returndata);

};


#endif