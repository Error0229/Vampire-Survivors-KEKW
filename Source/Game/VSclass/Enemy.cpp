#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Enemy.h"
#include "Pickup.h"
#include "VSUtil.h"
#include <fstream>
#include <sstream>
Enemy::Enemy()
{
	obj_type = ENEMY;
	_last_time_got_hit_by_projectile.resize(100, -1000000);
	_last_time_got_hit = -1000000;
}
Enemy::~Enemy() 
{
}

void Enemy::set_level(int level)
{
	this->_level = level;
}

void Enemy::set_enable(bool enable) 
{
	_is_enable = enable;
}

bool Enemy::is_dead()
{
	return (_hp <= 0);
}

bool Enemy::is_enable()
{
	return _is_enable;
}

int Enemy::get_xp_value()
{
	return (int)_xp_value; //will change
}

int Enemy::get_power()
{
	return _power;
}
int Enemy::get_id() {
	return _id;
}
int Enemy::get_spawn_limit() {
	return _spawn_limit;
}
void Enemy::show_skin(double factor)
{
	if ( !_is_enable )
		return;
	if ( !is_dead() ) {
		this->_skin.SetTopLeft(this->_position.x - ( get_width() >> 1 ) + player_dx, this->_position.y - ( get_height() >> 1 ) + player_dy);
		this->_skin.ShowBitmap(factor, _is_mirror);
	}
	else {
		if (_death_animation.is_animation_done() ) {
			_death_animation.unshow_skin();
			_is_enable = false;
		}
		else {
			_death_animation.set_pos(_position);
			_death_animation.show_skin(factor);
		}
	}
}
void Enemy::update_pos(CPoint pos) {
	if (_is_stun) {
		this->_speed = (int)_stun_speed;
		if (clock() - _last_time_got_hit > 240) { // set to 2x of wiki said (120ms) 
			_is_stun = false;
			_speed = _mspeed;
		}
	}
	else {
		this->_speed = _mspeed;
	}
	VSObject::update_pos(pos);
}

bool Enemy::hurt(int damage) 
{
	if (!is_dead()) {
		_hp -= damage;
		if (is_dead()) {
			unshow_skin();
			Xp::spawnXP(this->_position, static_cast<int>(_xp_value));
			if (_is_drop_chest) 
				Chest::spawnChest(this->_position);
			_death_animation.set_pos(get_pos());
			_death_animation.set_animation(100, true);
			_death_animation.set_is_mirror(_is_mirror);
			_death_animation.enable_animation();
			return true;
		}
	}
	return false;
}

bool Enemy::is_collide_with(VSObject& obj, double overlap_bound)
{
	if (is_dead() || (!_is_enable))
		return false;
	return is_overlapped(*this, obj, overlap_bound);
}

bool Enemy::is_collide_with(Enemy& obj, double overlap_bound)
{
	if (is_dead() || (!_is_enable) || obj.is_dead() || (!obj._is_enable))
		return false;
	return is_overlapped(*this, obj, overlap_bound);
}

void Enemy::spawn(CPoint pos, int move_animation_delay, int death_animation_delay, int player_lvl, bool drop_chest)
{
	set_animation(move_animation_delay, false);
	_death_animation.set_animation(death_animation_delay, true);
	_is_enable = true;
	_position = pos;
	_is_drop_chest = drop_chest;
	_hp = (_hp_scale) ? (_hp_max * player_lvl) : (_hp_max);
}

void Enemy::load_template_enemies()
{
	static bool is_loaded = false;
	if (is_loaded)
		return;
	is_loaded = true;
	ifstream file("source/game/VSclass/enemy_stats.csv");
	string header, line;
	string number, id, file_name, hp, power, mspeed, kb, kb_max, res_f, res_k, res_d, xp_value, hp_scale, spawn_limit;
	getline(file, header);
	while (getline(file, line)) {
		stringstream ss(line);
		getline(ss, number, ',');
		getline(ss, id, ',');
		getline(ss, file_name, ',');
		getline(ss, hp, ',');
		getline(ss, power, ',');
		getline(ss, mspeed, ',');
		getline(ss, kb, ',');
		getline(ss, kb_max, ',');
		getline(ss, res_f, ',');
		getline(ss, res_k, ',');
		getline(ss, res_d, ',');
		getline(ss, xp_value, ',');
		getline(ss, hp_scale, ',');
		getline(ss, spawn_limit, ',');
		template_enemies.push_back(load_enemy(stoi(number), (char*)file_name.c_str(), stoi(hp), stoi(power), stoi(mspeed), stod(kb), stoi(kb_max), stod(res_f), stoi(res_k), stoi(res_d), stod(xp_value), stoi(hp_scale), stoi(spawn_limit)));
	}
}

Enemy Enemy::load_enemy(int id, char* name, int health, int power, int mspeed, double kb, int kb_max, double res_f, bool res_k, bool res_d, double xp_value, bool hp_scale, int spawn_limit)
{
	Enemy enemy;
	char tmp[100];
	for (int i = 0; i < 5; i++) {
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, ".\\Resources\\enemy\\%s_i0%d.bmp", name, (i + 1));
		struct stat buffer;
		if (stat(tmp, &buffer) == 0) {
			//the file exist
			enemy.load_skin(tmp);
		}
		else {
			//the file doesnt exist
			//TRACE(_T("stop at %s\n"), tmp);
			break;
		}
	}
	for (int i = 0; i < 20; i++) {
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, ".\\Resources\\enemy\\%s_%d.bmp", name, i);
		struct stat buffer;
		if (stat(tmp, &buffer) == 0) {
			//the file exist
			enemy._death_animation.load_skin(tmp);
		}
		else {
			//the file doesnt exist
			//TRACE(_T("stop at %s\n"), tmp);
			break;
		}
	}
	enemy._type = id;
	enemy._id = id;
	enemy._hp_max = health;
	enemy._power = power;
	enemy._mspeed = mspeed;
	enemy._kb = kb;
	enemy._kb_max = kb_max;
	enemy._res_f = res_f;
	enemy._res_k = res_k;
	enemy._res_d = res_d;
	enemy._xp_value = xp_value;
	enemy._hp_scale = hp_scale;
	enemy._spawn_limit = spawn_limit;

	enemy._is_enable = false;
	enemy._is_mirror = false;
	enemy._is_drop_chest = false;
	enemy._position = CPoint(0, 0);
	enemy.set_type(id);
	
	enemy._speed = 50;
	return enemy;
}

Enemy Enemy::get_template_enemy(int id)
{
	return template_enemies[id];
}

vector<Enemy> Enemy::template_enemies;