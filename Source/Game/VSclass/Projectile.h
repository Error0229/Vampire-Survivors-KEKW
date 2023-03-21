#pragma once
#include "Enemy.h"
class Projectile : public VSObject
{
public:
	Projectile();
	Projectile(vector<char*> filename, COLORREF color = BLACK);
	~Projectile();
	void set_order(int);
	bool operator < (const Projectile& rhs) const;
	Projectile& operator = (const Projectile& rhs) {
		this->_position = rhs._position;
		this->_skin = rhs._skin;
		this->_order = rhs._order;
		return *this;
	}

	void collide_with_enemy(Enemy& ,int damage, int duration, int weapon_kb);
	// int _type, _level, _max_level, _damage, _speed, _rarity,  _amount, _duration, _pierce, _cooldown, _proj_interval, _hitbox_delay, _knock_back, _pool_limit, _chance, _crit_multi, _block_by_wall;
	static void create_projectile(int player_x, int player_y, int type, int damage,int speed, int amount, int duration, int pierce, int proj_interval, int hitbox_delay, int knock_back, int pool_limit, int chance, int criti_multi, int block_by_wall);
	static void create_projectile(Projectile p);
	static int order;
protected:
	int _order;
	int _type, _level, _max_level, _damage, _speed, _rarity,
		_amount, _duration, _pierce, _proj_interval,
		_hitbox_delay, _knock_back, _pool_limit, _chance,
		_crit_multi, _block_by_wall;

	static vector<Projectile> _all_proj;
};

