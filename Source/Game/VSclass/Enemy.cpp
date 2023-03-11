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
		if ( _corpse.is_animation_done() ) {
			_corpse.unshow_skin();
			_is_enable = false;
		}
		else {
			_corpse.show_skin(factor);
		}
	}
}
void Enemy::hurt(int damage) 
{
	if (!is_dead()) {
		_health -= damage;
		if (is_dead()) {
			unshow_skin();
			_corpse.set_pos(get_pos());
			_corpse.set_animation(100, true);
			_corpse.set_is_mirror(_is_mirror);
			_corpse.enable_animation();
		}
	}
}

void Enemy::set_enable(bool enable) 
{
	_is_enable = enable;
}

void load_enemy_type(vector<Enemy>& vec, char* name, int n, int level, int health, int damage, int speed) {
	Enemy tmp;
	tmp.load_by_name(name);
	tmp.set_stat(level, health, damage, speed);
	tmp.set_animation(50, false);
	for ( int i = 0; i < n; i++ ) {
		vec.push_back(tmp);
	}
}

void Enemy::resolve_collide(Enemy& other) {
	//when this object collide with other, move this to the extension of the vec
	if ( (_position.x == other._position.x && _position.y == other._position.y) || is_dead())
		return;
	//the dx, dy version is integer-optimize, but it works horribly
	double ratio;
	//int dx = _position.x - other._position.x;
	//int dy = _position.y - other._position.y;
	if (abs(_position.x - other._position.x) > abs(_position.y - other._position.y)) {
		ratio = (double)((other.get_width() >> 1) + (get_width() >> 1) - abs(_position.x - other._position.x)) / (double)abs(_position.x - other._position.x);
		//dx *= ((other.get_width() >> 1) + (get_width() >> 1) - abs(_position.x - other._position.x)) / abs(_position.x - other._position.x);
	}
	else {
		ratio = (double)((other.get_height() >> 1) + (get_height() >> 1) - abs(_position.y - other._position.y)) / (double)abs(_position.y - other._position.y);
		//dy *= ((other.get_height() >> 1) + (get_height() >> 1) - abs(_position.y - other._position.y)) / abs(_position.y - other._position.y);
	}
	set_pos(_position.x + (int)((_position.x - other._position.x) * ratio), _position.y + (int)((_position.y - other._position.y) * ratio));
	//set_pos(_position.x + dx, _position.y + dy);
}