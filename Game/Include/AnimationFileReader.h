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
	MISC_CAMERA_OFFSET_VALUE,

};

static const std::string IDLE_O1		= "idle_01";
static const std::string IDLE_O2		= "idle_02";
static const std::string IDLE_O3		= "idle_03";
static const std::string IDLE_O4		= "idle_04";
static const std::string IDLE_O5		= "idle_05";
static const std::string IDLE_O6		= "idle_06";

static const std::string SPRINT			= "sprint";

static const std::string WALK_FORWARD	= "walk_fwd";
static const std::string WALK_BACKWARD	= "walk_bwd";
static const std::string WALK_RIGHT		= "walk_rwd";
static const std::string WALK_LEFT		= "walk_lwd";

static const std::string JOG_FORWARD	= "jog_fwd";
static const std::string JOG_BACKWARD	= "jog_bwd";
static const std::string JOG_RIGHT		= "jog_rwd";
static const std::string JOG_LEFT		= "jog_lwd";

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