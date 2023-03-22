#pragma once
#include "Enemy.h"
class Projectile : public VSObject
{
public:
	Projectile();
	Projectile(vector<char*> filename, COLORREF color = BLACK);
	~Projectile();
	void set_delay(int);
	void set_create_time(clock_t);
	bool operator < (const Projectile& rhs) const;
	Projectile& operator = (const Projectile& rhs) {
		this->_position = rhs._position;
		this->_skin = rhs._skin;
		return *this;
	}
	void show_skin(double factor = 1.0) override;
	void set_offset(CPoint);
	void WHIP_transition();
	void VAMPIRICA_transition();
	void collide_with_enemy(Enemy&);
	// int _type, _level, _max_level, _damage, _speed, _rarity,  _amount, _duration, _pierce, _cooldown, _proj_interval, _hitbox_delay, _knock_back, _pool_limit, _chance, _crit_multi, _block_by_wall;
	static void create_projectile(Projectile proj, CPoint position, CPoint target_pos, int delay, int type, int damage,int speed, int duration, int pierce, int proj_interval, int hitbox_delay, int knock_back, int pool_limit, int chance, int criti_multi, int block_by_wall, bool is_mirror);
	static void create_projectile(Projectile p);
	static void update_position();
	static void show();
	static deque<Projectile> all_proj;
protected:
	int _type, _level, _max_level, _damage, _speed, _rarity,
		_amount, _duration, _pierce, _proj_interval,
		_hitbox_delay, _knock_back, _pool_limit, _chance,
		_crit_multi, _block_by_wall;
	int _delay;
	double _area;
	bool _is_over;
	clock_t _create_time;
	CPoint _offset;
};
