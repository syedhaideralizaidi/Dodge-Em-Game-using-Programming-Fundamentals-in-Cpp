//CHANGE LOGS//
/*
?? - 7th December 
Basic implementation and placement of arena and car, and collision detection in corners, Food placed, Food retriving and updating score.

7th December: 
1. Code Cleaned.
2. Boost Implemented.
3. Implemented Menu Base screen (Background picture yet to be displayed.)
4. Implemented main menu with baseScreen using an Image (PNG).
5. Implemented Lane Shifting properly so Car doesn't jump randomly.

8th December: 
TODO:
1. Complete game.
2. Optional (Implement Bonus Criteria).

Implementations:
1. Implemented bot movement.
2. HighScore Implemented.
3. Cleaned code.
4. DMA implementation in project.

Submitted.
*/

//============================================================================
// Name        : game.cpp
// Author      : Hassan Mustafa & Mustafa Zafar
// Version     : 
// Copyright   : (c) Reserved
// Description : Basic 2D game of Dodge 'Em...
//============================================================================

#ifndef DODGE_CPP_
#define DODGE_CPP_
#include "util.h"
#include <iostream>
#include <string>
#include <fstream>
#include "pngImage.h"
#include <cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;

//Using pragma anyways for making regions even tough the code is not on Visual Studio. 

#pragma region Functions
void menuDisplay();
void GameDisplay();
void DrawCar();
void DrawOpponentCar();
void DrawArena();
void FoodPosition(int x, int y);
void DrawFood();
void GameDisplayLoop();
void FindFood(int x, int y);
void CarMovement(int&x, int&y);
void OpponentMovement(int&x, int&y);
void menuCar(int ypos);
bool ChangeDirection(string directionChange, int x, int y);
int LaneDetection(int x, int y);
void BotMovement(int pLane, int oLane, int &x, int &y);
void ReadHighScore();
void checkLevel();
void levelUp(int newLevel);
bool reset();
void sortScore();
bool canJumpLane();
void DrawSecondCar(int &xpos, int &ypos);
bool carCollision(int x, int y, int oX, int oY);
void collisionReset(); 
void saveScore();
void highScoreDisplay();
void instructionsDisplay();
void menuBackGround();
void winnerScreen();
int randomizeSpawn();
#pragma endregion Functions

const int PLAYERSTARTINGX = 320;
const int PLAYERSTARTINGY = 40;
int OPPONENTSTARTINGY = 40;
int OPPONENTSTARTINGX = 420;
const int SECONDOPPONENTX = 420;
const int SECONDOPPONENTY = 140;
const int foodX = 60;
const int foodY = 60;

#pragma region GlobalVariables
int status = 0; //Status that the menu is open or game is being played.
int positionX = 320; //Initial position of car.
int positionY = 40;
int OpponentX = 420; //Initial position of Opponent Car.
int OpponentY = 40;
int OpponentTwoX = 420; //Initial position of second opponent car.
int OpponentTwoY = 40;
float multiplier = 4; //Mulitplier of boost.
int score = 0; //Initial Score.
int width = 804, height = 840; //Canvas and window size.
double openGLRefreshTime = 16.67; //60FPS glory.
int foodItemAxis[64][2]; //Food Items.
int sxvalue[] = {20, 120, 220, 320}; //sxvalue for arena.
int syvalue[] = {10, 110, 210, 310}; //syvalue for arena.
int sWidthInput[] = {765, 565, 365, 165}; // Width of arena.
bool boost = false; //Boost boolean.
float defaultSpeed = 5; //Default speed of car.
float defaultSpeedOpponent = 2; // Default speed of opponent car.
float playerMovementSpeed = defaultSpeed;// Playercar movement speed.
PngImage menuImage; //Image library variable.
int menuSelectedItem = 0; //Menu selected Item.
int maxMenuItems = 3; //Maximum value of options in menu.
bool proceed = false; //When enter is pressed.
int foodPositionAfterCollision = 2000; //Food position after collision.
string direction; // String that's used for passing direction function.
int playerLane = 0; //Initial lane of player.
int opponentLane = 0; //Initial lane of opponent car.
ifstream highScoreReading; //Variable for file reading.
ofstream highScoreWriting; //Variable for file writing.
string highScores; //Highscore string to read highscore as string in this and further stoi to convert.
int highScoresValue[11] = {};// Initializing of highscore array with 0 value.
int lives = 4; //No. of Lives.
int level = 1; //Initial Level.
bool horizontalJumpingBot = false; //For level one implementation.
bool activeMode = false; //Active passive mode based on level.
int jumpLimit = 1; //Opponent car jumping limit.
int jumped = 0; //If car jumped, increment happens here.
int jumpCounter = 0; //cooldown after jump.
bool twoOpponentCars = false; //Implementation for level 4.
int opponentTwoLane = 0; //Initial position of Opponent car.
int carProximity = 20; //Car proximity in which collision will be detcted.
string highScoreDirectory = "gameassets/highscores.txt"; //Highscore file directory.
bool randomize = true; //For random position of starting car.
#pragma endregion GlobalVariables

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bopositionXttom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

void winnerScreen() {
	//status = 2;
	sortScore();
	saveScore();
	menuBackGround(); //Calling the picture implementation.
	DrawString(300, 350, "YOU WON!!!!!!!", colors[BLACK]); //Displaying winning message.
	DrawString(300, 300, "PRESS ESC FOR MENU.", colors[BLACK]);
	glutSwapBuffers(); //Updating pixels for next display.
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menuDisplay() {
	menuBackGround(); //Calling picture.
	/*Menu implementation and options.*/
	if (menuSelectedItem == 0) {
		menuCar(400);
		DrawString( 300, 400, "CREATE NEW GAME", colors[DARK_RED]); 
	}
	else {
		DrawString( 300, 400, "CREATE NEW GAME", colors[DARK_BLUE]);
	}
	if (menuSelectedItem == 1) {
		menuCar(350);
		DrawString( 300, 350, "HIGH SCORES", colors[DARK_RED]);
	}
	else {
		DrawString( 300, 350, "HIGH SCORES", colors[DARK_BLUE]);
	}
	if (menuSelectedItem == 2) {
		menuCar(300);
		DrawString( 300, 300, "INSTRUCTIONS", colors[DARK_RED]);
	}
	else {
		DrawString( 300, 300, "INSTRUCTIONS", colors[DARK_BLUE]);
	}
	if (menuSelectedItem == 3) {
		menuCar(250);
		DrawString( 300, 250, "EXIT", colors[DARK_RED]);
	}
	else {
		DrawString( 300, 250, "EXIT", colors[DARK_BLUE]);
	}
	if (score > 0 || score == 256) {
		maxMenuItems = 4;
		if (menuSelectedItem == 4) {
			menuCar(200);
			DrawString( 300, 200, "RESUME", colors[DARK_RED]);
		}
		else {
			DrawString( 300, 200, "RESUME", colors[DARK_BLUE]);
		}
	}
	/*Now for when Enter is pressed on some options.*/
	if (proceed && menuSelectedItem == 0) {
		status = 1;
		reset();
		lives = 3;
		level = 1;
		score = 0;
		horizontalJumpingBot = false;
		activeMode = false;
		glutDisplayFunc(GameDisplay);
	}
	if (proceed && menuSelectedItem == 3) {
		exit(0);
	}
	if (proceed && menuSelectedItem == 4) {
		status = 1;
		glutDisplayFunc(GameDisplay);
	}
	if (proceed && menuSelectedItem == 1) {
		status = 2;
		glutDisplayFunc(highScoreDisplay);
	}
	if (proceed && menuSelectedItem == 2) {
		status = 2;
		glutDisplayFunc(instructionsDisplay);
	}
	proceed = false; //Resetting enter key.
	glutSwapBuffers(); //Pixel updating.
}


void menuCar(int ypos) {
	float x = 275; 
	float y = ypos; 
	float width = 6; 
	float height = 4;
	float *colour = colors[BLACK]; 
	float radius = 3;
	DrawRoundRect(x,y,width,height,colour,radius); // bottom left tyre
	DrawRoundRect(x+width*3,y,width,height,colour,radius); // bottom right tyre
	DrawRoundRect(x+width*3,y+height*4,width,height,colour,radius); // top right tyre
	DrawRoundRect(x,y+height*4,width,height,colour,radius); // top left tyre
	DrawRoundRect(x, y+height*2, width, height, colour, radius/2); // body left rect
	DrawRoundRect(x+width, y+height, width*2, height*3, colour, radius/2); // body center rect
	DrawRoundRect(x+width*3, y+height*2, width, height, colour, radius/2); // body right rect
}


void menuBackGround() { //Function that load image and display it.
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	for(int j = 0; j < menuImage.getHeight(); j++)
		for(int i = 0; i < menuImage.getWidth(); i++)
		{
			int r, g, b;
			menuImage.getRGB(i, j, r, g, b);
			glColor3f((float)r / 255, (float)g / 255, (float)b / 255);
			glVertex2f(i, j);
		}
	glEnd();
}


void highScoreDisplay() {
	menuBackGround();
	DrawString(50, 650, "HIGHSCORES OF PLAYERS: ", colors[BLACK]);
	for (int i = 0, scoreInit = 600; i < 10; i++, scoreInit -= 50) { //Loop to show highscores.
		DrawString( 50, scoreInit, to_string(i + 1) + ". " + to_string(highScoresValue[i]), colors[BLACK]);
	}
	DrawString(50, 50, "PRESS ESC FOR MAINMENU.", colors[BLACK]);
	glutSwapBuffers();
}


void instructionsDisplay() { //Function that to be called to display instructions.
	menuBackGround();
	DrawString(50, 700, "CONTROLS:", colors[BLACK]);
	DrawString(50, 650, "Press Arrow keys to move your car.", colors[BLACK]);
	DrawString(50, 600, "Press SpaceBar to increase speed of car.", colors[BLACK]);
	DrawString(50, 550, "Press Esc or P to pause the game.", colors[BLACK]);
	DrawString(50, 500, "Press Q to exit the game at any time.", colors[BLACK]);
	DrawString(50, 450, "OBJECTIVES:", colors[BLACK]);
	DrawString(50, 400, "Collect all the gift items in the game in four levels of the game to win.", colors[BLACK]);
	DrawString(50, 350, "Avoid collision with the opponent car.", colors[BLACK]);
	glutSwapBuffers();
}
int randomizeSpawn() {
	int dice = rand() % 3;
	if (dice == 0)
		return 140;
	if (dice == 1)
		return 240;
	if (dice == 2)
		return 340;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Main Canvas drawing function.
 * */
//Board *b;
void GameDisplay()/**/{
	if (randomize) { //Random position of Car at spawn.
		OpponentY = randomizeSpawn();
		randomize = false;
	}
	playerMovementSpeed = defaultSpeed; //Car movement speed.
	CarMovement(positionX, positionY); //Carmovement function that's called.
	OpponentMovement(OpponentX, OpponentY); //Opponent car movement.
	boost = false; //Boost to be given value again.
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	jumpLimit = (activeMode) ? 2 : 1; //Active mode implementation.
	if (jumped != 0) { //This is called when jump happened for cooldown.
		jumpCounter++;
		if (jumpCounter  >= 90) {
			jumped = 0;
			jumpCounter = 0;
		}
	}

	carCollision(positionX, positionY, OpponentX, OpponentY); //Car Collision check.
	playerLane = LaneDetection(positionX, positionY);
	opponentLane = LaneDetection(OpponentX, OpponentY);
	BotMovement(playerLane, opponentLane, OpponentX, OpponentY);
	glClearColor(0.0/*Red Component*/, 0.0,	//148.0/255/*Green Component*/,
			0.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors

	FindFood(positionX, positionY);
	
	checkLevel();
	
	DrawString( 50, 800, "Score = " + to_string(score), colors[MISTY_ROSE]); //Drawing string for the game thingy above.
	DrawString( 250, 800, "Lives = " + to_string(lives), colors[MISTY_ROSE]);
	DrawString( 450, 800, "Level = " + to_string(level), colors[MISTY_ROSE]);
	DrawString( 600, 800, "HighScore = " + to_string(highScoresValue[0]), colors[MISTY_ROSE]);
	
	if (twoOpponentCars) {
		DrawSecondCar(OpponentTwoX, OpponentTwoY);
		OpponentMovement(OpponentTwoX, OpponentTwoY);
		opponentTwoLane = LaneDetection(OpponentTwoX, OpponentTwoY);
		BotMovement(playerLane, opponentTwoLane, OpponentTwoX, OpponentTwoY);
		carCollision(positionX, positionY, OpponentTwoX, OpponentTwoY);
	}
	
	if (lives == 0) { //When the player run out of
		menuSelectedItem = 3;
		status = 0;
		glutDisplayFunc(menuDisplay);
	}
	DrawCar();
	DrawOpponentCar();
	DrawArena();
	DrawFood();

	glutSwapBuffers(); // do not modify this line.. or draw anything below this line
}

bool carCollision(int x, int y, int oX, int oY) {
	if ((x >= oX - carProximity && x <= oX + carProximity && y == oY) || (y >= oY - carProximity && y <= oY + carProximity && x == oX)) {
		collisionReset();
		return true;
	}
	else { return false;  }
}

void collisionReset() {
	if (score > highScoresValue[9]) {
		highScoresValue[10] = score;
		sortScore();
		saveScore();
	}
	score = 0;
	level = 1;
	horizontalJumpingBot = false;
	activeMode = false;
	reset();
	lives--;
}

void checkLevel() {
	if (level == 1 && score == 64) {
		levelUp(2);
	}
	if (level == 2 && score == 128) {
		levelUp(3);
	}
	if (level == 3 && score == 192) {
		levelUp(4);
	}
	if (level == 4 && score == 256) {
		glutDisplayFunc(winnerScreen);
	}
}
void levelUp(int newlevel) {
	reset();
	level++;
	if (newlevel == 2) {
		activeMode = true;
		horizontalJumpingBot = true;
	}
	else if (newlevel == 3) {
		defaultSpeedOpponent = 4;
	}
	else if (newlevel == 4) {
		twoOpponentCars = true;
	}
}

bool reset() {
	positionX = PLAYERSTARTINGX;
	positionY = PLAYERSTARTINGY;
	OpponentX = OPPONENTSTARTINGX;
	OpponentY = OPPONENTSTARTINGY;
	OpponentTwoX = SECONDOPPONENTX;
	OpponentTwoY = SECONDOPPONENTY;
	randomize = true;
	FoodPosition(foodX, foodY);
	//score = 0;
}

void DrawCar() {
	float x = positionX; 
	float y = positionY; 
	float width = 12; 
	float height = 8;
	float *colour = colors[BROWN]; 
	float radius = 7.5;
	DrawRoundRect(x,y,width,height,colour,radius); // bottom left tyre
	DrawRoundRect(x+width*3,y,width,height,colour,radius); // bottom right tyre
	DrawRoundRect(x+width*3,y+height*4,width,height,colour,radius); // top right tyre
	DrawRoundRect(x,y+height*4,width,height,colour,radius); // top left tyre
	DrawRoundRect(x, y+height*2, width, height, colour, radius/2); // body left rect
	DrawRoundRect(x+width, y+height, width*2, height*3, colour, radius/2); // body center rect
	DrawRoundRect(x+width*3, y+height*2, width, height, colour, radius/2); // body right rect
}

void DrawOpponentCar() {
	float x = OpponentX;
	float y = OpponentY;
	float width = 12;
	float height = 8;
	float* color = colors[DARK_BLUE];
	float radius = 7.5;
	DrawRoundRect(x,y,width,height,color,radius); // bottom left tyre
	DrawRoundRect(x+width*3,y,width,height,color,radius); // bottom right tyre
	DrawRoundRect(x+width*3,y+height*4,width,height,color,radius); // top right tyre
	DrawRoundRect(x,y+height*4,width,height,color,radius); // top left tyre
	DrawRoundRect(x, y+height*2, width, height, color, radius/2); // body left rect
	DrawRoundRect(x+width, y+height, width*2, height*3, color, radius/2); // body center rect
	DrawRoundRect(x+width*3, y+height*2, width, height, color, radius/2); // body right rect
}

void DrawSecondCar(int &xpos, int &ypos) {
	float x = xpos;
	float y = ypos;
	float width = 12;
	float height = 8;
	float* color = colors[DARK_BLUE];
	float radius = 7.5;
	DrawRoundRect(x,y,width,height,color,radius); // bottom left tyre
	DrawRoundRect(x+width*3,y,width,height,color,radius); // bottom right tyre
	DrawRoundRect(x+width*3,y+height*4,width,height,color,radius); // top right tyre
	DrawRoundRect(x,y+height*4,width,height,color,radius); // top left tyre
	DrawRoundRect(x, y+height*2, width, height, color, radius/2); // body left rect
	DrawRoundRect(x+width, y+height, width*2, height*3, color, radius/2); // body center rect
	DrawRoundRect(x+width*3, y+height*2, width, height, color, radius/2); // body right rect
}

void DrawArena() {
	float *scolor = colors[BROWN];
	for (int i = 0; i < 4; i++) {
		int gap_turn = 90;
		int sx = sxvalue[i];
		int sy = syvalue[i];
		int swidth = sWidthInput[i]/2 - gap_turn/2; // half width
		int sheight = 10;
		DrawRectangle(sx, sy, swidth, sheight, scolor); // bottom left
		DrawRectangle(sx + swidth + gap_turn, sy, swidth, sheight, scolor); // bottom right
		DrawRectangle(sx+swidth*2+gap_turn, sy+sheight, sheight*2, swidth, scolor); // right down
		DrawRectangle(sx+swidth*2+gap_turn, sy+sheight+swidth+gap_turn, sheight*2, swidth, scolor); // right up
		DrawRectangle(sx + swidth + gap_turn, sy+sWidthInput[i]+10, swidth, sheight, scolor); // top left
		DrawRectangle(sx, sy+sWidthInput[i]+10, swidth, sheight, scolor); // top right
		DrawRectangle(sx-sheight*2, sy+sheight+swidth+gap_turn, sheight*2, swidth, scolor); // left up
		DrawRectangle(sx-sheight*2, sy+sheight, sheight*2, swidth, scolor); // left down
	}
	DrawSquare(385, 382, 40, scolor);
}

void FoodPosition(int x, int y) {
	int indexofArray = 0;
	for (int i = 0; i < 8; i++) {
		x = 60;
		for (int j = 0; j < 8; j++) {
			foodItemAxis[indexofArray][0] = x;
			foodItemAxis[indexofArray++][1] = y;
			x += 96;
		}
		y += 96;
	}
}

void DrawFood() {
	for (int i = 0; i < 64; i++) 
		DrawRectangle(foodItemAxis[i][0], foodItemAxis[i][1], 20, 10, colors[RED]);
}

void FindFood(int x, int y) {
	for (int i = 0; i < 64; i++) {
		if ((foodItemAxis[i][0] + 25 >= x && foodItemAxis[i][0] - 25 <= x)&&(foodItemAxis[i][1] + 25 >= y && foodItemAxis[i][1] - 25 <= y)) {
			foodItemAxis[i][0] = foodPositionAfterCollision;
			foodItemAxis[i][1] = foodPositionAfterCollision;
			score++;
		}
	}
}

void OpponentMovement(int&x, int&y) {
	for (int lowerLimit = 40, upperLimit = 720, i = 0; i < 4; i++, lowerLimit += 100, upperLimit -= 100) {
		if(x<upperLimit && x >= lowerLimit && y <= lowerLimit)
		{
			x += defaultSpeedOpponent;
			break;
		}
		else if(y<upperLimit && y >= lowerLimit && x >= upperLimit)
		{	
			y += defaultSpeedOpponent;
			break;
		}
		else if(y >= upperLimit && x > lowerLimit && x <= upperLimit)
		{	
			x -= defaultSpeedOpponent;
			break;
		}
		else if(x <= lowerLimit && y <= upperLimit && y > lowerLimit)
    	{    
			y -= defaultSpeedOpponent;
			break;
		}
	}
	//cout << x << ' ' << y << endl;
		
}

void CarMovement(int&x, int&y) {	
	for (int lowerLimit = 40, upperLimit = 720, i = 0; i < 4; i++, lowerLimit += 100, upperLimit -= 100) {
		if(x <= upperLimit && x > lowerLimit && y <= lowerLimit)
 		{
	 		if (boost && x > lowerLimit + 20) {
				playerMovementSpeed = defaultSpeed * multiplier;
			}
			else {
				playerMovementSpeed = defaultSpeed;
			}
			x -= playerMovementSpeed;
			break;
		}
		if(y <= upperLimit && y > lowerLimit && x >= upperLimit)
		{
	 		if (boost && y > lowerLimit + 20) {
				playerMovementSpeed = defaultSpeed * multiplier;
			}
			else {
				playerMovementSpeed = defaultSpeed;
			}
			y -= playerMovementSpeed;
			break;
		}
		if( y >= upperLimit && x >= lowerLimit && x < upperLimit)
		{
			if (boost && x < upperLimit - 20) {
				playerMovementSpeed = defaultSpeed * multiplier;
			}
			else {
				playerMovementSpeed = defaultSpeed;
			}
			x += playerMovementSpeed;
			break;
		}
		if(x <= lowerLimit && y <= upperLimit && y >= lowerLimit)
		{
			if (boost && y < upperLimit - 20) {
				playerMovementSpeed = defaultSpeed * multiplier;
			}
			else {
				playerMovementSpeed = defaultSpeed;
			}
    		y += playerMovementSpeed;
			break;
    	}
	}
}

int LaneDetection(int x, int y) {
	for (int i = 0, max = 720, min = 40; i < 4; max -= 100, min += 100, i++) {
		if (
		(x == min && y >= min && y <= max)
		|| 
		(x == max && y >= min && y <= max)
		|| 
		(y == max && x <= max && x >= min) 
		|| 
		(y == min && x <= max && x >= min)
		) {
			return i;
			break;
		}
	}
}

bool canJumpLane() {
	if (jumped < jumpLimit)
		return true;
	else return false;
}

void BotMovement(int pLane, int oLane, int &x, int &y) {
	if (pLane != oLane) {
		for (int i = 0, min = 40, max = 720; i < 4;min += 100, max -= 100, i++) {
			if (x == min && y >= min && y <= max) {
				if (pLane > oLane) {
					if (ChangeDirection("RIGHT", x, y) && horizontalJumpingBot && canJumpLane()) {
						x += 100;
						jumped++;
					}
				}
				if (pLane < oLane) {
					if (ChangeDirection("LEFT", x, y) && horizontalJumpingBot && canJumpLane()) {
						x -= 100;
						jumped++;
					}
				}
			}
			if (x == max && y >= min && y <= max) {
				if (pLane > oLane) {
					if (ChangeDirection("LEFT", x, y) && horizontalJumpingBot && canJumpLane()) {
						x -= 100;
						jumped++;
					}
				}
				if (pLane < oLane) {
					if (ChangeDirection("RIGHT", x, y) && horizontalJumpingBot && canJumpLane()) {
						x += 100;
						jumped++;
					}
				}
			}
			if (y == max && x >= min && x <= max) {
				if (pLane > oLane) {
					if (ChangeDirection("DOWN", x, y) && canJumpLane()) {
						y -= 100;
						jumped++;
					}
				}
				if (pLane < oLane) {
					if (ChangeDirection("UP", x, y) && canJumpLane()) {
						y += 100;
						jumped++;
					}
				}
			}
			if (y == min && x >= min && x <= max) {
				if (pLane > oLane) {
					if (ChangeDirection("UP", x, y) && canJumpLane()) {
						y += 100;
						jumped++;
					}
				}
				if (pLane < oLane) {
					if (ChangeDirection("DOWN", x, y) && canJumpLane()) {
						y -= 100;
						jumped++;
					}
				}
			}
		}
	}
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer      when key was pressed.
 *
 * */

//bool ChangeAble()

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		if (ChangeDirection("LEFT", positionX, positionY))
			positionX -= 100;
	}
	else if (key == GLUT_KEY_RIGHT) { 
		if (ChangeDirection("RIGHT", positionX, positionY))
			positionX += 100;
	}
	else if (key == GLUT_KEY_UP) {
		if (status == 0) {
			menuSelectedItem--;
		} else {
			if (ChangeDirection("UP", positionX, positionY)) 
				positionY += 100;
		}
	}
	else if (key == GLUT_KEY_DOWN) {
		if (status == 0) {
			menuSelectedItem++;
		} else {
			if (ChangeDirection("DOWN", positionX, positionY))
				positionY -= 100;
		}
	}
	if (menuSelectedItem < 0) {
		menuSelectedItem = maxMenuItems;
	}
	if (menuSelectedItem > maxMenuItems) {
		menuSelectedItem = 0;
	}
}
bool ChangeDirection(string directionChange, int x, int y) {
	if (x >= 340 && x <= 420) {
		if (directionChange == "UP") {
			if ((y >= 40 && y <= 240) || (y >= 420 && y <= 620))
				return true;
			else return false;
		}
		if (directionChange == "DOWN") {
			if ((y <= 720 && y >= 520) || (y <= 340 && y >= 140))
				return true;
			else return false;
		}
	}
	if (y >= 340 && y <= 420) {
		if (directionChange == "RIGHT") {
			if ((x >= 40 && x <= 240)||( x >= 420 && x <= 620)) 
				return true;
			else return false;
		}
		if (directionChange == "LEFT") {
			if ((x <= 720 && x >= 520)||(x <= 340 && x >= 140))
				return true;
			else return false;
		}
	}
	return false;
}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27) {
		//exit(1);
		status = 0;
		glutDisplayFunc(menuDisplay); 
	}
	if (key == 'Q' || key == 'q') 
		exit(0);
	if (key == 32) {
		boost = true;
	}
	if (key == 13)
		proceed = true;

	if (key == 'p' || key == 'P')
	{
		status = 0;
		glutDisplayFunc(menuDisplay);
	}
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {

	// implement your functionality here

	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(openGLRefreshTime, Timer, 0); // For smooth 60FPS
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	cout << x << " " << height - y << endl;
}
void MouseMoved(int x, int y) {
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
			{
		cout << GLUT_DOWN << " " << GLUT_UP << endl;

	} else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
			{

	}
	//glutPostRedisplay();
}
/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {

	//b = new Board(60, 60); // create a new board object to use in the Display Function ...

	//int width = 804, height = 840; // i have set my window size to be 804 x 840
	//b->InitalizeBoard(width, height);
	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Dodge 'em"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...
	//glutFullScreen();
	ReadHighScore();
	menuImage.loadImage("gameassets/dodge.png");
	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutDisplayFunc(menuDisplay);
	
	//glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(openGLRefreshTime, Timer, 0); // FOR SMOOTH BUTTER 60FPS.

	FoodPosition(foodX, foodY);
	//foodCollisionPoints();
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	glutPostRedisplay();
	glutMainLoop();
	return 1;
}

void ReadHighScore() {
	highScoreReading.open(highScoreDirectory);
	if (highScoreReading.is_open()) {
		int i = 0;
		while ( getline (highScoreReading, highScores)) {
			highScoresValue[i++] = stoi(highScores);
		}
		sortScore();
	}
	else cout << "Error: File couldn't be open." << endl;
	highScoreReading.close();
}

void sortScore() {
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			if (highScoresValue[i] > highScoresValue[j]) {
				int temp = highScoresValue[i];
				highScoresValue[i] = highScoresValue[j];
				highScoresValue[j] = temp;
			}
		}
	}
}

void saveScore() {
	highScoreWriting.open(highScoreDirectory);
	{
		for (int i = 0; i < 10; i++) {
			highScoreWriting << highScoresValue[i] << endl;
		}
	}
	highScoreWriting.close();
}
#endif /* AsteroidS_CPP_ */
