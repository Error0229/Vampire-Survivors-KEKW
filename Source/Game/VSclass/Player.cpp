#include "stdafx.h"
#include "../../Library/gameutil.h"
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
void Player::set_pos(int x, int y)
{
	this->_x = x;
	this->_y = y;
}