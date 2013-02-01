#pragma once

#include <string>
#include <fstream>
#include "../MaloWLib/Malow.h"

static bool eraseDebugFile = false;

namespace DebugMsg
{
	inline void Debug(std::string msg)
	{
		if(eraseDebugFile)
		{
			std::ofstream writeFile;
			writeFile.open ("MapDebugData.txt", std::ios::out | std::ios::trunc);

			writeFile << "";

			writeFile.close();
			eraseDebugFile = false;
		}


		std::fstream writeFile;
		writeFile.open ("MapDebugData.txt", std::fstream::in | std::fstream::out | std::fstream::app);

		writeFile << msg << std::endl;

		writeFile.close();
	}

	inline void Debug(float nr)
	{
		if(eraseDebugFile)
		{
			std::ofstream writeFile;
			writeFile.open ("MapDebugData.txt", std::ios::out | std::ios::trunc);

			writeFile << "";

			writeFile.close();
			eraseDebugFile = false;
		}


		std::fstream writeFile;
		writeFile.open ("MapDebugData.txt", std::fstream::in | std::fstream::out | std::fstream::app);

		writeFile << MaloW::convertNrToString(nr) << std::endl;

		writeFile.close();
	}

	inline void ClearDebug()
	{
		//if(eraseDebugFile)
		//{
		std::ofstream writeFile;
		writeFile.open ("MapDebugData.txt", std::ios::out | std::ios::trunc);

		writeFile << "";

		writeFile.close();
		eraseDebugFile = false;
		//}
	}
}