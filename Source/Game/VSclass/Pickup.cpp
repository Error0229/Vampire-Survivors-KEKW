#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Pickup.h"

Pickup::Pickup()
{
}
Pickup::~Pickup()
{
}
int Pickup::get_xp_value()
{
	return _xp_value;
}
bool Pickup::get_can_evo()
{
	return _can_evo;
}
void Pickup::spawn_xp(CPoint pos, int xp)
{
	_xp_value = xp;
	spawn(pos);
}
void Pickup::spawn_chest(CPoint pos, bool can_evo)
{
	_can_evo = can_evo;
	spawn(pos);
}
void Pickup::spawn(CPoint pos)
{
	_is_enable = true;
	_position = pos;
	_speed = 0;
}
void Pickup::despawn()
{
	_is_enable = false;
	_speed = 0;
}
void Pickup::show_skin(double factor)
{
	if(!_is_enable)
		return;
	switch (_type)
	{
	case XP:
		if (_xp_value < 100)
			_skin.SelectShowBitmap(0);
		else if (_xp_value < 200)
			_skin.SelectShowBitmap(1);
		else
			_skin.SelectShowBitmap(2);
		break;
	}
	VSObject::show_skin(factor);
}

bool Pickup::is_enable()
{
	return _is_enable;
}

void Pickup::load_template_pickup()
{
	for (int i = 0; i < 2; i++) {
		template_pickup.push_back(Pickup());
		switch (i){
		case XP:
			template_pickup[i].load_skin({"Resources/pickup/GemBlue.bmp", "Resources/pickup/GemGreen.bmp", "Resources/pickup/GemRed.bmp"}, RGB(255, 255, 255));
			template_pickup[i]._type = XP;
			break;
		case CHEST:
			template_pickup[i].load_skin({ "Resources/pickup/BoxOpen.bmp" }, RGB(255, 255, 255));
			template_pickup[i]._type = CHEST;
			break;
		}
		template_pickup[i]._xp_value = 0;
		template_pickup[i]._can_evo = false;
		template_pickup[i]._speed = 0;
		template_pickup[i]._is_enable = false;
	}
}
Pickup Pickup::get_template_pickup(int type)
{
	return template_pickup[type];
}

vector<Pickup> Pickup::template_pickup;