/*
Made by Edänge Simon 2013-01-24
for project Not Dead Yet at Blekinge tekniska högskola.
*/
#pragma once

#include "../Network/ClientChannel.h"
#include "../../../../../Source/MaloWLib/Safe.h"
#include <vector>

#define DEFAULT_MAX_TIME_RESEND		3.0f
#define DEFAULT_MAX_NR_RESEND		2

struct ImportantMSG
{
	ImportantMSG()
	{
		MSG_ID = 0;
		msg = "None";
		timeSent = 0.0f;
		nrOfTimesSent = 0;
	}

	unsigned long MSG_ID ;
	int nrOfTimesSent;
	float timeSent;
	std::string msg;
};

class ClientData
{
public:
	ClientData(MaloW::ClientChannel* cc);
	virtual ~ClientData();

	inline MaloW::ClientChannel* GetClient() const {return zClient;}
	inline float GetCurrentPingTime() const {return zCurrentPingTime;}
	inline float GetTotalPingTime() const {return zTotalPingTime;}
	inline int GetNrOfPings() const {return zNrOfPings;}
	/*! Returns the next unique important message ID.*/
	inline long GetNextIPID() const {return zUniqeIPID; zUniqeIPID++;}

	inline void SetPinged(const bool pinged) {zPinged = pinged;}
	inline void SetCurrentPingTime(float const cpt) {zCurrentPingTime = cpt;}
	inline bool HasBeenPinged() const {return zPinged;}
	inline void IncPingTime(float dt) {zCurrentPingTime += dt;}
	inline void ResetPingCounter() {zPinged = 0; zTotalPingTime = 0.0f;}

	/*! Sends a message to the client.*/
	inline void SendMessage(const std::string& msg) {zClient->sendData(msg);}
	/*! Handle the ping from client.*/
	void HandlePingMsg();
	/*! Updates the latency of this client.*/
	bool CalculateLatency(float& latencyOut);
	/*! Creates and adds the message to the list.
		Returns false if the ID is not unique.
		Note: Check the function GetNextIPID().

		The message contains the message to be sent.
		Note: Message needs to contain the IMPORTANT_MESSAGE tag.

	*/
	bool CreateImportantMessage(const float sentTime, const std::string& message,
							const float uniqe_ID,
							const float timeToResend = DEFAULT_MAX_TIME_RESEND,  
							const float nrToResend = DEFAULT_MAX_NR_RESEND);

private:
	/*! Sort the important message list using insertion.*/
	void SortIPM();
	/*! Search in the important message list using binary search.*/
	int SearchIPM(unsigned long key);

private:
	static unsigned long zUniqeIPID;
	bool zPinged;
	float zCurrentPingTime;
	float zTotalPingTime;
	float zMaxPingTime;
	int zNrOfPings;

	MaloW::ClientChannel* zClient;
	std::vector<ImportantMSG *> zImportantMessages;

};