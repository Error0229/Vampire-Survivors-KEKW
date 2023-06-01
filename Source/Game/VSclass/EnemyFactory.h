#pragma once


struct Wave_enemy{
	int time, amount, interval_msec;
	int type[3];
	double weight[3];
};
struct Wave_boss{
	int time;
	int type[2], chest_chance[2][2];
	bool drop_chest[2], can_evo[2];
};
struct Wave_swarm{
	int time, swarm_type[2];
	int type[2][2], amount[2], duration[2], repeat[2], chance[2], interval[2];
	int spawned_cnt[2];
};

class EnemyFactory {
public:
	EnemyFactory();
	~EnemyFactory();
	void init();
	int get_number_type(int);
	int get_number_all();
	void update(clock_t tick, CPoint player_pos, int player_lvl, int luck, int curse);
	vector<Enemy*> add_enemy(int type, CPoint player_pos, int count, int player_lvl, int curse);
	static vector<Enemy*> live_enemy;
private:
	static ObjPool<Enemy> _all_enemy;
	static vector<int> _number_type;
	vector<Wave_enemy> wave_enemy;
	vector<Wave_boss> wave_boss;
	vector<Wave_swarm> wave_swarm;

	//variable used in upadate_wave
	clock_t enemy_last_t, swarm_last_t[2];
	int enemy_cnt, swarm_cnt;

	void load_wave_enemy();
	void load_wave_boss();
	void load_wave_swarm();
	void update_enemy(clock_t tick, CPoint player_pos, int player_lvl, int curse);
	void update_boss(clock_t tick, CPoint player_pos, int player_lvl, int curse);
	void update_swarm(clock_t tick, CPoint player_pos, int player_lvl, int luck, int curse);
};