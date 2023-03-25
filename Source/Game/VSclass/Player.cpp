#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Weapon.h"
#include "Passive.h"
#include "Player.h"
using namespace game_framework;
Player::Player()
{
	obj_type = PLAYER;
	_magnet = 100;
	_luck = 100;
	_speed = 200;
	_exp = 0;
	_max_exp = 5;
	_level = 1;
	//for some reason, load skin in constructor will cause the some error
	//_bleed_animation.load_skin({ "resources/character/Blood1.bmp", "resources/character/Blood2.bmp", "resources/character/Blood3.bmp" });
	//_bleed_animation.set_animation(50, false);
}
Player::~Player()
{
}
void Player::update_pos(CPoint target) {
	CPoint pos = _position;
	VSObject::update_pos(target);
	VSObject::player_dx -= _position.x - pos.x ;
	VSObject::player_dy -= _position.y - pos.y ;
}
void Player::load_bleed() {
	_bleed_animation.load_skin({ "resources/character/Blood1.bmp", "resources/character/Blood2.bmp", "resources/character/Blood3.bmp" });
	_bleed_animation.enable_animation();
}
void Player::show_skin(double factor) {
	VSObject::show_skin(factor);
	if (_is_hurt) {
		_bleed_animation.set_animation(100, false);	//set_animation everytime because unshow will make the animation ended
		_bleed_animation.set_pos(_position);
		_bleed_animation.show_skin();
		_is_hurt = false;
	}
	else {
		_bleed_animation.unshow_skin();
	}
}
void Player::hurt(int damage) {
	_hp -= damage;
	_is_hurt = true;
	if (_hp <= 0) {
		_hp = 0;
	}
}
void Player::acquire_weapon(Weapon& weapon) {
	Weapon::all_weapon.push_back(weapon);
}
void Player::acquire_passive(Passive& passive) {
	update_passive_effect(passive);
	Passive::all_passive.push_back(passive);
}
void Player::update_passive_effect(Passive& p) {
	int effect = p.get_effect();
	switch (p.get_type()) {
	case POWER:
		_might += effect;
		break;
	case ARMOR:
		_armor += effect;
		break;
	case MAXHEALTH:
		_max_health += effect;
		break;
	case REGEN:
		_recovery += effect;
		break;
	case COOLDOWN:
		_cooldown += effect;
		break;
	case AREA:
		_area += effect;
		break;
	case SPEED:
		_proj_speed += effect;
		break;
	case DURATION:
		_duration += effect;
		break;
	case AMOUNT:
		_amount += effect;
		break;
	case MOVESPEED:
		_speed += effect;
		break;
	case MAGNET:
		_magnet += effect;
		break;
	case LUCK:
		_luck += effect;
		break;
	case GROWTH:
		_growth += effect;
		break;
	case GREED:
		_greed += effect;
		break;
	case REVIVAL:
		_revival += effect;
		break;
	case CURSE:
		_curse += effect;
		break;
	case SILVER:
		_duration += effect;
		_area += effect;
		break;
	case GOLD:
		_curse += effect;
		break;
	case pLEFT:
		_max_health += effect;
		_recovery += p.get_alt_effect();
		break;
	case pRIGHT:
		_curse += effect;
		break;
	case PANDORA:
		if (p.is_max_level()) {
			_curse += effect;
		}
		else {
			_might += effect;
			_proj_speed += effect;
			_duration += effect;
			_area += effect;
		}
		break;
	}	
}
void Player::level_up_passive(Passive& p) {
	VS_ASSERT(!p.is_max_level(), "level up passive above max level.");
	p.level_up();
	update_passive_effect(p);
}

bool Player::pick_up_xp(int xp_value)
{
	_exp += xp_value;
	return (_exp >= _max_exp);
}
bool Player::apply_level_up()
{
	VS_ASSERT(_exp >= _max_exp, "attemp to lvl up when xp < max_xp");
	_exp -= _max_exp;

	if (_level < 20)
		_max_exp += 10;
	else if (_level < 40)
		_max_exp += 13;
	else
		_max_exp += 16;

	if (_level == 20) {
		_max_exp += 600;
		_growth += 100;
	}
	else if (_level == 40) {
		_max_exp += 2400;
		_growth += 100;
	}
	else if (_level == 21 && _level == 41) {
		_growth -= 100;
	}
	return (_exp >= _max_exp);
}
int Player::get_pickup_range()
{
	return _magnet;
}
int Player::get_level()
{
	return _level;
}
int Player::get_luck()
{
	return _luck;
}

int Player::passive_count()
{
	return Passive::all_passive.size();
}

void Player::obtain_item(int type)
{
	bool is_own = false;
	if (type < 32) {
		//weapon
		for (auto& i : Weapon::all_weapon) {
			if (i.get_type() == type) {
				is_own = true;
				if (i.is_max_level())
					Weapon::evolution(i.get_type());
				else
					i.upgrade();
				break;
			}
		}
		if (!is_own) {
			acquire_weapon(Weapon::_base_weapon[type]);
		}
	}
	else if (type < 63) {
		Weapon::evolution(type);
	}
	else{
		//passive
		for (auto& i : Passive::all_passive) {
			if (i.get_type() == type) {
				level_up_passive(i);
				is_own = true;
				break;
			}
		}
		if (!is_own) {
			acquire_passive(Passive(type));
		}
	}
}

bool Player::all_max()
{
	for (auto& i : Weapon::all_weapon) {
		if (!i.is_max_level())
			return false;
	}
	for (auto& i : Passive::all_passive) {
		if (!i.is_max_level())
			return false;
	}
	return true;
}
bool Player::full_inv()
{
	return Weapon::weapon_count() >= 6 && Passive::all_passive.size() >= 6;
}