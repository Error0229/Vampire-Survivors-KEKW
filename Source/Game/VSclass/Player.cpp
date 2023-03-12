#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Weapon.h"
#include "Passive.h"
#include "Player.h"
using namespace game_framework;
Player::Player()
{
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
void Player::acquire_weapon(Weapon* weapon) {
	_weapons.push_back(weapon);
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
