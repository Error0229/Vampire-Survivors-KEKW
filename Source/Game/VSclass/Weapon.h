#pragma once 
class Weapon : public VSObject {
public:
	Weapon();
	Weapon(int, char*, vector<char*>&, vector<int>);
	Weapon(int);
	~Weapon();
	void show_proj();
	void update_proj();
	static void load_weapon_stats();
protected:
	VSObject* _base_proj;
	set<VSObject*> _proj_set;
	static map<string, vector <int>> _weapon_stats; //name, stats
	string _name;
	int _type, _level, _max_level, _damage, _speed, _area, _rarity,   
		_amount, _duration, _pierce, _cooldown, _proj_interval,
		_hitbox_delay, _knock_back, _pool_limit, _chance, 
		_crit_multi, _block_by_wall;  
};
