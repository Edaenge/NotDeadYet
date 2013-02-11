#pragma once

#include <string>
#include <fstream>
#include <Malow.h>

static bool eraseFile = false;
static bool writeToFile = true;
static bool msgWrites = false;

namespace Messages
{
	inline void Debug(std::string msg)
	{
		if(eraseFile)
		{
			std::ofstream writeFile;
			writeFile.open ("ServerClientMessages.txt", std::ios::out | std::ios::trunc);

			writeFile << "";

			writeFile.close();
			eraseFile = false;
		}


		std::fstream writeFile;
		writeFile.open ("ServerClientMessages.txt", std::fstream::in | std::fstream::out | std::fstream::app);

		writeFile << msg << std::endl;

		writeFile.close();
	}

	inline void Debug(float nr)
	{
		if(eraseFile)
		{
			std::ofstream writeFile;
			writeFile.open ("ServerClientMessages.txt", std::ios::out | std::ios::trunc);

			writeFile << "";

			writeFile.close();
			eraseFile = false;
		}


		std::fstream writeFile;
		writeFile.open ("ServerClientMessages.txt", std::fstream::in | std::fstream::out | std::fstream::app);

		writeFile << MaloW::convertNrToString(nr) << std::endl;

		writeFile.close();
	}

	inline void ClearDebug()
	{
		//if(eraseFile)
		//{
		std::ofstream writeFile;
		writeFile.open ("ServerClientMessages.txt", std::ios::out | std::ios::trunc);

		writeFile << "";

		writeFile.close();
		eraseFile = false;
		//}
	}
	inline bool FileWrite()
	{
		return writeToFile;
	}
	inline bool MsgFileWrite()
	{
		return msgWrites;
	}
}