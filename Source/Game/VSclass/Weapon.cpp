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
		_speed = stats[3], _area = (double)stats[4], _rarity = stats[5], _amount = stats[6],
		_duration = stats[7], _pierce = stats[8], _cooldown = stats[9],
		_proj_interval = stats[10], _hitbox_delay = stats[11], _knock_back = stats[12],
		_pool_limit = stats[13], _chance = stats[14], _crit_multi = stats[15],
		_block_by_wall = stats[16];

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
	}
}
Weapon::Weapon(int type, char* skin, vector<char*> proj, vector<int> stats) {
	this->load_skin(skin);
	this->_base_proj = Projectile(proj, BLACK);
	this->_type = type;
	_level = stats[ 0 ], _max_level = stats[ 1 ], _damage = stats[ 2 ],
	_speed = stats[ 3 ], _area = (double)stats[ 4 ], _rarity = stats[ 5 ], _amount = stats[ 6 ],
	_duration = stats[ 7 ], _pierce = stats[ 8 ], _cooldown = stats[ 9 ],
	_proj_interval = stats[ 10 ], _hitbox_delay = stats[ 11 ], _knock_back = stats[ 12 ],
	_pool_limit = stats[ 13 ], _chance = stats[ 14 ], _crit_multi = stats[ 15 ],
	_block_by_wall = stats[ 16 ];

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
	}
}
void Weapon::update_proj(CPoint player_pos, int player_direction, int player_w, int player_h) {
	for (Projectile &proj : _proj_q ) {
		switch (this->_type){
		case WHIP:
			for ( int i = 0; i < this->_amount; i++ ) {
				proj.set_is_mirror( ( player_direction != proj.get_direct() ));
				if (( player_direction == RIGHT  && !(i&1) ) || ( player_direction == LEFT && ( i & 1 ) ) ) {
					proj.set_pos({ player_pos.x + (proj.get_width() >> 1) - (player_w >> 1) , player_pos.y - ( ( i * player_h ) >> 2 )});
				}
				else {
					proj.set_pos({ player_pos.x - ( proj.get_width() >> 1 ) + ( player_w >> 1 ) , player_pos.y - ( ( i * player_h ) >> 2 ) });
				}
			}
			break;
		}
	}
}
void Weapon::attack() {
	CPoint mouse_pos;
	GetCursorPos(&mouse_pos);
	HWND targetWindow = FindWindow(NULL, GAME_TITLE);
	ScreenToClient(targetWindow, &mouse_pos);
	CPoint player_pos = { -VSObject::player_dx, -VSObject::player_dy };
	mouse_pos += player_pos; // transfer into game position

	for (Weapon& w : all_weapon) {
		switch (w._type) {
		case (WHIP):
			for (int i = 0; i < w._amount; i++) {
				Projectile proj = w._base_proj;
				proj.set_create_time(clock());
				proj.set_player_origin_pos(player_pos);
				if ( mouse_pos.x > player_pos.x) {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - (i * 16) }, 
							mouse_pos, i * 400, _type, _damage, _speed, _duration, _pierce, _proj_interval, _hitbox_delay, 
							_knock_back, _pool_limit, _chance, _crit_multi, _block_by_wall, RIGHT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, 200 + (i - 1) * 400, _type, _damage, _speed, _duration, _pierce, _proj_interval, _hitbox_delay,
							_knock_back, _pool_limit, _chance, _crit_multi, _block_by_wall, LEFT != proj.get_direct());
					}
				}
				else {
					if (i ^ 1) {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - (i * 16) },
							mouse_pos, i * 400, _type, _damage, _speed, _duration, _pierce, _proj_interval, _hitbox_delay,
							_knock_back, _pool_limit, _chance, _crit_multi, _block_by_wall, RIGHT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, 200 + (i - 1) * 400, _type, _damage, _speed, _duration, _pierce, _proj_interval, _hitbox_delay,
							_knock_back, _pool_limit, _chance, _crit_multi, _block_by_wall, LEFT != proj.get_direct());
					}
				}
			}
			break;

		}
	}
}
void Weapon::show_proj() {
	for (Projectile& proj : _proj_q ) {
		proj.show_skin();
	}
}
deque<Projectile>& Weapon::get_all_proj() {
	return _proj_q;
}
void Weapon::upgrade()
{
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
		Weapon w = Weapon(type, const_cast<char*>(skin_file.c_str()), proj_vec, stats);
		Weapon w_new = Weapon(type, const_cast<char*>(skin_file.c_str()), stats);
		Projectile p_new(proj_vec, BLACK);
		switch (type) {
		case WHIP:
			p_new.set_default_direct(RIGHT);
			p_new.set_animation(w._proj_interval << 1, false, w_new._cooldown);
			break;
		}
		w_new._base_proj = p_new;
		for ( int i = 0; i < w._amount; i++ ) {
			Projectile p(proj_vec, BLACK);
			switch ( w._type ) {
			case WHIP:
				p.set_pos(0, 0);
				p.set_default_direct(RIGHT);
				p.set_animation(w._proj_interval << 1, false, w._cooldown);
				p.enable_animation();
				break;

			}
			w._proj_q.emplace_back(p);
		}
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