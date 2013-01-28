/*
Made by Edänge Simon 2013-01-24
for project Not Dead Yet at Blekinge tekniska högskola.
*/
#pragma once

#include "../Network/ClientChannel.h"
#include "../../../../../Source/MaloWLib/Safe.h"
#include <vector>

#define DEFAULT_MAX_TIME_RESEND		5.0f
#define DEFAULT_MAX_NR_RESEND		2

struct IMessage
{
	IMessage()
	{
		MSG_ID = 0;
		msg = "None";
		currentTime = 0.0f;
		nrOfTimesResent = 0;

		maxTimesToResend = 0;
		maxTimeToResend = 0;

		hasExceeded = false;
	}

	bool hasExceeded;
	unsigned long MSG_ID ;
	int nrOfTimesResent;
	int maxTimesToResend;
	float maxTimeToResend;
	float currentTime;
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
	inline const unsigned long GetNextIPID() {return zUniqeIPID++; }
	
	inline void SetPinged(const bool pinged) {zPinged = pinged;}
	inline void SetCurrentPingTime(float const cpt) {zCurrentPingTime = cpt;}
	inline bool HasBeenPinged() const {return zPinged;}
	inline void IncPingTime(float dt) {zCurrentPingTime += dt;}
	inline void ResetPingCounter() {zPinged = 0; zTotalPingTime = 0.0f;}

	/*! Sends a message to the client.*/
	inline void SendM(const std::string& msg) {zClient->sendData(msg);}
	/*! Handle the ping from client.*/
	void HandlePingMsg();
	/*! Updates the latency of this client.*/
	bool CalculateLatency(float& latencyOut);
	/*! Creates and adds an important message to the list and sends it.
		Returns false if the ID is not unique.
		Note: Check the function GetNextIPID().

		The message contains the message to be sent.
		Note: Message needs to contain the IMPORTANT_MESSAGE tag.

	*/
	bool SendIM(const std::string& message, const unsigned long uniqe_ID, 
				const float timeToResend = DEFAULT_MAX_TIME_RESEND, const int nrToResend = DEFAULT_MAX_NR_RESEND);

	/*! Handle unanswered Important messages, check them, resend them.*/
	void HandleNackIM(float dt);
	/*! Removes an important message.*/
	bool RemoveIM(unsigned long m_id);
	/*! */
	bool HasIM() const;
	/*! */
	int GetNrOfIMP() const {return zImportantMessages.size();}
	/*! Returns the number of how many important messages that has been exceeded.*/
	inline int GetNrOfExceededIM() const {return nrOfExceededMsg;}

private:
	/*! Sort the important message list using insertion.*/
	void SortIM();
	/*! Search in the important message list using binary search.*/
	int SearchIM(unsigned long key);

private:
	unsigned long zUniqeIPID;
	bool zPinged;
	float zCurrentPingTime;
	float zTotalPingTime;
	float zMaxPingTime;
	int zNrOfPings;
	int nrOfExceededMsg;

	MaloW::ClientChannel* zClient;
	std::vector<IMessage *> zImportantMessages;

};