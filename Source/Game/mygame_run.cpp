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
	Enemy::load_template_enemies();
	Icon::load_filename();

	_gamerun_status = PLAYING;
	_next_status = PLAYING;

	player.load_skin({ "resources/character/Dog_01.bmp", "resources/character/Dog_02.bmp" ,"resources/character/Dog_03.bmp" ,"resources/character/Dog_04.bmp" ,"resources/character/Dog_05.bmp" });
	player.set_pos(0, 0);
	player.set_speed(300);
	player.set_default_direct(RIGHT);
	player.set_animation(150, false);
	player.load_bleed();
	// player.acquire_weapon(Weapon::_base_weapon[KNIFE]);
	player.acquire_weapon(WHIP);
	// player.acquire_passive(Passive(POWER));
	player.acquire_passive(POWER);

	map.load_map({ "resources/map/dummy1.bmp" });
	map.set_pos(0, 0);
	QuadTree::VSPlain.clear();

	for (int i = 0; i < 100; i++) {
		enemy.push_back(Enemy::get_template_enemy(SKELETON2));
		xp.push_back(Xp());
		chest.push_back(Chest());
	}
	for ( int i = 0; i < (int)enemy.size(); i++ ) {
		enemy[i].spawn(CPoint(-300 + 30 * i/10, -400 + 40 * i%10));
	}

	event_background.load_skin("resources/ui/event_background.bmp");
	event_background.set_base_pos(0, 0);
	for (int i = 0; i < 4; i++) {
		level_up_button[i].load_skin("resources/ui/event_button.bmp");
		level_up_icon_frame[i].load_skin("resources/ui/frameB.bmp");
		level_up_button[i].set_base_pos(0, -75 + 75 * i);
		level_up_icon_frame[i].set_base_pos(-120, -90 + 75*i);
		level_up_icon[i].set_base_pos(-120, -90 + 75*i);
		level_up_icon[i].load_icon();
		level_up_choice[i] = -1;
	}
	chest_animation.load_skin({"resources/ui/TreasureIdle_01_big.bmp", "resources/ui/TreasureIdle_02_big.bmp" , "resources/ui/TreasureIdle_03_big.bmp" , "resources/ui/TreasureIdle_04_big.bmp" , "resources/ui/TreasureIdle_05_big.bmp" , "resources/ui/TreasureIdle_06_big.bmp" ,"resources/ui/TreasureIdle_07_big.bmp" ,"resources/ui/TreasureIdle_08_big.bmp", "resources/ui/TreasureOpen_01_big.bmp", "resources/ui/TreasureOpen_02_big.bmp" , "resources/ui/TreasureOpen_03_big.bmp" , "resources/ui/TreasureOpen_04_big.bmp" , "resources/ui/TreasureOpen_05_big.bmp" , "resources/ui/TreasureOpen_06_big.bmp" , "resources/ui/TreasureOpen_07_big.bmp" , "resources/ui/TreasureOpen_08_big.bmp" });
	chest_animation.set_animation(100, true);
	chest_animation.set_base_pos(5, 75);
	vector<CPoint> chest_item_pos = { CPoint(0,-50), CPoint(-80,-110), CPoint(80,-110), CPoint(-100,-10), CPoint(100,-10) };
	for (int i = 0; i < 5; i++) {
		chest_item_icon[i].load_icon();
		chest_item_icon[i].set_base_pos(chest_item_pos[i]);
		chest_item_frame[i].load_skin("resources/ui/PrizeBG.bmp");
		chest_item_frame[i].set_base_pos(chest_item_pos[i]);
		chest_item[i] = -1;
	}
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// A kill all enemies
	// B pick up 100 xp, but will not check level up
	switch (nChar) {
	case('A'):
		// Weapon::evolution(WHIP);
		for (int i = 0; i < (int)enemy.size();i++) {
			if (enemy[i].hurt(1000000)) {
				xp[i].spawn(enemy[i].get_pos(), enemy[i].get_xp_value());
				chest[i].spawn(enemy[i].get_pos(), 1);
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
	update_mouse_pos();
	switch (_gamerun_status) {
	case(PLAYING):
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
		for (int i = 0; i < 5; i++)
			chest_item[i] = -1;
		_next_status = PLAYING;
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
	// 0~31: weapon
	//32~62: evo
	//63~83: passive
	if (pull_from_inv) {
		if (Weapon::weapon_count() + player.passive_count() == 1) {
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
	for (int i = 0; i < 2; i++) {
		if (level_up_choice[0] == i || level_up_choice[1] == i || level_up_choice[2] == i || level_up_choice[3] == i)
			continue;
		if ((pull_from_inv && player_items[i] == 1) || (!pull_from_inv && player_items[i] == 0)) {
			weights[i] = Weapon::_base_weapon[i].get_rarity();
			no_weight = false;
		}
	}
	// calc passive weights
	for (int i = 63; i < 84; i++) {
		if (level_up_choice[0] == i || level_up_choice[1] == i || level_up_choice[2] == i || level_up_choice[3] == i)
			continue;
		if ((pull_from_inv && player_items[i] == 1) || (!pull_from_inv && player_items[i] == 0)) {
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
	static bool can_evo=false;

	_gamerun_status = _next_status;
	vector <VSObject*> plain_result = {};
	int offset = 300;
	switch (_gamerun_status) {
	case(PLAYING):
		//--------------------------------------------------------
		//playing status
		//--------------------------------------------------------
		player.update_pos(mouse_pos);
		QuadTree::VSPlain.set_range(-Player::player_dx - offset, -Player::player_dy - offset, (OPEN_AS_FULLSCREEN ? RESOLUTION_X : SIZE_X) + offset, (OPEN_AS_FULLSCREEN ? RESOLUTION_Y : SIZE_Y) + offset);
		for (Enemy& i_enemy : enemy) {
			if (!i_enemy.is_dead() && i_enemy.is_enable()) {
				QuadTree::VSPlain.insert((VSObject*)(&i_enemy));
			}
		}
		Weapon::attack();
		Projectile::update_position();
		for (Projectile& proj : Projectile::all_proj) {
			plain_result.clear();
			QuadTree::VSPlain.query_by_type(plain_result, (VSObject*)(&proj), ENEMY);
			for (VSObject* obj : plain_result) {
				proj.collide_with_enemy(*((Enemy*)obj));
			}
		}
		for (int i = 0; i < (int)enemy.size(); i++) {
			enemy[i].update_pos(player.get_pos());
			result = {};
			QuadTree::VSPlain.query_by_type(result, (VSObject*)(&enemy[i]), ENEMY);
			for (VSObject* obj : result) {
				enemy[i].append_collide(*((Enemy*)obj), 0.75, 0.5);
			}
			enemy[i].update_collide();
			if (enemy[i].is_collide_with(player)) {
				enemy[i].append_collide(player, 1, 0.5);
				enemy[i].update_collide();
				player.hurt(enemy[i].get_power());
			}
		}
		QuadTree::VSPlain.clear();
		// suck xp
		Xp::update_XP_pos(player.get_pickup_range());
		for (Xp& i : Xp::xp_all) {
			if (is_overlapped(player, i)) {
				i.despawn();
				if (player.pick_up_xp(i.get_xp_value()))
					_next_status = LEVEL_UP;
			}
		}
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
		for (auto& i : chest) {
			if (i.is_enable() && is_overlapped(player, i)) {
				i.despawn();
				can_evo = i.get_can_evo();
				_next_status = OPEN_CHEST;
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
		weights[1] = 1 + 0.3 * ((player.get_level() & 1) ? 1 : 2) / (double)player.get_luck() * 100;
		weights[0] = 1 - weights[1];

		// poll new level_up choices
		level_up_choice[0] = draw_level_up(poll(weights, true));
		level_up_choice[1] = draw_level_up(poll(weights, true));
		level_up_choice[2] = draw_level_up(false);

		// 4th_choice
		weights[1] = 1 - (1 / (double)player.get_luck() * 100);
		weights[0] = 1 - weights[1];
		level_up_choice[3] = (poll(weights, true))? draw_level_up(false) : -1;

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
		chest_animation.enable_animation();
		// poll chest item count
		weights[1] = 0.05 * (double)player.get_luck() / 100;
		weights[0] = 1 - weights[1];
		chest_item_count = 1;
		if (poll(weights, true))
			chest_item_count = 5;
		else {
			weights[1] = 0.2 * (double)player.get_luck() / 100;
			weights[0] = 1 - weights[1];
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
	}
}
void CGameStateRun::OnShow()
{
	map.map_padding(player.get_pos());
	map.show_map();
	player.show_skin();
	Weapon::show();
	for (int i = 0; i < (int)enemy.size(); i++) {
		enemy[i].show_skin();
	}
	for (auto& i : xp)
		i.show_skin();
	for(auto& i: chest)
		i.show_skin();
	Xp::show();
	if (_gamerun_status == LEVEL_UP) {
		event_background.show();
		for (int i = 0; i < 4; i++) {
			if (level_up_choice[i]>-1) {
				level_up_button[i].show();
				level_up_icon_frame[i].show();
				level_up_icon[i].show(level_up_choice[i]);
			}
		}
	}
	else if (_gamerun_status == OPEN_CHEST) {
		event_background.show();
		chest_animation.show();
		for (int i = 0; i < 5; i++) {
			if (chest_item[i] > -1) {
				chest_item_frame[i].show();
				chest_item_icon[i].show(chest_item[i]);
			}
		}
	}
}
