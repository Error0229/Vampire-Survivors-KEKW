﻿#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "config.h"

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0, "Start Initialize...");	// 一開始的loading進度為0%
	//
	// 開始載入資料
	//
	//Sleep(1000);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
	button_start.load_skin({"resources/ui/button_start.bmp"});
	button_start.activate_hover = true;
	background.load_skin({"resources/ui/background_1.bmp"});


}

void CGameStateInit::OnBeginState()
{
	 // load backgrond (開始頁面)
	VSObject::player_dx = w_size_x >> 1;
	VSObject::player_dy = w_size_y >> 1;
	background.set_pos(0, 0);
	button_start.set_pos(0, 0);
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{

	CPoint mouse_pos = point - CPoint((OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1), (OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1));
	if (button_start.is_hover(mouse_pos)) 
	{
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}
}

void CGameStateInit::OnShow()
{
	// show background
	background.show_skin();
	//button_start->show_button();
	button_start.show_skin();
}
