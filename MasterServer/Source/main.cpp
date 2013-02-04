#include <iostream>
#include "MasterServer.h"


int main()
{
	MasterServer *server;

	try
	{
		server = new MasterServer(26851);
		server->Start();
		server->WaitUntillDone();
	}
	catch(...)
	{
		std::cout << "Crashed!" << std::endl;
	}

	return 0;
}