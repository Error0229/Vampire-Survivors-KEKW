#pragma once
#include "Enemy.h"
class Projectile : public VSObject
{
public:
	Projectile();
	Projectile(vector<char*> filename, COLORREF color = BLACK);
	~Projectile();
	void set_delay(int);
	void set_create_time(clock_t);
	bool operator < (const Projectile& rhs) const;
	Projectile& operator = (const Projectile& rhs) {
		this->_position = rhs._position;
		this->_skin = rhs._skin;
		return *this;
	}
	void set_player_origin_pos(CPoint);
	void WHIP_transition();
	void collide_with_enemy(Enemy& ,int damage, int duration, int weapon_kb);
	// int _type, _level, _max_level, _damage, _speed, _rarity,  _amount, _duration, _pierce, _cooldown, _proj_interval, _hitbox_delay, _knock_back, _pool_limit, _chance, _crit_multi, _block_by_wall;
	static void create_projectile(Projectile proj, CPoint position, CPoint target_pos, int delay, int type, int damage,int speed, int duration, int pierce, int proj_interval, int hitbox_delay, int knock_back, int pool_limit, int chance, int criti_multi, int block_by_wall, bool is_mirror);
	static void create_projectile(Projectile p);
	static void update_projectile_position();
protected:
	int _type, _level, _max_level, _damage, _speed, _rarity,
		_amount, _duration, _pierce, _proj_interval,
		_hitbox_delay, _knock_back, _pool_limit, _chance,
		_crit_multi, _block_by_wall;
	int _delay;
	double _area;
	clock_t _create_time;
	static deque<Projectile> _all_proj;
	CPoint _origin_player_pos;
};
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
	ROCHER, LOOP, SIRE, STIGRANGATTI, MANNAGGIA,
	TRAPANO2, MISSPELL2, CORRIDOR, SHROUD, TRIASSO2,
	TRIASSO3, GUNS3, SILF3, VENTO2, SOLES, CANDYBOX2
};
