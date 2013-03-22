#include "AnimationFileReader.h"
#include <time.h>
#include "MaloW.h"

static const std::string ANIMATIONS		= "[Animations]";
static const std::string ANIMATIONTIMES = "[AnimationTimes]";
static const std::string BINDINGBONES	= "[BindingBones]";
static const std::string SUBMESHES		= "[SubMesh]";
static const std::string MISCELLANEOUS	= "[Misc]";
static const std::string END			= "#END";

static const std::string L_WEAPON_BONE	= "LWpn";
static const std::string R_WEAPON_BONE	= "RWpn";
static const std::string ROOT_BONE		= "Root";
static const std::string OFFSET_BONE	= "CamOffset";

static const std::string OFFSET_DIST	= "Offset";


AnimationFileReader::AnimationFileReader( const std::string& filename )
{
	this->zFileName = filename;
	this->ReadFromFile();
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

		if (line == ANIMATIONS)
		{
			while (!read.eof() && line != END)
			{
				std::getline(read, line);

				TrimAndSet(line, key, value);

				this->zAnimationNames[key] = value;
			}
		}
		else if (line == ANIMATIONTIMES)
		{
			while (!read.eof() && line != END)
			{
				std::getline(read, line);

				TrimAndSet(line, key, value);

				this->zAnimationTimes[key] = MaloW::convertStringToFloat(value);
			}
		}
		else if (line == SUBMESHES)
		{
			while (!read.eof() && line != END)
			{
				std::getline(read, line);

				TrimAndSet(line, key, value);

				this->zSubMeshes[key] = value;
			}
		}
		else if(line == BINDINGBONES)
		{
			while (!read.eof() && line != END)
			{
				std::getline(read, line);

				TrimAndSet(line, key, value);

				unsigned int keyVal = this->InterpCommand(key);

				if (keyVal != 10)
					this->zBindingBones[keyVal] = value;
			}
		}
		else if (line == MISCELLANEOUS)
		{
			while (!read.eof() && line != END)
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
	return true;
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

	static const std::string emptyString = "";
	return emptyString;
}

const std::string& AnimationFileReader::GetBindingBone( const unsigned int bone ) const
{
	auto it = this->zBindingBones.find(bone);
	if (it != this->zBindingBones.end())
		return it->second;

	static const std::string emptyString = "";
	return emptyString;
}

const float AnimationFileReader::GetAnimationTime( const std::string& animationName )
{
	auto it = this->zAnimationTimes.find(animationName);

	if (it != this->zAnimationTimes.end())
		return it->second / 30.0f;

	static const float zero = 0.0f;
	return zero;
}

const std::string& AnimationFileReader::GetSubMeshName( const std::string& name )
{
	auto it = this->zSubMeshes.find(name);
	if (it != this->zSubMeshes.end())
	{
		return it->second;
	}

	static const std::string none = "";

	return none;
}