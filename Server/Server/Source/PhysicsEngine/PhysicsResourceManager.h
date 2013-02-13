//--------------------------------------------------------------------------------------------------
//
//	Written by Markus Tillman for project "Not dead yet" at Blekinge Tekniska Högskola.
// 
//	Manager for handling physics resources (Server side). 
//	Resources include:
//		Mesh(strips) - MeshResource.h.
//		Textures - TextureResource.h. //**Tillman**
//		Buffers - BufferResource.h
//	Requirements: DirectX device & device context.
//--------------------------------------------------------------------------------------------------
#pragma once

#include "ObjectDataResource.h"

#include <string>
#include <map>

#include "Array.h"
#include "MaloW.h"
#include "MaloWFileDebug.h"



class PhysicsResourceManager
{
	private:
		std::map<std::string, ObjectDataResource*>			zObjectDataResources;
		HANDLE mutex;

	public:
		PhysicsResourceManager();
		virtual ~PhysicsResourceManager();
		

		//Preloading
		/*
			Supported resources are objectDataResources(.obj).
		*/
		void PreLoadResources(unsigned int nrOfResources, char const* const* const resourcesFileNames);



		//Object data
		ObjectDataResource* LoadObjectDataResourceFromFile(const char* filePath);
		void UnloadObjectDataResource(const char* filePath);

};

PhysicsResourceManager* GetPhysicsResourceManager();
bool FreePhysicsResourceManager();