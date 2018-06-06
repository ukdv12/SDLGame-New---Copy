/*
	The Maze Door class creates an object that is used to indicate where
	the player is trying to each. Visually, it is represented by a Lock
	texture. It is placed into the Room that is furthest from the starting 
	room in the maze. 
*/

#pragma once
#include "MazeObject.h"
#define IMG_MAZEDOOR "Images/Lock.png"
class MazeDoor :
	public MazeObject
{
private:
	static SDL_Texture * doorTexture;
public:
	MazeDoor(std::shared_ptr<Room> setRoom);
	~MazeDoor();
};

