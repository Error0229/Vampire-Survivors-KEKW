#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "../config.h"
#include "VSObject.h"
#include "VSUtil.h"
#include "QuadTree.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Passive.h"
#include <fstream>
#include <sstream>
#include <random>
using namespace game_framework;
#define var_name(var) (#var)
Weapon::Weapon()
{
	obj_type = WEAPON;
}
Weapon::~Weapon()
{
}
Weapon::Weapon(int type, char* skin, vector<double> stats) {
	this->load_skin(skin);
	this->_type = type;
	_level = static_cast<int>(stats[0]), _max_level = static_cast<int>(stats[1]), _damage = stats[2],
		_speed = static_cast<int>(stats[3] * stats[19]), _area = stats[4], _rarity = static_cast<int>(stats[5]),
		_amount = static_cast<int>(stats[6]), _duration = static_cast<int>(stats[7]), _pierce = static_cast<int>(stats[8]),
		_cooldown = static_cast<int>(stats[9]), _proj_interval = static_cast<int>(stats[10]),
		_hitbox_delay = static_cast<int>(stats[11]), _knock_back = stats[12], _pool_limit = static_cast<int>(stats[13]),
		_chance = static_cast<int>(stats[14]), _crit_multi = static_cast<int>(stats[15]),
		_block_by_wall = static_cast<int>(stats[16]), _evolution_type = static_cast<int>(stats[17]),
		_evolution_require = static_cast<int>(stats[18]);
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
	_modifier = {
		{"might", 100},
		{"cooldown", 100},
		{"proj_speed", 100},
		{"duration", 100},
		{"amount", 100},
		{"area", 100}
	};
	switch (_type) {
	case WHIP:
		_level_up_msg = {
			"",
			"Attacks horizontally, passes through enemies." ,
			"Fires 1 more projectile.",
			"Base damage up by 5.",
			"Base damage up by 5. Base area up by 10%.",
			"Base damage up by 5.",
			"Base damage up by 5. Base area up by 10%.",
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
	case AXE:
		_level_up_msg = {
			"",
			"High damage, high Area scaling.",
			"Fires 1 more projectile.",
			"Base Damage up by 20.",
			"Passes through 2 more enemies.",
			"Fires 1 more projectile.",
			"Base Damage up by 20.",
			"Passes through 2 more enemies.",
			"Base Damage up by 20."
		};
		break;
	case CROSS:
		_level_up_msg = {
			"",
			"Aims at nearest enemy, has boomerang effect.",
			"Base Damage up by 10.",
			"Base Area up by 10%. Base Speed up by 25%.",
			"Fires 1 more projectile.",
			"Base Damage up by 10.",
			"Base Area up by 10%. Base Speed up by 25%.",
			"Fires 1 more projectile.",
			"Base Damage up by 10."
		};
		break;
	case HOLYBOOK:
		_level_up_msg = {
			"",
			"Orbits around the character.",
			"Fires 1 more projectile.",
			"Base Area up by 25%. Base Speed up by 30%.",
			"Effect lasts 0.5 seconds longer. Base Damage up by 10.",
			"Fires 1 more projectile.",
			"Base Area up by 25%. Base Speed up by 30%.",
			"Effect lasts 0.5 seconds longer. Base Damage up by 10.",
			"Fires 1 more projectile."
		};
		break;
	case FIREBALL:
		_level_up_msg = {
			"",
			"Fires at a random enemy, deals heavy damage.",
			"Base Damage up by 10.",
			"Base Damage up by 10. Base Speed up by 20%.",
			"Base Damage up by 10.",
			"Base Damage up by 10. Base Speed up by 20%.",
			"Base Damage up by 10.",
			"Base Damage up by 10. Base Speed up by 20%.",
			"Base Damage up by 10."
		};
		break;
	case GARLIC:
		_level_up_msg = {
			"",
			"Damages nearby enemies. Reduces resistance to knockback and freeze.",
			"Base Area up by 40%. Base Damage up by 2.",
			"Cooldown reduced by 0.1 seconds. Base Damage up by 1.",
			"Base Area up by 20%. Base Damage up by 1.",
			"Cooldown reduced by 0.1 seconds. Base Damage up by 2.",
			"Base Area up by 20%. Base Damage up by 1.",
			"Cooldown reduced by 0.1 seconds. Base Damage up by 1.",
			"Base Area up by 20%. Base Damage up by 1."
		};
		break;
	case HOLYWATER:
		_level_up_msg = {
			"",
			"Generates damaging zones.",
			"Fires 1 more projectile. Base Area up by 20%.",
			"Effect lasts 0.5 seconds longer. Base Damage up by 10.",
			"Fires 1 more projectile. Base Area up by 20%.",
			"Effect lasts 0.3 seconds longer. Base Damage up by 10.",
			"Fires 1 more projectile. Base Area up by 20%.",
			"Effect lasts 0.3 seconds longer. Base Damage up by 10.",
			"Base Area up by 20%. Base Damage up by 5."
		};
		break;
	case DIAMOND:
		_level_up_msg = {
			"",
			"Passes through enemies, bounces around.",
			"Base Damage up by 5. Base Speed up by 20%.",
			"Effect lasts 0.3 seconds longer. Base Damage up by 5.",
			"Fires 1 more projectile.",
			"Base Damage up by 5. Base Speed up by 20%.",
			"Effect lasts 0.3 seconds longer. Base Damage up by 5.",
			"Fires 1 more projectile.",
			"Effect lasts 0.5 seconds longer."
		};
		break;
	case VAMPIRICA:
		_level_up_msg = { "", "Can deal critical damage and absorb HP." };
		break;
	case HOLY_MISSILE:
		_level_up_msg = { "", "Fires with no delay." };
		break;
	case THOUSAND:
		_level_up_msg = { "", "Evolved Knife. Fires with no delay." };
		break;
	case SCYTHE:
		_level_up_msg = { "", "Evolved Axe. Passes through enemies." };
		break;
	case HEAVENSWORD:
		_level_up_msg = { "", "Evolved Cross. Can do critical damage." };
		break;
	case VESPERS:
		_level_up_msg = { "", "Evolved King Bible. Never Ends." };
		break;
	case HELLFIRE:
		_level_up_msg = { "", "Evolved Fire Wand. Passes through enemies." };
		break;
	case VORTEX:
		_level_up_msg = { "", "Evolved Garlic. Steals hearts. Power increases when recovering HP."};
		break;
	case BORA:
		_level_up_msg = { "", "Evolved Santa water. Damaging zones follow you and grow when they move." };
		break;
	case ROCHER:
		_level_up_msg = { "", "Evolved Runetracer. Explodes when bouncing and in retaliation." };
		break;
	}
}

void Weapon::attack() {
	CPoint mouse_pos;
	GetCursorPos(&mouse_pos);
	HWND targetWindow = FindWindow(NULL, GAME_TITLE);
	ScreenToClient(targetWindow, &mouse_pos);
	CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
	mouse_pos += CPoint{-VSObject::player_dx, -VSObject::player_dy}; 
	static bool VESPER_attack = false;
	static std::random_device rnd;
	static std::mt19937 e2(rnd());
	static std::uniform_real_distribution<> dist(0.4 * MATH_PI, 0.6 * MATH_PI);
	static std::uniform_real_distribution<> dist2(0, 2 * MATH_PI);
	// create projectile
	for (Weapon& w : all_weapon) {
		int factor = 0;
		if (w._type == HOLYBOOK || w._type == ROCHER) {
			factor = w._duration;
		}
		if (clock() - w._last_time_attack < w._cooldown + factor) {
			continue;
		}
		w._last_time_attack = clock();

		switch (w._type) {
		case (WHIP): case (VAMPIRICA):
			for (int i = 0; i < w._amount; i++) {
				Projectile& proj = Projectile::pool.get_obj(w._type);
				if (mouse_pos.x > player_pos.x) {
					if (!(i & 1)) {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - (i * 16) },
							mouse_pos, w._type, i * 2 * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, w._type, w._proj_interval + (i - 1) * 2 * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
				}
				else {
					if (!(i & 1)) {
						Projectile::create_projectile(proj, { player_pos.x - (proj.get_width() >> 1) + (16) , player_pos.y - (i * 16) },
							mouse_pos, w._type, i * 2 * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, LEFT != proj.get_direct());
					}
					else {
						Projectile::create_projectile(proj, { player_pos.x + (proj.get_width() >> 1) - (16) , player_pos.y - ((i - 1) * 16) },
							mouse_pos, w._type, w._proj_interval + (i - 1) * 2 * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, RIGHT != proj.get_direct());
					}
				}
			}
			break;

		case MAGIC_MISSILE: case HOLY_MISSILE: case CROSS: case HEAVENSWORD:  // reducing work
			for (int i = 0; i < w._amount; i++) {
				Projectile& proj = Projectile::pool.get_obj(w._type);
				CPoint target = player_pos;
				if (i == 0) {
					proj.set_pos(player_pos);
					int min_dis = 1000000000;
					QuadTree::VSPlain.query_nearest_enemy_pos(target, (VSObject*)(&proj), min_dis);
					double rad = atan2(target.y - player_pos.y, target.x - player_pos.x);
					proj.set_rotation(rad);
					proj.set_target_vec((target != player_pos ? target - player_pos : (mouse_pos.x > player_pos.x ? CPoint{1000, 0} : CPoint{ -1000, 0})));
				}
				Projectile::create_projectile(proj, player_pos, target, w._type, i * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
					w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, false);
			}
			break;
		case KNIFE: case THOUSAND:
			for (int i = 0; i < w._amount; i++) {
				Projectile& proj = Projectile::pool.get_obj(w._type);
				if (i == 0) {
					proj.set_target_vec(mouse_pos - player_pos);
					double rotate = atan2(mouse_pos.y - player_pos.y, mouse_pos.x - player_pos.x);
					proj.set_rotation(rotate);
				}
				int x_factor = (mouse_pos.x > player_pos.x ? 1 : -1);
				int y_factor = (mouse_pos.y > player_pos.y ? 1 : -1);
				Projectile::create_projectile(proj, { player_pos.x + x_factor * (rand()%9)   , player_pos.y + y_factor * (2 * rand()%9) },
							mouse_pos, w._type, i * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, false);
			}
			break;
		case AXE:
			for (int i = 0; i < w._amount; i++) {
				Projectile& proj = Projectile::pool.get_obj(AXE);
				double angle = dist(e2);
				proj.set_angle(angle);
				Projectile::create_projectile(proj, player_pos, player_pos, w._type, i * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, false);
			}
			break;
		case HOLYBOOK: {
			double angle = 2 * MATH_PI / w._amount, current, x, y;
			double initial = -MATH_PI / 2;
			CPoint origin;
			for (int i = 0; i < w._amount; i++) {
				Projectile& proj = Projectile::pool.get_obj(w._type);
				current = initial + angle * i;
				x = cos(current) * w._area * 50;
				y = sin(current) * w._area * 50;
				origin = { player_pos.x + static_cast<int>(x), player_pos.y + static_cast<int>(y) };
				proj.set_angle(sqrt(x * x + y * y));
				Projectile::create_projectile(proj, origin, origin, w._type, i * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, false);
			}

		}	break;
		case FIREBALL: case HELLFIRE:{
			double mid = dist2(e2);
			double da = MATH_PI / 5 / w._amount, angle, x, y;
			for (int i = 0; i < w._amount; i++) {
				Projectile& proj = Projectile::pool.get_obj(w._type);
				angle = mid + da * ((i & 1) ? ((i + 1) >> 1) : -((i + 1) >> 1));
				x = cos(angle) * 1000;
				y = sin(angle) * 1000;
				proj.set_target_vec({static_cast<int>(x), static_cast<int>(y)});
				proj.set_rotation(atan2(y , x ));
				Projectile::create_projectile(proj, player_pos, player_pos, w._type, i * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, false);
			}
		}break;
		case GARLIC: case VORTEX: {
			Projectile& proj = Projectile::pool.get_obj(w._type);
			Projectile::create_projectile(proj, player_pos,
							player_pos, w._type,  w._proj_interval, w._area + 2, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, false);
		}break;
		case HOLYWATER: case BORA: {
			for (int i = 0; i < w._amount; i++) {
				Projectile& proj = Projectile::pool.get_obj(w._type);
				CPoint target = player_pos;
				if (i == 0) {
					proj.set_pos(player_pos);
					int min_dis = 1000000000;
					QuadTree::VSPlain.query_nearest_enemy_pos(target, (VSObject*)(&proj), min_dis);
					target = ((target != player_pos) ? (target) : (player_pos + CPoint(rand() % 400 - 200, rand() % 400 - 200)));
					proj.set_target_vec(target - player_pos);
				}
				Projectile::create_projectile(proj, player_pos, target, w._type, i * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
					w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, false);
			}
		}break;
		case DIAMOND: case ROCHER: {
			double angle;
			for (int i = 0; i < w._amount; i++) {
				angle = dist2(e2);
				Projectile& proj = Projectile::pool.get_obj(w._type);
				proj.set_rotation(angle);
				int x = static_cast<int>(cos(angle) * 1000);
				int y = static_cast<int>(sin(angle) * 1000);
				proj.set_target_vec({ x, y });
				Projectile::create_projectile(proj, player_pos, player_pos + CPoint{ x,y }, w._type, i * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
										w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, false);
			}
		}break;
		case VESPERS: {
			if (VESPER_attack)
				break;
			double angle = 2 * MATH_PI / 26, current, x, y;
			double initial = -MATH_PI / 2;
			CPoint origin;
			for (int i = 0; i < 26; i++) {
				Projectile& proj = Projectile::pool.get_obj(w._type);
				current = initial + angle * i;
				x = cos(current) * w._area * 80;
				y = sin(current) * w._area * 80;
				origin = { player_pos.x + static_cast<int>(x), player_pos.y + static_cast<int>(y) };
				proj.set_angle(sqrt(x * x + y * y));
				proj.set_selector(i);
				Projectile::create_projectile(proj, origin, origin, w._type, i * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, false);
			}
			VESPER_attack = true;
		}break;
		case SCYTHE: {
			double angle = 2 * MATH_PI / w._amount, current, x, y;
			double initial = -MATH_PI / 2; 
			CPoint target;
			for (int i = 0; i < w._amount; i++) {
				Projectile& proj = Projectile::pool.get_obj(w._type);
				current = initial + angle * i;
				x = cos(current) * 1000;
				y = sin(current) * 1000;
				target = { static_cast<int>(x), static_cast<int>(y) };
				proj.set_target_vec(target);
				Projectile::create_projectile(proj, player_pos, player_pos, w._type, i * w._proj_interval, w._area, w._damage, w._speed, w._duration, w._pierce, w._proj_interval, w._hitbox_delay,
							w._knock_back, w._pool_limit, w._chance, w._crit_multi, w._block_by_wall, false);
			}
		}	break;
		}

	}
}
void Weapon::show() {
	Projectile::show();
}
void Weapon::update_weapon_stats(int might, int cooldown, int proj_speed, int duration, int amount, int area) {
	_modifier["might"] = might;
	_modifier["cooldown"] = cooldown;
	_modifier["proj_speed"] = proj_speed;
	_modifier["duration"] = duration;
	_modifier["amount"] = amount;
	_modifier["area"] = area;
	recalaulte_stat();
}
void Weapon::recalaulte_stat() {
	_damage = any_cast<double>(_base_stats["damage"]) * _modifier["might"] / 100;
	_cooldown = any_cast<int>(_base_stats["cooldown"]) * _modifier["cooldown"] / 100;
	_speed = any_cast<int>(_base_stats["speed"]) * _modifier["proj_speed"] / 100;
	_duration = any_cast<int>(_base_stats["duration"]) * _modifier["duration"] / 100;
	_amount = any_cast<int>(_base_stats["amount"]) + _modifier["amount"];
	_area = any_cast<double>(_base_stats["area"]) * _modifier["area"] / 100;
}
void Weapon::update_all_weapon_stats(int might, int cooldown, int proj_speed, int duration, int amount, int area) {
	for (Weapon& w : all_weapon) {
		w.update_weapon_stats(might, cooldown, proj_speed, duration, amount, area);
	}
}
void Weapon::modify_base(string type, double effect) {
	if (type == "damage") {
		_base_stats[type] = any_cast<double>(_base_stats[type]) + effect;
	}
	else if (type == "speed" ) {
		_base_stats[type] = static_cast<int>(static_cast<double>(any_cast<int>(_base_stats[type])) * (100.0+effect)/100.0);
	}
	else if (type == "area") {
		_base_stats[type] = any_cast<double>(_base_stats[type]) * (100.0 + effect) / 100.0;
	}
	else {
		_base_stats[type] = any_cast<int>(_base_stats[type]) + static_cast<int>(effect);
	}
}
void Weapon::upgrade(int weapon_type) {
	for (Weapon& w : all_weapon) {
		if (w._type == weapon_type) {
			w.upgrade();
			return;
		}
	}
}
void Weapon::upgrade()
{
	VS_ASSERT(!this->is_evo_weapon(), "this weapon is evolution weapon, cannot be upgrade")
	_level++;
	switch (_type) {
	case WHIP:
		switch (_level) {
		case 2:
			modify_base("amount", 1);
			break;
		case 3:
			modify_base("damage", 5);
			break;
		case 4:
			modify_base("damage", 5);
			modify_base("area", 0.1);
			break;
		case 5:
			modify_base("damage", 5);
			break;
		case 6:
			modify_base("damage", 5);
			modify_base("area", 0.1);
			break;
		case 7:
			modify_base("damage", 5);
			break;
		case 8:
			modify_base("damage", 5);
			break;
		}
		break;
	case MAGIC_MISSILE:
		switch (_level) {
		case 2:
			modify_base("amount", 1);
			break;
		case 3:
			modify_base("cooldown", -200);
			break;
		case 4:
			modify_base("amount", 1);
			break;
		case 5:
			modify_base("damage", 10);
			break;
		case 6:
			modify_base("amount", 1);
			break;
		case 7:
			modify_base("pierce", 1);
			break;
		case 8:
			modify_base("damage", 10);
			break;
		}
	case KNIFE:
		switch (_level) {
		case 2:
			modify_base("amount", 1);
			break;
		case 3:
			modify_base("amount", 1);
			modify_base("damage", 5);
			break;
		case 4:
			modify_base("amount", 1);
			break;
		case 5:
			modify_base("pierce", 1);
			break;
		case 6:
			modify_base("damage", 5);
			break;
		case 7:
			modify_base("amount", 1);
			modify_base("damage", 5);
			break;
		case 8:
			modify_base("pierce", 1);
			break;
		}
		break;
	case AXE:
		switch (_level) {
		case 2:
			modify_base("amount", 1);
			break;
		case 3:
			modify_base("damage", 20);
			break;
		case 4:
			modify_base("pierce", 2);
			break;
		case 5:
			modify_base("amount", 1);
			break;
		case 6:
			modify_base("damage", 20);
			break;
		case 7:
			modify_base("pierce", 2);
			break;
		case 8:
			modify_base("damage", 20);
			break;
		}
		break;
	case CROSS:
		switch (_level) {
		case 2:
			modify_base("damage", 10);
			break;
		case 3:
			modify_base("area", 10);
			modify_base("speed", 25);
			break;
		case  4:
			modify_base("amount", 1);
			break;
		case 5:
			modify_base("damage", 10);
			break;
		case 6:
			modify_base("area", 10);
			modify_base("speed", 25);
			break;
		case 7:
			modify_base("amount", 1);
			break;
		case 8:
			modify_base("damage", 10);
			break;
		}
		break;
	case HOLYBOOK:
		switch (_level) {
		case 2:
			modify_base("amount", 1);
			break;
		case 3:
			modify_base("area", 25);
			modify_base("speed", 30);
			break;
		case 4:
			modify_base("duration", 500);
			modify_base("damage", 10);
			break;
		case 5:
			modify_base("amount", 1);
			break;
		case 6:
			modify_base("area", 25);
			modify_base("speed", 30);
			break;
		case 7:
			modify_base("duration", 500);
			modify_base("damage", 10);
			break;
		case 8:
			modify_base("amount", 1);
			break;
		}
		break;
	case FIREBALL:
		switch(_level) {
		case 2:
			modify_base("damage", 10);
			break;
		case 3:
			modify_base("damage", 10);
			modify_base("speed", 20);
			break;
		case 4:
			modify_base("damage", 10);
			break;
		case 5:
			modify_base("damage", 10);
			modify_base("speed", 20);
			break;
		case 6:
			modify_base("damage", 10);
			break;
		case 7 :
			modify_base("damage", 10);
			modify_base("speed", 20);
			break;
		case 8:
			modify_base("damage", 10);
			break;
		}
		break;
	case GARLIC:
		switch (_level) {
		case 2:
			modify_base("area", 40);
			modify_base("damage", 2);
			break;
		case 3: case 7:
			modify_base("cooldown", -100);
			modify_base("damage", 1);
			break;
		case 4: case 6: case 8:
			modify_base("area", 20);
			modify_base("damage", 2);
			break;
		case 5:
			modify_base("cooldown", -100);
			modify_base("damage", 2);
			break;
		}
		break;
	case HOLYWATER:
		switch (_level) {
		case 2: case 4: case 6:
			modify_base("amount", 1);
			modify_base("area", 20);
			break;
		case 3:
			modify_base("duration", 500);
			modify_base("damage", 10);
			break;
		case 5: case 7:
			modify_base("duration", 300);
			modify_base("damage", 10);
			break;
		case 8:
			modify_base("area", 20);
			modify_base("damage", 5);
			break;
		}
		break;
	case DIAMOND:
		switch (_level) {
		case 2: case 5:
			modify_base("damage", 5);
			modify_base("speed", 20);
			break;
		case 3: case 6:
			modify_base("duration", 300);
			modify_base("damage", 5);
			break;
		case 4: case 7:
			modify_base("amount", 1);
			break;
		case 8:
			modify_base("duration", 500);
			break;
		}
	}
	
	recalaulte_stat();
}
void Weapon::load_weapon_stats() {
	ifstream file("source/game/VSclass/weapon_stats.csv");
	int type;
	string header, line, skin_file, base_proj, name, token;
	getline(file, header); // no use

	while (getline(file, line)) {
		vector <string> proj_vec;
		stringstream ss(line);
		vector<double> stats;
		getline(ss, skin_file, ',');
		getline(ss, base_proj, ',');
		while (token != base_proj) {
			token = base_proj.substr(0, base_proj.find_first_of(" "));
			base_proj = base_proj.substr(base_proj.find_first_of(" ") + 1);
			proj_vec.emplace_back(token);
		}
		getline(ss, name, ',');
		getline(ss, token, ',');
		type = stoi(token);
		while (getline(ss, token, ',')) {
			stats.push_back(stod(token));
		}
		Weapon w = Weapon(type, const_cast<char*>(skin_file.c_str()), stats);
		w._name = name;
		Projectile p(type, proj_vec);
		switch (type) {
		case WHIP: case VAMPIRICA:
			p.load_mirror_skin();
			p.set_default_direct(RIGHT);
			p.set_animation(200, true, 0);
			p.enable_animation();
			break;
		case MAGIC_MISSILE:	case HOLY_MISSILE: case KNIFE: case THOUSAND: case FIREBALL: case HELLFIRE: case DIAMOND: case ROCHER:
			p.load_rotation();
			break;
		case AXE: 
			p.load_rotation();
			p.set_animation(50, false, 0);
			p.enable_animation();
			break;
		case CROSS: case HOLYWATER: case BORA:
			p.load_rotation();
			p.set_animation(30, false, 0);
			p.enable_animation();
			break;
		case HOLYBOOK: 
			break;
		case GARLIC: case VORTEX:
			p.load_rotation();
			p.set_animation(250, false, 0);
			p.enable_animation();
			break;
		case VESPERS:
			p.set_animation(100,false, 0);
			p.enable_animation();
			break;
		case SCYTHE:
			p.load_rotation();
			p.set_animation(10, false, 0);
			p.enable_animation();
			break;
		case HEAVENSWORD:
			p.load_rotation();
			p.set_animation(30, false, 0);
			p.disable_animation();
			break;
		
		}
		w._base_proj = p;
		Projectile::template_proj[type] = p;
		Projectile::pool.add_obj(type, w._pool_limit);
		Weapon::_base_weapon[type] = w;
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
int Weapon::get_level() {
	return _level;
}
string Weapon::get_level_up_msg(bool is_new) {
	return (is_new) ? (_level_up_msg[1]) : (_level_up_msg[_level + 1]);
}
bool Weapon::is_max_level() {
	return _level >= _max_level;
}
int Weapon::get_max_level() {
	return _max_level;
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
	{SILF3, SILF * 100 + SILF2}, {GUNS3, GUNS * 100 + GUNS2},
	{STIGRANGATTI, GATTI},{MANNAGGIA, SONG},{TRAPANO2, TRAPING},
	{CORRIDOR, LANCET}, {SHROUD, LAUREL},{VENTO2, VENTO},
	{CANDYBOX2, CANDYBOX}, {TRIASSO2, TRIASSO1}, {TRIASSO3, TRIASSO2},
	{SOLES, VICTORY}
};

deque<Weapon> Weapon::all_weapon = {};