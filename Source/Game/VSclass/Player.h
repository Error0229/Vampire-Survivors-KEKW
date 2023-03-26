#pragma once
class Player :public VSObject{
public:
	Player();
	~Player();
	void update_pos(CPoint) override;
	void show_skin(double factor = 1.0);

	//weapon & passive
	void acquire_weapon(Weapon&);
	void acquire_passive(Passive&);
	void update_passive_effect(Passive&);
	void level_up_passive(Passive&);
	void obtain_item(int);
	
	//getter
	int get_level();
	int get_pickup_range();
	int get_luck();
	int passive_count();
	int get_exp_percent();
	
	//things
	void hurt(int damage);
	void load_bleed();
	void pick_up_xp(int);
	bool apply_level_up();
	bool all_max();
	bool full_inv();
private:
	VSObject _bleed_animation;
	// vector<Weapon> _weapons;
	// vector<Passive> _passives;
	vector<int> stats;
	int _might, _armor, _max_health, _recovery, _cooldown, _area, 
		_proj_speed, _duration, _amount, _move_speed, _magnet,
		_luck, _growth, _greed, _revival, _curse;
	int _hp;
	int _exp, _max_exp;
	int _level;
	int _weapon_type, _weapon_level;
	int _passive_type, _passive_level;
	int _direction;
	int _range;
	int _speed;	//this have same name with VSObject::speed, might change in the future
	int _money;
	int _reroll;
	bool _is_hurt = false;
};
