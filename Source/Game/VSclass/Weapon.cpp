#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "Weapon.h"
using namespace game_framework;
Weapon::Weapon()
{
}
Weapon::~Weapon()
{
}
Weapon::Weapon(CMovingBitmap skin, CMovingBitmap bullet, int type, int level, int damage, int speed, int range) {
	this->_skin = skin;
	this->_bullet = bullet;
	this->_type = type;
	this->_level = level;
	this->_damage = damage;
	this->_speed = speed;
	this->_range = range;
}
void Weapon::show_bullet() {
	this->_bullet.ShowBitmap();
}