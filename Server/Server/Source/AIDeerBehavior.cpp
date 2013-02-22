#include "AIDeerBehavior.h"
#include <time.h>


//STILL NEED TO ADD TARGETS AND HANDLE THEM

AIDeerBehavior::AIDeerBehavior( Actor* actor, World* world ) : Behavior(actor, world)
{
	InitValues();
}

bool AIDeerBehavior::InitValues()
{
	this->InitPathfinder();

	Target vectorFilling; 
	vectorFilling.health = 0;
	vectorFilling.movementNoise = 0;
	vectorFilling.position = Vector3(0,0,0);
	vectorFilling.valid = false;

	for(int i = 0; i < 32; i++)
	{
		this->zTargets.push_back(vectorFilling);
	}

	this->zIntervalCounter = 0;

	if(!this->SetValuesFromFile() ) 
	{
		//Values could not be retrieved from file, set default values.
		this->zFearInterval = 1.0f;
		this->zMinimumDistance = 30;
		this->zFieldOfView = 0.4f;
		this->zThreatMovementSpeedThreshold = 2.0f;
		this->zConfidenceKoef = 0.2f;
		this->zTooCloseInDistance = 10.0f;
		this->zExtraFearWithNumberOfPlayers = 10.0f;
		this->zExtraFearWithCloseProximity = 12.0f;
		this->zExtraFearAtSight = 4.0f;
		this->zExtraFearAtThreatMovementSpeed = 8.0f;
		this->zFearDecrease = 2.0f;
		this->zFearAtDamageDone = 30.0f;
		this->zSupspiciousToAggressiveThreshold = 20.0f;
		this->zAggressiveToAfraidThreshold = 35.0f;
		this->zDistanceToWalkWhenCalm = 14;
		this->zDistanceToWalkWhenSuspicious = 8;
		this->zNewTargetCloseByAFactorOf = 4;
		this->zFleeDistance = 80;
		this->zLargeSoundEventFearIncrease = 60;
		this->zCalmRandomInterval = 5;
		this->zCalmRandomAddition = 4;
		this->zSupsiciousRandomInterval = 8;
		this->zSupsiciousAddition = 6;
		this->zWalkingVelocity = 1.2f;
		this->zAttackingVelocity = 4.4f;
		this->zFleeingVelocity = 7.8f;
	}
	this->SetFearLevel(0.0f);
	this->zFearMax = 100.0f;
	DeerActor* dActor = dynamic_cast<DeerActor*>(this->zActor);
	this->SetPreviousHealth( dActor->GetHealth());

	
	this->zFearIntervalCounter = 0.0f;
	this->zIntervalCounter = 0.0f;

	return true;
}

bool AIDeerBehavior::SetValuesFromFile()
{
	float numberFromFile = 0;
	bool valuesRetrieved = false;
	char characters[16];

	std::ifstream infile("deerVariables.txt");
	if(infile.good())
	{

		//fear interval
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFearInterval = numberFromFile;

		//minimum distance
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atoi(characters);
		this->zMinimumDistance = (int)numberFromFile;

		//field of view
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFieldOfView = numberFromFile;

		//Threat movement speed threshold
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zThreatMovementSpeedThreshold = numberFromFile;

		//Confidence koef
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zConfidenceKoef = numberFromFile;

		//Distance too close for comfort
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zTooCloseInDistance = numberFromFile;

		//extra fear with more players
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zExtraFearWithNumberOfPlayers = numberFromFile;

		//extra fear with something too close
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zExtraFearWithCloseProximity = numberFromFile;

		//extra fear when seeing threat
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zExtraFearAtSight = numberFromFile;

		//extra fear when threat moves too quickly
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zExtraFearAtThreatMovementSpeed = numberFromFile;

		//amount of fear decrease
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFearDecrease = numberFromFile;

		//fear when hurt
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFearAtDamageDone = numberFromFile;

		//fear level between suspicious and aggressive.
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zSupspiciousToAggressiveThreshold = numberFromFile;

		//fear level between aggressive and afraid
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zAggressiveToAfraidThreshold = numberFromFile;

		//distance to walk when calm
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zDistanceToWalkWhenCalm = (int)numberFromFile;

		//distance to walk when suspicious
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zDistanceToWalkWhenSuspicious = (int)numberFromFile;

		//factor to use when chosing a new target
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zNewTargetCloseByAFactorOf = (int)numberFromFile;

		//flee distance
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFleeDistance = (int)numberFromFile;

		//How afraid to be when a large sound even happens
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zLargeSoundEventFearIncrease = (int)numberFromFile;

		//The range of the pausing time when calm.
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zCalmRandomInterval = (int)numberFromFile;

		//The minimum value 
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zCalmRandomAddition = (int)numberFromFile;

		//the range of the pausing time when suspicious.
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zSupsiciousRandomInterval = (int)numberFromFile;

		//the minimum value
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zSupsiciousAddition = (int)numberFromFile;

		//the walking velocity
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zWalkingVelocity = numberFromFile;

		//the velocity when attacking
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zAttackingVelocity = numberFromFile;
		
		//the fleeing velocity
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFleeingVelocity = numberFromFile;

		valuesRetrieved = true;
	}

	if(infile.is_open())
	{
		infile.close();
	}

	return valuesRetrieved;

}

bool AIDeerBehavior::InitPathfinder()
{
	this->zPathfinder.InitAI(0.5,95);
	this->zPathfinder.SetWorldPointer(this->zWorld);
	return true;
}

Vector3 AIDeerBehavior::ExaminePathfindingArea()
{
	DeerActor* dActor = dynamic_cast<DeerActor*>(this->zActor);

	Vector3 dest;
	//We make a check to see if the position is available, to avoid wasting time in the pathfinder functions.
	dest = this->zMainTarget.position - dActor->GetPosition();
	dest.Normalize();
	dest = dest * -1;
	dest *= (float)this->zFleeDistance;

	dest = dActor->GetPosition() + dest;
				
	bool getEmergencyDirection = false;
	bool foundPath = false;
	int counter = 0; //Just for testing stuff
	getEmergencyDirection = false;


	while(foundPath == false)
	{
		getEmergencyDirection = false;
		if(dest.x < 0)
		{
			dest.x = 0;
		}
		else if(dest.x > this->zWorld->GetWorldSize().x)
		{
			dest.x = this->zWorld->GetWorldSize().x - 1; 
		}

		if(dest.z < 0)
		{
			dest.z = 0;
		}
		else if(dest.z > this->zWorld->GetWorldSize().y)
		{
			dest.z = this->zWorld->GetWorldSize().y - 1;
		}
		while(this->zWorld->IsBlockingAt(Vector2(dest.x, dest.z)) && ( dest - dActor->GetPosition() ).GetLength() > 2)
		{
			dest = (dest - dActor->GetPosition());
			dest = dActor->GetPosition() + (dest - dest * 0.75);
						
		}
		if(( dest - dActor->GetPosition() ).GetLength() < 2)
		{
			getEmergencyDirection = true; //There is basically a thick wall in the way...
		}

		if(getEmergencyDirection) //...so let's find another direction.
		{
			//this->zDestination.Normalize();
			counter++;
			float angle = (-10 * counter) * 3.14f/180;
			float oldX, oldY;
			oldX = dest.x;
			oldY = dest.z;

			dest.x = cos(angle) * oldX - sin(angle) * oldY;
			dest.z = cos(angle) * oldY + sin(angle) * oldX;
			dest.Normalize();
			dest = dest * (float)zFleeDistance;

					
		}
		else
		{
			foundPath = true;
			return dest;
		}
	}
}

void AIDeerBehavior::SetTargetInfo(int number, Vector3 pos, float velocity, float health, Type kind)
{
	this->zTargets[number].position = pos;
	this->zTargets[number].movementNoise = velocity;
	this->zTargets[number].health = health;
	this->zTargets[number].kind = kind;
}

bool AIDeerBehavior::Update( float dt )
{
	if ( Behavior::Update(dt) )
		return true;

	DeerActor* dActor = dynamic_cast<DeerActor*>(this->zActor);

	static float testInterval = 0; //Just for debugging.
	testInterval += dt;
	this->zIntervalCounter += dt;
	//this->zIntervalCounter += dt;
	this->zFearIntervalCounter += dt;
	//this->zAlertnessIntervalCounter += deltaTime;
	
	int nrOfPredators = 0;
	bool nearbyPredatorsExist = false;
		
	//Perform checking for entities here.
	float shortestDistance = 99999;

	float xDistance = 0;
	float yDistance = 0;
	float zDistance = 0;
	float finalDistance = 0;

	int maximumNodesTest = 40;

	//Determine closest threat/target
	for(int i = 0; i < this->GetCurrentTargets(); i++)
	{
		xDistance = dActor->GetPosition().x - this->zTargets[i].position.x; //Math, could use optimization, I think.
		//yDistance = this->GetPosition().y - this->zTargets[i].position.y;
		zDistance = dActor->GetPosition().z - this->zTargets[i].position.z;
		finalDistance = sqrt(xDistance * xDistance + zDistance * zDistance);
		if( finalDistance < this->zMinimumDistance && this->zTargets[i].kind != DEER) 
		{
			this->zTargets[i].valid = true;
			if(finalDistance < shortestDistance)
			{
				shortestDistance = finalDistance;
				this->zMainTarget = this->zTargets[i]; //Decide which is the biggest threat here, i.e. the main target. For the moment, proximity is the deciding factor. Could use some more complexity.
			}
			nrOfPredators++;
		}
		else
		{
			this->zTargets[i].valid = false;
		}
	}
	if(nrOfPredators > 0)
	{
		nearbyPredatorsExist = true;
	}
	else
	{
		nearbyPredatorsExist = false;
	}

	//Time to assess threats.

	if( dActor->GetHealth() < this->GetPreviousHealth() ) //In theory, used to check if the animal has been attacked.
	{
		this->SetFearLevel( this->GetFearLevel() + this->zFearAtDamageDone);
	}
	this->SetPreviousHealth( dActor->GetHealth() );
	
	if(this->zFearIntervalCounter > this->zFearInterval) //Basically, each amount of fearinterval seconds the fear increases or decreases.
	{
		this->zFearIntervalCounter = 0;
		
		if(nearbyPredatorsExist)
		{
			//Getting values and such, comparing animal health against that of other entities, types and more.

			//calculate current fear level:
			float fear = 0;
			//- for each dangerous entity detected, add some fear, fear could be based on type. Deers are afraid of humans for example.
			fear += this->zExtraFearWithNumberOfPlayers * nrOfPredators;
			
			if(shortestDistance < this->zTooCloseInDistance) //The target is too close. Could expand this to incorporate more than one target.
			{
				fear += zExtraFearWithCloseProximity;
			}

			for(int i = 0; i < this->GetCurrentTargets(); i++)
			{
				if(this->zTargets[i].valid == true)
				{
				//Do a mathematical check, see if anyone is right in front of the deer. But... how? http://www.youtube.com/watch?v=gENVB6tjq_M
					Vector3 actorPosition = dActor->GetPosition();
					float dotProduct = dActor->GetDir().GetDotProduct( this->zTargets[i].position - actorPosition );
					if(dotProduct > this->zFieldOfView)//This sight is relatively wide, since it is a deer. If this is true, then the deer sees a player.
					{
						//Which means, it is even more afraid.
						fear += zExtraFearAtSight;
					}
					if(this->zTargets[i].movementNoise > this->zThreatMovementSpeedThreshold)
					{
						fear += this->zExtraFearAtThreatMovementSpeed;
					}

					if(this->zTargets[i].health != 0) // No dbz here!
					{
						fear -= (dActor->GetHealth() / this->zTargets[i].health) / nrOfPredators; //If the animal is faced with a very weak player(s), it gets some confidence. This is reduced with each player present.
					}
				}
			}			

			this->SetFearLevel( this->GetFearLevel() + fear * this->zConfidenceKoef); //5 is unrelated to the movementNoise. Probably not good enough math. The theory is that the animal is constantly getting more afraid.
		}
		else //No threat detected. Calming down.
		{
			this->SetFearLevel( this->GetFearLevel() - this->zFearDecrease);
		}
	}

	//Change state of mind.
	if(this->GetFearLevel() == 0 && !nearbyPredatorsExist)
	{
		this->SetMentalState(CALM);
		//this->SetScale(Vector3(0.01f, 0.01f, 0.01f));
	}
	else if(this->GetFearLevel() > 0 && this->GetFearLevel() <= this->zSupspiciousToAggressiveThreshold)
	{
		this->SetMentalState(SUSPICIOUS);
		//this->SetScale(Vector3(0.03f, 0.03f, 0.03f));
	}
	else if(this->GetFearLevel() > this->zSupspiciousToAggressiveThreshold && this->GetFearLevel() <= zAggressiveToAfraidThreshold && nearbyPredatorsExist)
	{
		if(this->GetMentalState() != AGGRESSIVE)
		{
			this->zCurrentPath.clear();
			this->SetIfNeedPath(true);
			this->SetMentalState(AGGRESSIVE);
			//this->SetScale(Vector3(0.05f, 0.05f, 0.05f));
		}
	}
	else if(this->GetFearLevel() > zAggressiveToAfraidThreshold && this->GetFearLevel() <= this->GetFearMax())
	{
		if(this->GetMentalState() != AFRAID)
		{
			this->zCurrentPath.clear();
			this->SetIfNeedPath(true);
			this->SetMentalState(AFRAID);
			//this->SetScale(Vector3(3.09f, 3.09f, 3.09f));
		}
	}
	
	//Act based on state of mind.
	if(this->GetMentalState() == CALM) //Relaxed behaviour. No threat detected.
	{
		this->zCurrentDistanceFled = 0;
		this->zPanic = false;
		if(this->zIntervalCounter > this->zCalmActionInterval && this->GetIfNeedPath())
		{
			this->zIntervalCounter = 0;
			srand(time(0));
			this->zCalmActionInterval = (float)(rand() % this->zCalmRandomInterval + this->zCalmRandomAddition); 
			this->zCurrentPath.clear(); //Since a new path is gotten, and the old one might not have been completed, we clear it just in case.
			//this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, this->GetPosition().x + rand() % 14 - 7, this->GetPosition().z + rand() % 14 - 7, this->zCurrentPath, 20); //Get a small path to walk, short and does not have to lead anywhere.
			this->zPathfinder.Pathfinding(dActor->GetPosition().x, dActor->GetPosition().z, dActor->GetPosition().x + rand() % zDistanceToWalkWhenCalm - zDistanceToWalkWhenCalm/2, dActor->GetPosition().z + rand() % zDistanceToWalkWhenCalm - zDistanceToWalkWhenCalm/2, this->zCurrentPath, maximumNodesTest);
			this->SetIfNeedPath(false);
		}
	}
	else if(this->GetMentalState() == SUSPICIOUS) //Might have heard something, is suspicious.
	{
		this->zCurrentDistanceFled = 0;
		this->zPanic = false;
		if(this->zIntervalCounter > this->zCalmActionInterval && this->GetIfNeedPath()) //The increase in time is supposed to represent listening, waiting for something to happen.
		{
			this->zIntervalCounter = 0;
			srand(time(0));
			this->zCalmActionInterval = (float)(rand() % this->zSupsiciousRandomInterval + this->zSupsiciousAddition);
			this->zCurrentPath.clear();
			//this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, this->GetPosition().x + rand() % 8 - 4, this->GetPosition().z + rand() % 8 - 4, this->zCurrentPath, 20); //Get a small path to walk, quite short (since the animal is nervous) and does not have to lead anywhere.
			this->zPathfinder.Pathfinding(dActor->GetPosition().x, dActor->GetPosition().z, dActor->GetPosition().x + rand() % zDistanceToWalkWhenSuspicious - zDistanceToWalkWhenSuspicious/2, dActor->GetPosition().z + rand() % zDistanceToWalkWhenSuspicious - zDistanceToWalkWhenSuspicious/2, this->zCurrentPath, maximumNodesTest);
			this->SetIfNeedPath(false);
		}
	}
	else if(this->GetMentalState() == AGGRESSIVE) //Is outright trying to harm the target.
	{
		this->zCurrentDistanceFled = 0;
		this->zPanic = false;
		xDistance = dActor->GetPosition().x - this->zMainTarget.position.x;
		//yDistance = this->GetPosition().y - this->zMainTarget.position.y;
		zDistance = dActor->GetPosition().z - this->zMainTarget.position.z;
		float lastDistance = sqrt(xDistance * xDistance + zDistance * zDistance);
		if( this->GetIfNeedPath() == true )
		{
			this->SetIfNeedPath(false);
			this->zCurrentPath.clear();
			if( !this->zPathfinder.Pathfinding(dActor->GetPosition().x, dActor->GetPosition().z, this->zMainTarget.position.x, this->zMainTarget.position.z, this->zCurrentPath, maximumNodesTest) == false ) //Get the path, with the target that is to be attacked as the goal position. Depending on the animal, make the distance slightly large. //!this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, this->zMainTarget.position.x, this->zMainTarget.position.z, this->zCurrentPath, 40) == false
			{
				this->SetIfNeedPath(true);
			}
			this->SetLastDistanceCheck( lastDistance );
		}
		if( this->zIntervalCounter > 1.5 && this->GetIfNeedPath() == false )
		{
			this->zIntervalCounter = 0;

			if( lastDistance < this->GetLastDistanceCheck() / 2) // The animal has traveled towards its goal halfway, at this point, it is safe to asume the goal has moved.
			{
				this->zCurrentPath.clear();
				this->zPathfinder.Pathfinding(dActor->GetPosition().x, dActor->GetPosition().z, this->zMainTarget.position.x, this->zMainTarget.position.z, this->zCurrentPath, maximumNodesTest);
				//this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, this->zMainTarget.position.x, this->zMainTarget.position.z, this->zCurrentPath, 40);
			}

			xDistance = 0;
			yDistance = 0;
			zDistance = 0;
			float distance;
			float shortestDistance = 99999;
			Target mostLikelyTarget = this->zMainTarget;

			for(int i = 0; i < this->GetCurrentTargets(); i++)
			{
				if(this->zTargets[i].valid == true)
				{
					xDistance = dActor->GetPosition().x - this->zTargets[i].position.x;
					//yDistance = this->GetPosition().y - this->zTargets[i].position.y;
					zDistance = dActor->GetPosition().z - this->zTargets[i].position.z;
					distance = sqrt(xDistance * xDistance + zDistance * zDistance);
					
					if(distance < shortestDistance) //Something that is a larger threat is based on distance.
					{
						shortestDistance = distance;
						mostLikelyTarget = this->zTargets[i];
					}
				}
			}
			if(shortestDistance < this->GetLastDistanceCheck() / this->zNewTargetCloseByAFactorOf) // The animal has gotten closer to a larger threat, and is now following that target instead.
			{
				this->SetIfNeedPath(true);
				this->zMainTarget = mostLikelyTarget;
			}
		}
	}
	else if(this->GetMentalState() == AFRAID) //Is afraid, need to run.
	{
		if(this->GetIfNeedPath() == true)
		{
			this->SetIfNeedPath(false);
			
			if(nearbyPredatorsExist)
			{
				this->zDestination = this->ExaminePathfindingArea();
				
				this->zCurrentPath.clear();
				if(!this->zPathfinder.Pathfinding(dActor->GetPosition().x, dActor->GetPosition().z, this->zDestination.x, this->zDestination.z,this->zCurrentPath, maximumNodesTest) ) //!this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, awayFromThreatX, awayFromThreatZ,this->zCurrentPath,80)
				{
					this->SetIfNeedPath(true);
				}
			}
			else if(this->zPanic == true)
			{
				//Get random direction and run there.
				float awayFromThreatX;
				float awayFromThreatZ;
				int directionX = rand() % 1; 
				int directionZ = rand() % 1; 
				if(directionX == 0)
				{
					awayFromThreatX = dActor->GetPosition().x + this->zFleeDistance;
				}
				else
				{
					awayFromThreatX = dActor->GetPosition().x - this->zFleeDistance;
				}
				if(directionZ == 0)
				{
					awayFromThreatZ = dActor->GetPosition().z + this->zFleeDistance;
				}
				else
				{
					awayFromThreatZ = dActor->GetPosition().z - this->zFleeDistance;
				}
				
				this->zCurrentPath.clear();
				if( !this->zPathfinder.Pathfinding(dActor->GetPosition().x, dActor->GetPosition().z, awayFromThreatX, awayFromThreatZ,this->zCurrentPath, maximumNodesTest) ) //!this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, awayFromThreatX, awayFromThreatZ,this->zCurrentPath,80)
				{
					this->SetIfNeedPath(true);
				}
			}
			else //It has started to run, but still need to go further.
			{
				Vector3 direction = dActor->GetDir();

				direction.Normalize();

				this->zDestination.x += ( direction.x * this->zFleeDistance );
				this->zDestination.z += ( direction.z * this->zFleeDistance );
				
				this->zCurrentPath.clear();
				if( !this->zPathfinder.Pathfinding(dActor->GetPosition().x, dActor->GetPosition().z, this->zDestination.x, this->zDestination.z,this->zCurrentPath, maximumNodesTest) ) //!this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, awayFromThreatX, awayFromThreatZ,this->zCurrentPath,80)
				{
					this->SetIfNeedPath(true);
				}
			}
		}
		else//It is already running.
		{
			//one or more entities should not collide with each other and stop. (I am not sure this is something to be handled here or elsewhere.	
		}
	}

	//Move the animal along path.
	if(this->zCurrentPath.size() > 0)
	{

		this->zPreviousVelocity = dActor->GetVelocity();
		this->zPanic = false;
		
		//this->zPreviousPos = this->GetPosition();

		bool reachedNode = false;
		if( (dActor->GetPosition().x > this->zCurrentPath.back().x - 0.2 && dActor->GetPosition().x < this->zCurrentPath.back().x + 0.2) && ( dActor->GetPosition().z > this->zCurrentPath.back().y - 0.2 && dActor->GetPosition().z < this->zCurrentPath.back().y + 0.2 ) )
		{
			reachedNode = true;
		}

		if(reachedNode)
		{
			this->zCurrentPath.pop_back();
			//reachedNode = false;
		}

		if(this->GetMentalState() == CALM && this->zCurrentPath.size() > 0 || this->GetMentalState() == SUSPICIOUS && this->zCurrentPath.size() > 0) // && !this->zCurrentPath.empty() is necessary to be used again to avoid getting into an unlikely but posssible error.
		{
			/*double result = atan2( (this->zCurrentPath.back().y - this->GetPosition().z), (this->zCurrentPath.back().x - this->GetPosition().x) );

			result = result;
			this->SetDirection( Vector3( cos(result), 0.0f, sin(result) )); */

			Vector3 goal(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y);
			Vector3 direction = goal - dActor->GetPosition();
			direction.Normalize();
			dActor->SetDir( direction ); 

			/*if(dActor->GetVelocity() > this->zWalkingVelocity)
			{
				dActor->SetVelocity(this->zPreviousVelocity - 100 * dt);
			}
			else if(dActor->GetVelocity() < this->zWalkingVelocity)
			{
				dActor->SetVelocity(this->zPreviousVelocity + 100 * dt);
			}*/
			dActor->SetVelocity(this->zWalkingVelocity);
			
			//if(testInterval > 2.0) //Mainly for testing purposes.
			//{
			//	testInterval = 0;
			//	dActor->SetPosition(Vector3(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y) );
			//}
			
			dActor->SetPosition(dActor->GetPosition() + dActor->GetDir() * dt * dActor->GetVelocity());
		
		}
		else if(this->GetMentalState() == AGGRESSIVE  && this->zCurrentPath.size() > 0)
		{
			/*double result = atan2( (this->zCurrentPath.back().y - this->GetPosition().z), (this->zCurrentPath.back().x - this->GetPosition().x) );

			result = result;
			this->SetDirection( Vector3( cos(result), 0.0f, sin(result) )); */

			Vector3 goal(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y);
			Vector3 direction = goal - dActor->GetPosition();
			direction.Normalize();
			dActor->SetDir( direction ); 
			
		/*	if(dActor->GetVelocity() > this->zAttackingVelocity)
			{
				dActor->SetVelocity(this->zPreviousVelocity - 100 * dt);
			}
			else if(dActor->GetVelocity() < this->zAttackingVelocity)
			{
				dActor->SetVelocity(this->zPreviousVelocity + 100 * dt);
			}*/
			dActor->SetVelocity(this->zAttackingVelocity);

			dActor->SetPosition(dActor->GetPosition() + dActor->GetDir() * dt * dActor->GetVelocity());

		}
		else if(this->GetMentalState() == AFRAID && this->zCurrentPath.size() > 0)
		{
			/*double result = atan2( (this->zCurrentPath.back().y - this->GetPosition().z), (this->zCurrentPath.back().x - this->GetPosition().x) );

			result = result;
			this->SetDirection( Vector3( cos(result), 0.0f, sin(result) )); */

			Vector3 goal(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y);
			Vector3 direction = goal - dActor->GetPosition();
			direction.Normalize();
			dActor->SetDir( direction ); 

			/*if(dActor->GetVelocity() > this->zFleeingVelocity)
			{
				dActor->SetVelocity(this->zPreviousVelocity - 100 * dt);
			}
			else if(dActor->GetVelocity() < this->zFleeingVelocity)
			{
				dActor->SetVelocity(this->zPreviousVelocity + 100 * dt);
			}*/
			dActor->SetVelocity(this->zFleeingVelocity);

			dActor->SetPosition(dActor->GetPosition() + dActor->GetDir() * dt * dActor->GetVelocity());
			this->zCurrentDistanceFled += dt * dActor->GetVelocity();

		}
		else if(this->GetMentalState() == AFRAID && this->zCurrentDistanceFled < this->zFleeDistance)
		{
			this->SetIfNeedPath(true);
		}
	}
	else
	{
		this->SetIfNeedPath(true);
	}

	//Rotate Animal
	static Vector3 defaultMeshDir = Vector3(0.0f, 0.0f, -1.0f);
	Vector3 meshDirection = dActor->GetDir();
	meshDirection.y = 0;
	meshDirection.Normalize();

	Vector3 around = Vector3(0.0f, 1.0f, 0.0f);
	float angle = acos(meshDirection.GetDotProduct(defaultMeshDir));

	if (meshDirection.x > 0.0f)
	 angle *= -1;

	dActor->SetRotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	dActor->SetRotation(around, angle);

	return false;
}