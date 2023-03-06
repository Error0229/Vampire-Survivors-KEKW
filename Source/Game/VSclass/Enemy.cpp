#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
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
