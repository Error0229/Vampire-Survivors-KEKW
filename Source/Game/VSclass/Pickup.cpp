#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "../config.h"
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
	load_skin({ "Resources/pickup/GemBlue.bmp", "Resources/pickup/GemGreen.bmp", "Resources/pickup/GemRed.bmp" });
	_type = XP;
	_xp_value = 0;
}
Xp::Xp(CPoint pos, int xp_value) 
{
	load_skin({ "Resources/pickup/GemBlue.bmp", "Resources/pickup/GemGreen.bmp", "Resources/pickup/GemRed.bmp" });
	_type = XP;
	_xp_value = xp_value;
	_speed = 600;
	_position = pos;
	if (_xp_value < 2)
		set_selector(0);
	else if (_xp_value < 9)
		set_selector(1);
	else
		set_selector(2);
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
void Xp::spawnXP(CPoint pos, int val) {
	Xp xp = Xp(pos, val);
	xp._is_enable = true;
	xp_all.emplace_back(xp);
}
void Xp::update_XP_pos(int player_magnet) {
	CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
	for (auto& xp : xp_all) {
		if (!xp.is_enable())
			continue;
		if (xp._is_moving || VSObject::distance(xp._position, player_pos) < player_magnet) {
			xp.update_pos(player_pos);
			xp._is_moving = true;
		}
	}
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
void Xp::show()
{
	deque<Xp> tmp;
	for (auto& xp : xp_all) {
		if (xp.is_enable()) {
			xp.show_skin();
			tmp.emplace_back(xp);
		}
	}
	xp_all = std::move(tmp); // this moght just call copy constructor
}

Chest::Chest()
{
	load_skin({ "Resources/pickup/BoxOpen.bmp" });
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
deque<Xp> Xp::xp_all = {};