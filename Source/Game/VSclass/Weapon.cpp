#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Weapon.h"
#include "Weapon_stat.h"
using namespace game_framework;
#define var_name(var) (#var)
enum weapon_names {
	Whip, Magic_Wand, Knife, Axe, Cross, King_Bible,
	Fire_Wand, Garlic, Santa_water, Runetracer, Lightning_Ring,
	Pentagram, Peacechone, Ebony_Wings, Phiera_Der_Tuphello,
	Eight_The_Sparrow, Gatti_Amari, Song_of_Mana, Shadow_Pinion,
	Clock_Lancet, Laurel, Vento_Sacro, Bone, Cherry_Bomb,
	Carrello, Celestial_Dusting, La_Robba, Greastest_Jubilee,
	Bracelet, Candybox, Victory_Sword, Flame_of_Misspell
};
enum evolution_weapon_names {
	Bloody_Tear, Holy_Wand, Thousand_Edge, Death_Spiral, Heaven_Sword,
	Unholy_Vespers, Hellfire, Soul_Easter, La_Borra, NO_FUTURE,
	Thunder_Loop, Gorgeous_Moon, Vicious_Hunger, Mannajja,
	Valkyrie_Turner, Infinite_Corridor, Crimson_Shroud,
	Bi_Bracelet, Tri_Bracelet, Ashe_of_Muspell, Anima_of_Mortaccio,
	Vandalier, Phieraggi, Fuwalafuwaloo, Sole_Solution, Super_Candybox_II_Turbo,
	Cygnus, Zhar_Ptytsia, Red_Muscle, Twice_Upon_a_Time, Flock_Destroyer
};
Weapon::Weapon()
{
}
Weapon::~Weapon()
{
}
Weapon::Weapon(int type, vector<char*>& skin, vector<char*>& proj, vector<int> stats) {
	this->load_skin(skin);
	this->_base_proj->load_skin(proj);
	this->_type = type;
	_level = stats[ 0 ], _max_level = stats[ 1 ], _damage = stats[ 2 ],
	_speed = stats[ 3 ], _area = stats[ 4 ], _rarity = stats[ 5 ], _amount = stats[ 6 ],
	_duration = stats[ 7 ], _pierce = stats[ 8 ], _cooldown = stats[ 9 ],
	_proj_interval = stats[ 10 ], _hitbox_delay = stats[ 11 ], _knock_back = stats[ 12 ],
	_pool_limit = stats[ 13 ], _chance = stats[ 14 ], _crit_multi = stats[ 15 ],
	_block_by_wall = stats[ 16 ];
}
void Weapon::update_proj() {
	for ( auto& bullet : _proj_set ) {
		bullet->update_pos();
	}
}
void Weapon::show_proj() {
	for ( auto& bullet : _proj_set ) {
		bullet->show_skin();
	}
}