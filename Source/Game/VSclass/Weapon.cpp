﻿#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "../config.h"
#include "VSObject.h"
#include "QuadTree.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Passive.h"
#include <fstream>
#include <sstream>
using namespace game_framework;
#define var_name(var) (#var)

Weapon::Weapon()
{
	obj_type = WEAPON;
}
Weapon::~Weapon()
{
}
Weapon::Weapon(int type, char* skin, vector<int> stats) {
	this->load_skin(skin);
	this->_type = type;
	_level = stats[0], _max_level = stats[1], _damage = (double)stats[2] / 100.0,
		_speed = stats[3] * 10, _area = (double)(stats[4])/100.0, _rarity = stats[5], _amount = stats[6],
		_duration = stats[7], _pierce = stats[8], _cooldown = stats[9],
		_proj_interval = stats[10], _hitbox_delay = stats[11], _knock_back = stats[12] / 100.0,
		_pool_limit = stats[13], _chance = stats[14], _crit_multi = stats[15],
		_block_by_wall = stats[16], _evolution_type = stats[17], _evolution_require = stats[18];

	switch (_type) {
	case WHIP:
		_level_up_msg = {
			"",
			"Attacks horizontally, passes through enemies." ,
			"Fires 1 more projectile.",
			"Base damage up by 5."
			"Base damage up by 5. Base area up by 10%."
			"Base damage up by 5."
			"Base damage up by 5. Base area up by 10%."
			"Base damage up by 5.",
			"Base damage up by 5.",
			"Base damage up by 5."
		};
		break;
	case MAGIC_MISSILE:
		_level_up_msg = {
			"",
			"Fires at the nearest enemy.",
			"Fire 1 more projectile.",
			"Cooldown reduced by 0.2 seconds",
			"Fire 1 more projectile.",
			"Base damage up by 10.",
			"Fire 1 more projectile.",
			"Passes through 1 more enemy.",
			"Base damage up by 10.",
		};
		break;
	case VAMPIRICA:
		_level_up_msg = { "", "Can deal critical damage and absorb HP."};
		break;
	case HOLY_MISSILE:
		_level_up_msg = { "", "Fires with no delay."};
		break;

	}
}

void Weapon::attack() {
	CPoint mouse_pos;
	GetCursorPos(&mouse_pos);
	HWND targetWindow = FindWindow(NULL, GAME_TITLE);
	ScreenToClient(targetWindow, &mouse_pos);
	CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
	mouse_pos += CPoint{- VSObject::player_dx, - VSObject::player_dy}; // transfer into game position
	// create projectile
	for (Weapon& w : all_weapon) {
		if (clock() - w._last_time_attack < w._cooldown) {
			continue;
		}
		w._last_time_attack = clock();
		switch (w._type) {
		case (WHIP):
			for (int i = 0; i < w._amount; i++) {
				Projectile proj = w._base_proj;
				proj.set_create_time(clock());
				if ( mouse_pos.x > player_pos.x) {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - (i * 16) }, 
							mouse_pos, w._type, i * 400, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay, 
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, w._type, 200 + (i - 1) * 400, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
				}
				else {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - (i * 16) },
							mouse_pos, w._type, i * 400, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, w._type, 200 + (i - 1) * 400, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
				}
			}
			break;

		case MAGIC_MISSILE:
			for (int i = 0; i < w._amount; i++) {
				Projectile proj = w._base_proj;
				proj.set_pos(player_pos);
				proj.set_create_time(clock());
				CPoint target = player_pos;
				int min_dis = 1000000000;
				QuadTree::VSPlain.query_nearest_enemy_pos(target, (VSObject*)(&proj), min_dis);
				proj.set_target_vec((target != player_pos ? target - player_pos : (mouse_pos.x > player_pos.x ? (1000, 1000) : (-1000, 1000))));
				Projectile::create_projectile(proj, player_pos, (target), w._type, i * w._proj_interval, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
					w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, (target.x > player_pos.x ? RIGHT:LEFT) == proj.get_direct());
			}
			break;
		case (VAMPIRICA):
			for (int i = 0; i < w._amount; i++) {
				Projectile proj = w._base_proj;
				proj.set_create_time(clock());
				if (mouse_pos.x > player_pos.x) {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - (i * 16) },
							mouse_pos, w._type, i * 400, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, w._type, 200 + (i - 1) * 400, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
				}
				else {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - (i * 16) },
							mouse_pos, w._type, i * 400, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, w._type, 200 + (i - 1) * 400, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
				}
			}
			break;
		case HOLY_MISSILE:
			Projectile proj = w._base_proj;
			proj.set_pos(player_pos);
			proj.set_create_time(clock());
			CPoint target = player_pos;
			int min_dis = 1000000000;
			QuadTree::VSPlain.query_nearest_enemy_pos(target, (VSObject*)(&proj), min_dis);
			proj.set_target_vec((target != player_pos ? target - player_pos : (mouse_pos.x > player_pos.x ? (100,10) : (-100,10))));
			for (int i = 0; i < w._amount; i++) {
				Projectile::create_projectile(proj, player_pos, (target), w._type, i * w._proj_interval, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
					w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, (target.x > player_pos.x ? RIGHT : LEFT) == proj.get_direct());
			}
			break;
		}
	}
}
void Weapon::show() {
	Projectile::show();
}
deque<Projectile>& Weapon::get_all_proj() {
	return _proj_q;
}
void Weapon::upgrade()
{
	VS_ASSERT(!this->is_evo_weapon(), "this weapon is evolution weapon, cannot be upgrade");
	_level++;
	switch (_type) {
	case WHIP:
		switch (_level) {
		case 2:
			_amount += 1;
			break;
		case 3:
			_damage += 5;
			break;
		case 4:
			_damage += 5;
			_area += 0.1;
			break;
		case 5:
			_damage += 5;
			break;
		case 6:
			_damage += 5;
			_area += 0.1;
			break;
		case 7:
			_damage += 5;
			break;
		case 8:
			_damage += 5;
			break;
		}
		break;
	case HOLY_MISSILE:
		switch (_level) {
		case 2:
			_amount += 1;
			break;
		case 3:
			_cooldown -= 200;
			break;
		case 4:
			_amount += 1;
			break;
		case 5:
			_damage += 10;
			break;
		case 6:
			_amount += 1;
			break;
		case 7:
			_pierce += 1;
			break;
		case 8:
			_damage += 10;
			break;
		}

	}

	
}
void Weapon::load_weapon_stats() {
	ifstream file("source/game/VSclass/weapon_stats.csv");
	int type;
	string header, line, skin_file, base_proj, name, token;
	getline(file, header); // no use

	while ( getline(file, line) ) {
		vector <char*> proj_vec;
		stringstream ss(line);
		vector<int> stats;
		getline(ss, skin_file, ',');
		getline(ss, base_proj, ',');
		while ( token != base_proj ) {
			token = base_proj.substr(0, base_proj.find_first_of(" "));
			base_proj = base_proj.substr(base_proj.find_first_of(" ") + 1);
			proj_vec.emplace_back(_strdup(token.c_str()));
		}
		getline(ss, name, ',');
		getline(ss, token, ',');
		type = stoi(token);
		while ( getline(ss, token, ',') ) {
			stats.push_back(stoi(token));
		}
		Weapon w = Weapon(type, const_cast<char*>(skin_file.c_str()), stats);
		Projectile p(proj_vec);
		switch (type) {
		case WHIP:
			p.set_default_direct(RIGHT);
			p.set_animation(300, true, 0);
			p.set_life_cycle(300);
			p.enable_animation();
			break;
		case MAGIC_MISSILE:
			p.set_default_direct(RIGHT);
			p.set_life_cycle(-1);
			break;
		case VAMPIRICA:
			p.set_default_direct(RIGHT);
			p.set_animation(300, true, 0);
			p.set_life_cycle(300);
			p.enable_animation();
			break; 
		case HOLY_MISSILE:
			p.set_default_direct(RIGHT);
			p.set_life_cycle(-1);
			break;
		}
		w._base_proj = p;
		Weapon::_base_weapon[ type ] = w;
	}
}
double Weapon::get_damage() {
	return _damage;
}
int Weapon::get_duration() {
	return _duration;
}
int Weapon::get_rarity() {
	return _rarity;
}
int Weapon::get_type() {
	return _type;
}
bool Weapon::is_max_level() {
	return _level >= _max_level;
}
int Weapon::get_pierce() {
	return _pierce;
}
double Weapon::get_kb() {
	return _knock_back;
}
int Weapon::get_evo_passive() {
	return _evolution_require;
}
int Weapon::weapon_count() {
	return static_cast<int>  (Weapon::all_weapon.size());
}
bool Weapon::is_evo_weapon() {
	return this->_type >= 32;
}
bool Weapon::can_evo() {
	if (_level < _max_level || _type >= 32)
		return false;
	for (auto& i : Passive::all_passive) {
		if (i.get_type() == _evolution_require) {
			return true;
		}
	}
	return false;
}
void Weapon::evolution(int type) {
	for (auto& w : all_weapon) {
		if (w._type == Weapon::evolution_pair_reverse.find(type)->second) {
			VS_ASSERT(w.can_evo(), "This weapon can't not be evolve");
			w = Weapon::_base_weapon[type];
			return;
		}
	}
	VS_ASSERT(false, "You don't own this weapon for evolution ><");
}
map <int, Weapon> Weapon::_base_weapon;

map <int, int> Weapon::evolution_pair = { 
	{WHIP, VAMPIRICA}, {MAGIC_MISSILE, HOLY_MISSILE},{KNIFE, THOUSAND},
	{AXE, SCYTHE},{CROSS, HEAVENSWORD},{HOLYBOOK, VESPERS},
	{FIREBALL, HELLFIRE},{GARLIC, VORTEX},{HOLYWATER, BORA},
	{DIAMOND, ROCHER},{LIGHTNING, LOOP},{PENTAGRAM, SIRE},
	{SILF, SILF3},{SILF2, SILF3},{GUNS, GUNS3},{GUNS2, GUNS3},
	{GATTI, STIGRANGATTI},{SONG, MANNAGGIA},{TRAPING, TRAPANO2},
	{LANCET, CORRIDOR}, {LAUREL, SHROUD},{VENTO, VENTO2},
	{CANDYBOX, CANDYBOX2}, {TRIASSO1, TRIASSO2}, {TRIASSO2, TRIASSO3},
	{VICTORY, SOLES}
};
// make a reverse map of evolution_pair
map <int, int> Weapon::evolution_pair_reverse = {
	{VAMPIRICA, WHIP}, {HOLY_MISSILE, MAGIC_MISSILE},{THOUSAND, KNIFE},
	{SCYTHE, AXE},{HEAVENSWORD, CROSS},{VESPERS, HOLYBOOK},
	{HELLFIRE, FIREBALL},{VORTEX, GARLIC},{BORA, HOLYWATER},
	{ROCHER, DIAMOND},{LOOP, LIGHTNING},{SIRE, PENTAGRAM},
	{SILF3, SILF*100 + SILF2}, {GUNS3, GUNS*100 + GUNS2},
	{STIGRANGATTI, GATTI},{MANNAGGIA, SONG},{TRAPANO2, TRAPING},
	{CORRIDOR, LANCET}, {SHROUD, LAUREL},{VENTO2, VENTO},
	{CANDYBOX2, CANDYBOX}, {TRIASSO2, TRIASSO1}, {TRIASSO3, TRIASSO2},
	{SOLES, VICTORY}
};

deque<Weapon> Weapon::all_weapon = {};