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
	int time;
	int type[2], amount[2], duration_sec[2], repeat[2], chance[2], delay_msec[2];
};

class EnemyFactory {
public:
	EnemyFactory();
	~EnemyFactory();
	void init();
	
	void add_enemy(int type, CPoint player_pos, int player_lvl, int count=1, bool random_pos=true, bool drop_chest=false, bool can_evo=false, int chest_chance0=0, int chest_chance1=0);
	int get_number_type(int);
	int get_number_all();
	void update_enemy(clock_t tick, CPoint player_pos, int player_lvl);
	static vector<Enemy*> live_enemy;
private:
	int wave_boss_cnt = 0, wave_swarm_cnt = 0;
	static ObjPool<Enemy> _all_enemy;
	static bool _is_init;
	static vector<int> _number_type;
	vector<Wave_enemy> wave_enemy;
	vector<Wave_boss> wave_boss;
	vector<Wave_swarm> wave_swarm;

	void load_wave_enemy();
	void load_wave_boss();
	void load_wave_swarm();
};