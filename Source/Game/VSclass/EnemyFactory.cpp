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
	vector<Enemy*> 😈_🛖_😈;
	for (int i = 0; i < count; i++) {
		😈 = _all_enemy.get_obj_ptr(type);
		😈->set_spawn(player_pos, 100, 100);
		😈->set_scale(player_lvl, curse);
		live_enemy.push_back(😈);
		😈_🛖_😈.push_back(😈);
	}
	_number_type[type] += count;
	return 😈_🛖_😈;
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
	int min = tick/1000/60;
	int interval_msec = wave_enemy[min].interval_msec * 100 / curse;
	int amount = wave_enemy[min].amount * curse / 100;
	if(tick - enemy_last_t >= interval_msec){
		enemy_last_t = tick;
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
	// static int cnt = 0;
	if(enemy_cnt >= (int)wave_boss.size() || wave_boss[enemy_cnt].time != min)
		return;
	Enemy* 😈;
	for (int i = 0; i < 2; i++) {
		if (wave_boss[enemy_cnt].type[i] != -1) {
			😈 = add_enemy(wave_boss[enemy_cnt].type[i], player_pos, 1, player_lvl, curse)[0];
			😈->set_chest(wave_boss[enemy_cnt].can_evo[i], wave_boss[enemy_cnt].chest_chance[i][0], wave_boss[enemy_cnt].chest_chance[i][1]);
			😈->set_spawn_pos();
			is_spawn = true;
		}
	}
	if (is_spawn)
		enemy_cnt++;
}

void EnemyFactory::update_swarm(clock_t tick, CPoint player_pos, int player_lvl, int luck, int curse)
{
	static vector<double> swarm_pos_weights(20, 1);
	int min = tick / 1000 / 60;
	if (swarm_cnt >= (int)wave_swarm.size() || wave_swarm[swarm_cnt].time != min)
		return;
	Enemy* 😈;
	vector<Enemy*> 😈😈😈;
	vector<double> weights(2, 0);
	for (int i = 0; i < 2; i++) {
		if (tick - swarm_last_t[i] >= wave_swarm[swarm_cnt].interval[i]*100/curse && wave_swarm[swarm_cnt].spawned_cnt[i] < wave_swarm[swarm_cnt].repeat[i]) {
			weights[1] = (wave_swarm[swarm_cnt].chance[i]!=-1)?(wave_swarm[swarm_cnt].chance[i] * 100 / luck):100;
			weights[0] = 100 - weights[1];
			int swarm_pos_i = poll(swarm_pos_weights);
			if(poll(weights, true)){
				if (wave_swarm[swarm_cnt].type[i][1] != -1) {
					//two enemy type
					int amount = wave_swarm[swarm_cnt].amount[i] * curse / 100;
					for (int j = 0; j < amount; j++) {
						😈 = add_enemy(wave_swarm[swarm_cnt].type[i][j & 1], player_pos, 1, player_lvl, curse)[0];
						😈->set_scale(player_lvl, curse);
						😈->set_swarm(wave_swarm[swarm_cnt].swarm_type, wave_swarm[swarm_cnt].duration[i], tick, swarm_pos_i);
						😈->set_spawn_pos(j, amount);
					}
				}
				else {
					//one enemy type
					😈😈😈 = add_enemy(wave_swarm[swarm_cnt].type[i][0], player_pos, wave_swarm[swarm_cnt].amount[i] * curse / 100, player_lvl, curse);
					for (auto 😈 : 😈😈😈) {
						😈->set_scale(player_lvl, curse);
						😈->set_swarm(wave_swarm[swarm_cnt].swarm_type, wave_swarm[swarm_cnt].duration[i], tick, swarm_pos_i);
						😈->set_spawn_pos();
					}
				}
			}
			swarm_last_t[i] = tick;
			wave_swarm[swarm_cnt].spawned_cnt[i]++;
		}
	}
	if (wave_swarm[swarm_cnt].spawned_cnt[0] >= wave_swarm[swarm_cnt].repeat[0] && wave_swarm[swarm_cnt].spawned_cnt[1] >= wave_swarm[swarm_cnt].repeat[1]) {
		swarm_last_t[0] = 0;
		swarm_last_t[1] = 0;
		swarm_cnt++;
	}
}
void EnemyFactory::reset() {
	for (auto 😈 : live_enemy) {
		_all_enemy.free_obj_ptr(😈);
	}
	live_enemy.clear();
	//reset wave_swarm cnt
	for(auto &i: wave_swarm){
		i.spawned_cnt[0] = 0;
		i.spawned_cnt[1] = 0;
	}
	
	//reset all the static cnt
	enemy_last_t = -1;
	swarm_last_t[0] = -1;
	swarm_last_t[1] = -1;
	enemy_cnt = 0;
	swarm_cnt = 0;
}

vector<Enemy*> EnemyFactory::live_enemy;
ObjPool<Enemy> EnemyFactory::_all_enemy;
bool EnemyFactory::_is_init = false;
vector<int> EnemyFactory::_number_type;