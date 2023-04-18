#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Enemy.h"
#include "Pickup.h"
#include "VSUtil.h"
#include "ObjPool.h"
#include "EnemyFactory.h"
#include <fstream>
#include <sstream>
#include <string>

EnemyFactory::EnemyFactory()
{
}

EnemyFactory::~EnemyFactory()
{
}

void EnemyFactory::init()
{
	VS_ASSERT(!_is_init, "EnemyFactory::init() is called more than once");
	_is_init = true;
	Enemy::load_template_enemies();
	Enemy 😈;
	for (int i = 0; i < (int)Enemy::template_enemies.size(); i++) {
		😈 = Enemy::get_template_enemy(i);
		_all_enemy.add_obj(😈, 😈.get_spawn_limit());
		_number_type.push_back(0);
	}
	load_wave_enemy();
	load_wave_boss();
	load_wave_swarm();
}

void EnemyFactory::load_wave_enemy()
{
	ifstream file("source/game/VSclass/stage1_wave_enemy.csv");
	string header, line;
	string 👀; // 👀: string for reading
	int cnt = 0;
	getline(file, header);
	while(getline(file, line)){
		stringstream ss(line);
		wave_enemy.push_back(Wave_enemy());
		// time_min
		getline(ss, 👀, ',');
		wave_enemy[cnt].time = stoi(👀);
		// 3 enemy
		for(int i=0; i<3; i++){
			getline(ss, 👀, ',');
			wave_enemy[cnt].type[i] = stoi(👀);
			getline(ss, 👀, ',');
			wave_enemy[cnt].weight[i] = stod(👀);
		}
		// amount
		getline(ss, 👀, ',');
		wave_enemy[cnt].amount = stoi(👀);
		// interval
		getline(ss, 👀, ',');
		wave_enemy[cnt].interval_msec = stoi(👀);
		
		// 2 swarm
		// for(int i=0; i<2; i++){
		// 	getline(ss, 👀, ',');
		// 	if(👀=="Flower Wall") // WIP
		// 		_wave[cnt].swarm_type[i] = -1;
		// 	else
		// 		_wave[cnt].swarm_type[i] = stoi(👀);
		// 	getline(ss, 👀, ',');
		// 	_wave[cnt].swarm_amount[i] = stoi(👀);
		// 	getline(ss, 👀, ',');
		// 	_wave[cnt].swarm_delay_msec[i] = stoi(👀);
		// 	getline(ss, 👀, ',');
		// 	_wave[cnt].swarm_duration_sec[i] = stoi(👀);
		// 	getline(ss, 👀, ',');
		// 	_wave[cnt].swarm_repeat[i] = stoi(👀);
		// 	getline(ss, 👀, ',');
		// 	_wave[cnt].swarm_chance[i] = stoi(👀);
		// }
		cnt++;
	}
}
void EnemyFactory::load_wave_boss()
{
	ifstream file("source/game/VSclass/stage1_wave_boss.csv");
	string header, line;
	string 👀; // 👀: string for reading
	int cnt = 0;
	getline(file, header);
	while(getline(file, line)){
		stringstream ss(line);
		wave_boss.push_back(Wave_boss());
		getline(ss, 👀, ',');
		wave_boss[cnt].time = stoi(👀);
		for(int i=0; i<2; i++){
			getline(ss, 👀, ',');
			wave_boss[cnt].type[i] = stoi(👀);
			getline(ss, 👀, ',');
			wave_boss[cnt].drop_chest[i] = stoi(👀);
			getline(ss, 👀, ',');
			wave_boss[cnt].can_evo[i] = stoi(👀);
			getline(ss, 👀, ',');
			wave_boss[cnt].chest_chance[i][0] = stoi(👀);
			getline(ss, 👀, ',');
			wave_boss[cnt].chest_chance[i][1] = stoi(👀);
		}
		cnt++;
	}
}

void EnemyFactory::load_wave_swarm()
{

}

void EnemyFactory::add_enemy(int type, CPoint player_pos, int player_lvl, int count, bool random_pos, bool drop_chest, bool can_evo, int chest_chance0, int chest_chance1)
{
	static int pos_loop_cnt = 0;
	Enemy* 👿;
	CPoint pos;
	vector<CPoint> pos_offset = { CPoint(-424,  320),CPoint(-424,  240),CPoint(-424,  160),CPoint(-424,   80),CPoint(-424,    0),CPoint(-424,  -80),CPoint(-424, -160),CPoint(-424, -240),CPoint(-424, -320),CPoint(-318, -320),CPoint(-212, -320),CPoint(-106, -320),CPoint(0, -320),CPoint(106, -320),CPoint(212, -320),CPoint(318, -320),CPoint(424, -320),CPoint(424, -240),CPoint(424, -160),CPoint(424,  -80),CPoint(424,    0),CPoint(424,   80),CPoint(424,  160),CPoint(424,  240),CPoint(424,  320),CPoint(318,  320),CPoint(212,  320),CPoint(106,  320),CPoint(0,  320),CPoint(-106,  320),CPoint(-212,  320),CPoint(-318,  320)};
	for (int i = 0; i < count; i++) {
		👿 = _all_enemy.get_obj_ptr(type);
		if (random_pos) {
			pos = player_pos + pos_offset[pos_loop_cnt];
			if (++pos_loop_cnt == 32)
				pos_loop_cnt = 0;
		}
		👿->spawn(pos, 100, 100, player_lvl);
		if(drop_chest)
			👿->set_chest(can_evo, chest_chance0, chest_chance1);
		live_enemy.push_back(👿);
	}
	_number_type[type] += count;
}

int EnemyFactory::get_number_type(int type)
{
	return _number_type[type];
}
int EnemyFactory::get_number_all()
{
	return live_enemy.size();
}

void EnemyFactory::update_enemy(clock_t tick, CPoint player_pos, int player_lvl)
{
	static clock_t last_tick = -1;
	int min = tick/1000/60;
	if(tick - last_tick >= wave_enemy[min].interval_msec)
		last_tick = tick;
	else
		return;
	//delete disable enemy
	for (auto 😈: live_enemy) {
		if (😈->is_enable())
			_number_type[😈->get_id()]--;
		else
			_all_enemy.free_obj_ptr(😈);
	}
	auto itor = std::remove_if(live_enemy.begin(), live_enemy.end(), [](Enemy* e) {return !e->is_enable(); });
	live_enemy.erase(itor, live_enemy.end());
	
	// spawn enemy
	vector<double> weights = { wave_enemy[min].weight[0], wave_enemy[min].weight[1], wave_enemy[min].weight[2]};
	if(get_number_all() < 300){
		for(int i=0; i<((300 - get_number_all() > wave_enemy[min].amount)?(wave_enemy[min].amount):(300-get_number_all())); i++)
			add_enemy(wave_enemy[min].type[poll(weights)], player_pos, player_lvl, 1, true);
	}

	//spawn boss
	bool is_spawn_boss = false;
	for(int i=0; i<2; i++){
		if(wave_boss_cnt < (int)wave_boss.size() && wave_boss[wave_boss_cnt].time == min && wave_boss[wave_boss_cnt].type[i] != -1){
			add_enemy(wave_boss[wave_boss_cnt].type[i], player_pos, player_lvl, 1, true, wave_boss[wave_boss_cnt].drop_chest[i], wave_boss[wave_boss_cnt].can_evo[i], wave_boss[wave_boss_cnt].chest_chance[i][0], wave_boss[wave_boss_cnt].chest_chance[i][1]);
			is_spawn_boss = true;
		}
	}
	if(is_spawn_boss)
		wave_boss_cnt ++;
}

vector<Enemy*> EnemyFactory::live_enemy;
ObjPool<Enemy> EnemyFactory::_all_enemy;
bool EnemyFactory::_is_init = false;
vector<int> EnemyFactory::_number_type;