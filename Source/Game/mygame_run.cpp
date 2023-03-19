#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <random>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include "mygame.h"
#include "VSclass/VS.h"

using namespace game_framework;

enum gamerun_status {
	PLAYING,
	LEVEL_UP,
	OPEN_CHEST
};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}


void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	Weapon::load_weapon_stats();
	Pickup::load_template_pickup();
	Enemy::load_template_enemies();

	_gamerun_status = PLAYING;
	_next_status = PLAYING;

	player.load_skin({ "resources/character/Dog_01.bmp", "resources/character/Dog_02.bmp" ,"resources/character/Dog_03.bmp" ,"resources/character/Dog_04.bmp" ,"resources/character/Dog_05.bmp" });
	player.set_pos(0, 0);
	player.set_speed(300);
	player.set_default_direct(RIGHT);
	player.set_animation(150, false);
	player.load_bleed();
	player.acquire_weapon(Weapon::_base_weapon[0]);
	player.acquire_passive(Passive(0));
	map.load_map({ "resources/map/dummy1.bmp" });
	map.set_pos(0, 0);
	QT = QuadTree(-Player::player_dx, -Player::player_dy, 800, 600, 5, 10, 0);
	QT.clear();

	for (int i = 0; i < 100; i++) {
		enemy.push_back(Enemy::get_template_enemy(GHOST));
		xp.push_back(Pickup::get_template_pickup(XP));
	}
	for ( int i = 0; i < (int)enemy.size(); i++ ) {
		enemy[i].spawn(CPoint(-300 + 30 * i/10, -400 + 40 * i%10));
	}

	event_background.load_skin("resources/ui/event_background.bmp", BLACK);
	level_up_button[0].load_skin("resources/ui/event_button.bmp", BLACK);
	level_up_button[1].load_skin("resources/ui/event_button.bmp", BLACK);
	level_up_button[2].load_skin("resources/ui/event_button.bmp", BLACK);
	level_up_button[3].load_skin("resources/ui/event_button.bmp", BLACK);
	level_up_choice[0] = -1;
	level_up_choice[1] = -1;
	level_up_choice[2] = -1;
	level_up_choice[3] = -1;

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// A kill all enemies
	// B pick up 100 xp, but will not check level up
	switch (nChar) {
	case('A'):
		player.level_up_passive(0);
		for (int i = 0; i < (int)enemy.size();i++) {
			if (enemy[i].hurt(1000000)) {
				xp[i].spawn_xp(enemy[i].get_pos(), enemy[i].get_xp_value());
			}
		}
		break;
	case('B'):
		player.pick_up_xp(100);
		break;
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	switch (_gamerun_status) {
	case(PLAYING):
		break;
	case(LEVEL_UP):
		for (int i = 0; i < 4; i++) {
			if (level_up_button[i].is_hover(point)) {
				level_up_choice[0] = -1;
				level_up_choice[1] = -1;
				level_up_choice[2] = -1;
				level_up_choice[3] = -1;
				if (player.apply_level_up())
					_next_status = LEVEL_UP;
				else
					_next_status = PLAYING;
				break;
			}
		}
		break;
	case(OPEN_CHEST):
		break;
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	// mouse_pos = CPoint(point.x - VSObject::player_dx, point.y - VSObject::player_dy);
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

int CGameStateRun::draw_level_up(bool has_only)
{
	// 0~20: passive
	// 21~52: Weapon
	// 53~83: evo
	if (!has_only && player.weapon_count() >= 6 && player.passive_count() >= 6)
		return draw_level_up(true);
	vector<double> weights(53, 0);
	bool can_draw;
	for (int i = 0; i < 21; i++) {
		can_draw = true;
		for (auto& choice : level_up_choice) {
			if (i == choice) {
				can_draw = false;
				break;
			}
		}
		if (!can_draw)
			continue;
		if (has_only) {
			if(i<21 && (player.find_passive(i)!=nullptr) && (!player.find_passive(i)->is_max_level()))
				weights[i] = Passive(i).get_rarity();
			else if(i>20 && (player.find_weapon(i-21) != nullptr) && (!player.find_weapon(i-21)->is_max_level()))
				weights[i] = Weapon::_base_weapon[i - 21].get_rarity();
		}
		else {
			if (i < 21 && (player.find_passive(i) == nullptr) && (player.passive_count() < 6))
				weights[i] = Passive(i).get_rarity();
			else if(i > 20 && (player.find_weapon(i - 21) == nullptr) && (player.weapon_count() < 6))
				weights[i] = Weapon::_base_weapon[i - 21].get_rarity();
		}
		//TRACE(_T("%d:%lf\n"), i, weights[i]);
	}
	random_device rd;
	mt19937 gen(rd());
	discrete_distribution<> dist(weights.begin(), weights.end());
	return dist(gen);
}
int CGameStateRun::draw_open_chest(bool can_evo)
{
	// 0~20: passive
	// 21~52: Weapon
	// 53~83: evo
	return -1; //WIP
}

void CGameStateRun::update_mouse_pos()
{
	CPoint p;
	GetCursorPos(&p);
	HWND targetWindow = FindWindow(NULL, GAME_TITLE);
	ScreenToClient(targetWindow, &p);
	mouse_pos.x = p.x - VSObject::player_dx;
	mouse_pos.y = p.y - VSObject::player_dy;
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	update_mouse_pos();
	vector <VSObject*> result;
	switch (_gamerun_status) {
	case(PLAYING):
		//--------------------------------------------------------
		//playing status
		//--------------------------------------------------------
		player.update_pos(mouse_pos);
		player.update_proj_pos();
		QT.set_range(-Player::player_dx, -Player::player_dy, 800, 600);
		for (auto& weapon : player.get_weapon_all()) {
			for (Projectile& proj : weapon.get_all_proj()) {
				QT.insert((VSObject*)(&proj));
			}
		}
		for (Enemy& i_enemy : enemy) {
			if (!i_enemy.is_dead() && i_enemy.is_enable())
				QT.insert((VSObject*)(&i_enemy));
		}
		for (auto& weapon : player.get_weapon_all()) {
			for (Projectile& proj : weapon.get_all_proj()) {
				result = {};
				QT.query(result, (VSObject*)(&proj));
				for (VSObject* obj : result) {
					if (obj->obj_type == ENEMY) {
						proj.collide_with_enemy(*((Enemy*)obj), weapon.get_damage(), weapon.get_duration());
					}
				}
			}
		}
		for (int i = 0; i < (int)enemy.size(); i++) {
			enemy[i].update_pos(player.get_pos());
			result = {};
			QT.query(result, (VSObject*)(&enemy[i]));
			for (VSObject* obj : result) {
				enemy[i].append_collide(*((Enemy*)obj), 0.75, 0.5);
			}
			enemy[i].update_collide();
			if (enemy[i].is_collide_with(player)) {
				enemy[i].append_collide(player, 1, 0.5);
				enemy[i].update_collide();
				player.hurt(enemy[i].get_power());
				//if (enemy[i].hurt(1)) {
				//	//when the enemy die from this damage
				//	xp[i].spawn_xp(enemy[i].get_pos(), enemy[i].get_xp_value());
				//}
			}
		}
		QT.clear();
		// suck xp
		for (auto& i : xp) {
			if (i.is_enable() && distance(player, i) < player.get_pickup_range()) {
				i.set_speed(1000);
				i.update_pos(player.get_pos());
				if (is_overlapped(player, i)) {
					i.despawn();
					if (player.pick_up_xp(i.get_xp_value()))
						_next_status = LEVEL_UP;
				}
			}
		}
		break;
	case(LEVEL_UP):
		//--------------------------------------------------------
		//level up status
		//--------------------------------------------------------
		if (level_up_choice[0] != -1)
			break;
		level_up_choice[0] = draw_level_up(false);
		level_up_choice[1] = draw_level_up(false);
		level_up_choice[2] = draw_level_up(false);
		level_up_choice[3] = draw_level_up(false);
		break;
	case(OPEN_CHEST):
		//--------------------------------------------------------
		// chest status
		//--------------------------------------------------------
		break;
	}
	_gamerun_status = _next_status;
}
void CGameStateRun::OnShow()
{

	map.map_padding(player.get_pos());
	map.show_map();
	player.show_skin();
	player.show_proj_skin();
	for (int i = 0; i < (int)enemy.size(); i++) {
		enemy[i].show_skin();
	}
	for (auto& i : xp)
		i.show_skin();
	
	if (_gamerun_status == LEVEL_UP) {
		event_background.set_pos(player.get_pos());
		level_up_button[0].set_pos(player.get_pos() + CPoint(0, -90));
		level_up_button[1].set_pos(player.get_pos() + CPoint(0, -10));
		level_up_button[2].set_pos(player.get_pos() + CPoint(0, 70));
		level_up_button[3].set_pos(player.get_pos() + CPoint(0, 150));
		event_background.show_skin();
		for (int i = 0; i < 4; i++) {
			if (level_up_choice[i]!=-1) {
				level_up_button[i].show_button();
			}
		}
	}
}
