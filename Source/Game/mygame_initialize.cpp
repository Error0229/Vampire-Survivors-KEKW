#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "config.h"
#include <fstream>
#include <sstream>

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
	🆗.load_skin({"Resources/ui/ok.bmp"});
	🆖.load_skin({"Resources/ui/no.bmp"});

	button_start.activate_hover = true;
	vector <string> character_skins;
	vector <string> weapon_icon = {
		"resources/weapon/whip.bmp", 
		"resources/weapon/wandholy.bmp", 
		"resources/weapon/Diamond2.bmp",
		"resources/weapon/knife.bmp", 
		"resources/weapon/wandfire.bmp", 
		"resources/weapon/axe.bmp", 
		"resources/weapon/garlic.bmp",
		"resources/weapon/holywater.bmp",
		"resources/weapon/holybook.bmp",
		"resources/weapon/cross.bmp",
		"resources/weapon/heavensword.bmp"
	};

	ifstream file("source/game/VSclass/player_data.csv");
	string line, skin_file, token;
	getline(file, token); // no use
	while (getline(file, line)) {
		Ui 🧝;
		stringstream ss(line);
		getline(ss, skin_file, ',');
		getline(ss, token, ',');
		getline(ss, token, ',');
		🧝.set_name(token);
		character_short_name.emplace_back(token.substr(0, token.find_first_of(' ')));
		getline(ss, token, ',');
		skin_file = "resources/character/" + skin_file + "_01.bmp";
		🧝.load_skin(vector<string>{skin_file});
		character_skins.push_back(skin_file);
		characters.push_back(🧝);
	}
	for (int i = 0; i < 11; i++) {
		Ui weapon, s_bg;
		weapon.load_skin(vector<string>{weapon_icon[i]});
		s_bg.load_skin({ "Resources/ui/character_bg.bmp", "Resources/ui/character_bg_s.bmp" });
		weapon.set_pos(-60 + (i % 3) * 80, -93 + (i / 3) * 80);
		s_bg.set_pos(-80 + (i % 3) * 80, -110 + (i / 3) * 80);
		s_bg.activate_hover = true;
		weapons.emplace_back(weapon);
		character_bg.push_back(s_bg);
		characters[i].set_pos(-90 + (i % 3) * 80, -103 + (i / 3) * 80);
	}
	maps = { Ui(), Ui() };
	maps[0].load_skin("Resources/ui/stage_forest.bmp");
	maps[0].set_pos(0, -90);
	maps[0].set_name("Mad Forest");
	maps[0].activate_hover = true;
	maps[1].load_skin("Resources/ui/stage_library.bmp");
	maps[1].set_pos(0, 20);
	maps[1].set_name("Inlaid Library");
	maps[1].activate_hover = true;
	map_select_effect.load_skin("Resources/ui/map_selected.bmp");
	map_selected = -1;
	🆗.activate_hover = true;
	🆖.activate_hover = true;
	background.set_pos(0, 0);
	button_start.set_pos(0, 0);
	🆗.set_pos( 100, 200);
	🆖.set_pos(-100, 200);
	select_bg.set_pos(0, 0);

}

void CGameStateInit::OnBeginState()
{
	 // load backgrond (開始頁面)
	STATE = menu_state::init;
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
		static int selected = -1;
		for (int i = 0; i < 11; i++) {
			if (character_bg[i].is_hover(mouse_pos)) {
				if (selected != -1)
					character_bg[selected].set_selector(0);
				if (selected == i) {
					selected = -1;
				}else{
					character_bg[i].set_selector(1);
					game->Set🚹(characters[i].get_name());
					selected = i;
				}
				break;
			}
		}
		break;
	}
	case menu_state::select_map: {

		if (🆗.is_hover(mouse_pos) && map_selected != -1) {
			game->Set🗺️(map_selected);
			STATE = menu_state::start;
		}
		else if (🆖.is_hover(mouse_pos)) {
			STATE = menu_state::select_character;
		}
		for (int i = 0; i < static_cast<int>(maps.size()); i++) {
			if (maps[i].is_hover(mouse_pos)) {
				map_selected = map_selected == i ? -1 : i;
				map_select_effect.set_pos(maps[i].get_pos());
			}
		}
		break;
	}
	}


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
		text_device.add_text("Character selection", CPoint(0, -180), 1, FONT_24x18_B, ALIGN_CENTER);
		select_bg.show_skin();
		🆗.show_skin();
		🆖.show_skin();
		for (int i = 0; i < 11; i++) {
			character_bg[i].show_skin();
			characters[i].show_skin();
			weapons[i].show_skin();
			text_device.add_text(character_short_name[i], characters[i].get_pos() + CPoint(-20, -30), 1, FONT_NORM, ALIGN_LEFT);
		}
		
		break;
	}
	case menu_state::select_map: {
		text_device.add_text("Map selection", CPoint(0, -180), 1, FONT_24x18_B, ALIGN_CENTER);
		select_bg.show_skin();
		if (map_selected != -1) {
			map_select_effect.show_skin();
		}
		for (auto& m : maps) {
			m.show_skin();
			text_device.add_text(m.get_name(), m.get_pos() + CPoint(10, -30), 1, FONT_L, ALIGN_CENTER);
		}
		🆗.show_skin();
		🆖.show_skin();
		break;
	}
	case menu_state::start:{
		GotoGameState(GAME_STATE_RUN);
		break;
	}
	}
	text_device.print_all();
}
