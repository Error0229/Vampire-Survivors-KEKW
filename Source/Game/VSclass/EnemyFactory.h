#pragma once

class EnemyFactory {
public:
	EnemyFactory();
	~EnemyFactory();
	static void init();
	
	void add_enemy(int type, CPoint player_pos, int player_lvl, int count=1, bool random_pos=true);
	int get_number_type(int);
	int get_number_all();
	void show_enemy(int time_sec, CPoint player_pos, int player_lvl);
	static vector<Enemy*> live_enemy;
private:
	static ObjPool<Enemy> _all_enemy;
	static bool _is_init;
	static vector<int> _number_type;

};