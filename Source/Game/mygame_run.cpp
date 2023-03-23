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
	player.acquire_passive(Passive(POWER));
	map.load_map({ "resources/map/dummy1.bmp" });
	map.set_pos(0, 0);
	QT = QuadTree(-Player::player_dx, -Player::player_dy, (OPEN_AS_FULLSCREEN ? RESOLUTION_X : SIZE_X), (OPEN_AS_FULLSCREEN ? RESOLUTION_Y : SIZE_Y), 6, 10, 0);
	QT.clear();

	for (int i = 0; i < 100; i++) {
		enemy.push_back(Enemy::get_template_enemy(XLBAT));
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
	level_up_icon_frame[0].load_skin("resources/ui/frameB.bmp"); // need to update the file
	level_up_icon_frame[1].load_skin("resources/ui/frameB.bmp");
	level_up_icon_frame[2].load_skin("resources/ui/frameB.bmp");
	level_up_icon_frame[3].load_skin("resources/ui/frameB.bmp");
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
		Weapon::evolution(WHIP);
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
				player.obtain_item(level_up_choice[i]);
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

int CGameStateRun::draw_level_up(bool pull_from_inv)
{
	//0~31: weapon
	//32~62: evo
	//63~83: passive
	if (!pull_from_inv && Weapon::weapon_count() >= 6 && player.passive_count() >= 6)
		return draw_level_up(true);
	if (pull_from_inv && Weapon::weapon_count() + player.passive_count() == 1)
		return draw_level_up(false);
	vector<double> weights(84, 0);
	int player_items[84];
	memset(player_items, 0, sizeof(player_items));
	// store player's items, 0: not owned, 1: owned, 2: max level
	for (auto& i : Weapon::all_weapon) {
		player_items[i.get_type()] = (i.is_max_level()) ? 2 : 1;
	}
	for (auto i : player.get_passives()) {
		player_items[i.get_type()] = (i.is_max_level()) ? 2 : 1;
	}
	// calc weapon weights
	// increase this once we made a new weapom.
	for (int i = 0; i < 1; i++) {
		if (level_up_choice[0] == i || level_up_choice[1] == i || level_up_choice[2] == i || level_up_choice[3] == i)
			continue;
		if ((pull_from_inv && player_items[i] == 1) || (!pull_from_inv && player_items[i] == 0))
			weights[i] = Weapon::_base_weapon[i].get_rarity();
	}
	// calc passive weights
	for (int i = 63; i < 84; i++) {
		if (level_up_choice[0] == i || level_up_choice[1] == i || level_up_choice[2] == i || level_up_choice[3] == i)
			continue;
		if ((pull_from_inv && player_items[i] == 1) || (!pull_from_inv && player_items[i] == 0))
			weights[i] = Passive(i).get_rarity();
	}
	random_device rd;
	mt19937 gen(rd());
	discrete_distribution<> dist(weights.begin(), weights.end());
	return dist(gen);
}
int CGameStateRun::draw_open_chest(bool can_evo)
{
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
	//for level-up polling
	double owned_chance, forth_chance;
	vector<double> pull_owned, pull_forth;
	random_device rd;
	mt19937 gen(rd());
	discrete_distribution<> dist_own, dist_forth;
	switch (_gamerun_status) {
	case(PLAYING):
		//--------------------------------------------------------
		//playing status
		//--------------------------------------------------------
		Weapon::attack();
		Projectile::update_position();
		player.update_pos(mouse_pos);
		QT.set_range(-Player::player_dx, -Player::player_dy, (OPEN_AS_FULLSCREEN ? RESOLUTION_X : SIZE_X ), (OPEN_AS_FULLSCREEN ? RESOLUTION_Y  : SIZE_Y ));

		for (Projectile& proj : Projectile::all_proj) {
			QT.insert((VSObject*)(&proj));
		}

		for (Enemy& i_enemy : enemy) {
			if (!i_enemy.is_dead() && i_enemy.is_enable())
				QT.insert((VSObject*)(&i_enemy));
		}
		for (Projectile& proj :Projectile::all_proj) {
			result = {};
			QT.query(result, (VSObject*)(&proj));
			for (VSObject* obj : result) {
				if (obj->obj_type == ENEMY) {
					proj.collide_with_enemy(*((Enemy*)obj));
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
			if (i.is_enable() && VSObject::distance(player, i) < player.get_pickup_range()) {
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

		// owned_chance
		owned_chance = 1 - 0.3 * ((player.get_level() & 1) ? 1 : 2) / (double)player.get_luck() * 100;
		pull_owned = {((1 - owned_chance > 0) ? 1 - owned_chance : 0), owned_chance };
		dist_own = discrete_distribution<>(pull_owned.begin(), pull_owned.end());
		
		// 4th_chance
		forth_chance = 1 - (1 / (double)player.get_luck() * 100);
		pull_forth = { 1 - forth_chance , forth_chance };
		dist_forth = discrete_distribution<>(pull_forth.begin(), pull_forth.end());

		// poll new level_up choices
		level_up_choice[0] = draw_level_up(dist_own(gen));
		level_up_choice[1] = draw_level_up(dist_own(gen));
		level_up_choice[2] = draw_level_up(false);
		level_up_choice[3] = dist_forth(gen)? draw_level_up(false):-1;
		TRACE(_T("0:%d\t1:%d\t2:%d\t3:%d\n"), level_up_choice[0], level_up_choice[1], level_up_choice[2], level_up_choice[3]);
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
	// player.show_proj_skin();
	Weapon::show();
	for (int i = 0; i < (int)enemy.size(); i++) {
		enemy[i].show_skin();
	}
	for (auto& i : xp)
		i.show_skin();
	
	if (_gamerun_status == LEVEL_UP) {
		event_background.set_pos(player.get_pos());
		level_up_button[0].set_pos(player.get_pos() + CPoint(0, -75));
		level_up_button[1].set_pos(player.get_pos() + CPoint(0, 0));
		level_up_button[2].set_pos(player.get_pos() + CPoint(0, 75));
		level_up_button[3].set_pos(player.get_pos() + CPoint(0, 150));
		level_up_icon[0].set_pos(player.get_pos() + CPoint(-120, -90));
		level_up_icon[1].set_pos(player.get_pos() + CPoint(-120, -15));
		level_up_icon[2].set_pos(player.get_pos() + CPoint(-120, 60));
		level_up_icon[3].set_pos(player.get_pos() + CPoint(-120, 135));
		level_up_icon_frame[0].set_pos(player.get_pos() + CPoint(-120, -90));
		level_up_icon_frame[1].set_pos(player.get_pos() + CPoint(-120, -15));
		level_up_icon_frame[2].set_pos(player.get_pos() + CPoint(-120, 60));
		level_up_icon_frame[3].set_pos(player.get_pos() + CPoint(-120, 135));
		event_background.show_skin();
		for (int i = 0; i < 4; i++) {
			if (level_up_choice[i]!=-1) {
				level_up_button[i].show_button();
				level_up_icon_frame[i].show_skin();
				level_up_icon[i].show_icon(level_up_choice[i]);
			}
		}
	}
}
