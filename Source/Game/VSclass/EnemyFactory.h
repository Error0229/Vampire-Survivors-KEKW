#pragma once

typedef struct Wave{
	// enemy
	int time_min, amount, interval_msec;
	int type[3];
	double weight[3];
	// boss
	int boss_type[2];
	bool drop_chest[2], spawned_boss[2];
	// swarm
	int swarm_type[2], swarm_amount[2], swarm_duration_sec[2], swarm_repeat[2], swarm_chance[2], swarm_delay_msec[2];
} Wave;

class EnemyFactory {
public:
	EnemyFactory();
	~EnemyFactory();
	void init();
	
	void add_enemy(int type, CPoint player_pos, int player_lvl, int count=1, bool random_pos=true, bool drop_chest=false);
	int get_number_type(int);
	int get_number_all();
	void update_enemy(clock_t tick, CPoint player_pos, int player_lvl);
	static vector<Enemy*> live_enemy;
private:
	static ObjPool<Enemy> _all_enemy;
	static bool _is_init;
	static vector<int> _number_type;
	Wave _wave[31];

	void load_wave();
};