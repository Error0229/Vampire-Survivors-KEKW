#pragma once 
#include "Projectile.h"
#include <memory>
enum weapon_names {
	WHIP = 0, MAGIC_MISSILE, KNIFE, AXE, CROSS, HOLYBOOK,
	FIREBALL, GARLIC, HOLYWATER, DIAMOND, LIGHTNING,
	PENTAGRAM, SILF, SILF2, GUNS, GUNS2, GATTI, SONG,
	TRAPING, LANCET, LAUREL, VENTO, BONE, CHERRY,
	CART2, FLOWER, LAROBBA, JUBILEE, TRIASSO1, CANDYBOX,
	VICTORY, MISSPELL
};
enum evolution_weapon_names {
	VAMPIRICA = 32, HOLY_MISSLE, THOUSAND, SCYTHE, 
	HEAVENSWORD, VESPERS, HELLFIRE, VORTEX, BORA, 
	ROCHER,	LOOP, SIRE, STIGRANGATTI, MANNAGGIA,
	TRAPANO2, MISSPELL2, CORRIDOR, SHROUD, TRIASSO2,
	TRIASSO3, GUNS3, SILF3, VENTO2, SOLES, CANDYBOX2
};
class Weapon : public VSObject {
public:
	Weapon();
	Weapon(int, char*, vector<char*>, vector<int>);
	Weapon(int, char*, vector<int>);
	~Weapon();
	void show_proj();
	void update_proj(CPoint, int, int, int);
	void upgrade();
	deque<Projectile>& get_all_proj();
	static void load_weapon_stats();
	static map<int, Weapon> _base_weapon; //name, stats
	int get_damage();
	int get_duration();
	int get_rarity();
	int get_type();
	bool is_max_level();
	int get_kb();
	int get_pierce();
	static map <int, int> evolution_pair;
	void attack();
	static deque<Weapon> all_weapon;
protected:
	Projectile _base_proj;
	deque<Projectile> _proj_q;
	deque<Projectile> _unused_proj_q;
	string _name;
	int _type, _level, _max_level, _damage, _speed, _rarity,   
		_amount, _duration, _pierce, _cooldown, _proj_interval,
		_hitbox_delay, _knock_back, _pool_limit, _chance, 
		_crit_multi, _block_by_wall;
	double _area;
	vector<string> _level_up_msg;
};
