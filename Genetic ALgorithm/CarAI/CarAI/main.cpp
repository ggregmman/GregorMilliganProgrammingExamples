
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <time.h>
#include <SDKDDKVer.h>

//object headers 
#include "AItargetarea.h" // target area 
#include "Vehicle.h"
#include "AItargetwall.h"



// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <sstream>
#include <string>
#include <list>
#include <iostream>

using namespace std;


//Window Size
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600


//Variables


double MUTATION_RATE = 0.05;
const int population = 20;
#define CROSSOVER_RATE	0.7
bool gaSuccess = false;
int generationCount = 0;
int generation;
float totalFitness;


// functions declarations 
void initGame();
void updateGame();
void drawGame(HDC &hdc);
void getAI();
void evolve();
void updateFitness();
void popSorting();


//declares the target area parmeters, location on screen is 600 by 300 and 50, 50 is the size of the location  
AItargetarea aiTarget(600, 300, 50, 50);
//creates the wall population, this can be changed in the AItargetwall.h header file 
AItargetwall aiWall[wallPop];
//Create the car population, this can be edited in the Vehicle.h function 
Vehicle cars[population]; 



void crossOver(Vehicle&, Vehicle&, Vehicle&, Vehicle&);
void mutate(Vehicle&);

LRESULT CALLBACK WindowProc(HWND   hwnd,
	UINT   msg,
	WPARAM wParam,
	LPARAM lParam)
{
	//Pens and drawing
	static HPEN BluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	static HPEN RedPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	static HPEN OldPen = NULL;
	static HBRUSH RedBrush = CreateSolidBrush(RGB(200, 128, 0));
	static HBRUSH YellowBrush = CreateSolidBrush(RGB(255, 255, 0));
	static HBRUSH ltBlueBrush = CreateSolidBrush(RGB(200, 200, 255));
	static HBRUSH ltGreenBrush = CreateSolidBrush(RGB(50, 255, 255));
	static HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	static HBRUSH blackBrush = CreateSolidBrush(RGB(255, 255, 255));
	static HBRUSH OldBrush = NULL;
	static int cxClient, cyClient;

	//back buffer
	static HDC		hdcBackBuffer;
	static HBITMAP	hBitmap;
	static HBITMAP	hOldBitmap;

	switch (msg)
	{

	case WM_CREATE:
	{
		//assigning the size of the window to a rectangle 
		RECT rect;
		GetClientRect(hwnd, &rect);
		cxClient = rect.right;
		cyClient = rect.bottom;

		//seed random number generator
		srand((unsigned)time(NULL));

		//render surface

		hdcBackBuffer = CreateCompatibleDC(NULL);
		HDC hdc = GetDC(hwnd);
		hBitmap = CreateCompatibleBitmap(hdc,
			cxClient,
			cyClient);
		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);
		ReleaseDC(hwnd, hdc);
		initGame();
	}

	break;

	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			PostQuitMessage(0);
		}
		break;
		}
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);

		//paint the baack buffer white
		BitBlt(hdcBackBuffer,
			0,
			0,
			cxClient,
			cyClient,
			NULL,
			NULL,
			NULL,
			WHITENESS);

		(HBRUSH)SelectObject(hdcBackBuffer, blackBrush);
		Rectangle(hdcBackBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		drawGame(hdcBackBuffer);

		//now blit backbuffer to front
		BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);

	}
	break;

	//resizing window 
	case WM_SIZE:
	{
	
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		SelectObject(hdcBackBuffer, hOldBitmap);
		DeleteObject(hBitmap);
		HDC hdc = GetDC(hwnd);
		hBitmap = CreateCompatibleBitmap(hdc,
			cxClient,
			cyClient);
		ReleaseDC(hwnd, hdc);
		SelectObject(hdcBackBuffer, hBitmap);

	}
	break;

	case WM_DESTROY:
	{
		//delete the drawing pens
		DeleteObject(BluePen);
		DeleteObject(OldPen);
		DeleteObject(RedBrush);
		DeleteObject(OldBrush);
		SelectObject(hdcBackBuffer, hOldBitmap);
		DeleteDC(hdcBackBuffer);
		DeleteObject(hBitmap);

		// Quit out message
		PostQuitMessage(0);
	}
	break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}//main entry point
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     szCmdLine,
	int       iCmdShow)
{
	HWND						hWnd;

	WNDCLASSEX     winclass;

	// first fill in the window class stucture
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = NULL;
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = "Genetic Algorith";
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//register the window class
	if (!RegisterClassEx(&winclass))
	{
		MessageBox(NULL, "Registration Failed!", "Error", 0);

		//exit the application
		return 0;
	}

	//create the window and assign its ID to hwnd    
	hWnd = CreateWindowEx(NULL,                 // extended style
		"Genetic Algorith",  // window class name
		"Genetic Algorith",  // window caption
		WS_OVERLAPPEDWINDOW,  // window style
		0,                    // initial x position
		0,                    // initial y position
		WINDOW_WIDTH,         // initial x size
		WINDOW_HEIGHT,        // initial y size
		NULL,                 // parent window handle
		NULL,                 // window menu handle
		hInstance,            // program instance handle
		NULL);                // creation parameters

							  //make sure the window creation has gone OK
	if (!hWnd)
	{
		MessageBox(NULL, "CreateWindowEx Failed!", "Error!", 0);
	}

	//make the window visible
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	// Enter the message loop
	bool bDone = false;
	MSG msg;
	while (!bDone)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				// Stop loop if it's a quit message
				bDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		//this will call WM_PAINT which will render our scene
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);

		//*** your game loop goes here ***//
		updateGame();
	}//end while

	UnregisterClass("Backbuffer", winclass.hInstance);
	return msg.wParam;
}
//Where the game starts 
void initGame()
{
	//resets car locations 
	for (int p = 0; p < population; p++)
	{
		cars[p].x = 100;
		cars[p].y = 100;
	}
	//start making walls 
	for (int z = 0; z < wallPop; z++)
	{
		//location of walls 
		aiWall[z].x = rand() % 600 ;
		aiWall[z].y = rand() % 600;
		//size of walls 
		aiWall[z].width = rand() % 100;
		aiWall[z].height = rand() % 100;
	}
	// initiate Instructions 
	for (int i = 0; i < numOfInst; i++)
	{
		instructions[i] = rand() % 4;
	}
}
//drawing the game 
void drawGame(HDC &hdcBackBuffer)
{
	//player
	static HBRUSH ltBlueBrush = CreateSolidBrush(RGB(100, 80, 255));
	static HBRUSH redBrush = CreateSolidBrush(RGB(100, 0, 250));
	static HBRUSH greenBrush = CreateSolidBrush(RGB(200, 173, 42));
	SelectObject(hdcBackBuffer, ltBlueBrush);

	//draw population
	for (int p = 0; p < population; p++)
	{
		Ellipse(hdcBackBuffer, cars[p].x, cars[p].y, cars[p].x + 20, cars[p].y + 20);
		MoveToEx(hdcBackBuffer, cars[p].x + 10, cars[p].y + 10, NULL); //+10 is offset
		LineTo(hdcBackBuffer, cars[p].x + 10 + cars[p].px, cars[p].y + 10 + cars[p].py);
	}

	for (int z = 0; z < wallPop; z++)
	{
		SelectObject(hdcBackBuffer, redBrush);
		Rectangle(hdcBackBuffer, aiWall[z].x, aiWall[z].y, aiWall[z].x + aiWall[z].width, aiWall[z].y + aiWall[z].height);
	}

	//Draw AI Target
	SelectObject(hdcBackBuffer, greenBrush);
	Rectangle(hdcBackBuffer, aiTarget.x, aiTarget.y, aiTarget.x + aiTarget.width, aiTarget.y + aiTarget.height);

	TCHAR buff2[15];
	TextOut(hdcBackBuffer, 10, 5, buff2, wsprintf(buff2, "Gen Count: ", "%d"));
	TCHAR buff[15];
	TextOut(hdcBackBuffer, 90, 5, buff, wsprintf(buff, "%d", generationCount));
}

// Checking collision With the target
void checkCollision()
{
	for (int p = 0; p < population; p++)
	{
		{
			//checking wall collision
			for (int i = 0; i < wallPop; i++)
			{
				if (cars[p].x > aiWall[i].x && cars[p].x < aiWall[i].x + aiWall[i].width)
				{
					if (cars[p].y > aiWall[i].y && cars[p].y < aiWall[i].y + aiWall[i].height)
					{
						cars[p].wallCollide = true;
					}
				}
			}
		}

		//check ai target collision
		if (cars[p].x > aiTarget.x && cars[p].x < aiTarget.x + aiTarget.width)
		{
			if (cars[p].y > aiTarget.y && cars[p].y < aiTarget.y + aiTarget.height)
			{
				MessageBox(NULL, "STOP", "at goal", 0);
			}
		}
	}
}
//updating game 
void updateGame()
{
	if (gaSuccess == false)
	{
		getAI();
		checkCollision();
	}
}
//creating ai 
void getAI()
{	//checking instruction count 
	int currentInstruction = instructions[instPtr];
	if (currInst < numOfInst)
	{
		currInst++;
	}
	//if there is no more instrutions let them evolve and move the gen counter up by 1 
	if (currInst >= chromoLength)
	{
		evolve();
		generationCount++;
		// recreate population 
		for (int p = 0; p < population; p++)
		{
			cars[p].x = 100;
			cars[p].y = 100;
			cars[p].currInst = 0;

			cars[p].px = 15;
			cars[p].py = 0;
			cars[p].carRot = 0;
		}
		currInst = 0;
	}
	// recreate population
	for (int p = 0; p < population; p++)
	{
		int inst = cars[p].instr[cars[p].currInst];
		if (cars[p].currInst < cars[p].numOfInst)
		{
			cars[p].currInst++;
		}
		else
		{
			cars[p].currInst = 0;
		}
		//--------------------------Algorithms-------------------------------
		/*
		------alg 1---- 
		
		 */

		if (inst == 0) //*******	***** check with doug
		{
			if (cars[p].wallCollide == false)
			{
				
				cars[p].carRot += 0.1f;
				cars[p].px = 15 * cos(cars[p].carRot) - (0 * sin(cars[p].carRot));
				cars[p].py = 15 * sin(cars[p].carRot) + (0 * cos(cars[p].carRot));
			}
			else
			{
				//playerx++;
				cars[p].carRot -= 0.1f;
				//2d point rotation - Radians not degrees
				cars[p].px = 15 * cos(cars[p].carRot) - (0 * sin(cars[p].carRot));
				cars[p].py = 15 * sin(cars[p].carRot) + (0 * cos(cars[p].carRot));
			}
		}

		/*
		-----Algorthm 2-----
		Moves the car to the right?
		
		*/
		if (inst == 1) 
		{
			if (cars[p].wallCollide == false)
			{
				//playerx++;
				cars[p].carRot -= 0.1f;
				//2d point rotation - Radians not degrees
				cars[p].px = 15 * cos(cars[p].carRot) - (0 * sin(cars[p].carRot));
				cars[p].py = 15 * sin(cars[p].carRot) + (0 * cos(cars[p].carRot));
			}
			else
			{
				//playerx--;
				cars[p].carRot += 0.1f;
				cars[p].px = 15 * cos(cars[p].carRot) - (0 * sin(cars[p].carRot));
				cars[p].py = 15 * sin(cars[p].carRot) + (0 * cos(cars[p].carRot));
			}

		}

		if (inst == 2) 
		{
			if (cars[p].wallCollide == false)
			{
				//translate 2d point
				float x = cars[p].x + (2 * cos(cars[p].carRot));
				float y = cars[p].y + (2 * sin(cars[p].carRot));
				cars[p].x = x;
				cars[p].y = y;
			}
			else
			{
				float x = cars[p].x + (-2 * cos(cars[p].carRot));
				float y = cars[p].y + (-2 * sin(cars[p].carRot));
				cars[p].x = x;
				cars[p].y = y;
			}
		}

		if (inst == 3) 
		{
			if (cars[p].wallCollide == false)

				//---------Reversing has been cuasing bugs so has been taken out currently 
			{
				/*float x = cars[p].x + (-2 * cos(cars[p].carRot));
				float y = cars[p].y + (-2 * sin(cars[p].carRot));
				cars[p].x = x;
				cars[p].y = y;*/
			}
			else
			{
				//translate 2d point
				/*float x = cars[p].x + (2 * cos(cars[p].carRot));
				float y = cars[p].y + (2 * sin(cars[p].carRot));
				cars[p].x = x;
				cars[p].y = y;*/
			}
			//This algorithm does not do anything on the first generation, then is starts learning
			if (inst == 4) //for sloth mode
			{
				
			}
		}
	}
}

//----------------Evolution----------- GREGOR START HERE!!!!!
void evolve()
{
	updateFitness();
	popSorting();
	Vehicle childCars[population];

	int newPop = 0;
	while (newPop<population)
	{
		Vehicle& tournementSelection();
		Vehicle mum = tournementSelection();
		Vehicle dad = tournementSelection();
		Vehicle childCar1, childCar2;
		crossOver(mum, dad, childCar1, childCar2);

		//mutate
		mutate(childCar1);
		mutate(childCar2);

		//copy babys into babypopulation
		childCars[newPop] = childCar1;
		childCars[newPop + 1] = childCar2;
		newPop += 2;
	}
	for (int i = 0; i<population; i++)
	{
		cars[i] = childCars[i];
	}
}

Vehicle& tournementSelection()
{
	int selectedPlayer = 0;
	//select 2 parents
	int p1 = (rand() % population);
	int p2 = (rand() % population);
	if (cars[p1].fitness > cars[p2].fitness)
	{
		return cars[p1];
	}
	else
	{
		return cars[p2];
	}
}

void updateFitness()
{
	totalFitness = 0;
	for (int i = 0; i<population; i++)
	{
		float playerDiffx = abs(cars[i].x - aiTarget.x);
		float playerDiffy = abs(cars[i].y - aiTarget.y);
		/////
		double sumNum = (playerDiffx * playerDiffx) + (playerDiffy * playerDiffy);
		double sumRoot = sqrt(sumNum);
		////
		double fit = 1 / (playerDiffx + playerDiffy + 1);
		cars[i].fitness = fit;
		totalFitness += fit;


	}
}

void popSorting()
{
	//sorting population 
	for (int i = 0; i < population - 1; i++)
	{
		for (int j = 0; j < population - i - 1; j++)
		{
			if (cars[j].fitness < cars[j + 1].fitness)
			{
				Vehicle temp = cars[j];
				cars[j] = cars[j + 1];
				cars[j + 1] = temp;
			}
		}
	}
	//set rank fitness based on position of player
	for (int i = 0; i < population; i++)
	{
		float temp = (i + 1) / (float)population; //add 1 to stop divide by zero
		cars[i].bestFitness = temp;
	}
}

void crossOver(Vehicle &parent1, Vehicle &parent2, Vehicle &childCar1, Vehicle &childCar2)
{
	if (((rand()) / (RAND_MAX + 1.0)>CROSSOVER_RATE) || parent1 == parent2)
	{
		childCar1 = parent1;
		childCar2 = parent2;
		return;
	}

	int xover = rand() % chromoLength;
	for (int i = 0; i<xover; i++)
	{
		childCar1.instr[i] = parent1.instr[i];
		childCar2.instr[i] = parent2.instr[i];
	}
	for (int i = xover; i<chromoLength; i++)
	{
		childCar1.instr[i] = parent2.instr[i];
		childCar2.instr[i] = parent1.instr[i];
	}
}

void mutate(Vehicle& currPlayer)
{
	for (int bit = 0; bit<chromoLength; bit++)
	{
		if (((rand()) / (RAND_MAX + 1.0)<MUTATION_RATE))
		{
			//new - prevents same instruction from being chosen. 
			int newInst = 0;
			do
			{
				newInst = (rand() % 4);
			} while (newInst == currPlayer.instr[bit]);
			currPlayer.instr[bit] = newInst;
		}
	}
}