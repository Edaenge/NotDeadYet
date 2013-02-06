///*
//Made by Edänge Simon 04-02-13
//for project Not Dead Yet at Blekinge tekniska högskola.
//*/
//
//#pragma once
//
//#include "ObjectManager.h"
//#include "PhysicsEngine.h"
//#include "Weapon.h"
//#include "Food.h"
//#include "Container.h"
//#include "Projectile.h"
//#include "Material.h"
//
///*! This class is used to create objects.
//  Valid objects are read with the ObjectManager.
//  ObjectCreator is not responsible for deallocation.
//*/
//class ObjectCreator
//{
//public:
//	ObjectCreator(PhysicsEngine* zPhysicEngine);
//	virtual ~ObjectCreator();
//	/*! Reads valid objects.*/
//	bool InitObjectCreator();
//
//	/*! Creates a StaticObject with default values.
//		ObjectCreator is not responsible for deallocation.
//	*/
//	bool CreateStaticObjectActor(const int type, FoodObject*& foodObj, const bool genID = false);
//	bool CreateStaticObjectActor(const int type, WeaponObject*& weaponObj, const bool genID = false);
//	bool CreateStaticObjectActor(const int type, MaterialObject*& materialObj, const bool genID = false);
//	bool CreateStaticObjectActor(const int type, ContainerObject*& containerObj, const bool genID = false);
//	//bool CreateStaticObjectActor(const int type, StaticProjectileObject*& projectileObj, const bool genID = false);
//
//	/*! Creates a DynamicObject with default values.
//		ObjectCreator is not responsible for deallocation.
//	*/
//	//bool CreateDynamicObjectActor(const int type, DynamicProjectileObject*& projectileObj, bool genID);
//
//	/*! Creates an Object From the Item Data.
//		ObjectCreator is not responsible for deallocation.
//	*/
//	bool CreateObjectFromItem(const Vector3& pos, Weapon* weapon_Item, WeaponObject*& objOut);
//	bool CreateObjectFromItem(const Vector3& pos, Food* food_Item, FoodObject*& objOut);
//	bool CreateObjectFromItem(const Vector3& pos, Container* container_item, ContainerObject*& objOut);
//	//bool CreateObjectFromItem(const Vector3& pos, Projectile* projectile_Item, StaticProjectileObject*& objOut);
//	bool CreateObjectFromItem(const Vector3& pos, Material* material_Item, MaterialObject*& objOut);
//
//	/*! Creates an item from default values.
//		ObjectCreator is not responsible for deallocation.
//	*/
//	Item* CreateItemFromDefault(const int ItemType);
//	/*! Converts a dynamic projectile to a static projectile.
//		ObjectCreator is not responsible for deallocation.
//	*/
//	//void HandleConversion(DynamicProjectileObject* dynamicProjObj, StaticProjectileObject*& objOut);
//
//	/*! Returns the ObjectManager.*/
//	ObjectManager* GetObjectManagerPtr() const {return this->zObjectManager;}
//
//private:
//	ObjectManager* zObjectManager;
//	PhysicsEngine* zPhysicEngine;
//};