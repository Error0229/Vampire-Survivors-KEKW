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
		getline(ss, 👀, ',');
		wave_enemy[cnt].time = stoi(👀);
		for(int i=0; i<3; i++){
			getline(ss, 👀, ',');
			wave_enemy[cnt].type[i] = stoi(👀);
			getline(ss, 👀, ',');
			wave_enemy[cnt].weight[i] = stod(👀);
		}
		getline(ss, 👀, ',');
		wave_enemy[cnt].amount = stoi(👀);
		getline(ss, 👀, ',');
		wave_enemy[cnt].interval_msec = stoi(👀);
		
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
	ifstream file("source/game/VSclass/stage1_wave_swarm.csv");
	string header, line;
	string 👀; // 👀: string for reading
	int cnt = 0;
	getline(file, header);
	while(getline(file, line)){
		stringstream ss(line);
		wave_swarm.push_back(Wave_swarm());
		getline(ss, 👀, ',');
		wave_swarm[cnt].time = stoi(👀);
		getline(ss, 👀, ',');
		wave_swarm[cnt].swarm_type = stoi(👀);
		for(int i=0; i<2; i++){
			getline(ss, 👀, ',');
			wave_swarm[cnt].type[i][0] = stoi(👀);
			getline(ss, 👀, ',');
			wave_swarm[cnt].type[i][1] = stoi(👀);
			getline(ss, 👀, ',');
			wave_swarm[cnt].amount[i] = stoi(👀);
			getline(ss, 👀, ',');
			wave_swarm[cnt].interval[i] = stoi(👀);
			getline(ss, 👀, ',');
			wave_swarm[cnt].duration[i] = stoi(👀);
			getline(ss, 👀, ',');
			wave_swarm[cnt].repeat[i] = stoi(👀);
			getline(ss, 👀, ',');
			wave_swarm[cnt].chance[i] = stoi(👀);
		}
		wave_swarm[cnt].spawned_cnt[0] = 0;
		wave_swarm[cnt].spawned_cnt[1] = 0;
		cnt++;
	}
}

int EnemyFactory::get_number_type(int type)
{
	return _number_type[type];
}
int EnemyFactory::get_number_all()
{
	return live_enemy.size();
}

vector<Enemy*> EnemyFactory::add_enemy(int type, CPoint player_pos, int count, int player_lvl, int curse)
{
	Enemy* 😈;
	vector<Enemy*> 😈😈😈;
	for (int i = 0; i < count; i++) {
		😈 = _all_enemy.get_obj_ptr(type);
		😈->set_spawn(player_pos, 100, 100);
		😈->set_scale(player_lvl, curse);
		live_enemy.push_back(😈);
		😈😈😈.push_back(😈);
	}
	_number_type[type] += count;
	return 😈😈😈;
}

void EnemyFactory::update(clock_t tick, CPoint player_pos, int player_lvl, int luck, int curse)
{
	//delete disable enemy
	for (auto 😈: live_enemy) {
		if (😈->is_enable())
			_number_type[😈->get_id()]--;
		else
			_all_enemy.free_obj_ptr(😈);
	}
	auto itor = std::remove_if(live_enemy.begin(), live_enemy.end(), [](Enemy* e) {return !e->is_enable(); });
	live_enemy.erase(itor, live_enemy.end());
	
	update_enemy(tick, player_pos, player_lvl, curse);
	update_boss(tick, player_pos, player_lvl, curse);
	update_swarm(tick, player_pos, player_lvl, luck, curse);
}
void EnemyFactory::update_enemy(clock_t tick, CPoint player_pos, int player_lvl, int curse)
{
	static clock_t last_tick = -1;
	int min = tick/1000/60;
	int interval_msec = wave_enemy[min].interval_msec * 100 / curse;
	int amount = wave_enemy[min].amount * curse / 100;
	if(tick - last_tick >= interval_msec){
		last_tick = tick;
	}else{
		return;
	}
	Enemy* 😈;
	if(get_number_all() < amount){
		// if alive enemy < minimum amout, spawn it till minimum amount
		vector<double> weights = { wave_enemy[min].weight[0], wave_enemy[min].weight[1], wave_enemy[min].weight[2]};
		for(int i=0; i<amount - get_number_all(); i++){
			😈 = add_enemy(wave_enemy[min].type[poll(weights)], player_pos, 1, player_lvl, curse)[0];
			😈->set_scale(player_lvl, curse);
			😈->set_spawn_pos();
		}
	}
	else if(get_number_all() < 300){
		// if alive enemy >= minimum amout, spawn each type of enemies once 
		for(int i=0; i<3; i++){
			if(wave_enemy[min].weight[i]){
				😈 = add_enemy(wave_enemy[min].type[i], player_pos, 1, player_lvl, curse)[0];
				😈->set_scale(player_lvl, curse);
				😈->set_spawn_pos();
			}
		}
	}
}
void EnemyFactory::update_boss(clock_t tick, CPoint player_pos, int player_lvl, int curse)
{
	bool is_spawn = false;
	int min = tick / 1000 / 60;
	static int cnt = 0;
	if(cnt >= (int)wave_boss.size() || wave_boss[cnt].time != min)
		return;
	Enemy* 😈;
	for (int i = 0; i < 2; i++) {
		if (wave_boss[cnt].type[i] != -1) {
			😈 = add_enemy(wave_boss[cnt].type[i], player_pos, 1, player_lvl, curse)[0];
			😈->set_chest(wave_boss[cnt].can_evo[i], wave_boss[cnt].chest_chance[i][0], wave_boss[cnt].chest_chance[i][1]);
			😈->set_spawn_pos();
			// add_enemy(wave_boss[cnt].type[i], player_pos, player_lvl, 1, true, wave_boss[cnt].drop_chest[i], wave_boss[cnt].can_evo[i], wave_boss[cnt].chest_chance[i][0], wave_boss[cnt].chest_chance[i][1]);
			is_spawn = true;
		}
	}
	if (is_spawn)
		cnt++;
}

void EnemyFactory::update_swarm(clock_t tick, CPoint player_pos, int player_lvl, int luck, int curse)
{
	static clock_t last_tick[2] = { -69000, -69000 };
	static int cnt = 0;
	int min = tick / 1000 / 60;
	if (cnt >= (int)wave_swarm.size() || wave_swarm[cnt].time != min)
		return;
	Enemy* 😈;
	vector<Enemy*> 😈😈😈;
	for (int i = 0; i < 2; i++) {
		if (tick - last_tick[i] >= wave_swarm[cnt].interval[i] && wave_swarm[cnt].spawned_cnt[i] < wave_swarm[cnt].repeat[i]) {
			if (wave_swarm[cnt].type[i][1] != -1) {
				//two enemy type
				for (int j = 0; j < wave_swarm[cnt].amount[i]; j++) {
					😈 = add_enemy(wave_swarm[cnt].type[i][j & 2], player_pos, 1, player_lvl, curse)[0];
					😈->set_scale(player_lvl, curse);
					😈->set_swarm(wave_swarm[cnt].swarm_type, wave_swarm[cnt].duration[i], tick);
					😈->set_spawn_pos();
				}
			}
			else {
				//one enemy type
				😈😈😈 = add_enemy(wave_swarm[cnt].type[i][0], player_pos, wave_swarm[cnt].amount[i], player_lvl, curse);
				for (auto 😈 : 😈😈😈) {
					😈->set_scale(player_lvl, curse);
					😈->set_swarm(wave_swarm[cnt].swarm_type, wave_swarm[cnt].duration[i], tick);
					😈->set_spawn_pos();
				}
			}
			last_tick[i] = tick;
			wave_swarm[cnt].spawned_cnt[i]++;
		}
	}
	if (wave_swarm[cnt].spawned_cnt[0] >= wave_swarm[cnt].repeat[0] && wave_swarm[cnt].spawned_cnt[1] >= wave_swarm[cnt].repeat[1]) {
		cnt++;
	}
}

vector<Enemy*> EnemyFactory::live_enemy;
ObjPool<Enemy> EnemyFactory::_all_enemy;
bool EnemyFactory::_is_init = false;
vector<int> EnemyFactory::_number_type;