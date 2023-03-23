#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "../config.h"
#include "VSObject.h"
#include "Projectile.h"
#include "Weapon.h"
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
	_level = stats[0], _max_level = stats[1], _damage = stats[2],
		_speed = stats[3], _area = (double)(stats[4])/100.0, _rarity = stats[5], _amount = stats[6],
		_duration = stats[7], _pierce = stats[8], _cooldown = stats[9],
		_proj_interval = stats[10], _hitbox_delay = stats[11], _knock_back = stats[12],
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
	case VAMPIRICA:
		_level_up_msg = { "" };
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
							mouse_pos, i * 400, w._type, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay, 
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, 200 + (i - 1) * 400, w._type, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
				}
				else {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - (i * 16) },
							mouse_pos, i * 400, w._type, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, 200 + (i - 1) * 400, w._type, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
				}
			}
			break;

		case (VAMPIRICA):
			for (int i = 0; i < w._amount; i++) {
				Projectile proj = w._base_proj;
				proj.set_create_time(clock());
				if (mouse_pos.x > player_pos.x) {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - (i * 16) },
							mouse_pos, i * 400, w._type, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, 400 + (i - 1) * 400, w._type, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
				}
				else {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - (i * 16) },
							mouse_pos, i * 400, w._type, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, 400 + (i - 1) * 400, w._type, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
				}
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
		Projectile p(proj_vec, BLACK);
		switch (type) {
		case WHIP:
			p.set_default_direct(RIGHT);
			p.set_animation(w._proj_interval << 1, true, 0);
			p.enable_animation();
			break;
		case VAMPIRICA:
			p.set_default_direct(RIGHT);
			p.set_animation(w._proj_interval << 1, true, 0);
			p.enable_animation();
			break;
		}
		w._base_proj = p;
		Weapon::_base_weapon[ type ] = w;
	}
}
int Weapon::get_damage() {
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
	return _level == _max_level;
}
int Weapon::get_pierce() {
	return _pierce;
}
int Weapon::get_kb() {
	return _knock_back;
}
int Weapon::weapon_count() {
	return static_cast<int>  (Weapon::_base_weapon.size());
}
bool Weapon::is_evo_weapon() {
	return this->_type >= 32;
}
map <int, Weapon> Weapon::_base_weapon;

map <int, int> Weapon::evolution_pair = { 
	{WHIP, VAMPIRICA}, {MAGIC_MISSILE, HOLY_MISSLE},{KNIFE, THOUSAND},
	{AXE, SCYTHE},{CROSS, HEAVENSWORD},{HOLYBOOK, VESPERS},
	{FIREBALL, HELLFIRE},{GARLIC, VORTEX},{HOLYWATER, BORA},
	{DIAMOND, ROCHER},{LIGHTNING, LOOP},{PENTAGRAM, SIRE},
	{SILF, SILF3},{SILF2, SILF3},{GUNS, GUNS3},{GUNS2, GUNS3},
	{GATTI, STIGRANGATTI},{SONG, MANNAGGIA},{TRAPING, TRAPANO2},
	{LANCET, CORRIDOR}, {LAUREL, SHROUD},{VENTO, VENTO2},
	{CANDYBOX, CANDYBOX2}, {TRIASSO1, TRIASSO2}, {TRIASSO2, TRIASSO3},
	{VICTORY, SOLES}
};
deque<Weapon> Weapon::all_weapon = {};