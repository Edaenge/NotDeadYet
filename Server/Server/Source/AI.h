//Made by Berg Wilhelm 15/12/12 created) for project Not Dead Yet at Blekinge tekniska högskola



#ifndef AI_H
#define AI_H
#include <bitset>
#include <list>
#include <vector>
#include <Vector.h>
#include "World/World.h"
//#include <map>


struct Node
{
	float x, y;
	int fCost;
	int gCost;
	Node* parent;
	Node(){this->x = -1; this->y = -1; this->parent = NULL; fCost = 99999;};
	Node(float x, float y){this->x = x; this->y = y; this->parent = NULL; fCost = 99999;};
};

const int GRIDSIZE = 80;

class AI
{
private:
			
	std::string		zName;
	float			zNodeDistance; //The distance between nodes.
	float			zMaximumRange; //The size of the actuall world, the edge, so to speak.

	World*			zWorld;

	//Node			zNodes[GRIDSIZE*GRIDSIZE];	//I made this to a list instead.
	std::list<Node> zNodeList;

	std::bitset<GRIDSIZE*GRIDSIZE>			zGrid;
	std::list<Node*>						zOpenList;
	std::list<Node*>						zClosedList;

public:
					AI();
	virtual			~AI();
					
					//The node distance needs to be the same as the distance between nodes in the grid as when they were created.
	void			InitAI(float nodeDistance, float mapRange);
	void			SetWorldPointer(World* theWorld);

					//The vector reference will be filled with the path between the start postion and the target. Don't forget to empty the vector if you want to use it again, or it will be filled with an entire extra path.
					//If the goalposition is corresponding to a blocked bit in the bitset, it will return false.
	bool			Pathfinding(float startXPos, float startYPos, float goalXPos, float goalYPos, std::vector<Vector2> &inVector, int maximumPathLenght = 300); //NOTE: For some reason the first two parameters need to be reversed. I have NO idea why at this stage.



};
#endif