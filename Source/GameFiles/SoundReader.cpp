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
		char line[256] = "";
		char key [52] = "";
		char command [126] = "";

		read.getline(line, sizeof(line));

		if(strcmp(line, "") == 0)
			continue;

		if(strcmp(line, MUSIC.c_str()) == 0)
			aSound = false;
		else if(strcmp(line, SOUND.c_str()) == 0)
			aSound = true;
		else
			continue;
		
		while (!read.eof() && strcmp(line, END.c_str()) != 0)
		{
			char path [52];
			bool loop;

			read.getline(line, sizeof(line));
			TrimAndSet(line);

			sscanf_s(line, "%s = %s", &command, sizeof(command), &key, sizeof(key));

			if(strcmp(command, PATH.c_str()) == 0)
			{
				strcpy(path, key);
			}
			else if(strcmp(command, LOOP.c_str()) == 0)
			{
				if(strcmp(key, "TRUE") == 0)
					loop = true;
				else
					loop = false;
			}
			else if(strcmp(command, END.c_str()) == 0)
			{
				if(!aSound)
				{
					if(!zEngine->LoadMusicIntoSystem(path, loop))
						MaloW::Debug("Cant load music sound:" + std::string(line));
				}
				else
				{
					if(!zEngine->LoadSoundIntoSystem(path, loop))
						MaloW::Debug("Cant load sound sound:" + std::string(line));
				}
			}
		}

		
	}
}