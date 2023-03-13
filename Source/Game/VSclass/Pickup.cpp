#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Pickup.h"

enum PickupType {
	XP,
	CHEST
};

Pickup::Pickup()
{
}
Pickup::~Pickup()
{
}
void Pickup::load_xp()
{
	_type = XP;
	_is_enable = false;
	load_skin({ "Resources/pickup/GemBlue.bmp", "Resources/pickup/GemGreen.bmp", "Resources/pickup/GemRed.bmp" }, RGB(255, 255, 255));
}
void Pickup::load_xp(vector<Pickup>& vec, int size)
{
	Pickup tmp;
	tmp.load_xp();
	for (int i = 0; i < size; i++)
		vec.push_back(tmp);
}
void Pickup::set_enable(bool enable)
{
	_is_enable = enable;
}
int Pickup::get_xp_value()
{
	return _xp_value;
}
void Pickup::spawn_xp(CPoint pos, int xp)
{
	_is_enable = true;
	_position = pos;
	_xp_value = xp;
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
