#pragma once 
#include "Projectile.h"
#include <memory>
class Weapon : public VSObject {
public:
	Weapon();
	Weapon(int, char*, vector<char*>, vector<int>);
	Weapon(int, char*, vector<int>);
	~Weapon();
	// void show_proj();
	// void update_proj(CPoint, int, int, int);
	void upgrade();
	deque<Projectile>& get_all_proj();
	static void load_weapon_stats();
	static map<int, Weapon> _base_weapon; //name, stats
	int get_damage();
	int get_duration();
	int get_rarity();
	int get_type();
	bool is_max_level();
	int get_kb();
	int get_pierce();
	static map <int, int> evolution_pair;
	static void attack();
	static void show();
	static deque<Weapon> all_weapon;
protected:
	Projectile _base_proj;
	deque<Projectile> _proj_q;
	deque<Projectile> _unused_proj_q;
	string _name;
	int _type, _level, _max_level, _damage, _speed, _rarity,   
		_amount, _duration, _pierce, _cooldown, _proj_interval,
		_hitbox_delay, _knock_back, _pool_limit, _chance, 
		_crit_multi, _block_by_wall, _evolution_type;
	double _area;
	clock_t _last_time_attack = -1;
	vector<string> _level_up_msg;
};
