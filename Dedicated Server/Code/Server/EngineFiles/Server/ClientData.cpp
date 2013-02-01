#include "ClientData.h"

ClientData::ClientData(MaloW::ClientChannel* cc)
{
	zClient = cc;
	zPinged = false;
	zCurrentPingTime = 0.0f;
	zTotalPingTime = 0.0f;
	zMaxPingTime = 0.0f;
	zNrOfPings = 0;
	nrOfExceededMsg = 0;
	zMaxPingTime = 10.0f;
	zUniqeIPID = 0;
}

ClientData::~ClientData()
{
	SAFE_DELETE(this->zClient);

	for(auto it = zImportantMessages.begin(); it < zImportantMessages.end(); it++)
	{
		SAFE_DELETE((*it));
	}
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

bool ClientData::SendIM(const std::string& message, 
						const unsigned long uniqe_ID, const float timeToResend /*= DEFAULT_MAX_TIME_RESEND*/, 
						const int nrToResend /*= DEFAULT_MAX_NR_RESEND*/ )
{
	SendM(message);
	return true;
	if(uniqe_ID >= this->zUniqeIPID)
		return false;

	IMessage* new_important_msg = new IMessage();

	new_important_msg->msg = message;
	new_important_msg->currentTime = 0.0f;
	new_important_msg->maxTimesToResend = nrToResend;
	new_important_msg->maxTimeToResend = timeToResend;
	new_important_msg->MSG_ID = uniqe_ID;

	this->zImportantMessages.push_back(new_important_msg);

	SortIM();

	SendM(message);

	return true;
}

void ClientData::SortIM()
{
	unsigned int i, j;
	IMessage* key;

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

int ClientData::SearchIM( unsigned long key )
{
	if(!HasIM())
		return -1;

	 int first = 0;
	 int last = this->zImportantMessages.size()-1;

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

void ClientData::HandleNackIM( float dt )
{
	if(!HasIM())
		return;

	for (auto it = this->zImportantMessages.begin(); it < this->zImportantMessages.end(); it++)
	{
		(*it)->currentTime += dt;
		if( (*it)->currentTime >= (*it)->maxTimeToResend )
		{
			if((*it)->nrOfTimesResent >= (*it)->maxTimesToResend)
			{
				(*it)->hasExceeded = true;
				this->nrOfExceededMsg++;

				continue;
			}
			
			SendM((*it)->msg);
			(*it)->nrOfTimesResent++;
			(*it)->currentTime = 0.0f;
		}
	}
}

bool ClientData::HasIM() const
{
	if(zImportantMessages.empty())
		return false;

	return true;
}

bool ClientData::RemoveIM( unsigned long m_id )
{
	int index = SearchIM(m_id);

	if(index == -1)
		return false;

	IMessage* im = zImportantMessages[index];

	if(im->hasExceeded)
		nrOfExceededMsg--;

	this->zImportantMessages.erase(zImportantMessages.begin() + index);
	SAFE_DELETE(im);

	return true;
}