#pragma once 
#include "Projectile.h"
enum weapon_names {
	WHIP = 0, MAGIC_MISSILE, KNIFE, AXE, CROSS, HOLYBOOK,
	FIREBALL, GARLIC, HOLYWATER, DIAMOND, LIGHTNING,
	PENTAGRAM, SILF, SILF2, GUNS, GUNS2, GATTI, SONG, 
	TRAPING, LANCET, LAUREL, VENTO, BONE, CHERRY,
	CART2, FLOWER, LAROBBA, JUBILEE, TRIASS01, CANDYBOX,
	VICTORY, MISSPELL
};
enum evolution_weapon_names {
	VAMPIRICA = 32, HOLY_MISSLE, THOUSAND, SCYTHE, Heaven_Sword,
	Unholy_Vespers, Hellfire, Soul_Easter, La_Borra, NO_FUTURE,
	Thunder_Loop, Gorgeous_Moon, Vicious_Hunger, Mannajja,
	Valkyrie_Turner, Infinite_Corridor, Crimson_Shroud,
	Bi_Bracelet, Tri_Bracelet, Ashe_of_Muspell, Anima_of_Mortaccio,
	Vandalier, Phieraggi, Fuwalafuwaloo, Sole_Solution, Super_Candybox_II_Turbo,
	Cygnus, Zhar_Ptytsia, Red_Muscle, Twice_Upon_a_Time, Flock_Destroyer
};
class Weapon : public VSObject {
public:
	Weapon();
	Weapon(int, char*, vector<char*>, vector<int>);
	Weapon(int);
	~Weapon();
	void show_proj();
	void update_proj(CPoint, int, int, int);
	static void load_weapon_stats();
	static map<int, Weapon*> _base_weapon; //name, stats
protected:
	Projectile* _base_proj;
	set<Projectile*> _proj_set;
	string _name;
	int _type, _level, _max_level, _damage, _speed, _area, _rarity,   
		_amount, _duration, _pierce, _cooldown, _proj_interval,
		_hitbox_delay, _knock_back, _pool_limit, _chance, 
		_crit_multi, _block_by_wall;  
};
