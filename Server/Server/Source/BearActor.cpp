#include "BearActor.h"

BearActor::BearActor()
{
	/*this->SetIfPlayerControlled(isPlayerControlled);
	this->InitBear();
	*/
}

BearActor::~BearActor()
{

}
//
//void BearActor::InitBear() throw(...)
//{
//	this->SetType(BEAR);
//
//	float numberFromFile = 0;
//	char characters[16];
//
//	std::ifstream infile("bearVariables.txt");
//	if(infile.good()) //Getting values from a text file needs to be more properly implemented
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
//		numberFromFile = atof(characters);
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
//		//fear level between calm and aggressive.
//		infile.getline(characters, 256,' ');
//		infile.ignore(256, '\n');
//		numberFromFile = atof(characters);
//		this->zCalmToAggressiveThreshold = numberFromFile;
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
//	}
//	else
//	{
//		this->SetHealth(200);
//		this->SetFearMax(200);
//		this->zFearInterval = 1;
//		this->zMinimumDistance = 40;
//		this->zFieldOfView = 0.4;
//		this->zThreatMovementSpeedThreshold = 2;
//		this->zConfidenceKoef = 1;
//		this->zExtraFearWithNumberOfPlayers = 10;
//		this->zExtraFearWithCloseProximity = 12;
//		this->zExtraFearAtSight = 4;
//		this->zExtraFearAtThreatMovementSpeed = 8;
//		this->zFearDecrease = 2;
//		this->zFearAtDamageDone = 30;
//		this->zCalmToAggressiveThreshold = 20;
//		this->zDistanceToWalkWhenCalm = 14;
//		this->zDistanceToWalkWhenSuspicious = 8;
//		this->zNewTargetCloseByAFactorOf = 4;
//		this->zFleeDistance = 40;
//		this->zLargeSoundEventFearIncrease = 60;
//		this->zCalmRandomInterval = 5;
//		this->zCalmRandomAddition = 3;
//
//		this->zWalkingVelocity = 2.0;
//		this->zAttackingVelocity = 5.0;
//		this->zFleeingVelocity = 4.8;
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
//	//this->SetAlertnessLevel(0);
//	//this->zAlertnessIntervalCounter = 0;
//	this->SetBehaviour(CALM);
//	this->zCalmActionInterval = rand () % this->zCalmRandomInterval + this->zCalmRandomAddition; 
//	this->SetIfNeedPath(true);
//
//	this->SetPreviousHealth( this->GetHealth() );
//
//}
//
//void BearActor::Update( float deltaTime ) //Has become a rather large funtion. Could probably use some optimization and downsizing.
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
//void BearActor::UpdateForAnimal(float deltaTime)
//{
//	this->zIntervalCounter += deltaTime;
//	this->zFearIntervalCounter += deltaTime;
//	//this->zAlertnessIntervalCounter += deltaTime;
//	
//	int nrOfPredators = 0;
//	bool nearbyPredatorsExist = false;
//
//	//Perform checking for entities here.
//
//	int shortestDistance = 99999;
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
//		if( finalDistance < this->zMinimumDistance && this->GetPosition().x != this->zTargets[i].position.x && this->GetPosition().z != this->zTargets[i].position.z)  //Since everything could be prey to a bear, even other bears, this check is needed to make sure he does not make himself his enemy.
//		{
//			this->zTargets[i].valid = true;
//
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
//		this->SetFearLevel( this->GetFearLevel() + 1);
//	}
//	this->SetPreviousHealth( this->GetHealth() );
//	
//	if(this->zFearIntervalCounter > 1)
//	{
//		this->zFearIntervalCounter = 0;
//		//Basically, each second the fear increases or decreases.
//		//NOTE: A bear's fear basically only indicates how much he wants to kill you.
//
//		if(nearbyPredatorsExist)
//		{
//			//Getting values and such, comparing animal health against that of other entities, types and more.
//
//			//calculate current fear level:
//			float fear = 0;
//			//- for each dangerous entity detected.
//			//fear += 2 * nrOfPredators;
//			
//			if(shortestDistance < this->zTooCloseInDistance) //The target is too close. Could expand this to incorporate more than one target.
//			{
//				fear += this->zExtraFearWithCloseProximity;
//			}
//
//			for(int i = 0; i < this->GetCurrentTargets(); i++)
//			{
//				if(this->zTargets[i].valid == true)
//				{
//				//Do a mathematical check, see if anyone is right in front of the bear. But... how? http://www.youtube.com/watch?v=gENVB6tjq_M
//					float dotProduct = this->GetDirection().GetDotProduct( this->zTargets[i].position - this->GetPosition() );
//					if(dotProduct > this->zFieldOfView)//This sight is relatively narrrow, since it is a bear. If this is true, then the bear sees a target.
//					{
//						//Which means, it is even more likely to target someone.
//						fear += this->zExtraFearAtSight;
//					}
//					if(this->zTargets[i].movementNoise > this->zThreatMovementSpeedThreshold) //5 is just a temporary number right now. It is supposed to be the speed of a running target.
//					{
//						fear += this->zExtraFearAtThreatMovementSpeed;
//					}
//
//					if(this->zTargets[i].health != 0) // No dbz here!
//					{
//						fear -= (this->GetHealth() / this->zTargets[i].health) / nrOfPredators; //If the animal is faced with a very weak player(s), it is actually less likely to attack. Like it's prey is unworthy or something.
//					}
//				}
//			}			
//
//			this->SetFearLevel( this->GetFearLevel() + fear * this->zConfidenceKoef); //Possibly not good enough math. The theory is that the animal is constantly getting angrier
//	
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
//	if(this->GetFearLevel() == 0)
//	{
//		this->SetBehaviour(CALM);
//	}
//	/*else if(this->GetFearLevel() > 0 && this->GetFearLevel() <= 10)
//	{
//		this->SetBehaviour(SUSPICIOUS);
//	}*/
//	else if(this->GetFearLevel() > this->zCalmToAggressiveThreshold && nearbyPredatorsExist)
//	{
//		this->SetBehaviour(AGGRESSIVE);
//	}
//	else if(this->zPanic == true)
//	{
//		this->SetBehaviour(AFRAID);
//	}
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
//			this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().z, this->GetPosition().x + rand() % this->zDistanceToWalkWhenCalm - this->zDistanceToWalkWhenCalm/2, this->GetPosition().z + rand() % this->zDistanceToWalkWhenCalm - this->zDistanceToWalkWhenCalm/2, this->zCurrentPath, 20);
//			this->SetIfNeedPath(false);
//		}
//	}
//	//else if(this->GetBehaviour() == SUSPICIOUS) //Might have heard something, is suspicious.
//	//{
//	//	this->zPanic = false;
//	//	if(this->zIntervalCounter > this->zCalmActionInterval && this->GetIfNeedPath()) //The increase in time is supposed to represent listening, waiting for something to happen.
//	//	{
//	//		this->zIntervalCounter = 0;
//	//		srand(time(NULL));
//	//		this->zCalmActionInterval = rand() % 8 + 6;
//	//		this->zCurrentPath.clear();
//	//		//this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, this->GetPosition().x + rand() % 8 - 4, this->GetPosition().z + rand() % 8 - 4, this->zCurrentPath, 20); //Get a small path to walk, quite short (since the animal is nervous) and does not have to lead anywhere.
//	//		this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().z, this->GetPosition().x + rand() % 14 - 7, this->GetPosition().z + rand() % 14 - 7, this->zCurrentPath, 20);
//	//		this->SetIfNeedPath(false);
//	//	}
//	//}
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
//			if(shortestDistance < this->GetLastDistanceCheck() / this->zNewTargetCloseByAFactorOf) // The animal has gotten closer to another threat and is following that now.
//			{
//				this->SetIfNeedPath(true);
//				this->zMainTarget = mostLikelyTarget;
//			}
//		}
//		
//	}
//	else if(this->GetBehaviour() == AFRAID) //Is afraid, needs to run.
//	{
//
//		if(this->GetIfNeedPath() == true)
//		{
//			this->SetIfNeedPath(false);
//
//			if(nearbyPredatorsExist)
//			{
//				
//				this->zDestination = this->ExaminePathfindingArea();				
//				this->zCurrentPath.clear();
//				if(!this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().z, this->zDestination.x, this->zDestination.z,this->zCurrentPath,30) ) //!this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, awayFromThreatX, awayFromThreatZ,this->zCurrentPath,80)
//				{
//					this->SetIfNeedPath(true);
//				}
//
//			}
//			else
//			{
//				this->zPanic = false;
//			}
//			
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
//		if(this->GetBehaviour() == CALM && this->zCurrentPath.size() > 0 /*|| this->GetBehaviour() == SUSPICIOUS && this->zCurrentPath.size() > 0*/) // && !this->zCurrentPath.empty() is necessary to be used again to avoid getting into an unlikely but posssible error.
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
//void BearActor::UpdateForPlayer(float deltaTime)
//{
//	//Empty for now. Unsure if it is I that has to work on this or otherwise.
//}
//
//void BearActor::LargeSuddenSoundReaction()
//{
//	this->zPanic = true;
//	this->SetFearLevel( this->GetFearLevel() + zLargeSoundEventFearIncrease);
//}