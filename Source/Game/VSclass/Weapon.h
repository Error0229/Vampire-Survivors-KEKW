#pragma once 
class Weapon : public VSObject {
public:
	Weapon();
	Weapon(int, vector<char*>&, vector<char*>&, vector<int>);
	~Weapon();
	void show_proj();
	void update_proj();
protected:
	VSObject* _base_proj;
	set<VSObject*> _proj_set;
	int _type, _level, _max_level, _damage, _speed, _area, _rarity,   
		_amount, _duration, _pierce, _cooldown, _proj_interval,
		_hitbox_delay, _knock_back, _pool_limit, _chance, 
		_crit_multi, _block_by_wall;  
};
