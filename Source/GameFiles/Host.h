#pragma once

#include "stdafx.h"
#include "Process.h"

class Host : MaloW::Process
{
public:
	Host();
	virtual ~Host();
	/*! Creates a Server locally
	returns a code that describes error or success*/
	int InitHost();
	void Life();

private:

};
