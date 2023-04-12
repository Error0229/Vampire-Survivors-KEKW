#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Enemy.h"
#include "Pickup.h"
#include "VSUtil.h"
#include "ObjPool.h"
#include "EnemyFactory.h"

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
}

void EnemyFactory::add_enemy(int type, int count, bool random_pos)
{
	
	Enemy* 👿;
	CPoint pos;
	for (int i = 0; i < count; i++) {
		👿 = _all_enemy.get_obj_ptr(type);
		if (random_pos) {
			pos = (0, i*10);
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

void EnemyFactory::update(int time_sec)
{
	//delete dead enemy, NOT EFFICINT
	for (auto 😈 : live_enemy) {
		if (!😈->is_enable()) {
			live_enemy.erase(find(live_enemy.begin(), live_enemy.end(), 😈));
			_all_enemy.free_obj_ptr(😈);
		}
	}

	//temporay respawn enemy
	if (100 - get_number_all()) {
		add_enemy(BAT1, 100 - get_number_all());
	}
}

vector<Enemy*> EnemyFactory::live_enemy;
ObjPool<Enemy> EnemyFactory::_all_enemy;
bool EnemyFactory::_is_init = false;
vector<int> EnemyFactory::_number_type;