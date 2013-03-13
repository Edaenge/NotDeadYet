#include "ClientChannel.h"
#include "Winsock.h"
#include "ClientDisconnectedEvent.h"
#include "NetworkException.h"
#include "ReceiverProcess.h"
#include "SenderProcess.h"
#include "NetworkChannel.h"

using namespace MaloW;


ClientChannel::ClientChannel(MaloW::Process* observerProcess, SOCKET sock, const std::string& ip_adress) :
	zSocket(zSocket),
	zObserverProcess(observerProcess),
	zNetworkChannel(0),
	zReceiveProcess(0),
	zSenderProcess(0)
{
	static unsigned int id = 0;
	this->zID = id++;

	// IP Address
	this->zIP = ip_adress;

	// Create Network Channel
	zNetworkChannel = new NetworkChannel(sock);

	// Create Receiver Process
	zReceiveProcess = new ReceiverProcess(zObserverProcess, this);
}

ClientChannel::~ClientChannel()
{
	// Makes the threads quit
	if ( zSocket )
	{
		if ( shutdown(zSocket, SD_BOTH) == SOCKET_ERROR )
		{
			throw NetworkException("Failed Shutting Down Socket!", WSAGetLastError());
		}
	}

	// Wait For Receiver Process
	if ( zReceiveProcess )
	{
		zReceiveProcess->Close();
		zReceiveProcess->WaitUntillDone();
		delete zReceiveProcess;
		zReceiveProcess = 0;
	}

	// Wait For Sender Process
	if ( zSenderProcess )
	{
		zSenderProcess->Close();
		zSenderProcess->WaitUntillDone();
		delete zSenderProcess;
		zSenderProcess = 0;
	}

	// Delete Socket
	if ( zSocket )
	{
		closesocket(zSocket);
	}
}

void ClientChannel::Start()
{
	if ( zReceiveProcess ) zReceiveProcess->Start();
	if ( zSenderProcess ) zSenderProcess->Start();
}

void ClientChannel::Disconnect()
{
	zNetworkChannel->Disconnect();
}

void ClientChannel::Send(const std::string& message)
{
	zNetworkChannel->Send(message);
}

void ClientChannel::TrySend(const std::string& message)
{
	zNetworkChannel->TrySend(message);
}