/*
	The main cpp file is where gameplay actually occurs. It includes the game loop that waits for inputs from the user. This game only updates with player input. 
*/

#include <iostream>
#include <math.h>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Maze.h"
#include "Room.h"
#include "Player.h"
#include "MazeObject.h"
#include "MazeKey.h"

//PNG and Text files To Use
#define IMG_TITLE "Images/Title.png"
#define TEXT "Text/TitleFont.ttf"

//Integers
int curWindowWidth = 1024;
int curWindowHeight = 768;

//Gameplay Area Display Parameters
int GameArea_Width = 640;
int GameArea_Height = 640;
int GameArea_xOffset = 0;
int GameArea_yOffset = (curWindowHeight - 640);

//Game Title Parameters
int GameTitle_Width = curWindowWidth;
int GameTitle_Height = curWindowHeight - GameArea_Height;
int GameTitle_xOffset = 0;
int GameTitle_yOffset = 0;
int GameTitle_FontSize = 72;

//Game Info GUI Parameters
int GameInfo_Width = curWindowWidth - GameArea_Width;
int GameInfo_Height = curWindowHeight - GameTitle_Height;
int GameInfo_xOffset = GameArea_Width;
int GameInfo_yOffset = GameTitle_Height;

//Game Window
SDL_Window* window = NULL;

//Game Renderer
SDL_Renderer* renderer = NULL;

std::shared_ptr<Maze> curMaze;
std::shared_ptr<Player> curPlayer;

//Function to Display a Full Screen Texture
void FullScreenTexture(const char* fileName)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_Texture* screenTexture = IMG_LoadTexture(renderer, fileName);
	if (screenTexture == NULL)
	{
		printf("SDL could not load the Texture");
		return;
	}
	SDL_Rect textureRect;
	textureRect.x = 0;
	textureRect.y = 0;
	textureRect.w = curWindowWidth;
	textureRect.h = curWindowHeight;
	SDL_RenderCopy(renderer, screenTexture, NULL, &textureRect);
	SDL_RenderPresent(renderer);
}

//Initialize the SDL2
bool initialization()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{	
		
		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, curWindowWidth, curWindowHeight, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			
			//Create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			//Initialize Fonts
			else if (TTF_Init() == -1)
			{
				printf("SDL TTF could not initialize");
				success = false;
			}
			
		}
	}
	return success;
}

//Draw Text On Screen 
void DrawText(std::string text, int xOffset, int yOffset, int width, int height, int fontSize)
{
	//Select Font and Color
	TTF_Font *font = TTF_OpenFont(TEXT, fontSize);
	SDL_Color fontColor = { 255, 255, 255, 255 };

	//Create Rect where Text will go
	SDL_Rect textRect = { 
		xOffset,
		yOffset,
		width,
		height 
	};
	
	//Create Text Surface
	SDL_Surface * textSurface = TTF_RenderText_Solid(font, text.c_str(), fontColor);

	//Offsets required to make the text the proper size and scale on screen
	int text_xOffset = textRect.x + (textRect.w - textSurface->w) / 2;
	int text_yOffset = textRect.y + (textRect.h - textSurface->h) / 2;

	//Creates Texture and Rect for displaying texture
	SDL_Texture * textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect DisplayTextRect = { text_xOffset, text_yOffset, textSurface->w, textSurface->h };

	//Adds Texture and Outline to Renderer
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &textRect);
	SDL_RenderCopy(renderer, textTexture, NULL, &DisplayTextRect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &textRect);
}

//Draw Text on Screen that has a number next to it
void DrawText(std::string text, std::string count, int xOffset, int yOffset, int width, int height, int fontSize)
{
	//Write Text Portion
	int textXOffset = xOffset;
	int textYOffset = yOffset;
	int textWidth = width - height;
	int textHeight = height;
	DrawText(text, xOffset, yOffset, textWidth, textHeight, fontSize);
	
	//WriteIntPortion
	int intXOffset = textXOffset + textWidth;
	int intYOffset = yOffset;
	int intWidth = height;
	int intHeight = height;
	DrawText(count, intXOffset, intYOffset, intWidth, intHeight, fontSize);	
}

//Adds Multiple Rows of Text to the Renderer
void DrawText_MultipleRows(std::vector<std::string> &text, std::vector<std::string> &data, int xOffset, int yOffset, int width, int height, int fontSize)
{
	int rowCount = text.size();
	int subsetHeight = height / rowCount;
	for (int i = 0; i < rowCount; i++)
	{
		if(i < data.size())
			DrawText(text[i], data[i], xOffset, yOffset + subsetHeight * i, width, subsetHeight, fontSize);
		else
			DrawText(text[i], xOffset, yOffset + subsetHeight * i, width, subsetHeight, fontSize);
	}
}

//Draws All in game information
void DrawInGameGUI(std::string level, std::string steps, std::string lives)
{
	std::vector<std::string> infoTexts = {"Level", "Steps", "Lives", "R to Reset", "ESC to Menu" };
	std::vector<std::string> dataTexts =  { level, steps, lives };
	DrawText("A MAZE GAME", GameTitle_xOffset, GameTitle_yOffset, GameTitle_Width, GameTitle_Height, GameTitle_FontSize);
	
	DrawText_MultipleRows(infoTexts, dataTexts, GameInfo_xOffset, GameInfo_yOffset, GameInfo_Width, GameInfo_Height, 42);
}

void DrawInGameGUI(int level, int steps, int lives)
{
	DrawInGameGUI(std::to_string(level), std::to_string(steps), std::to_string(lives));
}

void DrawInGameGUI()
{
	DrawInGameGUI("-", "-", "-");
}

//Main Menu of Game. Returns true if you press the Escape key or close out, returns if you press any other key
bool MainMenu()
{
	//Create Front End
	FullScreenTexture(IMG_TITLE);
	SDL_Event * startEvent = new SDL_Event;
	while (startEvent->type != SDL_KEYDOWN && startEvent->type != SDL_QUIT)
	{
		//Do something when there is an event
		SDL_WaitEvent(startEvent);
	}
	if (startEvent->key.keysym.sym == SDLK_ESCAPE || startEvent->type == SDL_QUIT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Pop up after a level is complete. Returns True if Quitting, returns false if staying
bool LevelComplete(int steps, int deaths)
{
	int LevelComplete_FontSize = 64;
	
	//Level Complete Background Rect
	SDL_Rect fillRect;
	fillRect.x = curWindowWidth / 10;
	fillRect.y = curWindowHeight / 10;
	fillRect.w = curWindowWidth * 8 / 10;
	fillRect.h = curWindowHeight * 8 / 10;

	//Add Background Rect and Outline to Renderer
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &fillRect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &fillRect);

	//Level Complete Title Rect
	SDL_Rect levelCompleteTitleRect;
	levelCompleteTitleRect.x = fillRect.x;
	levelCompleteTitleRect.y = fillRect.y;
	levelCompleteTitleRect.w = fillRect.w;
	levelCompleteTitleRect.h = fillRect.h / 3;

	//Add Title To Renderer
	DrawText("Level Complete!", levelCompleteTitleRect.x, levelCompleteTitleRect.y, levelCompleteTitleRect.w, levelCompleteTitleRect.h, LevelComplete_FontSize);

	//Information in Level Complete Panel
	std::vector<std::string> levelComplete_TextRows = { "Steps Taken", "Lives Lost", "Press Space To Continue" };
	std::vector<std::string> levelComplete_DataRows = { std::to_string(steps), std::to_string(deaths) };

	//Level Complete Information Rect
	SDL_Rect multipleRows;
	multipleRows.x = levelCompleteTitleRect.x;
	multipleRows.y = levelCompleteTitleRect.y + levelCompleteTitleRect.h;
	multipleRows.w = levelCompleteTitleRect.w;
	multipleRows.h = fillRect.h - levelCompleteTitleRect.h;

	//Add Information To Renderer in Multiple Rows
	DrawText_MultipleRows(levelComplete_TextRows, levelComplete_DataRows, multipleRows.x, multipleRows.y, multipleRows.w, multipleRows.h, 42);
	SDL_RenderPresent(renderer);
	SDL_Event * continueEvent = new SDL_Event;
	
	//Check for User Input to see what to do next
	while (continueEvent->key.keysym.sym != SDLK_SPACE && continueEvent->type != SDL_QUIT )
	{
		//Do something when there is an event
		SDL_WaitEvent(continueEvent);
	}
	//Quits the Game
	if (continueEvent->type == SDL_QUIT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Renders all necessary objects to the screen
void RenderAllGameObjects(Maze &maze,  Player &player, int &steps)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	DrawInGameGUI(maze.mazeLevel, steps, player.playerLives);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	maze.AddMazeRoomsToRenderer(0);
	player.AddObjToRenderer();
	
	//Only start rendering obstacles after a particular level
	if (maze.mazeLevel > TRAPS_START)
		maze.AddMazeObstaclesToRenderer();
	
	//Check if the player has the key
	if (!player.hasKey)
		maze.mazeKeyPtr->AddObjToRenderer();
	
	maze.mazeDoorPtr->AddObjToRenderer();
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderPresent(renderer);
}

void ResetToStart(Maze* maze, Player * player, int *level, int* steps, int* lives)
{
	*level = 1;
	*steps = 0;
	maze->SetMaze(3, 3, 1);
	player->ResetPlayer(maze->FindRoomByPos(maze->startPos), 10);
	*lives = player->playerLives;
	RenderAllGameObjects(*maze, *player, *steps);
}

int main(int argc, char *argv[])
{
	//Initialize SDL2
	if (!initialization())
	{
		printf("Failed to initialize!\n");
	}
	else 
	{
		//Initialize Maze
		//Sets the Level, step count, and starting lives
		int level = 1, steps = 0;

		//Determines Starting Size of Maze 
		int mazeX = 3, mazeY = 3;

		bool quit = false;
		quit = MainMenu();
		if (quit)
		{
			return 0;
		}
		DrawInGameGUI();

		

		//Create New Maze
		Maze curMaze(mazeX, mazeY, GameArea_xOffset, GameArea_yOffset, GameArea_Width, GameArea_Height, renderer, level);
		//Create a new Player and render it
		Player curPlayer(curMaze.FindRoomByPos(curMaze.startPos), renderer);

		int levelStartLives = curPlayer.playerLives;

		RenderAllGameObjects(curMaze, curPlayer, steps);
		//Get the SDL Game Event
		SDL_Event* gameEvent = new SDL_Event();

		//Game Loop
		while (!quit && gameEvent->type != SDL_QUIT)
		{
			//Do something when there is an event
			SDL_WaitEvent(gameEvent);
			//If the player presses a key
			if (gameEvent->type == SDL_KEYDOWN)
			{
				//Gets the Key Input
				auto keyInput = gameEvent->key.keysym.sym;

				//If "ESC" was pressed, quit the game
				if (keyInput == SDLK_ESCAPE)
				{
					quit = MainMenu();
					if (quit)
						break;
					else
					{
						ResetToStart(&curMaze, &curPlayer, &level, &steps, &levelStartLives);
					}
				}

				//If "R" was pressed, reset the maze, reset the player position, and reduce the player life count by 1
				else if (keyInput == SDLK_r)
				{
					curPlayer.playerLives--;
					curMaze.ResetMaze();
					curPlayer.SetPlayerToStart(curMaze.FindRoomByPos(curMaze.startPos));
				}
				
				//If the player presses an arrow key, check for a movement
				else if(keyInput == SDLK_DOWN  ||
						keyInput == SDLK_LEFT ||
						keyInput == SDLK_RIGHT ||
						keyInput == SDLK_UP)
				{
					//Moves the Player According to the Key Input
					if (curPlayer.PlayerMove(keyInput, renderer))
					{
						steps++;
						curMaze.NextMazeCycle();
						curPlayer.CheckForObstacles();
					}
				}
				else {
					std::cout << "Not a valid input!" << std::endl;
				}
				RenderAllGameObjects(curMaze, curPlayer, steps);
				
			}
			//If the player reaches the final position
			if ((curPlayer.objPos == curMaze.finalPos && curPlayer.hasKey))
			{
				//Display level complete stats
				quit = LevelComplete(steps, std::max(0, levelStartLives - curPlayer.playerLives));
				if (quit)
				{ 
					break;
				}
				//Next Level Changes
				SDL_RenderClear(renderer);
				DrawInGameGUI();

				//Store how many lives the player started with
				levelStartLives = curPlayer.playerLives;
				//Increment level count
				level++;
				//Reset Step Count
				steps = 0;
				//Increment Maze Size
				mazeX+=1;
				mazeY+=1;


				//Reset the maze to be larger
				curMaze.NextLevelMaze();

				//Set Player to Start Position
				curPlayer.SetPlayerToStart(curMaze.FindRoomByPos(curMaze.startPos));
				
				RenderAllGameObjects(curMaze, curPlayer, steps);
			}
			
		}
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}
