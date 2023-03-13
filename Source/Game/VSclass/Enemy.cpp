#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Enemy.h"
#include "VSMath.h"
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
void Enemy::set_stat(int level, int health, int damage, int speed, int xp_value)
{
	_level = level;
	_health = health;
	_damage = damage;
	_speed = speed;
	_xp_value = xp_value;
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
			_death_animation.load_skin(tmp, RGB(255, 255, 255));
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

bool Enemy::is_enable()
{
	return _is_enable;
}
void Enemy::show_skin(double factor)
{
	if ( !_is_enable )
		return;
	if ( !is_dead() ) {
		this->_skin.SetTopLeft(this->_position.x - ( this->_skin.Width() >> 1 ) + player_dx, this->_position.y - ( this->_skin.Height() >> 1 ) + player_dy);
		this->_skin.ShowBitmap(factor, _is_mirror);
	}
	else {
		if (_death_animation.is_animation_done() ) {
			_death_animation.unshow_skin();
			_is_enable = false;
		}
		else {
			_death_animation.show_skin(factor);
		}
	}
}
bool Enemy::hurt(int damage) 
{
	if (!is_dead()) {
		_health -= damage;
		if (is_dead()) {
			unshow_skin();
			_death_animation.set_pos(get_pos());
			_death_animation.set_animation(100, true);
			_death_animation.set_is_mirror(_is_mirror);
			_death_animation.enable_animation();
			return true;
		}
	}
	return false;
}

void Enemy::set_enable(bool enable) 
{
	_is_enable = enable;
}

int Enemy::get_xp_value()
{
	return _xp_value;
}

void load_enemy_type(vector<Enemy>& vec, char* name, int n, int level, int health, int damage, int speed, int xp_value) {
	Enemy tmp;
	tmp.load_by_name(name);
	tmp.set_stat(level, health, damage, speed, xp_value);
	tmp.set_animation(50, false);
	for ( int i = 0; i < n; i++ ) {
		vec.push_back(tmp);
	}
}
