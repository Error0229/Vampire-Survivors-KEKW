#pragma once
class Player :public VSObject{
public:
	Player();
	~Player();
	void update_pos(CPoint) override;
	void show_skin(double factor = 1.0);

	//weapon & passive
	void acquire_weapon(Weapon&);
	void acquire_weapon(int type);
	void acquire_passive(Passive&);
	void acquire_passive(int type);
	void level_up_passive(Passive&);
	void level_up_passive(int type);
	void update_all_passive_effect();
	void obtain_item(int);
	
	//getter
	int get_level();
	int get_pickup_range();
	int get_luck();
	int passive_count();
	
	//things
	void set_speed(double) override;
	void set_speed(int) override;
	bool have(int type);
	void hurt(int damage);
	void load_bleed();
	bool pick_up_xp(int);
	bool apply_level_up();
	bool all_max();
	bool full_inv();
private:
	VSObject _bleed_animation;
	// vector<Weapon> _weapons;
	// vector<Passive> _passives;
	vector<int> stats;
	unordered_map <string, any> _base_stats;
	int _armor, _max_health, _revival, _magnet, _amount, _base_max_health, _base_magnet, _base_speed;
	int _coef_might, _coef_max_health, _coef_magnet, 
		_coef_cooldown, _coef_area, _coef_proj_speed, 
		_coef_duration, _coef_move_speed, _coef_luck, 
		_coef_growth, _coef_curse, _coef_greed; // coefficients

	double _recovery;
	int _hp;
	int _exp, _max_exp;
	int _level;
	int _direction;
	int _money;
	int _reroll;
	bool _is_hurt = false;
};
