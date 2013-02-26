#pragma once

#include "FileReader.h"
#include <map>

enum ANIM_DIRECTION
{
	ANIM_FORWARD,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT
};

class AnimationFileReader : public FileReader
{
public:
	AnimationFileReader() {this->zFileName = "";}
	AnimationFileReader(const std::string& filename);
	virtual ~AnimationFileReader();

	bool ReadFromFile();

	const std::string& GetAnimation(const std::string& animationName) const;
private:
	std::string zFileName;

	//Maps animation type with animation Name
	std::map<std::string, std::string> zAnimationNames;
};