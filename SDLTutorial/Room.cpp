#include "Room.h"

SDL_Texture *  Room::roomTextures[16];

Room::Room(Coordinate pos, SDL_Renderer * renderer) : roomPos(pos)
{
	roomRenderer = renderer;
	
	//Load All Textures
	if(roomTextures[0] == NULL)
	{
		roomTextures[0] = IMG_LoadTexture(roomRenderer, IMG_4BORDER);
		roomTextures[1] = IMG_LoadTexture(roomRenderer, IMG_3BORDER_DLU);
		roomTextures[2] = IMG_LoadTexture(roomRenderer, IMG_3BORDER_LUR);
		roomTextures[3] = IMG_LoadTexture(roomRenderer, IMG_3BORDER_DUR);
		roomTextures[4] = IMG_LoadTexture(roomRenderer, IMG_3BORDER_DLR);
		roomTextures[5] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_LU);
		roomTextures[6] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_UR);
		roomTextures[7] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_DL);
		roomTextures[8] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_DR);
		roomTextures[9] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_DU);
		roomTextures[10] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_LR);
		roomTextures[11] = IMG_LoadTexture(roomRenderer, IMG_1BORDER_D);
		roomTextures[12] = IMG_LoadTexture(roomRenderer, IMG_1BORDER_L);
		roomTextures[13] = IMG_LoadTexture(roomRenderer, IMG_1BORDER_U);
		roomTextures[14] = IMG_LoadTexture(roomRenderer, IMG_1BORDER_R);
		roomTextures[15] = IMG_LoadTexture(roomRenderer, IMG_0BORDER);
	}
	curRoomTexture = roomTextures[0];
}


Room::Room(const Room &other) :
	roomPos(other.roomPos),
	roomRect(other.roomRect), 
	adjRooms(other.adjRooms), 
	availRooms(other.availRooms), 
	connectRooms(other.connectRooms)
{
}

Room::~Room()
{
	std::cout << "Room Destructor for Room " << roomPos.xPos << ", " << roomPos.yPos << std::endl;
}



