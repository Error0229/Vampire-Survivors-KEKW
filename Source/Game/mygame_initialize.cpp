#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "config.h"
#include <array>
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
	button_start.load_skin("resources/ui/button_start.bmp");
	button_upgrade.load_skin("resources/ui/button_upgrade.bmp");
	button_go_upgrade.load_skin("resources/ui/button_go_upgrade.bmp");
	button_restore.load_skin("resources/ui/money_restore.bmp");
	background.load_skin("resources/ui/background_1.bmp");
	select_bg.load_skin("resources/ui/event_background.bmp");
	money_bg.load_skin("Resources/ui/button_c9_normal_mini.bmp");
	🆗.load_skin({"Resources/ui/ok.bmp"});
	🆖.load_skin({"Resources/ui/no.bmp"});
	Icon::load_filename();
	button_start.activate_hover = true;
	button_upgrade.activate_hover = true;
	button_go_upgrade.activate_hover = true;
	button_restore.activate_hover = true;
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
	passive_levels = vector<int>(16);
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
	file.close();
	for (int i = 0; i < 16; i++) {
		vector<Ui> tmp;
		for (int j = 0; j < passive_max_level[i]; j++) {
			Ui p;
			p.load_skin({ "Resources/ui/s_checkbox_empty_b.bmp","Resources/ui/s_checkbox_checked_b.bmp" });
			int offset = (passive_max_level[i] - 1) * 4;
			p.set_pos(-110 + (i % 4) * 73 - offset + j * 8, -87 + (i / 4) * 73);
			tmp.emplace_back(p);
		}	
		passive_checkbox.emplace_back(tmp);
	}
	Ui p_bg;
	p_bg.load_skin("Resources/ui/frameB.bmp");
	Icon tmp;
	tmp.load_icon();
	for (int i = 0; i < 16; i++) {
		tmp.set_selector(63 + i);
		tmp.set_pos(-110 + (i % 4) * 73, -110 + (i / 4) * 73);
		p_bg.set_pos(-110 + (i % 4) * 73, -110 + (i / 4) * 73);
		tmp.set_name(Icon::icon_name[63 + i]);
		passive_bg.emplace_back(p_bg);
		passive_icon.emplace_back(tmp);
	}
	Ui s_bg;
	s_bg.load_skin({ "Resources/ui/character_bg.bmp", "Resources/ui/character_bg_s.bmp" });
	s_bg.activate_hover = true;
	for (int i = 0; i < 16; i++) {
		s_bg.set_pos(-80 + (i % 3) * 80, -110 + (i / 3) * 80);
		character_bg.push_back(s_bg);
		if (i >= 11)
			continue;
		Ui weapon;
		weapon.load_skin(vector<string>{weapon_icon[i]});
		weapon.set_pos(-60 + (i % 3) * 80, -93 + (i / 3) * 80);
		weapons.emplace_back(weapon);
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
	money_bg.set_pos(0, -220);
	button_start.set_pos(0, 0);
	button_upgrade.set_pos(100, 200);
	button_go_upgrade.set_pos(0, 150);
	button_restore.set_pos(0, -170);
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
		else if (button_go_upgrade.is_hover(mouse_pos)) {
			STATE = menu_state::upgrade_passive;
			ifstream player_data("save/save_data.csv");
			string line, token;
			getline(player_data, line);
			getline(player_data, line);
			stringstream ss(line);
			getline(ss, token, ',');
			coin = stoi(token);
			for (int i = 0; i < 16; i++) {
				getline(ss, token, ',');
				passive_levels[i] = (stoi(token));
				for (int j = 0; j < passive_levels[i]; j++) {
					passive_checkbox[i][j].set_selector(1);
				}
			}
			player_data.close();

		}
		break;
	}
	case menu_state::select_character :{
		static int selected = -1;
		if (🆗.is_hover(mouse_pos) && selected != -1) {
			STATE = menu_state::select_map;
		}
		else if (🆖.is_hover(mouse_pos)) {
			STATE = menu_state::init;
			if (selected != -1) {
				character_bg[selected].set_selector(0);
				selected = -1;
			}
		}
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
	case menu_state::upgrade_passive: {
		static int passive_selected = -1;
		if (🆖.is_hover(mouse_pos)) {
			if (passive_selected != -1) {
				character_bg[passive_selected].set_selector(0);
				passive_selected = -1;
			}
			STATE = menu_state::init;
			ifstream p_data("save/save_data.csv");
			string header, line;
			getline(p_data, header);
			p_data.close();
			ofstream r_data("save/save_data.csv", std::ofstream::trunc);
			r_data << header << '\n';
			r_data << coin << ',';
			for (int i = 0; i < 16; i++) {
				r_data << passive_levels[i] << ", "[i == 15];
			}
			r_data.close();
		}
		else if (button_upgrade.is_hover(mouse_pos)) {
			if (passive_selected != -1 && passive_levels[passive_selected] < passive_max_level[passive_selected] && coin >= 100 + passive_levels[passive_selected] * 100) {
				coin -= 100 + passive_levels[passive_selected] * 100;
				passive_checkbox[passive_selected][passive_levels[passive_selected]].set_selector(1);
				passive_levels[passive_selected]++;
			}
		}
		else if (button_restore.is_hover(mouse_pos)) {
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < passive_levels[i]; j++) {
					passive_checkbox[i][j].set_selector(0);
					coin += (j + 1) * 100;
				}
				passive_levels[i] = 0;
			}
		}
		for (int i = 0; i < 16; i++) {
			if (character_bg[i].is_hover(mouse_pos)) {
				if (passive_selected != -1)
					character_bg[passive_selected].set_selector(0);
				if (passive_selected == i) {
					passive_selected = -1;
				}
				else {
					character_bg[i].set_selector(1);
					passive_selected = i;
				}
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
			if (map_selected != -1)
				map_selected = -1;
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
	switch (STATE) {
	case menu_state::init: {
		button_start.show_skin();
		button_go_upgrade.show_skin();
		break;
	}
	case menu_state::upgrade_passive:{
		select_bg.show_skin();
		button_upgrade.show_skin();
		🆖.show_skin();
		button_restore.show_skin();
		button_upgrade.show_skin();
		money_bg.show_skin();
		text_device.add_text(to_string(coin), CPoint(0, -220), 1, FONT_NORM, ALIGN_CENTER);
		for (int i = 0; i < 16; i++) {
			character_bg[i].set_pos(-110 + (i % 4) * 73, -110 + (i / 4) * 73);
			character_bg[i].show_skin();
			passive_bg[i].show_skin();
			passive_icon[i].show_skin();
			for (int j = 0; j < passive_max_level[i]; j++) {
				passive_checkbox[i][j].show_skin();
			}
			text_device.add_text(passive_icon[i].get_name(), passive_icon[i].get_pos() + CPoint(0, -25), 1, FONT_NORM, ALIGN_CENTER);
		}
		break;
	}
	case menu_state::select_character: {
		text_device.add_text("Character selection", CPoint(0, -180), 1, FONT_24x18_B, ALIGN_CENTER);
		select_bg.show_skin();
		🆗.show_skin();
		🆖.show_skin();
		for (int i = 0; i < 11; i++) {
			character_bg[i].set_pos(-80 + (i % 3) * 80, -110 + (i / 3) * 80);
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
