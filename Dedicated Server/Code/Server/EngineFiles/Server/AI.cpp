#include "AI.h"

AI::AI()
{
	this->zName = "Skippy";
	
}

AI::~AI()
{

}

void AI::InitAI(std::bitset<GRIDSIZE*GRIDSIZE> theGrid, float nodeDistance)
{
	this->zGrid = theGrid;

	this->zNodeDistance = nodeDistance;
}

//Note: by Y, we of course mean Z. I was merely thinking in a 2-d grid when designing this.
bool AI::Pathfinding(float startXPos, float startYPos, float goalXPos, float goalYPos, std::vector<Vector2> &inVector, int maximumPathLenght)
{
	//this->zNodeDistance was not made with a decimal value in mind, trying to work it out.
	//float sizeOfField = GRIDSIZE * this->zNodeDistance;
	//this->zNodeDistance = 20 * (0.5;


	int startX = ceil((startXPos/this->zNodeDistance)-0.5);
	int startY = ceil((startYPos/this->zNodeDistance)-0.5);
	int goalX = ceil((goalXPos/this->zNodeDistance)-0.5);
	int goalY = ceil((goalYPos/this->zNodeDistance)-0.5);

	//int startX = startXPos;
	//int startY = startYPos;
	//int goalX = goalXPos;
	//int goalY = goalYPos;

	if(startX < 0)
	{
		startX = 0;
	}
	
	if(startY < 0)
	{
		startY = 0;
	}

	if(startX >= GRIDSIZE)
	{
		startX = GRIDSIZE-1;
	}
	
	if(startY >= GRIDSIZE)
	{
		startY = GRIDSIZE-1;
	}

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

			unsigned int nextNode = -1;
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


	Node* tracer = zClosedList.front();

	while(tracer->parent != NULL && tracer->parent != tracer)
	{
		inVector.push_back(Vector2(tracer->x * this->zNodeDistance, tracer->y * this->zNodeDistance) ); //zNodeDistance is just a value representing the distance between the nodes.
		tracer = tracer->parent;
	}

	//Clear the lists, so repeating the function doesn't give bugs or such.
	zClosedList.clear();
	zOpenList.clear();
	zNodeList.clear();

	return true;

}