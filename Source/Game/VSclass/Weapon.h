#pragma once 
#include "Projectile.h"
#include <memory>
class Weapon : public VSObject {
public:
	Weapon();
	Weapon(int, char*, vector<int>);
	~Weapon();
	void upgrade();
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
	int get_max_level();
	bool is_evo_weapon();
	bool can_evo();
	double get_kb();
	int get_pierce();
	void recalaulte_stat();
	void modify_base(string type, double effect);
	void update_weapon_stats(int might, int cooldown, int proj_speed, int duration, int amount, int area);
	static void update_all_weapon_stats(int might, int cooldown, int proj_speed, int duration, int amount, int area);
	string get_name();
	static void evolution(int type);
	static map <int, int> evolution_pair;
	static map <int, int> evolution_pair_reverse;
	static void attack();
	static void upgrade(int weapon);
	static void show();
	static deque<Weapon> all_weapon;
	static int weapon_count();
protected:
	Projectile _base_proj;
	unordered_map<string, any> _base_stats;
	unordered_map<string, int> _modifier;
	string _name;
	int _type, _level, _max_level, _speed, _rarity,   
		_amount, _duration, _pierce, _cooldown, _proj_interval,
		_hitbox_delay, _pool_limit, _chance, 
		_crit_multi, _block_by_wall, _evolution_type, _evolution_require;
	double _area, _damage, _knock_back;
	clock_t _last_time_attack = -1;
	vector<string> _level_up_msg;
};
