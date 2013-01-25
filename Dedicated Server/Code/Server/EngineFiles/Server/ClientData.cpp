#include "ClientData.h"

unsigned long ClientData::zUniqeIPID = 0;

ClientData::ClientData(MaloW::ClientChannel* cc)
{
	zClient = cc;
	zPinged = false;
	zCurrentPingTime = 0.0f;
	zTotalPingTime = 0.0f;
	zMaxPingTime = 0.0f;
	zNrOfPings = 0;
	zMaxPingTime = 10.0f;
}

ClientData::~ClientData()
{
	SAFE_DELETE(this->zClient);
}

void ClientData::HandlePingMsg()
{
	if(zTotalPingTime > zMaxPingTime)
		ResetPingCounter();

	zTotalPingTime += zCurrentPingTime;
	zNrOfPings++;

	zPinged = false;
	zCurrentPingTime = 0.0f;
}

bool ClientData::CalculateLatency( float& latencyOut )
{
	if( zNrOfPings == 0)
		return false;

	latencyOut = zTotalPingTime / zNrOfPings;

	return true;
}

bool ClientData::CreateImportantMessage( const float sentTime, const std::string& message, const float timeToResend /*= DEFAULT_MAX_TIME_RESEND*/, const float nrToResend /*= DEFAULT_MAX_NR_RESEND*/, const float uniqe_ID )
{
	if(uniqe_ID >= this->zUniqeIPID)
		return false;

	ImportantMSG* new_important_msg = new ImportantMSG();

	new_important_msg->msg = message;
	new_important_msg->timeSent = sentTime;
	new_important_msg->nrOfTimesSent = nrToResend;
	new_important_msg->MSG_ID = uniqe_ID;

	this->zImportantMessages.push_back(new_important_msg);

	SortIPM();

	return true;
}

void ClientData::SortIPM()
{
	unsigned int i, j;
	ImportantMSG* key;

	for(i = 1; i < this->zImportantMessages.size(); i++)
	{
		key = this->zImportantMessages[i];               
		j = i - 1;  

		while(j >= 0 && this->zImportantMessages[j]->MSG_ID > key->MSG_ID)
		{
			this->zImportantMessages[j + 1] = this->zImportantMessages[j];
			j--;
		}

		this->zImportantMessages[j + 1] = key;
	}
}

int ClientData::SearchIPM( unsigned long key )
{
	unsigned int first = 0;
	unsigned int last = this->zImportantMessages.size()-1;

	while (first <= last) 
	{
		int mid = (first + last) / 2;  // compute mid point.
		unsigned long target = this->zImportantMessages[mid]->MSG_ID;

		if (key > target) 
			first = mid + 1;  // repeat search in top half.
		else if (key < target) 
			last = mid - 1; // repeat search in bottom half.
		else
			return mid;     // found it. return position 
	}

	return -1;    // failed to find key
}