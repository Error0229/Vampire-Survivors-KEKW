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

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	CPoint p;
	GetCursorPos(&p);
	mouse_pos.x = p.x - VSObject::player_dx;
	mouse_pos.y = p.y - VSObject::player_dy;
	player.update_pos(mouse_pos);
	bat1.update_pos(player.get_pos());
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	player.load_skin({ "resources/Dog_01.bmp", "resources/Dog_02.bmp" ,"resources/Dog_03.bmp" ,"resources/Dog_04.bmp" ,"resources/Dog_05.bmp" });
	player.set_pos(0, 0);
	player.set_default_direct(RIGHT);
	player.set_animation(150, false);
	player.set_speed(300);
	map.load_map({ "resources/dummy1.bmp" });
	map.set_pos(0, 0);
	bat1.load_by_name("XLMantis");
	bat1.set_pos(CPoint(-300, -400));
	bat1.set_stat();
	bat1.set_animation(200, false);
	bat1.enable_animation();
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//bat1.hurt(1000000);
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

void CGameStateRun::OnShow()
{
	map.show_skin();
	player.show_skin();

	bat1.show_skin();
}
