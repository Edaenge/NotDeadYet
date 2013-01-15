#ifndef MALOWFILEDEBUG_H
#define MALOWFILEDEBUG_H

#include <string>
#include <fstream>
#include "MaloW.h"

static bool erase = false;

namespace MaloW
{
	inline void Debug(std::string msg)
	{
		if(erase)
		{
			std::ofstream writeFile;
			writeFile.open ("MaloWDebug.txt", std::ios::out | std::ios::trunc);

			writeFile << "";

			writeFile.close();
			erase = false;
		}
		

		std::fstream writeFile;
		writeFile.open ("MaloWDebug.txt", std::fstream::in | std::fstream::out | std::fstream::app);

		writeFile << msg << std::endl;

		writeFile.close();
	}

	inline void Debug(float nr)
	{
		if(erase)
		{
			std::ofstream writeFile;
			writeFile.open ("MaloWDebug.txt", std::ios::out | std::ios::trunc);

			writeFile << "";

			writeFile.close();
			erase = false;
		}
		

		std::fstream writeFile;
		writeFile.open ("MaloWDebug.txt", std::fstream::in | std::fstream::out | std::fstream::app);

		writeFile << MaloW::convertNrToString(nr) << std::endl;

		writeFile.close();
	}

	inline void ClearDebug()
	{
		//if(erase)
		//{
			std::ofstream writeFile;
			writeFile.open ("MaloWDebug.txt", std::ios::out | std::ios::trunc);

			writeFile << "";

			writeFile.close();
			erase = false;
		//}
	}
}
#endif