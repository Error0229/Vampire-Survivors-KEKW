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
#include <string>

using namespace game_framework;

enum gamerun_status {
	PLAYING,
	LEVEL_UP,
	OPEN_CHEST,
	GAME_OVER,
	PAUSE,
	REVIVE
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
	QuadTree::VSPlain.clear();
	Weapon::all_weapon.clear();
	Passive::all_passive.clear();
	Projectile::reset();
	Xp::reset_XP();
	Chest::reset_chest();
	LightSourcePickup::reset();
	enemy_factory.reset();
	light_source_factory.reset();
	timer.reset();
	timer.start();
	// CAudio::Instance()->Play(0, true); // 🆗
	player = Player(game->Get🚹());
	player.set_default_direct(RIGHT);
	player.set_animation(150, false);
	player.load_mirror_skin();
	player.load_bleed();
	player.set_pos(0, 0);
	player.set_speed(300);
	map = Map();
	MAP_ID = game->Get🗺️();
	GOLD_NUM = 0;
	KILL_NUM = 0;
	switch (MAP_ID) {
	case 0:
		map.load_map({ "resources/map/dummy1.bmp" });
		break;
	case 1:
		map.load_map({ "resources/map/dummy2.bmp" });
		break;
	}
	map.set_pos(0, 0);
	event_background.set_base_pos(0, 0);
	_gamerun_status = PLAYING;
	_next_status = PLAYING;
}


void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	Weapon::load_weapon_stats();
	enemy_factory.init();
	light_source_factory.init();
	Icon::load_filename();
	Xp::init_XP();
	Chest::init_chest();
	LightSourcePickup::init_lightsource_pickup();
	Damage::damage_device()->init();
	Player::init_player();
	map.load_map({ "resources/map/dummy1.bmp" });
	event_background.load_skin("resources/ui/event_background.bmp");
	coin.load_skin("Resources/pickup/CoinGold.bmp");
	skull.load_skin("Resources/ui/SkullToken.bmp");
	button_pause.load_skin("Resources/ui/pause.bmp");
	button_resume.load_skin("Resources/ui/button_resume.bmp");
	evolution_chart.load_skin("Resources/ui/evolutions.bmp");
	coin.set_base_pos(373, -260);
	skull.set_base_pos(373, -240);
	button_pause.set_base_pos(370, -210);
	button_resume.set_base_pos(0 , 200);
	evolution_chart.set_base_pos(0, 0);
	button_pause.activate_hover = true;
	button_resume.activate_hover = true;
	for (int i = 0; i < 4; i++) {
		level_up_button[i].load_skin("resources/ui/event_button.bmp");
		level_up_icon_frame[i].load_skin("resources/ui/frameB.bmp");
		level_up_button[i].set_base_pos(0, -75 + 75 * i);
		level_up_icon_frame[i].set_base_pos(-120, -90 + 75 * i);
		level_up_icon[i].set_base_pos(-120, -90 + 75 * i);
		level_up_icon[i].load_icon();
		level_up_choice[i] = -1;
	}
	// chest_animation.load_skin({ "resources/ui/TreasureIdle_01_big.bmp", "resources/ui/TreasureIdle_02_big.bmp" , "resources/ui/TreasureIdle_03_big.bmp" , "resources/ui/TreasureIdle_04_big.bmp" , "resources/ui/TreasureIdle_05_big.bmp" , "resources/ui/TreasureIdle_06_big.bmp" ,"resources/ui/TreasureIdle_07_big.bmp" ,"resources/ui/TreasureIdle_08_big.bmp", "resources/ui/TreasureOpen_01_big.bmp", "resources/ui/TreasureOpen_02_big.bmp" , "resources/ui/TreasureOpen_03_big.bmp" , "resources/ui/TreasureOpen_04_big.bmp" , "resources/ui/TreasureOpen_05_big.bmp" , "resources/ui/TreasureOpen_06_big.bmp" , "resources/ui/TreasureOpen_07_big.bmp" , "resources/ui/TreasureOpen_08_big.bmp" });
	string base = "Resources/chest_animation/Chest1/chest1_";
	vector<string> chest_animation_filename;
	for (int i = 1; i <= 330; i++) {
		chest_animation_filename.emplace_back(base + to_string(i) + ".bmp");
	}
	chest_animation.load_skin(chest_animation_filename);
	//chest_animation.set_animation(100, true);
	chest_animation.set_animation(30, true);
	// chest_animation.set_base_pos(5, 75);
	chest_animation.set_base_pos(0, -33);
	/*CAudio::Instance()->Load(0, "Resources/AudioClip/bgm_elrond_bone.wav");
	CAudio::Instance()->Load(1, "Resources/AudioClip/sfx_gem.wav");
	CAudio::Instance()->Load(2, "Resources/AudioClip/sfx_enemyHit.wav");
	CAudio::Instance()->SetVolume(0, 10);
	CAudio::Instance()->SetVolume(1, 10);
	CAudio::Instance()->SetVolume(2, 10);*/
	CPoint chest_item_pos[] = { CPoint(0,-50), CPoint(-80,-110), CPoint(80,-110), CPoint(-100,-10), CPoint(100,-10) };
	for (int i = 0; i < 5; i++) {
		chest_item_icon[i].load_icon();
		chest_item_icon[i].set_base_pos(chest_item_pos[i]);
		chest_item_frame[i].load_skin("resources/ui/PrizeBG.bmp");
		chest_item_frame[i].set_base_pos(chest_item_pos[i]);
		chest_item[i] = -1;
	}
	game_over_frame.load_skin("Resources/ui/gameOver.bmp");
	game_over_frame.set_base_pos(0, 0);
	game_over_button.load_skin("Resources/ui/button_play_again.bmp");
	game_over_button.set_base_pos(0, 80);
	game_over_button.activate_hover = true;

	xp_bar_frame.load_skin("resources/ui/xp_bar_frame.bmp");
	xp_bar_frame.set_base_pos(-8, -300 + (xp_bar_frame.get_height() >> 1));
	xp_bar_cover.load_skin("resources/ui/xp_bar_cover.bmp");
	xp_bar_cover.set_base_pos(-8, -300 + (xp_bar_frame.get_height() >> 1));
	xp_bar.load_skin({ "resources/ui/xp_bar.bmp", "resources/ui/xp_bar_1.bmp", "resources/ui/xp_bar_2.bmp", "resources/ui/xp_bar_3.bmp", "resources/ui/xp_bar_4.bmp", "resources/ui/xp_bar_5.bmp" });
	xp_bar.set_base_pos(-8, -300 + (xp_bar.get_height() >> 1));
	xp_bar.set_animation(1, false);;
	xp_bar.disable_animation();

	inv_slot.load_skin("resources/ui/weaponSlots.bmp");
	inv_slot.set_base_pos(-400 + (inv_slot.get_width() >> 1), -300 + 24 + (inv_slot.get_height() >> 1));
	for (int i = 0; i < 12; i++) {
		inv_icon[i].load_icon();
		inv_icon[i].set_base_pos(-400 + 8 + i % 6 * 16, -300 + 24 + 8 + i / 6 * 16);
	}

	inv_detail_frame.load_skin("resources/ui/inv_detail_frame.bmp");
	inv_detail_frame.set_base_pos(-400 + (inv_detail_frame.get_width() >> 1), -300 + 24 + (inv_detail_frame.get_height() >> 1));
	for (int i = 0; i < 12; i++) {
		inv_detail_item_icons[i].load_icon();
		inv_detail_item_icons[i].set_base_pos(-400 + 20 + i % 6 * 26, -300 + 24 + 20 + i / 6 * 58);
		for (int j = 0; j < 12; j++) {
			inv_detail_item_knots[i][j][0].load_skin("resources/ui/weaponLevelEmpty.bmp");
			inv_detail_item_knots[i][j][1].load_skin("resources/ui/weaponLevelFull.bmp");
			inv_detail_item_knots[i][j][0].set_base_pos(-388 + j % 3 * 8 + i % 6 * 26, -244 + j / 3 * 8 + i / 6 * 58);
			inv_detail_item_knots[i][j][1].set_base_pos(-388 + j % 3 * 8 + i % 6 * 26, -244 + j / 3 * 8 + i / 6 * 58);
		}
	}

	stat_frame.load_skin("resources/ui/stat_frame.bmp");
	stat_frame.set_base_pos(-400 + (stat_frame.get_width() >> 1), -300 + 154 + (stat_frame.get_height() >> 1));
	for (int i = 0; i < 16; i++)
		stat_icon[i].load_icon();

	hp_bar.load_skin({ "resources/ui/hp_bar_0.bmp", "resources/ui/hp_bar_1.bmp", "resources/ui/hp_bar_2.bmp", "resources/ui/hp_bar_3.bmp", "resources/ui/hp_bar_4.bmp", "resources/ui/hp_bar_5.bmp", "resources/ui/hp_bar_6.bmp", "resources/ui/hp_bar_7.bmp", "resources/ui/hp_bar_8.bmp", "resources/ui/hp_bar_9.bmp", "resources/ui/hp_bar_10.bmp", "resources/ui/hp_bar_11.bmp", "resources/ui/hp_bar_12.bmp", "resources/ui/hp_bar_13.bmp", "resources/ui/hp_bar_14.bmp", "resources/ui/hp_bar_15.bmp", "resources/ui/hp_bar_16.bmp", "resources/ui/hp_bar_17.bmp", "resources/ui/hp_bar_18.bmp", "resources/ui/hp_bar_19.bmp" });;
	hp_bar.set_base_pos(0, 15);
	button_revive.load_skin("resources/ui/button_revive.bmp");
	button_revive.set_base_pos(0, 80);
	button_revive.activate_hover = true;
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// A: kill all enemies
	// B: pick up 10 xp
	// C: spawn a chest above player

	static int chest_cnt = 0; //tmp
	switch (nChar) {
	case('A'):
		for (auto 😈 : enemy_factory.live_enemy) {
			😈->hurt(1000);
		}
		break;
	case('B'):
		player.pick_up_xp(20);
		break;
	case('C'):
		Chest::spawnChest(player.get_pos() + CPoint(0, -100), 1, 100, 100);
		break;
	case('D'):
		timer.add_time(10000);
		break;
	case('E'):
		static int type = 0;
		LightSourcePickup::spawn_lightsource_pickup(player.get_pos() + CPoint(0, -100), type++);
		if (type == 7)
			type = 0;
		break;
	}
	

}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	update_mouse_pos();
	switch (_gamerun_status) {
	case(PLAYING):
		if (button_pause.is_hover(mouse_pos)) {
			_next_status = PAUSE;
			timer.pause();
		}
		break;
	case(PAUSE):
		if (button_resume.is_hover(mouse_pos)) 
			_next_status = PLAYING;
		break;
	case(LEVEL_UP):
		for (int i = 0; i < 4; i++) {
			if (level_up_button[i].is_hover(mouse_pos)) {
				player.obtain_item(level_up_choice[i]);
				//reset all choice/button
				for (int j = 0; j < 4; j++) {
					level_up_choice[j] = -1;
					level_up_button[j].activate_hover = false;
				}
				//switch status
				if (player.apply_level_up())
					_next_status = LEVEL_UP;
				else
					_next_status = PLAYING;
				break;
			}

		}
		break;
	case(OPEN_CHEST):
		if (!chest_animation.done())
			break;
		for (int i = 0; i < 5; i++)
			chest_item[i] = -1;
		chest_animation.reset();
		chest_animation.set_animation(30, true);
		_next_status = PLAYING;
		break;
	case (REVIVE): {
		if (button_revive.is_hover(mouse_pos)) {
			_next_status = PLAYING;
			player.revive();
		}
		break;
	}
	case(GAME_OVER):
		if (game_over_button.is_hover(mouse_pos)) {
			GotoGameState(GAME_STATE_OVER);
		}
		break;
	}
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

int CGameStateRun::draw_level_up(bool pull_from_inv)
{
	// 0~31: weapon
	//32~62: evo
	//63~83: passive
	if (pull_from_inv) {
		if (Weapon::weapon_count() + Passive::passive_count() == 1) {
			return draw_level_up(false);
		}
		if (player.all_max()) {
			if (player.full_inv()) {
				TRACE("level up: inv full and all max.\n");
				return -1;
			}
			else {
				return draw_level_up(false);
			}
		}
	}
	else {
		if (player.full_inv()) {
			return draw_level_up(true);
		}
	}
	vector<double> weights(84, 0);
	bool no_weight = true;
	int player_items[84];
	int base_weapon;
	memset(player_items, 0, sizeof(player_items));
	// store player's items, 0: not owned, 1: owned, 2: max level
	for (auto& i : Weapon::all_weapon) {
		player_items[i.get_type()] = (i.is_max_level()) ? 2 : 1;
		if (i.is_evo_weapon()) {
			base_weapon = Weapon::evolution_pair_reverse.find(i.get_type())->second;
			player_items[base_weapon % 100] = 2;
			if (base_weapon / 100)
				player_items[base_weapon / 100] = 2;
		}
	}
	for (auto i : Passive::all_passive) {
		player_items[i.get_type()] = (i.is_max_level()) ? 2 : 1;
	}
	// calc weapon weights
	// increase this once we made a new weapom.
	// we made weapon till diamond
	for (int i = 0; i < DIAMOND+1; i++) {
		if (level_up_choice[0] == i || level_up_choice[1] == i || level_up_choice[2] == i || level_up_choice[3] == i)
			continue;
		if ((pull_from_inv && player_items[i] == 1) || (!pull_from_inv && Weapon::weapon_count() < 6 && player_items[i] == 0)) {
			weights[i] = Weapon::_base_weapon[i].get_rarity();
			no_weight = false;
		}
	}
	// calc passive weights
	for (int i = 63; i < 84; i++) {
		if (level_up_choice[0] == i || level_up_choice[1] == i || level_up_choice[2] == i || level_up_choice[3] == i)
			continue;
		if ((pull_from_inv && player_items[i] == 1) || (!pull_from_inv && Passive::passive_count() < 6 && player_items[i] == 0)) {
			weights[i] = Passive(i).get_rarity();
			no_weight = false;
		}
	}
	if (no_weight && pull_from_inv)
		return draw_level_up(false);
	return poll(weights, true);
}
int CGameStateRun::draw_open_chest(bool pull_evo)
{
	// 0~31: weapon
	//32~62: evo
	//63~83: passive
	vector<double> weights;
	vector<int> index_to_type;

	bool all_max = true, can_evo = false;
	for (auto& i : Weapon::all_weapon) {
		if (i.can_evo())
			can_evo = true;
		if (!i.is_max_level())
			all_max = false;
	}
	for (auto& i : Passive::all_passive) {
		if (!i.is_max_level())
			all_max = false;
	}
	if (all_max && (!can_evo || !pull_evo)) {
		TRACE("open chest: all max and cant evo.\n");
		return -2;
	}
	for (auto& i : Passive::all_passive) {
		if (!i.is_max_level()) {
			weights.push_back(i.get_rarity());
			index_to_type.push_back(i.get_type());
		}
	}
	for (auto& i : Weapon::all_weapon) {
		if (!i.is_max_level()) {
			weights.push_back(i.get_rarity());
			index_to_type.push_back(i.get_type());
		}
		else if (i.can_evo()) {
			weights.push_back(i.get_rarity());
			index_to_type.push_back(Weapon::evolution_pair.find(i.get_type())->second);
		}
	}
	return index_to_type[poll(weights)];
}
void CGameStateRun::show_inv() {
	inv_detail_frame.show();
	for (int i = 0; i < Weapon::weapon_count(); i++) {
		inv_detail_item_icons[i].show(Weapon::all_weapon[i].get_type());
		for (int j = 0; j < Weapon::all_weapon[i].get_max_level(); j++)
			inv_detail_item_knots[i][j][Weapon::all_weapon[i].get_level() > j].show();
	}
	for (int i = 0; i < Passive::passive_count(); i++) {
		inv_detail_item_icons[i + 6].show(Passive::all_passive[i].get_type());
		for (int j = 0; j < Passive::all_passive[i].get_max_level(); j++)
			inv_detail_item_knots[i + 6][j][Passive::all_passive[i].get_level() > j].show();
	}
}
void CGameStateRun::show_stat(vector<stat_struct> &player_stats, CPoint player_pos) {
	stat_frame.show();
	player_stats = player.get_stats_string();
	int cnt = 0;
	for (int i = 0; i < 16; i++) {
		text_device.add_text(player_stats[i].name_string, CPoint(-375, -130 + 16 * cnt) + player_pos, 1, FONT_12x08, ALIGN_LEFT);
		text_device.add_text(player_stats[i].val_string, CPoint(-235, -130 + 16 * cnt) + player_pos, 1, FONT_12x08, ALIGN_RIGHT);
		stat_icon[i].set_base_pos(-385, -130 + 16 * cnt);
		stat_icon[i].show(player_stats[i].type);
		cnt++;
		cnt += (i % 4 == 3); //empty line
	}
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
	
	//polling
	vector<double> weights(2, 0);

	//open chest
	int chest_item_count;
	static bool can_evo = false;
	static int chest_upgrade_chance_0 = 0, chest_upgrade_chance_1 = 0;

	_gamerun_status = _next_status;
	vector <VSObject*> plain_result = {};
	int offset = 300;
	switch (_gamerun_status) {
	case(PLAYING):
		//--------------------------------------------------------
		//playing status
		//--------------------------------------------------------
		timer.resume();
		Damage::damage_device()->update();
		player.update_pos(mouse_pos);
		QuadTree::VSPlain.set_range(-Player::player_dx - offset, -Player::player_dy - offset, (OPEN_AS_FULLSCREEN ? RESOLUTION_X : SIZE_X) + offset, (OPEN_AS_FULLSCREEN ? RESOLUTION_Y : SIZE_Y) + offset);
		for (auto i_enemy : enemy_factory.live_enemy) {
			if (!i_enemy->is_dead()) {
				QuadTree::VSPlain.insert((VSObject*)(i_enemy));
			}
		}
		Weapon::attack();
		Projectile::update_position();
		for (Projectile& proj : Projectile::all_proj) {
			plain_result.clear();
			QuadTree::VSPlain.query_by_type(plain_result, (VSObject*)(&proj), ENEMY);
			for (VSObject* obj : plain_result) {
				proj.collide_with_enemy(*((Enemy*)obj), player.get_duration());
			}
		}
		for (auto 😈: enemy_factory.live_enemy) {
			😈->update_pos(player.get_pos(), timer.get_ticks());
			result = {};
			QuadTree::VSPlain.query_by_type(result, (VSObject*)(😈), ENEMY);
			for (VSObject* obj : result) {
				😈->append_collide(*((Enemy*)obj), 0.75, 0.5);
			}
			😈->update_collide();
			if (😈->is_collide_with(player)) {
				😈->append_collide(player, 1, 0.5);
				😈->update_collide();
				player.hurt(😈->get_power());
				if (player.get_hp_percent() == 0) {
					_next_status = player.get_revival() > 0 ? REVIVE : GAME_OVER;
					return;
				}
			}
		}

		// light source collision
		QuadTree::VSPlain.clear();
		for (auto i : light_source_factory.light_sourse_all){
			if(i->is_enable()){
				QuadTree::VSPlain.insert((VSObject*)(i));
			}
		}
		for (Projectile& proj : Projectile::all_proj) {
			plain_result.clear();
			QuadTree::VSPlain.query_by_type(plain_result, (VSObject*)(&proj), LIGHTSOURCE);
			for (VSObject* obj : plain_result) {
				proj.collide_with_lightsource(*((LightSource*)obj), player.get_duration());
			}
		}
		for(auto i: light_source_factory.light_sourse_all)
			i->update_pos(player.get_pos());
		
		QuadTree::VSPlain.clear();
		// suck xp
		Xp::update_XP_pos(player.get_pickup_range());
		for (auto i : Xp::xp_all) {
			if (is_overlapped(player, *i)) {
				// CAudio::Instance()->Play(1, false);
				i->despawn();
				player.pick_up_xp(i->get_xp_value());
			}
		}
		for (auto i : Chest::chest_all) {
			if (i->is_enable() && is_overlapped(player, *i)) {
				i->despawn();
				can_evo = i->get_can_evo();
				chest_upgrade_chance_0 = i->get_upgrade_chance_0();
				chest_upgrade_chance_1 = i->get_upgrade_chance_1();
				_next_status = OPEN_CHEST;
			}
		}
		for (auto i : LightSourcePickup::LSPickup_all) {
			if (i->is_enable() && is_overlapped(player, *i)) {
				i->despawn();
				double greed = static_cast<double>(player.get_greed()) / 100.0;
				switch (i->get_lightsource_pickup_type()) {
				case COIN:
					GOLD_NUM += static_cast<int> (1. * greed);
					break;
				case COIN_BAG:
					GOLD_NUM += static_cast<int> (10. * greed);
					break;
				case RICH_COIN_BAG:
					GOLD_NUM += static_cast<int> (100. * greed);
					break;
				case ROSARY:
					for (auto 😈 : enemy_factory.live_enemy) {
						😈->hurt(1000);
					}
					break;
				case VACUUM:
					Xp::update_XP_pos(1000);
					break;
				case CHICKEN:
					player.regen(30);
					break;
				case LITTLE_CLOVER:
					player.increase_luck(10);
					break;
				}
			}
		}

		if (!player.is_hurt())
			player.regen();

		if (player.get_exp_percent() == 100)
			_next_status = LEVEL_UP;

		break;
	case(LEVEL_UP):
		//--------------------------------------------------------
		//level up status
		//--------------------------------------------------------
		timer.pause();

		if (level_up_choice[0] != -1)
			break;

		// owned_chance
		weights[1] = 1 + 0.3 * ((player.get_level() & 1) ? 1 : 2) / (double)player.get_luck() * 100;
		weights[0] = 1 - weights[1];

		// poll new level_up choices
		level_up_choice[0] = draw_level_up(poll(weights, true));
		level_up_choice[1] = draw_level_up(poll(weights, true));
		level_up_choice[2] = draw_level_up(false);

		// 4th_choice
		weights[1] = 1 - (1 / (double)player.get_luck() * 100);
		weights[0] = 1 - weights[1];
		level_up_choice[3] = (poll(weights, true)) ? draw_level_up(false) : -1;

		// set which choice can be click
		for (int i = 0; i < 4; i++) {
			if (level_up_choice[i] != -1)
				level_up_button[i].activate_hover = true;
			else
				level_up_button[i].activate_hover = false;
		}

		break;
	case(OPEN_CHEST):
		//--------------------------------------------------------
		// chest status
		//--------------------------------------------------------
		if (chest_item[0] != -1)
			break;

		timer.pause();

		chest_animation.enable_animation();
		// poll chest item count
		weights[1] = (double)chest_upgrade_chance_0 / 100 * (double)player.get_luck() / 100;
		weights[0] = 1 - weights[1];
		chest_item_count = 1;
		TRACE("1:%lf\n", weights[1]);
		if (poll(weights, true))
			chest_item_count = 5;
		else {
			weights[1] = (double)chest_upgrade_chance_1 / 100 * (double)player.get_luck() / 100;
			weights[0] = 1 - weights[1];
			TRACE("2:%lf\n", weights[1]);
			if (poll(weights, true))
				chest_item_count = 3;
		}
		// poll chest item
		for (int i = 0; i < chest_item_count; i++) {
			chest_item[i] = draw_open_chest(can_evo);
			if (chest_item[i] > -1) {
				// -2 means pull empty
				player.obtain_item(chest_item[i]);
			}
		}
		break;
	case (GAME_OVER): case (REVIVE):
		break;
	}
}
void CGameStateRun::OnShow()
{
	CPoint player_pos = player.get_pos();
	map.map_padding(player_pos);
	map.show_map();
	Weapon::show();
	Xp::show();
	Chest::show();
	LightSourcePickup::show();
	coin.show();
	skull.show();
	text_device.add_text(to_string(GOLD_NUM), CPoint(368, -260) + player_pos, 1, FONT_NORM, ALIGN_RIGHT);
	text_device.add_text(to_string(KILL_NUM), CPoint(368, -240) + player_pos, 1, FONT_NORM, ALIGN_RIGHT);
	player.show_skin();
	for(auto 😈: enemy_factory.live_enemy)
		😈->show_skin();
	enemy_factory.update(timer.get_ticks(), player_pos, player.get_level(), player.get_luck(), player.get_curse());
	light_source_factory.update(timer.get_ticks(), player_pos, player.get_luck());
	for (auto i : light_source_factory.light_sourse_all)
		i->show_skin();
	Damage::damage_device()->show_damage();
	xp_bar_cover.show();
	xp_bar.set_base_pos(-8 - (xp_bar.get_width() * (100 - player.get_exp_percent()) / 100), -300 + (xp_bar.get_height() >> 1));
	xp_bar.show();
	xp_bar_frame.show();

	bool is_own;
	string level_up_desc, level_text, type_text;

	vector<stat_struct> player_stats;
	string 🍆;
	switch (_gamerun_status) {
	case(PLAYING):
		inv_slot.show();
		xp_bar.set_selector(0);
		xp_bar.disable_animation();
		button_pause.show();
		for (int i = 0; i < Weapon::weapon_count(); i++)
			inv_icon[i].show(Weapon::all_weapon[i].get_type());
		for (int i = 0; i < Passive::passive_count(); i++)
			inv_icon[i + 6].show(Passive::all_passive[i].get_type());
		hp_bar.set_selector((player.get_hp_percent() - 1) / 5);
		hp_bar.show();
		break;
	case(PAUSE):
		button_resume.show();
		evolution_chart.show(0.4);
		inv_detail_frame.show();
		show_inv();
		stat_frame.show();
		show_stat(player_stats, player_pos);
		break;
	case(LEVEL_UP):
		xp_bar.enable_animation();
		event_background.show();
		inv_detail_frame.show();
		for (int i = 0; i < 4; i++) {
			if (level_up_choice[i] > -1) {
				level_up_button[i].show();
				level_up_icon_frame[i].show();
				level_up_icon[i].show(level_up_choice[i]);

				// find the text
				is_own = false;
				if (level_up_choice[i] < 63) {
					for (auto& w : Weapon::all_weapon) {
						if (w.get_type() == level_up_choice[i]) {
							type_text = w.get_name();
							level_text = "level:" + to_string(w.get_level());
							level_up_desc = w.get_level_up_msg();
							is_own = true;
							break;
						}
					}
					if (!is_own) {
						type_text = Weapon::_base_weapon[level_up_choice[i]].get_name();
						level_text = "New!";
						level_up_desc = Weapon::_base_weapon[level_up_choice[i]].get_level_up_msg(true);
					}
				}
				else {
					for (auto& p : Passive::all_passive) {
						if (p.get_type() == level_up_choice[i]) {
							type_text = p.get_name();
							level_text = "level:" + to_string(p.get_level());
							level_up_desc = p.get_level_up_msg();
							is_own = true;
							break;
						}
					}
					if (!is_own) {
						type_text = Passive(level_up_choice[i]).get_name();
						level_text = "New!";
						level_up_desc = Passive(level_up_choice[i]).get_level_up_msg(true);
					}
				}
				text_device.add_text(type_text, CPoint(-85, -95 + 75 * i) + player_pos, 1, FONT_12x08, ALIGN_LEFT);
				text_device.add_text(level_text, CPoint(70, -95 + 75 * i) + player_pos, 1, FONT_12x08, ALIGN_LEFT);
				text_device.add_text(level_up_desc, CPoint(-130, -70 + 75 * i) + player_pos, 1, FONT_12x08, MULTILINE_LEFT);
			}
		}
		text_device.add_text("Level Up!", CPoint(0, -150) + player_pos, 1, FONT_24x18_B, ALIGN_CENTER);
		if ((level_up_choice[0] > -1) && (level_up_choice[1] > -1) && (level_up_choice[2] > -1) && (level_up_choice[3] == -1)) {
			text_device.add_text("Increase your luck", CPoint(0, 140) + player_pos, 1, FONT_12x08, ALIGN_CENTER);
			text_device.add_text("  for a chance to get 4 choices.", CPoint(0, 160) + player_pos, 1, FONT_12x08, ALIGN_CENTER);
		}
		//inventory detail 
		show_inv();
		// player stat text
		stat_frame.show();
		show_stat(player_stats, player_pos);
		break;
	case(OPEN_CHEST):
		event_background.show();
		chest_animation.start();
		chest_animation.show();
		if (chest_animation.done()) {
			for (int i = 0; i < 5; i++) {
				if (chest_item[i] > -1) {
					chest_item_frame[i].show();
					chest_item_icon[i].show(chest_item[i]);
				}
			}
		}
		break;
	case (GAME_OVER):
		game_over_frame.show();
		game_over_button.show();
		break;
	case (REVIVE):
		game_over_frame.show();
		button_revive.show();
		break;
	}
	text_device.add_text(timer.get_minute_string() + ":" + timer.get_second_string(), CPoint(0, -265) + player_pos, 1, FONT_24x18_B, ALIGN_CENTER);
	text_device.add_text("LV " + to_string(player.get_level()), CPoint(380, -287) + player_pos, 1, FONT_24x18_B, ALIGN_RIGHT);
	text_device.print_all();
}
