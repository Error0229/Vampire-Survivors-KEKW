#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Weapon.h"
#include "Passive.h"
#include "Player.h"
using namespace game_framework;
Player::Player()
{
	_magnet = 100;
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
	_bleed_animation.load_skin({ "resources/character/Blood1.bmp", "resources/character/Blood2.bmp", "resources/character/Blood3.bmp" }, RGB(0,0,0));
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
void Player::acquire_weapon(Weapon* weapon) {
	_weapons.push_back(weapon);
}
void Player::acquire_passive(Passive* passive) {
	_passives.push_back(passive);
	update_passive(passive);
}
void Player::update_passive(Passive* p) {
	int effect = p->get_effect();
	switch (p->get_type()) {
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
		_recovery += p->get_alt_effect();
		break;
	case pRIGHT:
		_curse += effect;
		break;
	case PANDORA:
		if (p->is_max_level()) {
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
void Player::level_up_passive(int index) {
	_passives[index]->level_up();
	update_passive(_passives[index]);
}

void Player::update_proj_pos() {
	for ( auto& w : _weapons ) {
		w->update_proj(_position, _direct, this->get_width(), this->get_width());
	}
}
void Player::show_proj_skin() {
	for ( auto& w : _weapons ) {
		w->show_proj();
	}
}
void Player::pick_up_xp(int xp_value)
{
	_exp += xp_value;
	if (_exp >= _max_exp) {
		//leveling up
	}
}
int Player::get_pickup_range()
{
	return _magnet;
}