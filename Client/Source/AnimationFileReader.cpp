#include "AnimationFileReader.h"
#include <time.h>

static const std::string ANIMATIONS		= "[Animations]";
static const std::string BINDINGBONES	= "[BindingBones]";
static const std::string MISCELLANEOUS	= "[Misc]";
static const std::string END			= "#END";

static const std::string L_WEAPON_BONE	= "LWpn";
static const std::string R_WEAPON_BONE	= "RWpn";
static const std::string ROOT_BONE		= "Root";
static const std::string OFFSET_BONE	= "CamOffset";

static const std::string OFFSET_DIST	= "Offset";

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

		if (key == ANIMATIONS)
		{
			while (!read.eof() && key != END)
			{
				std::getline(read, line);

				TrimAndSet(line, key, value);

				this->zAnimationNames[key] = value;
			}
		}
		else if(key == BINDINGBONES)
		{
			while (!read.eof() && key != END)
			{
				std::getline(read, line);

				TrimAndSet(line, key, value);

				unsigned int keyVal = this->InterpCommand(key);

				if (keyVal != 10)
					this->zBindingBones[keyVal] = value;
			}
		}
		else if (key == MISCELLANEOUS)
		{
			while (!read.eof() && key != END)
			{
				std::getline(read, line);

				TrimAndSet(line, key, value);

				if (key == OFFSET_DIST)
				{
					char a[10];
					char b[10];
					char c[10];

					sscanf_s(value.c_str(), "%[^','], %[^','], %[^','], %s", a, sizeof(a), b, sizeof(b), c, sizeof(c));

					float x = (float)atof(a);
					float y = (float)atof(b);
					float z = (float)atof(c);

					this->zMiscStuff[MISC_CAMERA_OFFSET_VALUE] = Vector3(x, y, z);
				}
			}
		}

		
	}
}

unsigned int AnimationFileReader::InterpCommand( const std::string& key )
{
	if (key == L_WEAPON_BONE)
		return BONE_L_WEAPON;
	else if (key == R_WEAPON_BONE)
		return BONE_R_WEAPON;
	else if (key == ROOT_BONE)
		return BONE_ROOT;
	else if (key == OFFSET_BONE)
		return BONE_CAMERA_OFFSET;

	return 10;
}

const std::string& AnimationFileReader::GetAnimation(const std::string& animationName) const
{
	auto it = this->zAnimationNames.find(animationName);

	if (it != this->zAnimationNames.end())
		return it->second;

	return "";
}