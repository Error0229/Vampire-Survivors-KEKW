#pragma once

struct stat_struct {
	int type;
	string name_string, val_string;
};

class Player :public VSObject{
public:
	Player();
	Player(string);
	~Player();
	CPoint update_pos(CPoint, bool ensure);
	void show_skin(double factor = 1.0);
	static void init_player();
	void init_stats();
	//weapon & passive
	void acquire_weapon(Weapon&);
	void acquire_weapon(int type);
	void acquire_passive(Passive&);
	void acquire_passive(int type);
	void level_up_passive(Passive&);
	void level_up_passive(int type);
	void update_all_passive_effect();
	void obtain_item(int);
	void revive();
	//getter
	int get_level();
	int get_exp_percent();
	int get_move_speed();
	int get_magnet();
	int get_luck();
	int get_curse();
	int get_pickup_range();
	int get_duration();
	int get_revival();
	int get_greed();
	vector<stat_struct> get_stats_string();
	int get_hp_percent();
	bool is_hurt();
	
	//things
	void set_speed(double) override;
	void set_speed(int) override;
	bool have(int type);
	void hurt(int damage);
	void load_bleed();
	void pick_up_xp(int);
	bool apply_level_up();
	bool all_max();
	bool full_inv();
	void regen(double amount = 0);
	void increase_luck(int amount);
private:
	static map<string,Player> template_player;
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
	clock_t _last_time_got_hit = -1;
	double _recovery;
	int _hp;
	int _exp, _max_exp;
	int _level;
	int _direction;
	int _reroll;
	bool _is_hurt = false;
	int _weapon_type;
	string stat_to_string(int val, bool percent=true);
};
