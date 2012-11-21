#pragma once

#include "stdafx.h"
#include "Process.h"


class Host : public MaloW::Process
{
public:
	Host();
	virtual ~Host();
	/*! Creates a Server locally
	returns a code that describes error or success*/
	int InitHost(int port);
	void Life();

private:

};
