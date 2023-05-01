#include "stdafx.h"
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
	background.load_skin({"resources/ui/background_1.bmp"});
	select_bg.load_skin({"resources/ui/event_background.bmp"});
	🆗.load_skin({"Resources/ui/button_c5_normal.bmp"});
	🆖.load_skin({"Resources/ui/button_c8_normal.bmp"});

	button_start.activate_hover = true;

	background.set_pos(0, 0);
	button_start.set_pos(0, 0);
	🆗.set_pos(100, -100);
	🆖.set_pos(100, 100);
	select_bg.set_pos(0, 0);

}

void CGameStateInit::OnBeginState()
{
	 // load backgrond (開始頁面)
	STATE = menu_state::start;
	VSObject::player_dx = w_size_x >> 1;
	VSObject::player_dy = w_size_y >> 1;
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{

	CPoint mouse_pos = point - CPoint((OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1), (OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1));
	switch (STATE) {
	case menu_state::init: {
		if (button_start.is_hover(mouse_pos)) {
			STATE = menu_state::select_character;
		}
		break;
	}
	case menu_state::select_character :{
		if (🆗.is_hover(mouse_pos)) {
			STATE = menu_state::select_map;
		}
		else if (🆖.is_hover(mouse_pos)) {
			STATE = menu_state::init;
		}
		break;
	}
	case menu_state::select_map: {
		if (🆗.is_hover(mouse_pos)) {
			game->Set🗺️🚹(1, "The Dog");
			STATE = menu_state::start;
		}
		else if (🆖.is_hover(mouse_pos)) {
			STATE = menu_state::select_character;
		}
		break;
	}
	}

	//if (button_start.is_hover(mouse_pos)) 
	//{
	//	game->Set🗺️🚹(1, "The Dog");
	//	GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	//}
}

void CGameStateInit::OnShow()
{
	// show background
	background.show_skin();
	//button_start->show_button();
	switch (STATE) {
	case menu_state::init: {
		button_start.show_skin();
		break;
	}
	case menu_state::select_character: {
		text_device.add_text("Character selection", CPoint(0, -150), 1, FONT_12x08, ALIGN_LEFT);
		select_bg.show_skin();
		🆗.show_skin();
		🆖.show_skin();
		break;
	}
	case menu_state::select_map: {
		text_device.add_text("map selection", CPoint(0, -150), 1, FONT_12x08, ALIGN_LEFT);
		select_bg.show_skin();
		🆗.show_skin();
		🆖.show_skin();
		break;
	}
	case menu_state::start:{
		GotoGameState(GAME_STATE_RUN);
		break;
	}
	}
}
