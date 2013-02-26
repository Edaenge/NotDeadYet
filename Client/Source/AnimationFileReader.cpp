#include "AnimationFileReader.h"
#include <time.h>

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

AnimationFileReader::AnimationFileReader( const std::string& filename )
{
	this->zFileName = filename;
}

AnimationFileReader::~AnimationFileReader()
{

}

bool AnimationFileReader::ReadFromFile()
{
	std::ifstream read;
	read.open(this->zFileName);

	if (!read.is_open())
		return false;

	while (!read.eof())
	{
		std::string line;
		std::string key;
		std::string value;

		std::getline(read, line);

		if (line.empty())
			continue;

		TrimAndSet(line, key, value);

		this->zAnimationNames[key] = value;
	}
}

const std::string& AnimationFileReader::GetAnimation(const std::string& animationName) const
{
	auto it = this->zAnimationNames.find(animationName);

	if (it != this->zAnimationNames.end())
		return it->second;

	return "";
}