#pragma once
const int numOfInst = 1200; // number of instructions (Moves) that each car can make per generation 
int instructions[numOfInst];
int instPtr = 0;
int currInst = 0;
const int chromoLength = numOfInst;
class Vehicle
{
public:
	//Dna
	const static int numOfInst = chromoLength;
	int instr[numOfInst];
	int currInst = 0;
	int distanceToGoal = 0;
	float fitness = 0;
	float bestFitness = 0;
	bool wallCollide = false;

	//Position on screen
	float x = 0;
	float y = 0;
	float px = 0;
	float py = 0;
	float carRot = 0;
	float width;
	float height;

	Vehicle();
	Vehicle(float nx, float ny, float nwidth, float nheight);

	int operator == (const Vehicle &p1)
	{
		return instr == p1.instr;
	}
};

Vehicle::Vehicle()
{

	x = 0; y = 0;
	width = 0; height = 0;
	for (int i = 0; i < numOfInst; i++)
	{
		

//		--------algorthm selection-------- 
		//instr[i] = 0; // turn 
		instr[i] = 2;	//born to move to the right 
		//instr[i] = (rand() % 4); //random genome
		//instr[i] = 4; //born to sit still
	}
	fitness = 0;
}

Vehicle::Vehicle(float nx, float ny, float nwidth, float nheight)
{
	x = nx, y = ny;
	width = nwidth; height = nheight;
}