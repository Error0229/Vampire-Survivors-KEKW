﻿#pragma once
#include "Enemy.h"
#include "ObjPool.h"
#include "LightSource.h"
class Projectile : public VSObject
{
public:
	Projectile();
	Projectile(int);
	Projectile(int type, vector<string> filename, COLORREF color = RGB(1, 11, 111));
	~Projectile();
	void set_delay(int);
	void set_create_time(clock_t);
	bool operator < (const Projectile& rhs) const;
	void show_skin(double factor = 1.0) override;
	void set_offset(CPoint);
	void collide_with_enemy(Enemy&, int duration);
	void collide_with_lightsource(LightSource&);
	void set_rotation(double);
	void set_angle(double angle);
	CPoint get_parabola(double angle, double speed, int time, int pre_y = 1 << 20);
	void load_rotation();

	static void init_projectile(int type, int count);
	static void create_projectile(Projectile& proj, CPoint position, CPoint target_pos, int delay, int type, double area, double damage, int speed, int duration, int pierce, int proj_interval, int hitbox_delay, double knock_back, int pool_limit, int chance, int criti_multi, int block_by_wall, bool is_mirror);
	static void create_projectile(Projectile p);
	static void update_position();
	static void show();
	static void reset();
	static vector<reference_wrapper<Projectile>> all_proj;
	static ObjPool<Projectile> pool;
	static map<int, Projectile> template_proj;
protected:
	int _duration, _pierce, _proj_interval,
		_hitbox_delay, _pool_limit, _chance,
		_crit_multi, _block_by_wall;
	int _delay = 0;
	int pre_y = 1<<20;
	std::map<int, game_framework::CMovingBitmap> _rotation_skin;
	double _area, _damage, _knock_back, _angle;
	bool _is_over = false, _is_start = false, _is_top = false;
	clock_t _create_time;
	// life cycle means after how many frames the projectile will be destroyed -1 means infinite or until it hits something
	CPoint _offset;
};
