#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "Enemy.h"
using namespace game_framework;
Enemy::Enemy()
{
}
Enemy::~Enemy() 
{
}
void Enemy::set_level(int level)
{
	this->_level = level;
}
void Enemy::set_pos(int x, int y)
{
	this->_x = x;
	this->_y = y;
}