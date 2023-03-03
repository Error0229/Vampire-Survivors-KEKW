#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "Map.h"
using namespace game_framework;

Map::Map()
{
}
Map::~Map() 
{
}
void Map::load_map()
{
	this->base_backgrounds.LoadBitmapByString({});
}
void Map::show_map()
{
	
}
void Map::map_padding()
{
	
}