#pragma once
#include "Coordinate.h"
#include <vector>
#include <algorithm>
#include "SDL.h"
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include "SDL_image.h"
//#include "MazeObject.h"

#define IMG_4BORDER "Images/Room_4Border.png"

#define IMG_3BORDER_DLU "Images/Room_3Border_D_L_U.png"
#define IMG_3BORDER_LUR "Images/Room_3Border_L_U_R.png"
#define IMG_3BORDER_DUR "Images/Room_3Border_D_U_R.png"
#define IMG_3BORDER_DLR "Images/Room_3Border_D_L_R.png"

#define IMG_2BORDER_LU "Images/Room_2Border_L_U.png"
#define IMG_2BORDER_UR "Images/Room_2Border_U_R.png"
#define IMG_2BORDER_DL "Images/Room_2Border_D_L.png"
#define IMG_2BORDER_DR "Images/Room_2Border_D_R.png"
#define IMG_2BORDER_DU "Images/Room_2Border_D_U.png"
#define IMG_2BORDER_LR "Images/Room_2Border_L_R.png"

#define IMG_1BORDER_D "Images/Room_1Border_D.png"
#define IMG_1BORDER_L "Images/Room_1Border_L.png"
#define IMG_1BORDER_U "Images/Room_1Border_U.png"
#define IMG_1BORDER_R "Images/Room_1Border_R.png"

#define IMG_0BORDER "Images/Room_0Border.png"

enum directions { up, left, down, right };

//Used to determine what rooms may do in the maze
enum RoomType{None, Start, Final, Key, Trap, Guard };
class Room
{
private:
public:
	
	RoomType roomType = None;
	std::vector<RoomType> roomTypes;
	Coordinate roomPos;
	bool inMaze = false;	//Tells us if the Room is in the maze
	
	std::vector<std::shared_ptr<Room>> adjRooms;		//Rooms adjacent to this room
	std::vector<std::shared_ptr<Room>> availRooms;		//Rooms that could be selected to connect to this room when creating the maze
	std::vector<std::shared_ptr<Room>> connectRooms;	//Rooms that are connected to the this room
	std::vector<directions> wallDirections = { down, left, up, right };	
	
	//std::shared_ptr<MazeObject> roomObj;
	//std::vector<std::shared_ptr<MazeObject>> roomObjects;

	SDL_Rect roomRect;
	SDL_Texture * curRoomTexture;
	
	static SDL_Texture * roomTextures[16];
	std::vector<SDL_Rect> wallRects;
	SDL_Renderer * roomRenderer;

	Room(Coordinate pos, SDL_Renderer * renderer);
	Room(const Room &obj);
	~Room();

	//Create Room Rect
	void MakeRoomRect(int &xOffset, int &yOffset, int &xSplits, int &ySplits, const int &mazeWidth, const int &mazeHeight)
	{
		roomRect = {
			xOffset + (int)((roomPos.xPos)* ceil(mazeWidth / xSplits)),
			yOffset + (int)((roomPos.yPos)* ceil(mazeHeight / ySplits)),
			(int)ceil(mazeWidth / (xSplits)),
			(int)ceil(mazeHeight / (ySplits))
		};
	}

	//Check the direction of an adjacent room
	directions CheckAdjRoomDir(Room &room)
	{
		directions dir;
		if (roomPos.xPos == room.roomPos.xPos)
		{
			if (roomPos.yPos < room.roomPos.yPos)
				dir = down;
			else if (roomPos.yPos > room.roomPos.yPos)
				dir = up;
		}
		else if (roomPos.yPos == room.roomPos.yPos)
		{
			if (roomPos.xPos < room.roomPos.xPos)
				dir = right;

			else if (roomPos.xPos > room.roomPos.xPos)
				dir = left;
		}
		return dir;
	}

	//Assign Texture to this room based off the number of walls (wallDirections)
	void AssignRoomTextures()
	{
		int RoomWallCount = wallDirections.size();
		switch (RoomWallCount)
		{
		case 0:
			curRoomTexture = roomTextures[15];
		case 1:
			if (std::find(begin(wallDirections), end(wallDirections), down) != end(wallDirections))
				curRoomTexture = roomTextures[11];
			else if (std::find(begin(wallDirections), end(wallDirections), left) != end(wallDirections))
				curRoomTexture = roomTextures[12];
			else if (std::find(begin(wallDirections), end(wallDirections), up) != end(wallDirections))
				curRoomTexture = roomTextures[13];
			else if (std::find(begin(wallDirections), end(wallDirections), right) != end(wallDirections))
				curRoomTexture = roomTextures[14];
			break;
		case 2:
			if (std::find(begin(wallDirections), end(wallDirections), left) != end(wallDirections) &&
				std::find(begin(wallDirections), end(wallDirections), up) != end(wallDirections))
				curRoomTexture = roomTextures[5];
			else if (std::find(begin(wallDirections), end(wallDirections), up) != end(wallDirections) &&
				std::find(begin(wallDirections), end(wallDirections), right) != end(wallDirections))
				curRoomTexture = roomTextures[6];
			else if (std::find(begin(wallDirections), end(wallDirections), down) != end(wallDirections) &&
				std::find(begin(wallDirections), end(wallDirections), left) != end(wallDirections))
				curRoomTexture = roomTextures[7];
			else if (std::find(begin(wallDirections), end(wallDirections), down) != end(wallDirections) &&
				std::find(begin(wallDirections), end(wallDirections), right) != end(wallDirections))
				curRoomTexture = roomTextures[8];
			else if (std::find(begin(wallDirections), end(wallDirections), down) != end(wallDirections) &&
				std::find(begin(wallDirections), end(wallDirections), up) != end(wallDirections))
				curRoomTexture = roomTextures[9];
			else if (std::find(begin(wallDirections), end(wallDirections), left) != end(wallDirections) &&
				std::find(begin(wallDirections), end(wallDirections), right) != end(wallDirections))
				curRoomTexture = roomTextures[10];
			break;
		case 3:
			if (std::find(begin(wallDirections), end(wallDirections), right) == end(wallDirections))
				curRoomTexture = roomTextures[1];
			else if (std::find(begin(wallDirections), end(wallDirections), down) == end(wallDirections))
				curRoomTexture = roomTextures[2];
			else if (std::find(begin(wallDirections), end(wallDirections), left) == end(wallDirections))
				curRoomTexture = roomTextures[3];
			else if (std::find(begin(wallDirections), end(wallDirections), up) == end(wallDirections))
				curRoomTexture = roomTextures[4];
			break;
		}
	}

	//Remove Wall if one is there 
	void RemoveWallDirection(std::shared_ptr<Room> &roomToConnectPtr)
	{
		directions dirToRemove = CheckAdjRoomDir(*roomToConnectPtr);
		auto iterToRemove = std::find(begin(wallDirections), end(wallDirections), dirToRemove);
		if (iterToRemove != end(wallDirections))
		{
			wallDirections.erase(iterToRemove);
		}
	}

	//Connect Room and Remove Wall Direction
	void ConnectRoom(std::shared_ptr<Room> &roomToConnectPtr)
	{
		connectRooms.push_back(roomToConnectPtr);
		RemoveWallDirection(roomToConnectPtr);
	}

	//Adds room to Renderer. If it has a delay, render immediately after waiting.
	void AddRoomToRenderer(int delay)
	{
		SDL_RenderCopy(roomRenderer, curRoomTexture, NULL, &roomRect);
		if(delay > 0)
		{
			SDL_Delay(delay);
			SDL_RenderPresent(roomRenderer);
		}
		/*
		if (roomObjects.size() > 0)
		{
			for_each(begin(roomObjects), end(roomObjects), [](std::shared_ptr<MazeObject> curMazeObject) {
				curMazeObject->AddObjToRenderer();
			});
		}*/
	}
	void AddRoomToRenderer()
	{
		AddRoomToRenderer(0);
	}

};

