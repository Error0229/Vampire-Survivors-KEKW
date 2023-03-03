#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "Pickups.h"
using namespace game_framework;
Pickups::Pickups()
{
}
Pickups::~Pickups()
{
}
void Pickups::set_pos(int x, int y)
{
	this->_x = x;
	this->_y = y;
}
