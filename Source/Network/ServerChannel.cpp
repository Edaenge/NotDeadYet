#include "ServerChannel.h"

using namespace MaloW;

#include "Network/NetworkMessageConverter.h"
#include "Network/NewPlayerEvent.h"

ServerChannel::ServerChannel()
{
	this->stayAlive = false;
	this->notifier = NULL;
}
int ServerChannel::InitConnection(std::string IP, int port)
{
	int returnCode = 0;
	this->stayAlive = true;
	this->notifier = NULL;

	WSADATA wsaData;
	int retCode = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(retCode != 0) 
	{
		returnCode = 2;
		this->stayAlive = false;
		MaloW::Debug("Failed to init Winsock library. Error: " + MaloW::convertNrToString(WSAGetLastError()));
		WSACleanup();
	}

	// open a socket
	this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(this->sock == INVALID_SOCKET) 
	{
		returnCode = 3;
		this->stayAlive = false;
		MaloW::Debug("Invalid socket, failed to create socket. Error: " + MaloW::convertNrToString(WSAGetLastError()));
		WSACleanup();
	}

	// connect
	sockaddr_in saServer;
	saServer.sin_port = htons(port);
	saServer.sin_addr.s_addr = inet_addr(IP.c_str());
	saServer.sin_family = AF_INET;
	retCode = connect(this->sock, (sockaddr*)&saServer, sizeof(sockaddr));
	if(retCode == SOCKET_ERROR)
	{
		returnCode = 1;
		this->stayAlive = false;
		MaloW::Debug("Connection to server failed. Error: " + MaloW::convertNrToString(WSAGetLastError()));
		WSACleanup();
	}

	this->buffer = "";
	this->unImportantFilter = "";

	return returnCode;
}
ServerChannel::~ServerChannel()
{
	this->stayAlive = false;
	if(this->sock != 0)
	{
		// shutdown socket
		int retCode = shutdown(this->sock, SD_BOTH);
		if(retCode == SOCKET_ERROR) 
		{
			MaloW::Debug("Error trying to perform shutdown on socket. Error: " + MaloW::convertNrToString(WSAGetLastError()));
		}
		// close server socket
		retCode = closesocket(this->sock);
		if(retCode == SOCKET_ERROR) 
		{
			MaloW::Debug("Error failed to close socket. Error: " + MaloW::convertNrToString(WSAGetLastError()));
		}
	}
	// Release WinSock DLL
	int retCode = WSACleanup();
	if(retCode == SOCKET_ERROR) 
	{
		MaloW::Debug("Error cleaning up Winsock Library. Error: " + MaloW::convertNrToString(WSAGetLastError()));;
	}
}

string ServerChannel::receiveData()
{
	std::string msg = "";
	bool getNewData = true;
	if(!this->buffer.empty())
	{
		if(int pos = this->buffer.find(10))
		{
			msg = this->buffer.substr(0, pos);
			this->buffer = this->buffer.substr(pos+1, this->buffer.length());
			getNewData = false;
		}
	}
	if(getNewData)
	{
		bool goAgain = true;
		do
		{
			char bufs[1024] = {0};
			int retCode = 0;
			retCode = recv(this->sock, bufs, sizeof(bufs), 0);
			if(retCode == SOCKET_ERROR)
			{
				this->Close();
				MaloW::Debug("Error recieving data. Error: " + MaloW::convertNrToString(WSAGetLastError()) + ". Probably due to crash/improper dissconnect");
			}
			else if(retCode == 0)
			{
				this->Close();
				MaloW::Debug("Server dissconnected, closing.");
			}
			else
			{
				for(int i = 0; i < 1024; i++)
				{
					if(bufs[i] == 10)
						goAgain = false;
					if(bufs[i] != 0)
						this->buffer += bufs[i];
					else
						i = 1024;
				}
				if(!goAgain)
				{
					for(int i = 0; i < 1024; i++)
					{
						if(this->buffer[i] != 10)
							msg += this->buffer[i];
						else
						{
							this->buffer = this->buffer.substr(i+1, this->buffer.length());
							i = 1024;
						}
					}
				}
			}
		}
		while(goAgain && this->stayAlive);
	}

	return msg;
}

void ServerChannel::sendData(string msg)
{
	msg += 10;
	char bufs[1024] = {0};
	for(int i = 0; i < msg.length(); i++)
	{
		bufs[i] = msg[i];
	}
	int retCode = send(this->sock, bufs, sizeof(bufs), 0);
	if(retCode == SOCKET_ERROR)
	{
		MaloW::Debug("Error sending data. Error: " + MaloW::convertNrToString(WSAGetLastError()));
	}
}

void ServerChannel::Life()
{
	NetworkMessageConverter converter = NetworkMessageConverter();
	while(this->stayAlive)
	{
		string msg = this->receiveData();
		if(msg != "")
		{
			if(this->notifier && this->stayAlive)
			{
				this->HandleMessage(msg);
			}
		}
	}
}

void ServerChannel::CloseSpecific()
{
	if(this->sock != 0)
	{
		int retCode = shutdown(this->sock, SD_BOTH);
		if(retCode == SOCKET_ERROR) 
			MaloW::Debug("Error trying to perform shutdown on socket from a ->Close() call. Error: " + MaloW::convertNrToString(WSAGetLastError()));
	
		retCode = closesocket(this->sock);
		if(retCode == SOCKET_ERROR) 
		{
			MaloW::Debug("Error failed to close socket. Error: " + MaloW::convertNrToString(WSAGetLastError()));
		}
	}
	this->sock = 0;
}
void ServerChannel::HandleMessage(std::string msg)
{
	NetworkMessageConverter msgHandler = NetworkMessageConverter();
	std::vector<std::string> msgArray;
	msgArray = msgHandler.SplitMessage(msg);
	if (msgArray.size() > 0)
	{
		if(strcmp(msgArray[0].c_str(), PLAYER_UPDATE.c_str()) == 0)
		{
			PlayerUpdateEvent* pue = new PlayerUpdateEvent();
			char key[512];
			for(int i = 0; i < msgArray.size(); i++)
			{
				sscanf(msgArray[i].c_str(), "%s ", key);

				if(strcmp(key, PLAYER_UPDATE.c_str()) == 0)
				{
					int id = msgHandler.ConvertStringToInt(PLAYER_UPDATE, msgArray[i]);
				}
				else if(strcmp(key, POSITION.c_str()) == 0)
				{
					D3DXVECTOR3 position = msgHandler.ConvertStringToVector(POSITION, msgArray[i]);
					pue->SetPlayerPosition(position);
				}
				else if(strcmp(key, ROTATION.c_str()) == 0)
				{
					D3DXQUATERNION rotation = msgHandler.ConvertStringToQuaternion(ROTATION, msgArray[i]);
					pue->SetPlayerRotation(rotation);
				}
				else if(strcmp(key, STATE.c_str()) == 0)
				{
					int state = msgHandler.ConvertStringToInt(STATE, msgArray[i]);
					pue->SetPlayerState(state);
				}
				else if(strcmp(key, MESH_MODEL.c_str()) == 0)
				{
					std::string filename = msgHandler.ConvertStringToSubstring(MESH_MODEL, msgArray[i]);
					pue->SetFilename(filename);
				}
			}
			this->notifier->PutEvent(pue);
		}
		else if (strcmp(msgArray[0].c_str(), NEW_PLAYER.c_str()) == 0)
		{
			NewPlayerEvent* npe = new NewPlayerEvent();
			char key[512];

			for(int i = 0; i < msgArray.size(); i++)
			{
				sscanf(msgArray[i].c_str(), "%s ", key);

				if(strcmp(key, PLAYER_UPDATE.c_str()) == 0)
				{
					int id = msgHandler.ConvertStringToInt(NEW_PLAYER, msgArray[i]);
					npe->SetClientID(id);
				}
				else if(strcmp(key, POSITION.c_str()) == 0)
				{
					D3DXVECTOR3 position = msgHandler.ConvertStringToVector(POSITION, msgArray[i]);
					npe->SetPlayerPosition(position);
				}
				else if(strcmp(key, ROTATION.c_str()) == 0)
				{
					D3DXQUATERNION rotation = msgHandler.ConvertStringToQuaternion(ROTATION, msgArray[i]);
					npe->SetPlayerRotation(rotation);
				}
				else if(strcmp(key, STATE.c_str()) == 0)
				{
					int state = msgHandler.ConvertStringToInt(STATE, msgArray[i]);
					npe->SetPlayerState(state);
				}
				else if(strcmp(key, MESH_MODEL.c_str()) == 0)
				{
					std::string filename = msgHandler.ConvertStringToSubstring(MESH_MODEL, msgArray[i]);
					npe->SetFilename(filename);
				}
			}
			this->notifier->PutEvent(npe);
		}
		else if(strcmp(msgArray[0].c_str(), PING.c_str()) == 0)
		{
			//Todo fixa kanske ID
			NetworkPacket* np = new NetworkPacket(msgArray[0], 0);

			this->notifier->PutEvent(np);
		}
	}
}