#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "Enemy.h"
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