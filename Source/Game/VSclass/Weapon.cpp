#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Weapon.h"
using namespace game_framework;
Weapon::Weapon()
{
}
Weapon::~Weapon()
{
}
Weapon::Weapon(int type, vector<char*>& skin, vector<char*>& bullet, int damage, float speed, float range) {
	this->load_skin(skin);
	this->_base_proj->load_skin(bullet);
	this->_type = type;
	this->_damage = damage;
	this->_speed = speed;
	this->_range = range;
}
void Weapon::update_proj() {
	for (auto& bullet : _proj_set) {
		bullet->update_pos();
	}
}
void Weapon::show_proj() {
	for (auto& bullet : _proj_set) {
		bullet->show_skin();
	}
}