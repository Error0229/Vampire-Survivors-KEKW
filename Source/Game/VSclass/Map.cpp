#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Map.h"

Map::Map()
{
}
Map::Map(vector<char*> filename) : VSObject(filename)
{
}
Map::~Map() 
{
}
void Map::load_map(vector<char*> filename, COLORREF color)
{
	VSObject::load_skin(filename);
}
void Map::show_map()
{
	this->_skin.SetTopLeft(this->_position.x - ( this->_skin.Width() >> 1 ) + player_dx, this->_position.y - ( this->_skin.Height() >> 1 ) + player_dy);
	this->_skin.ShowBitmap();
}
void Map::map_padding()
{
	
}