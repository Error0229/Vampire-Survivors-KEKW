#pragma once 
#include "Projectile.h"
#include <memory>
class Weapon : public VSObject {
public:
	Weapon();
	Weapon(int);
	Weapon(int, char*, vector<int>);
	~Weapon();
	// void show_proj();
	// void update_proj(CPoint, int, int, int);
	void upgrade();
	deque<Projectile>& get_all_proj();
	static void load_weapon_stats();
	static map<int, Weapon> _base_weapon; //name, stats
	double get_damage();
	int get_duration();
	int get_rarity();
	int get_type();
	int get_level();
	string get_level_up_msg(bool is_new = false);
	int get_evo_passive();
	bool is_max_level();
	bool is_evo_weapon();
	bool can_evo();
	double get_kb();
	int get_pierce();
	string get_name();
	static void evolution(int type);
	static map <int, int> evolution_pair;
	static map <int, int> evolution_pair_reverse;
	static void attack();
	static void show();
	static deque<Weapon> all_weapon;
	static int weapon_count();
protected:
	Projectile _base_proj;
	deque<Projectile> _proj_q;
	deque<Projectile> _unused_proj_q;
	string _name;
	int _type, _level, _max_level, _speed, _rarity,   
		_amount, _duration, _pierce, _cooldown, _proj_interval,
		_hitbox_delay, _pool_limit, _chance, 
		_crit_multi, _block_by_wall, _evolution_type, _evolution_require;
	double _area, _damage, _knock_back;
	clock_t _last_time_attack = -1;
	vector<string> _level_up_msg;
};
