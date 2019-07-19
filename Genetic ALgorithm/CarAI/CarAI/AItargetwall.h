#pragma once

#include <iostream>
const int wallPop = 15; // this is the number of walls spawned 

class AItargetwall
{
public:
	float x = 10;
	float y = 10;
	float width = 10;
	float height = 10;

	AItargetwall();
	AItargetwall(float wx, float wy, float wwidth, float wheight);
};
AItargetwall::AItargetwall(float wx, float wy, float wwidth, float wheight)
{
	x = wx, y = wy;
	width = wwidth; height = wheight;
}
//random wall generation 
AItargetwall::AItargetwall()
{
	x = (rand() % 5) + 10;
	y = (rand() % 5) + 50;
	width = (rand() % 10);
	height = (rand() % 10);
}