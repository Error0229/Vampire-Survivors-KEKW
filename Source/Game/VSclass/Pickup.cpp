#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "../config.h"
#include "VSObject.h"
#include "QuadTree.h"
#include "ObjPool.h"
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
void Pickup::spawn(CPoint pos)
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
void Xp::init_XP() {
	Xp::pool.add_obj(Xp(), 2000);
}
void Xp::spawn(CPoint pos, int val)
{
	// val: xp_value
	_xp_value = val;
	Pickup::spawn(pos);
}
void Xp::spawnXP(CPoint pos, int val) {
	// Xp xp = Xp(pos, val);
	Xp* xp = Xp::pool.get_obj_ptr(XP);
	xp-> spawn(pos, val);
	xp->_speed = 200;
	xp_all.emplace_back(xp);
}
void Xp::update_XP_pos(int player_magnet) {
	CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
	for (auto xp : xp_all) {
		if (!xp->is_enable())
			continue;
		if (xp->_is_moving || VSObject::distance(xp->_position, player_pos) < player_magnet) {
			xp->update_pos(player_pos);
			xp->_is_moving = true;
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
	for (auto xp : xp_all) {
		if (xp->is_enable()) {
			xp->show_skin();
		}
		else {
			Xp::pool.free_obj_ptr(xp);
		}
	}
	auto iter = remove_if(xp_all.begin(), xp_all.end(), [](Xp* xp) {return !xp->is_enable(); });
	xp_all.erase(iter, xp_all.end());
}

Chest::Chest()
{
	load_skin({ "Resources/pickup/BoxOpen.bmp" });
	_type = CHEST;
	_can_evo = true;
	_upgrade_chance_0 = 0;
	_upgrade_chance_1 = 0;
}
Chest::~Chest()
{
}
void Chest::spawn(CPoint pos, bool can_evo, int chance0, int chance1)
{
	_can_evo = can_evo;
	_upgrade_chance_0 = chance0;
	_upgrade_chance_1 = chance1;
	Pickup::spawn(pos);
}
bool Chest::get_can_evo()
{
	return _can_evo;
}
int Chest::get_upgrade_chance_0()
{
	return _upgrade_chance_0;
}
int Chest::get_upgrade_chance_1()
{
	return _upgrade_chance_1;
}
void Chest::init_chest()
{
	pool.add_obj(Chest(), 100);
}
void Chest::spawnChest(CPoint pos, bool can_evo, int chance0, int chance1)
{
	Chest* chest = pool.get_obj_ptr(CHEST);
	chest-> spawn(pos, can_evo, chance0, chance1);
	chest_all.emplace_back(chest);
}
void Chest::show()
{
	for (auto chest : chest_all) {
		if (chest->is_enable()) {
			chest->show_skin();
		}
		else {
			pool.free_obj_ptr(chest);
		}
	}
	auto iter = remove_if(chest_all.begin(), chest_all.end(), [](Chest* chest) {return !chest->is_enable(); });
	chest_all.erase(iter, chest_all.end());
}
deque<Xp*> Xp::xp_all = {};
ObjPool<Xp> Xp::pool(XP);
deque<Chest*> Chest::chest_all = {};
ObjPool<Chest> Chest::pool(CHEST);