#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "QuadTree.h"
#include "Pickup.h"
#include "Enemy.h"

Pickup::Pickup()
{
	_is_enable = false;
	_speed = 0;
	_weight = 0;
}
Pickup::~Pickup()
{
}
bool Pickup::is_enable()
{
	return _is_enable;
}
void Pickup::spawn(CPoint pos, int val)
{
	_position = pos;
	_speed = 0;
	_is_enable = true;
}
void Pickup::despawn()
{
	_is_enable = false;
	_speed = 0;
}
void Pickup::show_skin(double factor)
{
	if (!_is_enable)
		return;
	VSObject::show_skin(factor);
}

Xp::Xp()
{
	load_skin({ "Resources/pickup/GemBlue.bmp", "Resources/pickup/GemGreen.bmp", "Resources/pickup/GemRed.bmp" }, RGB(255, 255, 255));
	_type = XP;
	_xp_value = 0;
}
Xp::~Xp()
{
}
void Xp::spawn(CPoint pos, int val)
{
	// val: xp_value
	_xp_value = val;
	Pickup::spawn(pos, val);
}
int Xp::get_xp_value()
{
	return _xp_value;
}
void Xp::show_skin(double factor)
{
	if (_xp_value < 2)
		set_selector(0);
	else if (_xp_value < 9)
		set_selector(1);
	else
		set_selector(2);
	Pickup::show_skin(factor);
}

Chest::Chest()
{
	load_skin({ "Resources/pickup/BoxOpen.bmp" }, RGB(255, 255, 255));
	_type = CHEST;
	_can_evo = false;
}
Chest::~Chest()
{
}
void Chest::spawn(CPoint pos, int val)
{
	//val: can_evo
	_can_evo = val;
	Pickup::spawn(pos, val);
}
bool Chest::get_can_evo()
{
	return _can_evo;
}
