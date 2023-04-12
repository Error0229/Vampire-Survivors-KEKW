#pragma once

class EnemyFactory {
public:
	EnemyFactory();
	~EnemyFactory();
	static void init();
	
	void add_enemy(int type, int count=1, bool random_pos=true);
	int get_number_type(int);
	int get_number_all();
	void update(int time_sec);
	static vector<Enemy*> live_enemy;
private:
	static ObjPool<Enemy> _all_enemy;
	static bool _is_init;
	static vector<int> _number_type;

};