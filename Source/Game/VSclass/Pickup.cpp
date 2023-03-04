#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "Pickup.h"
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
