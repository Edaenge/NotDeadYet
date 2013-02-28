#include "DeerActor.h"

DeerActor::DeerActor(PhysicsObject* pObj) : 
	AnimalActor()
{
	this->SetPhysicsObject(pObj);
	this->SetCameraOffset(Vector3(0.0f,1.4f,0.0f));
	this->SetHealth(80.0f);
	this->zHealthMax = 80.0f;
	/*this->SetIfPlayerControlled(isPlayerControlled);
	this->InitDeer();*/
}

DeerActor::~DeerActor()
{

}
//
//void DeerActor::InitDeer() throw(...)
//{
//	this->SetType(DEER);
//
//	float numberFromFile = 0;
//	char characters[16];
//
//	std::ifstream infile("deerVariables.txt");
//	if(infile.good()) //Getting values from a textfile needs to be more properly implemented
//	{
//		//Health
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->SetHealth(numberFromFile);
//
//		//Max fear
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->SetFearMax(numberFromFile);
//
//		//fear interval
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zFearInterval = numberFromFile;
//
//		//minimum distance
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atoi(characters);
//		this->zMinimumDistance = numberFromFile;
//
//		//field of view
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zFieldOfView = numberFromFile;
//
//		//Threat movement speed threshold
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zThreatMovementSpeedThreshold = numberFromFile;
//
//		//Confidence koef
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zConfidenceKoef = numberFromFile;
//
//		//Distance too close for comfort
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zTooCloseInDistance = numberFromFile;
//
//		//extra fear with more players
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zExtraFearWithNumberOfPlayers = numberFromFile;
//
//		//extra fear with something too close
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zExtraFearWithCloseProximity = numberFromFile;
//
//		//extra fear when seeing threat
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zExtraFearAtSight = numberFromFile;
//
//		//extra fear when threat moves too quickly
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zExtraFearAtThreatMovementSpeed = numberFromFile;
//
//		//amount of fear decrease
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zFearDecrease = numberFromFile;
//
//		//fear when hurt
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zFearAtDamageDone = numberFromFile;
//
//		//fear level between suspicious and aggressive.
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zSupspiciousToAggressiveThreshold = numberFromFile;
//
//		//fear level between aggressive and afraid
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zAggressiveToAfraidThreshold = numberFromFile;
//
//		//distance to walk when calm
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zDistanceToWalkWhenCalm = numberFromFile;
//
//		//distance to walk when suspicious
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zDistanceToWalkWhenSuspicious = numberFromFile;
//
//		//factor to use when chosing a new target
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zNewTargetCloseByAFactorOf = numberFromFile;
//
//		//flee distance
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zFleeDistance = numberFromFile;
//
//		//How afraid to be when a large sound even happens
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zLargeSoundEventFearIncrease = numberFromFile;
//
//		//The range of the pausing time when calm.
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zCalmRandomInterval = numberFromFile;
//
//		//The minimum value 
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zCalmRandomAddition = numberFromFile;
//
//		//the range of the pausing time when suspicious.
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zSupsiciousRandomInterval = numberFromFile;
//
//		//the minimum value
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zSupsiciousAddition = numberFromFile;
//
//		//the walking velocity
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zWalkingVelocity = numberFromFile;
//
//		//the velocity when attacking
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zAttackingVelocity = numberFromFile;
//		
//		//the fleeing velocity
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zFleeingVelocity = numberFromFile;
//
//
//	}
//	else
//	{
//		this->SetHealth(100);
//		this->SetFearMax(100);
//		this->zFearInterval = 1;
//		this->zMinimumDistance = 40;
//		this->zFieldOfView = 0.4f;
//		this->zThreatMovementSpeedThreshold = 2;
//		this->zConfidenceKoef = 1;
//		this->zTooCloseInDistance = 10;
//		this->zExtraFearWithNumberOfPlayers = 10;
//		this->zExtraFearWithCloseProximity = 12;
//		this->zExtraFearAtSight = 4;
//		this->zExtraFearAtThreatMovementSpeed = 8;
//		this->zFearDecrease = 2;
//		this->zFearAtDamageDone = 30;
//		this->zSupspiciousToAggressiveThreshold = 20;
//		this->zAggressiveToAfraidThreshold = 35;
//		this->zDistanceToWalkWhenCalm = 14;
//		this->zDistanceToWalkWhenSuspicious = 8;
//		this->zNewTargetCloseByAFactorOf = 4;
//		this->zFleeDistance = 40;
//		this->zLargeSoundEventFearIncrease = 60;
//		this->zCalmRandomInterval = 5;
//		this->zCalmActionInterval = 4;
//		this->zSupsiciousRandomInterval = 8;
//		this->zSupsiciousAddition = 6;
//		
//		this->zWalkingVelocity = 1.2f;
//		this->zAttackingVelocity = 4.4f;
//		this->zFleeingVelocity = 7.8f;
//		//throw("File could not be opened!");
//
//
//	}
//	if(infile.is_open())
//	{
//		infile.close();
//	}
//	this->SetFearLevel(0);
//	this->zFearIntervalCounter = 0;
//	this->SetBehaviour(CALM);
//	this->zCalmActionInterval = rand () % zCalmRandomInterval + zCalmActionInterval; 
//	this->SetIfNeedPath(true);
//
//	this->SetPreviousHealth( this->GetHealth() );
//
//	this->zPanic = false;
//
//}
//
//void DeerActor::Update( float deltaTime ) //Has become a rather large funtion. Could probably use some optimization and downsizing.
//{
//	if(this->GetIfPlayerControlled() == true)
//	{
//		this->UpdateForPlayer(deltaTime);
//	}
//	else
//	{
//		this->UpdateForAnimal(deltaTime);
//	}
//	
//}
//
//void DeerActor::UpdateForAnimal(float deltaTime)
//{
//	//static float testInterval = 0; //Just for debugging.
//	//testInterval += deltaTime;
//	this->zIntervalCounter += deltaTime;
//	this->zFearIntervalCounter += deltaTime;
//	//this->zAlertnessIntervalCounter += deltaTime;
//	
//	int nrOfPredators = 0;
//	bool nearbyPredatorsExist = false;
//		
//	//Perform checking for entities here.
//	float shortestDistance = 99999;
//
//	float xDistance = 0;
//	float yDistance = 0;
//	float zDistance = 0;
//	float finalDistance = 0;
//
//	
//	//Determine closest threat/target
//	for(int i = 0; i < this->GetCurrentTargets(); i++)
//	{
//		xDistance = this->GetPosition().x - this->zTargets[i].position.x; //Math, could use optimization, I think.
//		//yDistance = this->GetPosition().y - this->zTargets[i].position.y;
//		zDistance = this->GetPosition().z - this->zTargets[i].position.z;
//		finalDistance = sqrt(xDistance * xDistance + zDistance * zDistance);
//		if( finalDistance < this->zMinimumDistance && this->zTargets[i].kind != DEER && this->zTargets[i].kind != RABBIT) 
//		{
//			this->zTargets[i].valid = true;
//			if(finalDistance < shortestDistance)
//			{
//				shortestDistance = finalDistance;
//				this->zMainTarget = this->zTargets[i]; //Decide which is the biggest threat here, i.e. the main target. For the moment, proximity is the deciding factor. Could use some more complexity.
//			}
//			nrOfPredators++;
//		}
//		else
//		{
//			this->zTargets[i].valid = false;
//		}
//		
//	}
//	if(nrOfPredators > 0)
//	{
//		nearbyPredatorsExist = true;
//	}
//	else
//	{
//		nearbyPredatorsExist = false;
//	}
//
//	
//	//Time to assess threats.
//
//	if( this->GetHealth() < this->GetPreviousHealth() ) //In theory, used to check if the animal has been attacked.
//	{
//		this->SetFearLevel( this->GetFearLevel() + this->zFearAtDamageDone);
//	}
//	this->SetPreviousHealth( this->GetHealth() );
//	
//	if(this->zFearIntervalCounter > this->zFearInterval) //Basically, each amount of fearinterval seconds the fear increases or decreases.
//	{
//		this->zFearIntervalCounter = 0;
//		
//		if(nearbyPredatorsExist)
//		{
//			//Getting values and such, comparing animal health against that of other entities, types and more.
//
//			//calculate current fear level:
//			float fear = 0;
//			//- for each dangerous entity detected, add some fear, fear could be based on type. Deers are afraid of humans for example.
//			fear += this->zExtraFearWithNumberOfPlayers * nrOfPredators;
//			
//			if(shortestDistance < this->zTooCloseInDistance) //The target is too close. Could expand this to incorporate more than one target.
//			{
//				fear += zExtraFearWithCloseProximity;
//			}
//
//			for(int i = 0; i < this->GetCurrentTargets(); i++)
//			{
//				if(this->zTargets[i].valid == true)
//				{
//				//Do a mathematical check, see if anyone is right in front of the deer. But... how? http://www.youtube.com/watch?v=gENVB6tjq_M
//					float dotProduct = this->GetDirection().GetDotProduct( this->zTargets[i].position - this->GetPosition() );
//					if(dotProduct > this->zFieldOfView)//This sight is relatively wide, since it is a deer. If this is true, then the deer sees a player.
//					{
//						//Which means, it is even more afraid.
//						fear += zExtraFearAtSight;
//					}
//					if(this->zTargets[i].movementNoise > this->zThreatMovementSpeedThreshold)
//					{
//						fear += this->zExtraFearAtThreatMovementSpeed;
//					}
//
//					if(this->zTargets[i].health != 0) // No dbz here!
//					{
//						fear -= (this->GetHealth() / this->zTargets[i].health) / nrOfPredators; //If the animal is faced with a very weak player(s), it gets some confidence. This is reduced with each player present.
//					}
//					
//				}
//			}			
//
//			this->SetFearLevel( this->GetFearLevel() + fear * this->zConfidenceKoef); //5 is unrelated to the movementNoise. Probably not good enough math. The theory is that the animal is constantly getting more afraid.
//		}
//		else //No threat detected. Calming down.
//		{
//			this->SetFearLevel( this->GetFearLevel() - this->zFearDecrease);
//		}
//	}
//		
//	
//	
//	
//	//Change state of mind.
//	if(this->GetFearLevel() == 0 && !nearbyPredatorsExist)
//	{
//		this->SetBehaviour(CALM);
//		//this->SetScale(Vector3(0.01f, 0.01f, 0.01f));
//	}
//	else if(this->GetFearLevel() > 0 && this->GetFearLevel() <= this->zSupspiciousToAggressiveThreshold)
//	{
//		this->SetBehaviour(SUSPICIOUS);
//		//this->SetScale(Vector3(0.03f, 0.03f, 0.03f));
//	}
//	else if(this->GetFearLevel() > this->zSupspiciousToAggressiveThreshold && this->GetFearLevel() <= zAggressiveToAfraidThreshold && nearbyPredatorsExist)
//	{
//		if(this->GetBehaviour() != AGGRESSIVE)
//		{
//			this->zCurrentPath.clear();
//			this->SetIfNeedPath(true);
//			this->SetBehaviour(AGGRESSIVE);
//			//this->SetScale(Vector3(0.05f, 0.05f, 0.05f));
//		}
//		
//	}
//	else if(this->GetFearLevel() > zAggressiveToAfraidThreshold && this->GetFearLevel() <= this->GetFearMax())
//	{
//		if(this->GetBehaviour() != AFRAID)
//		{
//			this->zCurrentPath.clear();
//			this->SetIfNeedPath(true);
//			this->SetBehaviour(AFRAID);
//			//this->SetScale(Vector3(3.09f, 3.09f, 3.09f));
//			
//		}
//		
//	}
//	
//	
//	//Act based on state of mind.
//	if(this->GetBehaviour() == CALM) //Relaxed behaviour. No threat detected.
//	{
//		this->zCurrentDistanceFled = 0;
//		this->zPanic = false;
//		if(this->zIntervalCounter > this->zCalmActionInterval && this->GetIfNeedPath())
//		{
//			this->zIntervalCounter = 0;
//			srand(time(NULL));
//			this->zCalmActionInterval = rand() % this->zCalmRandomInterval + this->zCalmRandomAddition; 
//			this->zCurrentPath.clear(); //Since a new path is gotten, and the old one might not have been completed, we clear it just in case.
//			//this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, this->GetPosition().x + rand() % 14 - 7, this->GetPosition().z + rand() % 14 - 7, this->zCurrentPath, 20); //Get a small path to walk, short and does not have to lead anywhere.
//			this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().z, this->GetPosition().x + rand() % zDistanceToWalkWhenCalm - zDistanceToWalkWhenCalm/2, this->GetPosition().z + rand() % zDistanceToWalkWhenCalm - zDistanceToWalkWhenCalm/2, this->zCurrentPath, 20);
//			this->SetIfNeedPath(false);
//		}
//	}
//	else if(this->GetBehaviour() == SUSPICIOUS) //Might have heard something, is suspicious.
//	{
//		this->zCurrentDistanceFled = 0;
//		this->zPanic = false;
//		if(this->zIntervalCounter > this->zCalmActionInterval && this->GetIfNeedPath()) //The increase in time is supposed to represent listening, waiting for something to happen.
//		{
//			this->zIntervalCounter = 0;
//			srand(time(NULL));
//			this->zCalmActionInterval = rand() % this->zSupsiciousRandomInterval + this->zSupsiciousAddition;
//			this->zCurrentPath.clear();
//			//this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, this->GetPosition().x + rand() % 8 - 4, this->GetPosition().z + rand() % 8 - 4, this->zCurrentPath, 20); //Get a small path to walk, quite short (since the animal is nervous) and does not have to lead anywhere.
//			this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().z, this->GetPosition().x + rand() % zDistanceToWalkWhenSuspicious - zDistanceToWalkWhenSuspicious/2, this->GetPosition().z + rand() % zDistanceToWalkWhenSuspicious - zDistanceToWalkWhenSuspicious/2, this->zCurrentPath, 20);
//			this->SetIfNeedPath(false);
//		}
//	}
//	else if(this->GetBehaviour() == AGGRESSIVE) //Is outright trying to harm the target.
//	{
//		this->zCurrentDistanceFled = 0;
//		this->zPanic = false;
//		xDistance = this->GetPosition().x - this->zMainTarget.position.x;
//		//yDistance = this->GetPosition().y - this->zMainTarget.position.y;
//		zDistance = this->GetPosition().z - this->zMainTarget.position.z;
//		float lastDistance = sqrt(xDistance * xDistance + zDistance * zDistance);
//		if( this->GetIfNeedPath() == true )
//		{
//			this->SetIfNeedPath(false);
//			this->zCurrentPath.clear();
//			if( !this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().z, this->zMainTarget.position.x, this->zMainTarget.position.z, this->zCurrentPath, 40) == false ) //Get the path, with the target that is to be attacked as the goal position. Depending on the animal, make the distance slightly large. //!this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, this->zMainTarget.position.x, this->zMainTarget.position.z, this->zCurrentPath, 40) == false
//			{
//				this->SetIfNeedPath(true);
//			}
//			this->SetLastDistanceCheck( lastDistance );
//		}
//		if( this->zIntervalCounter > 1.5 && this->GetIfNeedPath() == false )
//		{
//			this->zIntervalCounter = 0;
//
//			if( lastDistance < this->GetLastDistanceCheck() / 2) // The animal has traveled towards its goal halfway, at this point, it is safe to asume the goal has moved.
//			{
//				this->zCurrentPath.clear();
//				this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().z, this->zMainTarget.position.x, this->zMainTarget.position.z, this->zCurrentPath, 30);
//				//this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, this->zMainTarget.position.x, this->zMainTarget.position.z, this->zCurrentPath, 40);
//			}
//
//			xDistance = 0;
//			yDistance = 0;
//			zDistance = 0;
//			float distance;
//			float shortestDistance = 99999;
//			Target mostLikelyTarget = this->zMainTarget;
//
//			for(int i = 0; i < this->GetCurrentTargets(); i++)
//			{
//				
//				if(this->zTargets[i].valid == true)
//				{
//					xDistance = this->GetPosition().x - this->zTargets[i].position.x;
//					//yDistance = this->GetPosition().y - this->zTargets[i].position.y;
//					zDistance = this->GetPosition().z - this->zTargets[i].position.z;
//					distance = sqrt(xDistance * xDistance + zDistance * zDistance);
//					
//					if(distance < shortestDistance) //Something that is a larger threat is based on distance.
//					{
//						shortestDistance = distance;
//						mostLikelyTarget = this->zTargets[i];
//					}
//				}
//			}
//			if(shortestDistance < this->GetLastDistanceCheck() / this->zNewTargetCloseByAFactorOf) // The animal has gotten closer to a larger threat, and is now following that target instead.
//			{
//				this->SetIfNeedPath(true);
//				this->zMainTarget = mostLikelyTarget;
//			}
//		}
//		
//	}
//	else if(this->GetBehaviour() == AFRAID) //Is afraid, need to run.
//	{
//
//		if(this->GetIfNeedPath() == true)
//		{
//			this->SetIfNeedPath(false);
//			
//
//			if(nearbyPredatorsExist)
//			{
//				this->zDestination = this->ExaminePathfindingArea();
//				
//
//				this->zCurrentPath.clear();
//				if(!this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().z, this->zDestination.x, this->zDestination.z,this->zCurrentPath,30) ) //!this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, awayFromThreatX, awayFromThreatZ,this->zCurrentPath,80)
//				{
//					this->SetIfNeedPath(true);
//				}
//
//			}
//			else if(this->zPanic == true)
//			{
//				//Get random direction and run there.
//				float awayFromThreatX;
//				float awayFromThreatZ;
//				int directionX = rand() % 1; 
//				int directionZ = rand() % 1; 
//				if(directionX == 0)
//				{
//					awayFromThreatX = this->GetPosition().x + this->zFleeDistance;
//				}
//				else
//				{
//					awayFromThreatX = this->GetPosition().x - this->zFleeDistance;
//				}
//				if(directionZ == 0)
//				{
//					awayFromThreatZ = this->GetPosition().z + this->zFleeDistance;
//				}
//				else
//				{
//					awayFromThreatZ = this->GetPosition().z - this->zFleeDistance;
//				}
//				
//				this->zCurrentPath.clear();
//				if( !this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().z, awayFromThreatX, awayFromThreatZ,this->zCurrentPath,30) ) //!this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, awayFromThreatX, awayFromThreatZ,this->zCurrentPath,80)
//				{
//					this->SetIfNeedPath(true);
//				}
//			}
//			else //It has started to run, but still need to go further.
//			{
//				Vector3 direction = this->zDirection;
//
//				direction.Normalize();
//
//				this->zDestination.x += ( direction.x * this->zFleeDistance );
//				this->zDestination.z += ( direction.z * this->zFleeDistance );
//				
//				this->zCurrentPath.clear();
//				if( !this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().z, this->zDestination.x, this->zDestination.z,this->zCurrentPath,30) ) //!this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, awayFromThreatX, awayFromThreatZ,this->zCurrentPath,80)
//				{
//					this->SetIfNeedPath(true);
//				}
//			}
//
//			
//			
//		}
//		else//It is already running.
//		{
//			//one or more entities should not collide with each other and stop. (I am not sure this is something to be handled here or elsewhere.
//			
//		}
//	}
//
//
//	//Move the animal along path.
//	if(this->zCurrentPath.size() > 0)
//	{
//		this->zPanic = false;
//
//		this->zPreviousPos = this->GetPosition();
//
//		bool reachedNode = false;
//		if( (this->GetPosition().x > this->zCurrentPath.back().x - 0.2 && this->GetPosition().x < this->zCurrentPath.back().x + 0.2) && ( this->GetPosition().z > this->zCurrentPath.back().y - 0.2 && this->GetPosition().z < this->zCurrentPath.back().y + 0.2 ) )
//		{
//			reachedNode = true;
//		}
//
//		if(reachedNode)
//		{
//			this->zCurrentPath.pop_back();
//			//reachedNode = false;
//		}
//
//		if(this->GetBehaviour() == CALM && this->zCurrentPath.size() > 0 || this->GetBehaviour() == SUSPICIOUS && this->zCurrentPath.size() > 0) // && !this->zCurrentPath.empty() is necessary to be used again to avoid getting into an unlikely but posssible error.
//		{
//
//			/*double result = atan2( (this->zCurrentPath.back().y - this->GetPosition().z), (this->zCurrentPath.back().x - this->GetPosition().x) );
//
//			result = result;
//			this->SetDirection( Vector3( cos(result), 0.0f, sin(result) )); */
//
//
//			Vector3 goal(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y);
//			Vector3 direction = goal - this->GetPosition();
//			direction.Normalize();
//			this->SetDirection( direction ); 
//			this->SetVelocity(this->zWalkingVelocity);
//			//if(testInterval > 1.0) //Mainly for testing purposes.
//			//{
//			//	testInterval = 0;
//			//	this->SetPosition(Vector3(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y) );
//			//}
//			
//			this->SetPosition(this->GetPosition() + this->GetDirection() * deltaTime * this->GetVelocity());
//		
//		}
//		else if(this->GetBehaviour() == AGGRESSIVE  && this->zCurrentPath.size() > 0)
//		{
//			/*double result = atan2( (this->zCurrentPath.back().y - this->GetPosition().z), (this->zCurrentPath.back().x - this->GetPosition().x) );
//
//			result = result;
//			this->SetDirection( Vector3( cos(result), 0.0f, sin(result) )); */
//
//			Vector3 goal(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y);
//			Vector3 direction = goal - this->GetPosition();
//			direction.Normalize();
//			this->SetDirection( direction ); 
//			this->SetVelocity(this->zAttackingVelocity);
//
//			this->SetPosition(this->GetPosition() + this->GetDirection() * deltaTime * this->GetVelocity());
//
//		}
//		else if(this->GetBehaviour() == AFRAID && this->zCurrentPath.size() > 0)
//		{
//			/*double result = atan2( (this->zCurrentPath.back().y - this->GetPosition().z), (this->zCurrentPath.back().x - this->GetPosition().x) );
//
//			result = result;
//			this->SetDirection( Vector3( cos(result), 0.0f, sin(result) )); */
//
//			Vector3 goal(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y);
//			Vector3 direction = goal - this->GetPosition();
//			direction.Normalize();
//			this->SetDirection( direction ); 
//			this->SetVelocity(this->zFleeingVelocity);
//
//			this->SetPosition(this->GetPosition() + this->GetDirection() * deltaTime * this->GetVelocity());
//			zCurrentDistanceFled += deltaTime * this->GetVelocity();
//
//		}
//		else if(this->GetBehaviour() == AFRAID && this->zCurrentDistanceFled < this->zFleeDistance)
//		{
//			this->SetIfNeedPath(true);
//		}
//	
//	}
//	else
//	{
//		this->SetIfNeedPath(true);
//	}
//
//
//
//	
//}
//
//void DeerActor::UpdateForPlayer(float deltaTime)
//{
//	//Empty for now. Unsure if it is I that has to work on this or otherwise.
//}
//
//void DeerActor::LargeSuddenSoundReaction()
//{
//	this->zPanic = true;
//	this->SetFearLevel( this->GetFearLevel() + zLargeSoundEventFearIncrease);
//}