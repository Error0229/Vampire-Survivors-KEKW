#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Projectile.h"
#include "Weapon.h"
#include <fstream>
#include <sstream>
using namespace game_framework;
#define var_name(var) (#var)

Weapon::Weapon()
{
}
Weapon::~Weapon()
{
}
Weapon::Weapon(int type, char* skin, vector<char*> proj, vector<int> stats) {
	this->load_skin(skin);
	this->_base_proj = new Projectile(proj, BLACK);
	this->_type = type;
	_level = stats[ 0 ], _max_level = stats[ 1 ], _damage = stats[ 2 ],
	_speed = stats[ 3 ], _area = stats[ 4 ], _rarity = stats[ 5 ], _amount = stats[ 6 ],
	_duration = stats[ 7 ], _pierce = stats[ 8 ], _cooldown = stats[ 9 ],
	_proj_interval = stats[ 10 ], _hitbox_delay = stats[ 11 ], _knock_back = stats[ 12 ],
	_pool_limit = stats[ 13 ], _chance = stats[ 14 ], _crit_multi = stats[ 15 ],
	_block_by_wall = stats[ 16 ];
}
Weapon::Weapon(int type) {
	
}
void Weapon::update_proj(CPoint player_pos, int player_direction, int player_w, int player_h) {
	for ( Projectile* proj : _proj_set ) {
		switch (this->_type){
		case WHIP:
			for ( int i = 0; i < this->_amount; i++ ) {
				proj->set_is_mirror( ( player_direction != proj->get_direct() ));
				if (( player_direction == RIGHT  && !(i&1) ) || ( player_direction == LEFT && ( i & 1 ) ) ) {
					proj->set_pos({ player_pos.x - ( player_w ) , player_pos.y + ( ( i * player_h ) >> 2 ) });
				}
				else {
					proj->set_pos({ player_pos.x + ( player_w ) , player_pos.y + ( ( i * player_h ) >> 2 ) });
				}
			}
			break;
		
		}
	}
}
void Weapon::show_proj() {
	for ( auto& proj : _proj_set ) {
		proj ->show_skin();
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
		Weapon* w = new Weapon(type, const_cast<char*>(skin_file.c_str()), proj_vec, stats);
		for ( int i = 0; i < w->_amount; i++ ) {
			Projectile* p = new Projectile(proj_vec, BLACK);
			switch ( w->_type ) {
			case WHIP:
				p->set_pos(0, 0);
				p->set_default_direct(RIGHT);
				p->set_animation(w->_proj_interval << 1, false, w->_cooldown);
				p->enable_animation();
				break;

			}
			w->_proj_set.insert(p);

		}
		Weapon::_base_weapon[ type ] = w;
	}
}
map <int, Weapon*> Weapon::_base_weapon;