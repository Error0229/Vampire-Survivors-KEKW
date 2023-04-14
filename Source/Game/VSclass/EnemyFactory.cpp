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
	load_wave();
}

void EnemyFactory::load_wave()
{
	ifstream file("source/game/VSclass/stage_event_1.csv");
	string header, line;
	string 👀; // 👀: string for reading
	int cnt = 0;
	getline(file, header);
	memset(_wave, 0, sizeof(_wave));
	while(getline(file, line)){
		stringstream ss(line);
		// time_min
		getline(ss, 👀, ',');
		_wave[cnt].time_min = stoi(👀);
		// 3 enemy
		for(int i=0; i<3; i++){
			getline(ss, 👀, ',');
			_wave[cnt].type[i] = stoi(👀);
			getline(ss, 👀, ',');
			_wave[cnt].weight[i] = stod(👀);
		}
		// amount
		getline(ss, 👀, ',');
		_wave[cnt].amount = stoi(👀);
		// interval
		getline(ss, 👀, ',');
		_wave[cnt].interval_msec = stoi(👀);
		// 2 boss
		for(int i=0; i<2; i++){
			getline(ss, 👀, ',');
			_wave[cnt].boss_type[i] = stoi(👀);
			getline(ss, 👀, ',');
			_wave[cnt].chest_evo[i] = stoi(👀);
		}
		// 2 swarm
		for(int i=0; i<2; i++){
			getline(ss, 👀, ',');
			if(👀=="Flower Wall") // WIP
				_wave[cnt].swarm_type[i] = -1;
			else
				_wave[cnt].swarm_type[i] = stoi(👀);
			getline(ss, 👀, ',');
			_wave[cnt].swarm_amount[i] = stoi(👀);
			getline(ss, 👀, ',');
			_wave[cnt].swarm_delay_msec[i] = stoi(👀);
			getline(ss, 👀, ',');
			_wave[cnt].swarm_duration_sec[i] = stoi(👀);
			getline(ss, 👀, ',');
			_wave[cnt].swarm_repeat[i] = stoi(👀);
			getline(ss, 👀, ',');
			_wave[cnt].swarm_chance[i] = stoi(👀);
		}
		cnt++;
	}
}

void EnemyFactory::add_enemy(int type, CPoint player_pos, int player_lvl, int count, bool random_pos)
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
		👿->spawn(pos, 100, 100, 1);
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

void EnemyFactory::show_enemy(clock_t tick, CPoint player_pos, int player_lvl)
{
	static clock_t last_tick = -1;
	int min = tick/1000/60;
	if(tick - last_tick >= _wave[min].interval_msec)
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
	vector<double> weights = { _wave[min].weight[0], _wave[min].weight[1], _wave[min].weight[2]};
	if(get_number_all() < 300){
		for(int i=0; i<((300 - get_number_all() > _wave[min].amount)?(_wave[min].amount):(300-get_number_all())); i++)
			add_enemy(_wave[min].type[poll(weights)], player_pos, player_lvl);
	}
}

vector<Enemy*> EnemyFactory::live_enemy;
ObjPool<Enemy> EnemyFactory::_all_enemy;
bool EnemyFactory::_is_init = false;
vector<int> EnemyFactory::_number_type;