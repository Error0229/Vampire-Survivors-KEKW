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
	
}
void Map::map_padding()
{
	
}