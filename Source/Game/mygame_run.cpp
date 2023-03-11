#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
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
	player.load_skin({ "resources/character/Dog_01.bmp", "resources/character/Dog_02.bmp" ,"resources/character/Dog_03.bmp" ,"resources/character/Dog_04.bmp" ,"resources/character/Dog_05.bmp" });
	player.set_pos(0, 0);
	player.set_default_direct(RIGHT);
	player.set_animation(150, false);
	player.set_speed(300);
	player.acquire_weapon(Weapon::_base_weapon[0]);
	map.load_map({ "resources/map/dummy1.bmp" });
	map.set_pos(0, 0);
	
	load_enemy_type(xlmantis, "XLMantis", 10, 1, 1, 1, 200);
	for ( int i = 0; i < (int)xlmantis.size(); i++ ) {
		xlmantis[i].set_pos(-300 + 60 * i, -400 + 80 * i);
		//xlmantis[i].set_pos(-300 + 60 * i, 0);
		//xlmantis[i].set_pos(0, -400 + 80 * i);
	}
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	for ( auto& i : xlmantis )
		i.hurt(1000000);
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
	mouse_pos.x = p.x - VSObject::player_dx;
	mouse_pos.y = p.y - VSObject::player_dy;
	player.update_pos(mouse_pos);
	player.update_proj_pos();
	for ( int i = 0; i < (int)xlmantis.size(); i++ ) {
		xlmantis[i].update_pos(player.get_pos());
		//xlmantis[i].update_pos(CPoint(0, 0));
		for ( int j = 0; j < (int)xlmantis.size(); j++ ) {
			if (i!=j && is_overlapped(xlmantis[ i ], xlmantis[ j ])) {
				xlmantis[i].resolve_collide(xlmantis[ j ]);
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
	for ( int i = 0; i < (int)xlmantis.size(); i++ ) {
		xlmantis[ i ].show_skin();
	}
}
