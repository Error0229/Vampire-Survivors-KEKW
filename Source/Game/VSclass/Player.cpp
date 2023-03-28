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
	_max_health = 100;
	_recovery = 0;
	_armor = 0;
	_speed = 100; // 100%
	_might = 100; // 100%
	_area = 100;
	_proj_speed = 100;
	_duration = 100;
	_amount = 0;
	_cooldown = 100; // 100%
	_luck = 100;
	_growth = 100;
	_greed = 100;
	_curse = 100;
	_magnet = 100; // base value is 30
	_reroll = 0;
	_exp = 0;
	_max_exp = 5;
	_level = 1;
	_base_stats = {
		{"might", _might},
		{"armor", _armor},
		{"max_health", _max_health},
		{"recovery", _recovery},
		{"cooldown", _cooldown},
		{"area", _area},
		{"proj_speed", _proj_speed},
		{"duration", _duration},
		{"amount", _amount},
		{"move_speed", _move_speed},
		{"magnet", _magnet},
		{"luck", _luck},
		{"growth", _growth},
		{"greed", _greed},
		{"revival", _revival},
		{"curse", _curse }
	};
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
void Player::acquire_weapon(int weapon_id) {
	Weapon::all_weapon.push_back(Weapon::_base_weapon[weapon_id]);
	Weapon::update_all_weapon_stats(_might, _cooldown, _proj_speed, _duration, _amount, _area);
}
void Player::acquire_passive(Passive& passive) {
	Passive::all_passive.push_back(passive);
}
void Player::acquire_passive(int passive_id) {
	Passive::all_passive.push_back(Passive(passive_id));
}
void Player::update_all_passive_effect() {
	_might = any_cast<int>(_base_stats["might"]) + Passive::get_effect(POWER) + Passive::get_effect(PANDORA);
	_armor = any_cast<int>(_base_stats["armor"]) + Passive::get_effect(ARMOR);
	_max_health = any_cast<int>(_base_stats["max_health"]) + Passive::get_effect(MAXHEALTH) + Passive::get_effect(pLEFT);
	_recovery = any_cast<double>(_base_stats["recovery"]) + Passive::get_effect(REGEN)/100.0 + Passive::get_effect(pLEFT) / 500.0;
	_cooldown = any_cast<int>(_base_stats["cooldown"]) + Passive::get_effect(COOLDOWN);
	_area = any_cast<int>(_base_stats["area"]) + Passive::get_effect(AREA) + Passive::get_effect(SILVER) + Passive::get_effect(PANDORA);
	_proj_speed = any_cast<int>(_base_stats["proj_speed"]) + Passive::get_effect(SPEED) + Passive::get_effect(PANDORA);
	_duration = any_cast<int>(_base_stats["duration"]) + Passive::get_effect(DURATION) + Passive::get_effect(SILVER) + Passive::get_effect(PANDORA);
	_amount = any_cast<int>(_base_stats["amount"]) + Passive::get_effect(AMOUNT);
	_move_speed = any_cast<int>(_base_stats["move_speed"]) + Passive::get_effect(MOVESPEED);
	_magnet = any_cast<int>(_base_stats["magnet"]) + Passive::get_effect(MAGNET);
	_luck = any_cast<int>(_base_stats["luck"]) + Passive::get_effect(LUCK);
	_growth = any_cast<int>(_base_stats["growth"]) + Passive::get_effect(GROWTH);
	_greed = any_cast<int>(_base_stats["greed"]) + Passive::get_effect(GREED);
	_revival = any_cast<int>(_base_stats["revival"]) + Passive::get_effect(REVIVAL);
	_curse = any_cast<int>(_base_stats["curse"]) + Passive::get_effect(CURSE) + Passive::get_effect(GOLD) + Passive::get_effect(pRIGHT);
	Weapon::update_all_weapon_stats(_might, _cooldown, _proj_speed, _duration, _amount, _area); // should this be here ? 
}

void Player::level_up_passive(Passive& p) {
	VS_ASSERT(!p.is_max_level(), "level up passive above max level.");
	p.level_up();
}
void Player::level_up_passive(int passive_id) {
	Passive::upgrade(passive_id);
	update_all_passive_effect();
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
bool Player::have(int type) {
	for (auto& i : Weapon::all_weapon) {
		if (i.get_type() == type) {
			return true;
		}
	} 
	for (auto& i : Passive::all_passive) {
		if (i.get_type() == type) {
			return true;
		}
	}
	return false;
}
void Player::obtain_item(int type)
{
	bool is_own = false;
	if (type < 32) {
		//weapon
		if (this->have(type)) {
			Weapon::upgrade(type);
		}
		else {
			acquire_weapon(type);
		}
		//for (auto& i : Weapon::all_weapon) {
		//	if (i.get_type() == type) {
		//		i.upgrade();
		//		is_own = true;
		//		break;
		//	}
		//}
		//if (!is_own) {
		//	acquire_weapon(Weapon::_base_weapon[type]);
		//}
	}
	else if (type < 63) {
		Weapon::evolution(type);
	}
	else{
		//passive
		if (this->have(type)) {
			level_up_passive(type);
		}
		else {
			acquire_passive(type);
		}
		//for (auto& i : Passive::all_passive) {
		//	if (i.get_type() == type) {
		//		level_up_passive(i);
		//		is_own = true;
		//		break;
		//	}
		//}
		//if (!is_own) {
		//	acquire_passive(Passive(type));
		//}
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