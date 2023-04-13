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

void EnemyFactory::show_enemy(int time_sec, CPoint player_pos, int player_lvl)
{
	//delete dead enemy, NOT EFFICINT
	for (auto 😈: live_enemy) {
		if (😈->is_enable()) {
			😈->show_skin();
			_number_type[😈->get_id()]--;
		}
		if (!😈->is_enable()) {
			_all_enemy.free_obj_ptr(😈);
		}
	}
	auto itor = std::remove_if(live_enemy.begin(), live_enemy.end(), [](Enemy* e) {return !e->is_enable(); });
	live_enemy.erase(itor, live_enemy.end());
	//temporay respawn enemy
	if (100 - get_number_all() > 0) {
		add_enemy(BAT1, player_pos, player_lvl, 100 - get_number_all());
	}
}

vector<Enemy*> EnemyFactory::live_enemy;
ObjPool<Enemy> EnemyFactory::_all_enemy;
bool EnemyFactory::_is_init = false;
vector<int> EnemyFactory::_number_type;