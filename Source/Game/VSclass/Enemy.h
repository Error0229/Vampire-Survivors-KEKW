#pragma once
enum enemy_names {
	BAT1,
	BAT2,
	BAT3,
	BAT4,
	BAT5,
	FLOWER1,
	FLOWER2,
	GHOST,
	ZOMBIE1,
	ZOMBIE2,
	ZOMBIE3,
	MUDNAN1,
	MUDNAN2,
	SKELETON2,
	SKELETON3,
	SKELETON5,
	SKELETON6,
	WEREWOLF,
	XLBAT,
	XLFLOWER,
	XLMANTIS,
	XLMUMMY,
	SWARM_BAT,
	SWARM_GHOST,
	BOSS_XLMANTIS,
	BOSS_XLBAT,
	BOSS_WEREWOLF,
	BOSS_XLMUMMY,
	BOSS_XLFLOWER
};

class Enemy : public VSObject{
public:
	Enemy(); 
	~Enemy();
	void show_skin(double factor = 1.0) override;
	void set_level(int);
	void set_enable(bool enable=true);
	void spawn(CPoint pos, int move_animation_delay = 100, int death_animation_delay = 100, int player_lvl=1);
	void update_pos(CPoint) override;
	bool is_dead();
	bool is_enable();
	int get_id();
	int get_xp_value();
	int get_power();
	bool hurt(int damage); //this will return true if the enemy die from this damage, otherwise false
	bool is_collide_with(VSObject&, double overlap_bound=1);
	bool is_collide_with(Enemy&, double overlap_bound=0.5);
	static void load_template_enemies();
	static Enemy load_enemy(int id, char* name, int hp_max, int power, int mspeed, double kb, int kb_max, double res_f, bool res_k, bool res_d, double xp_value, bool hp_scale);
	static Enemy get_template_enemy(int id);
	friend class Projectile;
private:
	// stats
	clock_t _last_time_got_hit;
	vector <clock_t> _last_time_got_hit_by_projectile;
	int _alt_speed;
	int _id, _hp, _hp_max, _power, _mspeed, _kb_max;
	double _kb, _res_f, _xp_value;
	bool _res_k, _res_d, _hp_scale, _is_stun = 0;
	double _stun_speed;
	int  _level;
	bool _is_enable; //this name is not good
	VSObject _death_animation;
	static vector<Enemy> template_enemies;
};
