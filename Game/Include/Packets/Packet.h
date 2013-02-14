#pragma once

#include <iostream>


class Packet
{
public:
	virtual bool Serialize( std::ostream& ss ) const = 0;
	virtual bool Deserialize( std::istream& ss ) = 0;

	inline bool TrySerialize( std::ostream& ss ) const
	{
		try
		{
			return Serialize(ss);
		}
		catch(...)
		{
			return false;
		}
	}

	inline bool TryDeserialize( std::istream& ss )
	{
		try
		{
			return Deserialize(ss);
		}
		catch(...)
		{
			return false;
		}
	}
};