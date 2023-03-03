#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
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