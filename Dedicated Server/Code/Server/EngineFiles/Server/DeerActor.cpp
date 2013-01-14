#include "DeerActor.h"

DeerActor::DeerActor( bool genID /*= true*/ ) : AnimalActor(genID)
{
	this->InitDeer();
}

DeerActor::DeerActor( const Vector3& startPos, bool genID /*= true*/ ) : AnimalActor(startPos, genID)
{
	this->InitDeer();
}

DeerActor::DeerActor( const Vector3& startPos, const Vector4& rot, bool genID /*= true*/ ) : AnimalActor(startPos, rot, genID)
{
	this->InitDeer();
}

DeerActor::~DeerActor()
{

}

void DeerActor::InitDeer() throw(...)
{
	unsigned int numberFromFile = 0;
	char characters[16];

	std::ifstream infile("deerVariables.txt");
	if(infile.good())
	{
		//Health
	//	infile.getline(characters, 256,' ');
	//	infile.ignore(256, '\n');
	//	numberFromFile = atoi(characters);
	//	this->SetHealth(numberFromFile);

		//Max Alertness
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = atoi(characters);
		this->SetAlertnessMax(numberFromFile);

		//Initial Alertness
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = atoi(characters);
		this->SetAlertnessLevel(numberFromFile);

		//Max fear
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = atoi(characters);
		this->SetFearMax(numberFromFile);

	}
	else
	{
		throw("File could not be opened!");
	}
	if(infile.is_open())
	{
		infile.close();
	}
	this->SetFearLevel(0);
	this->zFearIntervalCounter = 0;
	this->SetAlertnessLevel(0);
	this->zAlertnessIntervalCounter = 0;
	this->SetBehaviour(CALM);
	this->zCalmActionInterval = rand () % 5 + 3; 
	this->SetIfNeedPath(true);

}

void DeerActor::Update( float deltaTime )
{
	this->zIntervalCounter += deltaTime;
	this->zFearIntervalCounter += deltaTime;
	this->zAlertnessIntervalCounter += deltaTime;
	//Perform checking for entities here.
	//Remember, deers are good at hearing.
	
	
	//Problems:
		//- How does it know something has been loud? Quick movement?
		//- How to check for things in the surroundings.
	
	
	//Time to asses threats.
	/*
	if(this->zFearIntervalCounter > 1)
	{
		this->zFearIntervalCounter = 0;
		//Basically, each second the fear increases or decreases or remains unchanged.
		if(deer knows of entities)
		{
			//Getting values and such, comparing animal health against that of other entities, types and more.

			calculate current fear level:
			int fear = 0;
			- for each dangerous entity detected, add some fear, fear is to be based on type. Deers are afraid of humans, but not rabbits, for instance.
			fear += 10 * nrofentities;
			- add the difference in health. So, if the player only has 10 hp, and the animal has a 100. Then 100/10 + in fear
			(possibly something like, if the entity has more health, the animal is more afraid)

			
			//Decide which is the biggest threat here:

			//

			this->SetFearLevel( this->GetFearLevel() + fear / 5)
		}
		else if(No entities detected.)
		{
			this->SetFearLevel( this->GetFearLevel() - 2);
		}
		
		
	}
		
	*/
	
	
	
	if(this->GetFearLevel() == 0)
	{
		this->SetBehaviour(CALM);
	}
	else if(this->GetFearLevel() > 0 && this->GetFearLevel() <= 20 || this->GetAlertnessLevel() > 20)
	{
		this->SetBehaviour(SUSPICIOUS);
	}
	else if(this->GetFearLevel() > 20 && this->GetFearLevel() <= 35)
	{
		this->SetBehaviour(AGGRESSIVE);
	}
	else if(this->GetFearLevel() < 35 && this->GetFearLevel() <= this->GetFearMax())
	{
		this->SetBehaviour(AFRAID);
	}
	
	//	
	if(this->GetBehaviour() == CALM) //Relaxed behaviour. No threat detected.
	{
		if(this->zIntervalCounter > this->zCalmActionInterval)
		{
			this->zIntervalCounter = 0;
			this->zCalmActionInterval = rand () % 5 + 3; 
			this->zCurrentPath.clear(); //Since a new path is gotten, and the old one might not have been completed, we clear it just in case.
			//this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().y, this->GetPosition().x + random small value , this->GetPosition().y + random small value  .....) ; //Get a small path to walk, short and does not have to lead anywhere.
			this->SetIfNeedPath(true);
		}
	}
	else if(this->GetBehaviour() == SUSPICIOUS) //Might have heard something, is suspicious.
	{
		if(this->zIntervalCounter > this->zCalmActionInterval * 1.5) //The increase in time is supposed to represent listening, waiting for something to happen.
		{
			this->zIntervalCounter = 0;
			this->zCalmActionInterval = rand () % 5 + 3;
			this->SetIfNeedPath(true);
			//this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().y, ... ); //Get a small path to walk, quite short (since the animal is nervous) and does not have to lead anywhere.

		}
	}
	else if(this->GetBehaviour() == AGGRESSIVE) //Is outright trying to harm the target.
	{
		if(this->GetIfNeedPath() == true)
		{
			this->SetIfNeedPath(false);
			//this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().y   ....  ); //Get the path, with the target that is to be attacked as the goal position. Depending on the animal, make the distance slightly large.
			
			//int xDistance = this->GetPosition().x - target's position.x;
			//int yDistance = this->GetPosition().y - target's position.y;
			//this->SetLastDistanceCheck( sqrt( pow(xDistance, 2) + pow(yDistance, 2)));
			
		}
		if(this->zIntervalCounter > 1)
		{
			this->zIntervalCounter = 0;
			int xDistance = 0;//this->GetPosition().x - target's position.x;
			int yDistance = 0;//this->GetPosition().y - target's position.y;

			if( sqrt( pow(xDistance, 2) + pow(yDistance, 2)) < this->GetLastDistanceCheck() / 2) // The animal has traveled towards its goal halfway, at this point, it is safe to asume the goal has moved.
			{
				//if(/*Target initial position is too far away from where the target currently is, i.e, it has moved too far.*/)
				//{
					//this->SetIfNeedPath(true);
				//}
			}
			//for(/*each surrounding possible target */)
			//{
				//if(possible target is a bigger threat or more sensible goal, and sqrt( pow(xDistance to possible target, 2) + pow(yDistance distance to possible target, 2)) < this->GetLastDistanceCheck() / 4) // The animal has gotten closer to a larger threat, and is not following that target instead.
				//{
					//this->SetIfNeedPath(false);
					
				//}

			//}
			
		}
	}
	else if(this->GetBehaviour() == AFRAID) //Is afraid, running.
	{
		if(this->GetIfNeedPath() == true)
		{
			this->SetIfNeedPath(false);

			//if(/*has something to be afraid of*/)
			float awayFromThreatX = 0; //= (-)threat's x.pos + (rand() % 50 - 25 ) //Possibly with random, I just want to add a bit of a random element
			float awayFRomThreatY = 0; //= (-)threat's Y.pos + (rand() % 50 - 25 )
			//this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().y, awayFromThreatX, awayFRomThreatY   ....  ); //Relatively far.
		}
	}
	

	
}
