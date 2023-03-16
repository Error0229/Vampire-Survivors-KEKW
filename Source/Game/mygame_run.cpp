#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include "mygame.h"
#include "VSclass/VS.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
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

	player.load_skin({ "resources/character/Dog_01.bmp", "resources/character/Dog_02.bmp" ,"resources/character/Dog_03.bmp" ,"resources/character/Dog_04.bmp" ,"resources/character/Dog_05.bmp" });
	player.set_pos(0, 0);
	player.set_speed(300);
	player.set_default_direct(RIGHT);
	player.set_animation(150, false);
	player.load_bleed();
	player.acquire_weapon(Weapon::_base_weapon[0]);
	player.acquire_passive(new Passive(0));
	map.load_map({ "resources/map/dummy1.bmp" });
	map.set_pos(0, 0);
	QT = QuadTree(-Player::player_dx, -Player::player_dy, 800, 600, 5, 10, 0);
	QT.clear();

	for (int i = 0; i < 100; i++) {
		enemy.push_back(Enemy::get_template_enemy(BAT2));
		xp.push_back(Pickup::get_template_pickup(XP));
	}
	for ( int i = 0; i < (int)enemy.size(); i++ ) {
		enemy[i].spawn(CPoint(-300 + 30 * i/10, -400 + 40 * i%10));
	}
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

  player.level_up_passive(0);
	for (int i = 0; i < (int)enemy.size();i++) {
		if (enemy[i].hurt(1000000)) {
			xp[i].spawn_xp(enemy[i].get_pos(), enemy[i].get_xp_value());
		}

	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
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

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	CPoint p;
	GetCursorPos(&p);
	HWND targetWindow = FindWindow(NULL, GAME_TITLE);
	ScreenToClient(targetWindow, &p);
	mouse_pos.x = p.x - VSObject::player_dx;
	mouse_pos.y = p.y - VSObject::player_dy;
	player.update_pos(mouse_pos);
	player.update_proj_pos();
	QT.set_range(-Player::player_dx, -Player::player_dy, 800, 600);
	for (Enemy& i_enemy : enemy) {
		if (!i_enemy.is_dead())
			QT.insert((VSObject*)(&i_enemy));
	}
	for ( int i = 0; i < (int)enemy.size(); i++ ) {
		enemy[i].update_pos(player.get_pos());
		/*for ( int j = 0; j < (int)enemy.size(); j++ ) {
			if (i != j && (!enemy[i].is_dead()) && (enemy[i].is_enable()) && is_overlapped(enemy[i], enemy[j])) {
				enemy[i].resolve_collide(enemy[ j ]);
			}
		}*/
		vector <VSObject*> result;
		QT.query(result, (VSObject*)(&enemy[i]));
		for (VSObject* obj : result) {
			enemy[i].append_collide(*((Enemy*)obj), 0.75, 0.5);
		}
		enemy[i].update_collide();
		if (enemy[i].is_collide_with(player)) {
			enemy[i].append_collide(player, 1, 0.5);
			enemy[i].update_collide();
			player.hurt(enemy[i].get_power());
			if (enemy[i].hurt(1)) {
				//when the enemy die from this damage
				xp[i].spawn_xp(enemy[i].get_pos(), enemy[i].get_xp_value());
			}
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
					TRACE("AAAAAAAAAAAAAAAAAAAAA\n");
			}
		}
	}
}
void CGameStateRun::OnShow()
{
	map.map_padding(player.get_pos());
	map.show_map();
	player.show_skin();
	player.show_proj_skin();
	for ( int i = 0; i < (int)enemy.size(); i++ ) {
		enemy[ i ].show_skin();
	}
	for (auto &i:xp)
		i.show_skin();
}
