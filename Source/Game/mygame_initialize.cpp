#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

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
}

void CGameStateInit::OnBeginState()
{
	 // load backgrond (開始頁面)
	CMovingBitmap& button_start_bitmap = button_start.get_skin();
	button_start_bitmap.LoadBitmapByString({ "resources/button_start.bmp" }, RGB(255, 255, 255));
	button_start_bitmap.SetTopLeft((SIZE_X - button_start_bitmap.Width())/2, (SIZE_Y - button_start_bitmap.Height()) / 2);
	
	background.LoadBitmapByString({ "resources/background.bmp "}, RGB(255, 255, 255));
	background.SetTopLeft((SIZE_X - background.Width()) / 2, (SIZE_Y - background.Height()) / 2);

}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(button_start.is_hover(point.x, point.y))
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
	// show background
	background.ShowBitmap();
	button_start.show_button();
}
