#include "SoundReader.h"
#include "MaloWFileDebug.h"

static const std::string FILENAME = "SoundFiles.ini";
static const std::string END =		"#END";
static const std::string MUSIC =	"[MUSIC]";
static const std::string SOUND =	"[SOUND]";
static const std::string PATH =		"PATH";
static const std::string LOOP =		"LOOP";


SoundReader::SoundReader(SoundHandler* engine)
{
	this->zEngine = engine;
}

SoundReader::~SoundReader()
{
	
}

bool SoundReader::ReadFromFile()
{
	if(!zEngine)
		return false;

	std::ifstream read;
	bool aSound = false;

	read.open(FILENAME);

	if(!read.is_open())
	{
		MaloW::Debug("Cannot open  \"" +FILENAME+ "\"in function CreateSoundsFromFile in Game.cpp.");

		return false;
	}

	std::string text;
	while(!read.eof())
	{
		std::string line;
		std::getline(read, line);

		if(line.empty())
			continue;

		if(line == MUSIC)
			aSound = false;
		else if(line == SOUND)
			aSound = true;
		else
			continue;
		
		while (!read.eof() && line == END)
		{
			std::string path;
			bool loop;
			std::getline(read, line);

			std::string command;
			std::string key;
			TrimAndSet(line, command, key);

			if(command == PATH)
			{
				path = key;
			}
			else if(command == LOOP)
			{
				if(key == "TRUE")
					loop = true;
				else
					loop = false;
			}
			else if(command == END)
			{
				if(!aSound)
				{
					if(!zEngine->LoadMusicIntoSystem(path.c_str(), loop))
						MaloW::Debug("Cant load music sound:" + line);
				}
				else
				{
					if(!zEngine->LoadSoundIntoSystem(path.c_str(), loop))
						MaloW::Debug("Cant load sound sound:" + line);
				}
			}
		}
	}
	return true;
}