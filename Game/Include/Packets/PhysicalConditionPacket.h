//Simon Edänge 2013-03-04

#include "Packet.h"

class PhysicalConditionPacket : public Packet
{
public:
	PhysicalConditionPacket()
	{
		this->zEnergy = -1.0f;
		this->zStamina = -1.0f;
		this->zHealth = -1.0f;
		this->zHunger = -1.0f;
		this->zHydration = -1.0f;
		this->zBleedingLevel = -1.0f;
	}
	virtual ~PhysicalConditionPacket() {}	

	virtual bool Serialize( std::ostream& ss ) const
	{
		ss.write( reinterpret_cast<const char*>(&this->zEnergy), sizeof(float) );
		ss.write( reinterpret_cast<const char*>(&this->zStamina), sizeof(float) );
		ss.write( reinterpret_cast<const char*>(&this->zHealth), sizeof(float) );
		ss.write( reinterpret_cast<const char*>(&this->zHunger), sizeof(float) );
		ss.write( reinterpret_cast<const char*>(&this->zHydration), sizeof(float) );
		ss.write( reinterpret_cast<const char*>(&this->zBleedingLevel), sizeof(float) );

		return true;
	}
	virtual bool Deserialize( std::istream& ss )
	{
		float temp;
		ss.read(reinterpret_cast<char*>(&temp), sizeof(float));
		zEnergy = temp;
		ss.read(reinterpret_cast<char*>(&temp), sizeof(float));
		zStamina = temp;
		ss.read(reinterpret_cast<char*>(&temp), sizeof(float));
		zHealth = temp;
		ss.read(reinterpret_cast<char*>(&temp), sizeof(float));
		zHunger = temp;
		ss.read(reinterpret_cast<char*>(&temp), sizeof(float));
		zHydration = temp;
		ss.read(reinterpret_cast<char*>(&temp), sizeof(float));
		zBleedingLevel = temp;

		return true;
	}

	inline virtual bool IsEmpty() const
	{
		return false;
	}

public:
	float zEnergy;
	float zStamina;
	float zHealth;
	float zHunger;
	float zHydration;
	float zBleedingLevel;
};