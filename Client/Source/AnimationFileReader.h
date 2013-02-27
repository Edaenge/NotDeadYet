#pragma once

#include "FileReader.h"
#include <map>
#include <Vector.h>

enum ANIMATION_INFORMATION
{
	BONE_L_WEAPON,
	BONE_R_WEAPON,
	BONE_ROOT,
	BONE_CAMERA_OFFSET,
	MISC_CAMERA_OFFSET_VALUE
};

class AnimationFileReader : public FileReader
{
public:
	AnimationFileReader() {this->zFileName = "";}
	AnimationFileReader(const std::string& filename);
	virtual ~AnimationFileReader();

	bool ReadFromFile();

	unsigned int InterpCommand(const std::string& key);
	const std::string& GetAnimation(const std::string& animationName) const;
	const std::string& GetBindingBone(const unsigned int bone) const;
private:
	std::string zFileName;

	//Maps animation type with animation Name
	std::map<std::string, std::string> zAnimationNames;
	std::map<unsigned int, std::string> zBindingBones;
	std::map<unsigned int, Vector3> zMiscStuff;
};