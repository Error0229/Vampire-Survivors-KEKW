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
	BOSS_XLFLOWER,
	DEATH,
	MUD,
	ECTO1,
	ECTO2,
	MUMMY,
	DULL1,
	DULL2,
	MEDUSA1,
	MEDUSA2,
	BUER,
	WITCH1,
	WITCH2,
	SKULL2,
	XLMEDUSA,
	SWARM_SHADERED,
	SWARM_SKULL2,
	BOSS_ECTO,
	BOSS_MUD,
	BOSS_BUER,
	BOSS_XLMEDUSA,
	BOSS_MEDUSA,
	BOSS_COUNT,
	BOSS_HAG,
	SWARM_MEDUSA
};

class Enemy : public VSObject {
public:
	Enemy();
	~Enemy();
	void show_skin(double factor = 1.0) override;
	void set_enable(bool enable = true);
	void set_spawn(CPoint pos, int move_animation_delay = 100, int death_animation_delay = 100);
	void set_scale(int player_lvl, int curse);
	void set_chest(bool can_evo, int chance0, int chance1);
	void set_spawn_pos(int count = 0, int amount = 0);
	void set_swarm(int swarm_type, int duraion, clock_t tick, int swarm_pos_i);
	void update_pos(CPoint, clock_t);
	bool is_dead();
	bool is_enable();
	int get_id();
	int get_xp_value();
	int get_power();
	int get_spawn_limit();
	int get_swarm_type();
	bool hurt(int damage); //this will return true if the enemy die from this damage, otherwise false
	bool is_collide_with(VSObject&, double overlap_bound = 1);
	bool is_collide_with(Enemy&, double overlap_bound = 0.5);
	static void load_template_enemies();
	static Enemy load_enemy(int id, char* name, int hp_max, int power, int mspeed, double kb, int kb_max, double res_f, bool res_k, bool res_d, double xp_value, bool hp_scale, int spawn_limit);
	static Enemy get_template_enemy(int id);
	static vector<Enemy> template_enemies;
	friend class Projectile;
	friend class EnemyFactory;
private:
	// stats
	clock_t _last_time_got_hit = -100000;
	vector <clock_t> _last_time_got_hit_by_projectile;
	int _alt_speed;
	int _id, _hp, _hp_max, _power, _mspeed, _kb_max, _spawn_limit;
	double _kb, _res_f, _xp_value;
	bool _res_k, _res_d, _hp_scale, _is_stun = 0;
	double _stun_speed;
	bool _is_enable;
	//chest thing
	bool _is_drop_chest, _chest_can_evo;
	int _chest_upgrade_chance_0, _chest_upgrade_chance_1;
	//swarm thing
	int _swarm_type, _swarm_pos_i;
	clock_t _swarm_duration, _swarm_start_time;
	VSObject _death_animation;
	VSObject _hit_animation;
};
