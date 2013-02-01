#include "AI.h"

AI::AI()
{
	this->zName = "Skippy";
	
}

AI::~AI()
{

}

void AI::InitAI(float nodeDistance, float mapRange)
{
	this->zGrid.reset();
	this->zNodeDistance = nodeDistance;
	this->zMaximumRange = mapRange;
}

void AI::SetWorldPointer(World* theWorld)
{
	this->zWorld = theWorld;
}

//Note: by Y, we of course mean Z. I was merely thinking in a 2-d grid when designing this.
bool AI::Pathfinding(float startXPos, float startYPos, float goalXPos, float goalYPos, std::vector<Vector2> &inVector, int maximumPathLenght)
{
	// 1 Issue(s):
		//- checking the world for good/bad nodes.

	//Here we determine which nodes are good or bad.
	for(int j = 0; j < GRIDSIZE; j++)
	{
		for(int i = 0; i < GRIDSIZE; i++)
		{
			//If the position is outside the map then ignore it, otherwise there are sector problems!

			Vector2 pos = Vector2((int) (startXPos + (i - GRIDSIZE/2) * this->zNodeDistance),(int) (startYPos + (j - GRIDSIZE/2) * this->zNodeDistance) );
			if(pos.x < 0 || pos.x > this->zMaximumRange)
			{
				this->zGrid[j * GRIDSIZE + i] = true;
			}
			else if(pos.y < 0 || pos.y > this->zMaximumRange)
			{
				this->zGrid[j * GRIDSIZE + i] = true;
			}
			else
			{
				if ( pos.x >= 0.0f && pos.y >= 0.0f && pos.x < zWorld->GetWorldSize().x & pos.y < zWorld->GetWorldSize().y )
				{
					this->zGrid[j * GRIDSIZE + i] = this->zWorld->IsBlockingAt(pos);
				}
				else
				{
					this->zGrid[j * GRIDSIZE + i] = false;
				}
			}	
		}
	}

	//int startX = ceil((startXPos/this->zNodeDistance)-0.5); //Static grid
	//int startY = ceil((startYPos/this->zNodeDistance)-0.5); //Static grid
	int startX = GRIDSIZE/2; //Dynamic grid, the animal always begins in the middle of it.
	int startY = GRIDSIZE/2; //Dynamic grid, the animal always begins in the middle of it.

	
	//int goalX = ceil((goalXPos/this->zNodeDistance)-0.5); //Static grid
	//int goalY = ceil((goalYPos/this->zNodeDistance)-0.5); //Static grid

	if(goalXPos < 1 ) //With a dynamic system, these branches make sure it does not get outside the level.
	{
		goalXPos = 1;
	}
	else if ( goalXPos > this->zMaximumRange )
	{
		goalXPos = this->zMaximumRange - 1;
	}
	if(goalYPos < 1 ) //With a dynamic system, these branches make sure it does not get outside the level.
	{
		goalYPos = 1;
	}
	else if ( goalYPos > this->zMaximumRange )
	{
		goalYPos = this->zMaximumRange - 1;
	}
	

	int goalX =	ceil(( (goalXPos - startXPos)/this->zNodeDistance )-0.5); //Dynamic
	goalX += startX;
	int goalY = ceil(( (goalYPos - startYPos)/this->zNodeDistance )-0.5); //Dynamic
	goalY += startY;

	

	//if(startX < 0) //These branches are not necessary when it is dynamic
	//{
	//	startX = 0;
	//}
	//
	//if(startY < 0)
	//{
	//	startY = 0;
	//}
	//if(startX >= GRIDSIZE)
	//{
	//	startX = GRIDSIZE-1;
	//}
	//
	//if(startY >= GRIDSIZE)
	//{
	//	startY = GRIDSIZE-1;
	//}

	if(goalX < 0) 
	{
		goalX = 0;
	}
	if(goalY < 0)
	{
		goalY = 0;
	}

	if(goalX >= GRIDSIZE)
	{
		goalX = GRIDSIZE-1;
	}

	if(goalY >= GRIDSIZE)
	{
		goalY = GRIDSIZE-1;
	}
	

	//Is the goal position in an impossible position in the bitset?
	if(zGrid[goalY * GRIDSIZE + goalX] == true)
	{
		return false;
	}


	bool done = false;
	bool valid = false;
	bool add = false;

	int smallestFCost = 99999;

	int currentNode = 0;

	zNodeList.push_front(Node(startY, startX));

	zOpenList.push_front(&zNodeList.back());

	/*zOpenList.push_front(&zNodes[startY*GRIDSIZE + startX]);

	for(unsigned int i = 0; i < GRIDSIZE; i++)
	{
		for(unsigned int j = 0; j < GRIDSIZE; j++)
		{
			zNodes[i+GRIDSIZE*j].x = i;
			zNodes[i+GRIDSIZE*j].y = j;
		}
	}*/

	while(!zOpenList.empty())
	{

		auto current = zOpenList.begin();
		auto selected = zNodeList.begin();

		smallestFCost = 99999;

		for(auto it = zOpenList.begin(); it != zOpenList.end(); it++)
		{
			if((*it)->fCost < smallestFCost)
			{
					smallestFCost = (*it)->fCost;
					current = it;
			}
		}

		for(auto it = zNodeList.begin(); it != zNodeList.end(); it++)
		{
			if((*current)->x == it->x && (*current)->y == it->y)
			{
					selected = it;
			}
		}


		zClosedList.splice(zClosedList.begin(),zOpenList,current);

		int x, y;

		x = zClosedList.front()->x;
		y = zClosedList.front()->y;

		currentNode = y * GRIDSIZE + x; //Possible relic


		for(int i = 0; i < 8; i++)
		{
			valid = false;
			add = false;

			int nextNode = -1;
			int nextX = x;
			int nextY = y;

			if(i == 0)
				{
					if(selected->x <= 0 || selected->y <= 0)//if(zNodes[currentNode].x <= 0 || zNodes[currentNode].y <= 0)
					{
						continue;
					}
					//nextNode = currentNode - 1 - GRIDSIZE;
					nextX = x - 1;
					nextY = y - 1;

					//OXX
					//XOX
					//XXX
				}


				else if(i == 1)
				{
					if(selected->y <= 0)//if(zNodes[currentNode].y <= 0)
					{	
						continue;
					}
					//nextNode = currentNode - GRIDSIZE;
					nextX = x;
					nextY = y - 1;
					//XOX
					//XOX
					//XXX
				}
				else if(i == 2)
				{
					if(selected->x >= GRIDSIZE -1 || selected->y <= 0)//if(zNodes[currentNode].x >= GRIDSIZE -1 || zNodes[currentNode].y <= 0)
					{
						continue;
					}
					//nextNode = currentNode + 1 - GRIDSIZE;
					nextX = x + 1;
					nextY = y - 1;

					//XXO
					//XOX
					//XXX
				}
				else if(i == 3)
				{
					if(selected->x <= 0)//if(zNodes[currentNode].x <= 0)
					{
						continue;
					}
					//nextNode = currentNode - 1;
					nextX = x - 1;
					nextY = y;
					//XXX
					//OOX
					//XXX
				}
				else if(i == 4)
				{
					if(selected->x >= GRIDSIZE -1)//if(zNodes[currentNode].x >= GRIDSIZE -1)
					{
						continue;
					} 
					//nextNode = currentNode + 1;
					nextX = x + 1;
					nextY = y;
					//XXX
					//XOO
					//XXX
				}
				else if(i == 5)
				{
					if(selected->x <= 0 || selected->y >= GRIDSIZE - 1)//if(zNodes[currentNode].x <= 0 || zNodes[currentNode].y >= GRIDSIZE-1)
					{
						continue;
					} 
					//nextNode = currentNode - 1 + GRIDSIZE;
					nextX = x - 1;
					nextY = y + 1;
					//XXX
					//XOX
					//OXX
				}
				else if(i == 6)
				{
					if(selected->y >= GRIDSIZE - 1)//if(zNodes[currentNode].y >= GRIDSIZE -1)
					{
						continue;
					} 
					//nextNode = currentNode + GRIDSIZE;
					nextX = x;
					nextY = y + 1;
					
					//XXX
					//XOX
					//XOX
				}
				else if(i == 7)
				{
					if(selected->x >= GRIDSIZE -1 || selected->y >= GRIDSIZE - 1)//if(zNodes[currentNode].x >= GRIDSIZE -1 || zNodes[currentNode].y >= GRIDSIZE -1)
					{
						continue;
					} 
					//nextNode = currentNode + 1 + GRIDSIZE;
					nextX = x + 1;
					nextY = y + 1;
					
					//XXX
					//XOX
					//XXO
					
				}


				//It is valid in terms of location.

				if(nextY > GRIDSIZE - 1)
				{
					nextY = GRIDSIZE - 1;
				}
				if(nextX > GRIDSIZE - 1)
				{
					nextX = GRIDSIZE - 1;
				}

				//Is it valid in the bitset? If not, ignore.
				if(zGrid[GRIDSIZE * nextY + nextX] == true )
				{
					//It is not valid in the bitset.
					continue;
				}

				Node theNextNode(nextX,nextY);
			
				//calculate the f cost.			
				theNextNode.fCost = abs(goalX - nextX);//zNodes[nextNode].fCost = abs( goalX - zNodes[nextNode].x);
				theNextNode.fCost += abs(goalY - nextY);//zNodes[nextNode].fCost += abs( goalY - zNodes[nextNode].y);

				theNextNode.fCost *= 10;//zNodes[nextNode].fCost *= 10;

				if(i == 1 || i == 3 || i == 4 || i == 6)
				{
					if(currentNode == 0)
					{
						selected->gCost = 0;//zNodes[currentNode].gCost = 0;
					}
					theNextNode.gCost = selected->gCost + 10;//zNodes[nextNode].gCost = zNodes[currentNode].gCost + 10;
					theNextNode.fCost += theNextNode.gCost;//zNodes[nextNode].fCost += zNodes[nextNode].gCost;
				}
				else
				{	
					if(currentNode == 0)
					{
						selected->gCost = 0;//zNodes[currentNode].gCost = 0;
					}
					theNextNode.gCost = selected->gCost + 14;//zNodes[nextNode].gCost = zNodes[currentNode].gCost + 14;
					theNextNode.fCost += theNextNode.gCost;//zNodes[nextNode].fCost += zNodes[nextNode].gCost;
				}


				add = true;

				for(auto iter = zOpenList.begin(); iter != zOpenList.end(); iter++)
				{
					if((*iter)->x == theNextNode.x && (*iter)->y == theNextNode.y)//if((*iter)->x == zNodes[nextNode].x && (*iter)->y == zNodes[nextNode].y)
					{
						add = false;
						break;
					}
				}

				for(auto iter = zClosedList.begin(); iter != zClosedList.end(); iter++)
				{
					if((*iter)->x == theNextNode.x && (*iter)->y == theNextNode.y)//if((*iter)->x == zNodes[nextNode].x && (*iter)->y == zNodes[nextNode].y)
					{
						add = false;
						break;
					}
				}

				if(add)
				{
					
				//Now we need to set the parent of the node.
				theNextNode.parent = (&*selected); //zNodes[zNodes[nextNode].y*GRIDSIZE+zNodes[nextNode].x].parent = &zNodes[currentNode];

				this->zNodeList.push_front(theNextNode);
				

				
				//Add it to the open list. 
				zOpenList.push_front(&this->zNodeList.front());//zOpenList.push_front(&zNodes[nextNode]);


					if(zNodeList.front().x == goalX && zNodeList.front().y == goalY)//if(zNodes[nextNode].x == goalX && zNodes[nextNode].y == goalY)
					{
						done = true;
						break;
					}
				}


		}

		if(done)
		{
			zClosedList.splice(zClosedList.begin(),zOpenList,zOpenList.begin());

			break;
		}

		if(zClosedList.size() > maximumPathLenght)
		{
				break;
		}

	}
	startXPos;
	startYPos;
	goalYPos;
	goalXPos;

	goalXPos - startXPos;
	goalYPos - startYPos;

	Node* tracer = zClosedList.front();

	while(tracer->parent != NULL && tracer->parent != tracer)
	{
		//inVector.push_back(Vector2(tracer->x * this->zNodeDistance, tracer->y * this->zNodeDistance) ); // Static
		inVector.push_back(Vector2( (startXPos + (tracer->x - GRIDSIZE/2) * this->zNodeDistance ), (startYPos + (tracer->y - GRIDSIZE/2) * this->zNodeDistance)) ); //Dynamic
		tracer = tracer->parent;
	}

	//Clear the lists, so repeating the function doesn't give bugs or such.
	zClosedList.clear();
	zOpenList.clear();
	zNodeList.clear();

	return true;

}