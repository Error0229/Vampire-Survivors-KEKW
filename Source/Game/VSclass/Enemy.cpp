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
void Enemy::set_stat(int level, int health, int damage, int speed)
{
	_level = level;
	_health = health;
	_damage = damage;
	_speed = speed;
}
void Enemy::load_by_name(char* name)
{
	char tmp[100];
	for (int i = 0; i < 5; i++) {
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, ".\\Resources\\enemy\\%s_i0%d.bmp", name, (i + 1));
		struct stat buffer;
		if (stat(tmp, &buffer) == 0) {
			//the file exist
			load_skin(tmp, RGB(255, 255, 255));
		}
		else {
			//the file doesnt exist
			TRACE(_T("stop at %s\n"), tmp);
			break;
		}
	}
	for (int i = 0; i < 20; i++) {
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, ".\\Resources\\enemy\\%s_%d.bmp", name, i);
		struct stat buffer;
		if (stat(tmp, &buffer) == 0) {
			//the file exist
			_corpse.load_skin(tmp, RGB(255, 255, 255));
		}
		else {
			//the file doesnt exist
			TRACE(_T("stop at %s\n"), tmp);
			break;
		}
	}
}
bool Enemy::is_dead()
{
	return (_health <= 0);
}
void Enemy::show_skin(double factor)
{
	if (!is_dead()) {
		this->_skin.SetTopLeft(this->_position.x - (this->_skin.Width() >> 1) + player_dx, this->_position.y - (this->_skin.Height() >> 1) + player_dy);
		this->_skin.ShowBitmap(factor, _is_mirror);
	}
	else {
		_corpse.show_skin(factor);
	}
}
void Enemy::hurt(int damage) 
{
	if (!is_dead()) {
		_health -= damage;
		if (is_dead()) {
			unshow_skin();
			_corpse.set_pos(get_pos());
			_corpse.set_animation(500, true);
			_corpse.enable_animation();
		}
	}
}
