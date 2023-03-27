#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "../config.h"
#include "VSObject.h"
#include "QuadTree.h"
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
	_level = stats[0], _max_level = stats[1], _damage = (double)stats[2] / 100.0,
		_speed = stats[3] * 10, _area = (double)(stats[4])/100.0, _rarity = stats[5], _amount = stats[6],
		_duration = stats[7], _pierce = stats[8], _cooldown = stats[9],
		_proj_interval = stats[10], _hitbox_delay = stats[11], _knock_back = stats[12] / 100.0,
		_pool_limit = stats[13], _chance = stats[14], _crit_multi = stats[15],
		_block_by_wall = stats[16], _evolution_type = stats[17], _evolution_require = stats[18];
	_base_stats = {
		{"level", _level},
		{"max_level", _max_level},
		{"damage", _damage},
		{"speed", _speed},
		{"area", _area},
		{"rarity", _rarity},
		{"amount", _amount},
		{"duration", _duration},
		{"pierce", _pierce},
		{"cooldown", _cooldown},
		{"proj_interval", _proj_interval},
		{"hitbox_delay", _hitbox_delay},
		{"knock_back", _knock_back},
		{"pool_limit", _pool_limit},
		{"chance", _chance},
		{"crit_multi", _crit_multi}
	};

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
	case KNIFE:
		_level_up_msg = {
			"", 
			"Fires quickly in the faced direction.", 
			"Fires 1 more projectile.", 
			"Fires 1 more projectile. Base damage up by 5.",
			"Fires 1 more projectile.", 
			"Passes through 1 more enemy.", 
			"Fires 1 more projectile.",
			"Fires 1 more projectile. Base damage up by 5.", 
			"Passes through 1 more enemy."
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
				if (mouse_pos.x > player_pos.x) {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - (i * 16) },
							mouse_pos, w._type, i * 2 * w._proj_interval, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, w._type, w._proj_interval + (i - 1) * 2 * w._proj_interval, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
				}
				else {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - (i * 16) },
							mouse_pos, w._type, i * 2 * w._proj_interval, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, w._type, w._proj_interval + (i - 1) * 2 * w._proj_interval, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
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
		case KNIFE:
			for (int i = 0; i < 10; i++) {
				Projectile proj = w._base_proj;
				proj.set_create_time(clock());
				proj.set_target_vec(mouse_pos - player_pos);
				int x_factor = (mouse_pos.x > player_pos.x ? 1 : -1);
				int y_factor = (mouse_pos.y > player_pos.y ? 1 : -1);
				Projectile::create_projectile(proj, { player_pos.x + x_factor * (2 * i % 4 + i % 2)   , player_pos.y + y_factor * (2 * i % 4 + i % 2)  },
							mouse_pos, w._type, i * w._proj_interval, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, (mouse_pos.x > player_pos.x ? RIGHT : LEFT) != proj.get_direct());

			}
			break;
		case (VAMPIRICA):
			for (int i = 0; i < w._amount; i++) {
				Projectile proj = w._base_proj;
				proj.set_create_time(clock());
				if (mouse_pos.x > player_pos.x) {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - (i * 16) },
							mouse_pos, w._type, i * 2 * w._proj_interval, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, w._type, w._proj_interval + (i - 1) * 2 * w._proj_interval, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
				}
				else {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - (i * 16) },
							mouse_pos, w._type, i * 2 * w._proj_interval, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, w._type, w._proj_interval + (i - 1) * 2 * w._proj_interval, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
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

void Weapon::update_weapon_stats(int might, int cooldown, int proj_speed, int duration, int amount, double area) {
	_damage = any_cast<double>(_base_stats["damage"]) * might / 100;
	_cooldown = any_cast<int>(_base_stats["cooldown"]) * cooldown / 100;	
	_speed = any_cast<int>(_base_stats["speed"]) * proj_speed / 100;
	_duration = any_cast<int>(_base_stats["duration"]) * duration / 100;
	_amount = any_cast<int>(_base_stats["amount"]) + amount;
	_area = any_cast<double>(_base_stats["area"]) * area / 100;
}
void Weapon::update_all_weapon_stats(int might, int cooldown, int proj_speed, int duration, int amount, double area) {
	for (Weapon& w : all_weapon) {
		w.update_weapon_stats(might, cooldown, proj_speed, duration, amount, area);
	}
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
			_base_stats["amount"] = any_cast<int>(_base_stats["amount"]) + 1;
			break;
		case 3:
			_damage += 5;
			_base_stats["damage"] = any_cast<double>(_base_stats["damage"]) + 5;
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
	case KNIFE:
		switch (_level) {
		case 2:
			_amount += 1;
			break;
		case 3:
			_amount += 1;
			_damage += 5;
			break;
		case 4:
			_amount += 1;
			break;
		case 5:
			_pierce += 1;
			break;
		case 6:
			_amount += 5;
			break;
		case 7:
			_amount += 1;
			_damage += 5;
		case 8:
			_pierce += 1;
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
			p.set_animation(300, true, 0);
			p.set_life_cycle(300);
			p.enable_animation();
			break;
		case MAGIC_MISSILE:
			p.set_default_direct(RIGHT);
			p.set_life_cycle(-1);
			break;
		case KNIFE:
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
int Weapon::weapon_count() {
	return static_cast<int>  (Weapon::_base_weapon.size());
}
bool Weapon::is_evo_weapon() {
	return this->_type >= 32;
}
void Weapon::evolution(int type) {
	VS_ASSERT(Weapon::evolution_pair.find(type) != Weapon::evolution_pair.end(), "This weapon can't not be evolve");
	for (auto& w : all_weapon) {
		if (w._type == type) {
			w = Weapon::_base_weapon[Weapon::evolution_pair[type]];
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
deque<Weapon> Weapon::all_weapon = {};