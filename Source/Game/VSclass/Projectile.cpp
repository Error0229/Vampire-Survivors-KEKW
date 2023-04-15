#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "../config.h"
#include "VSObject.h"
#include "QuadTree.h"
#include "Projectile.h"
#include "Enemy.h"
#include "ObjPool.h"
#include "VSUtil.h"

Projectile::Projectile() {
	obj_type = PROJECTILE;
	_is_over = false;
};
Projectile::Projectile(int type, vector<string> filename, COLORREF color) : Projectile() {
	this->_type = type;
	this->_skin.LoadBitmapByString(filename, color);
	this->_file_name = filename;
}
Projectile::Projectile(int type) {
	*this = template_proj[type];
}
Projectile::~Projectile() {
}
bool Projectile::operator < (const Projectile& rhs) const {
	return this->_type < rhs._type;
}
void Projectile::set_offset(CPoint os) {
	_offset = os;
}
void Projectile::init_projectile(int type, int count) {
	pool.add_obj(type, count);
}
void Projectile::set_angle(double angle) {
	_angle = angle;
}
void Projectile::collide_with_enemy(Enemy& 🥵, int player_duration) {
	clock_t now = clock();
	if (_is_over || !is_overlapped((*this), 🥵) || now - 🥵._last_time_got_hit_by_projectile[this->_type] < this->_hitbox_delay) {
		return;
	}
	🥵._is_stun = true;
	🥵._stun_speed = -1.0 * static_cast<double>(🥵._speed) * 🥵._kb * static_cast<double>(player_duration) * this->_knock_back / 100.0;
	🥵._last_time_got_hit = now;
	🥵._last_time_got_hit_by_projectile[this->_type] = now;
	this->_pierce -= 1;
	if (this->_pierce == 0)
		this->_is_over = true;
	🥵.hurt(static_cast<int>(this->_damage));
}
void Projectile::create_projectile(Projectile& proj, CPoint position, CPoint target_pos, int type, int delay, double area, double damage, int speed, int duration, int pierce, int proj_interval, int hitbox_delay, double knock_back, int pool_limit, int chance, int criti_multi, int block_by_wall, bool is_mirror) {
	proj._type = type;
	proj._position = position;
	proj._target = target_pos;
	proj._delay = delay;
	proj._area = area;
	proj._damage = damage;
	proj._speed = speed;
	proj._duration = duration;
	proj._pierce = pierce;
	proj._proj_interval = proj_interval;
	proj._hitbox_delay = hitbox_delay > 0 ? hitbox_delay : 100000;
	proj._knock_back = knock_back;
	proj._pool_limit = pool_limit;
	proj._chance = chance;
	proj._crit_multi = criti_multi;
	proj._block_by_wall = block_by_wall;
	proj._is_mirror = is_mirror;
	proj._is_start = (delay > 0 ? 0 : 1);
	proj._is_over = false;
	proj.set_create_time(clock());
	CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
	proj._offset = proj._position - player_pos;
	Projectile::all_proj.push_back(proj);
}

void Projectile::create_projectile(Projectile p) {
	Projectile::all_proj.push_back(p);
}
void Projectile::set_delay(int delay) {
	_delay = delay;
}
void Projectile::set_create_time(clock_t time) {
	_create_time = time;
}

void Projectile::update_position() {
	CPoint player_pos = get_player_pos();
	CPoint p;
	GetCursorPos(&p);
	HWND targetWindow = FindWindow(NULL, GAME_TITLE);
	ScreenToClient(targetWindow, &p);
	for (Projectile& proj : Projectile::all_proj) {
		int dt = clock() - proj._create_time - proj._delay;
		switch (proj._type) {
		case(WHIP): case (VAMPIRICA):
			proj.set_pos(player_pos + proj._offset);
			break;
		case(MAGIC_MISSILE): case(HOLY_MISSILE):
			if (!proj._is_start && (dt < 0 && dt > -100)) {
				int min_dis = 1000000000;
				proj.set_pos(player_pos);
				CPoint target = player_pos;
				QuadTree::VSPlain.query_nearest_enemy_pos(target, (VSObject*)(&proj), min_dis);
				proj.set_target_vec((target != player_pos ? target - player_pos : CPoint(420, 69)));
				double rad = atan2(target.y - player_pos.y, target.x - player_pos.x);
				proj.set_rotation(rad);
			}
			else {
				proj.update_pos_by_vec();
			}
			break;
		case (KNIFE): case (THOUSAND):
			if (!proj._is_start && (dt < 0 && dt > -100)) {
				p.x = p.x - VSObject::player_dx;
				p.y = p.y - VSObject::player_dy;
				proj.set_target_vec(p - player_pos);
				proj.set_pos(player_pos + proj._offset);
				double rad = atan2(p.y - player_pos.y, p.x - player_pos.x);
				proj.set_rotation(rad);
			}
			else {
				proj.update_pos_by_vec();
			}
			break;
		case (AXE):
			if (proj._is_start && dt >= 0) {
				proj.set_pos(proj.get_parabola(proj._angle, static_cast<double>(proj._speed), dt));
			}
			break;
		case CROSS: case HEAVENSWORD: {
			const double vertical = MATH_PI / 2;
			if (!proj._is_start && (dt < 0 && dt > -100)) {
				int min_dis = 1000000000;
				CPoint target;
				QuadTree::VSPlain.query_nearest_enemy_pos(target, (VSObject*)(&proj), min_dis);
				proj.set_target_vec(target - proj._position);
			}
			else {
				CPoint par = proj.get_parabola(vertical, static_cast<double>(proj._speed), dt);
				double vlen1 = sqrt(proj._target_vec.x * proj._target_vec.x + proj._target_vec.y * proj._target_vec.y);
				double angle = acos(proj._target_vec.y / vlen1);
				par.x -= proj._target.x;
				par.y -= proj._target.y;
				double x, y;
				if (proj._target_vec.x > 0) {
					x = par.x * cos(angle) - par.y * sin(angle);
					y = -par.x * sin(angle) - par.y * cos(angle);
				}
				else {
					x = -par.x * cos(angle) + par.y * sin(angle);
					y = -par.x * sin(angle) - par.y * cos(angle);
				}
				proj.set_pos(CPoint(static_cast<int>(x), static_cast<int>(y)) + proj._target);
				if (proj._is_top && !proj.is_animation()) {
					proj.enable_animation();
				}
			}
		}	break;
		case (HOLYBOOK): case (VESPERS): {
			double radius = proj._angle;
			double speed = proj._speed / (1000.0 / GAME_CYCLE_TIME);
			CPoint origin_pos = player_pos + proj._offset;
			double angular_velocity = speed / radius / 20;
			double initial_angle = atan2(origin_pos.y - player_pos.y, origin_pos.x - player_pos.x);
			double final_angle = initial_angle + dt * angular_velocity;
			double x = player_pos.x + radius * cos(final_angle);
			double y = player_pos.y + radius * sin(final_angle);
			proj.set_pos(CPoint(static_cast<int>(x), static_cast<int>(y)));
		}	break;
		case FIREBALL:case HELLFIRE: {
			if (!proj._is_start && (dt < 0 && dt > -100)) {
				proj.set_pos(player_pos);
			}
			else {
				proj.update_pos_by_vec();
			}
		}	break;
		

		case (SCYTHE): {
			if (proj._is_start && dt >= 0) {
				proj.update_pos_by_vec();
			}
		}	break;
		default :
			break;
		}
	}
}
void Projectile::show_skin(double factor) {
	VSObject::show_skin(_area);
	if (VSObject::distance(this->_position, CPoint{ (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx, (OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy }) > 700) {
		this->_is_over = true;
	}
	if (this->_duration == -1) return;
	if (this->_skin.IsAnimationDone() || clock() - this->_create_time - this->_delay >= this->_duration)
		this->_is_over = true;
}
void Projectile::show() {
	for (Projectile& rf : all_proj) {
		// Projectile& rf = r;
		if (clock() - rf._create_time >= rf._delay) {
			rf.show_skin();
			rf._is_start = true;
		}
	}
	for (auto it = all_proj.begin(); it != all_proj.end();) {
		if (it->get()._is_over) {
			pool.free_obj(it->get());
			it = all_proj.erase(it);
		}
		else {
			++it;
		}
	}
	if (clock() % 60000 == 0) {
		all_proj.shrink_to_fit(); // release memory
	}
}

void Projectile::set_rotation(double radien) {
	_angle = radien;
	int angle = static_cast<int>(radien * 180 / acos(-1));
	angle = (-angle + 360) % 360;
	int regular_angle = 10 * static_cast<int> (angle / 10.0);
	if (regular_angle == 0) return;
	this->_skin.SelectShowBitmap(regular_angle / 10);
}
void Projectile::load_rotation() {
	vector <string> rotated_filename;
	for (auto s : _file_name) {
		for (int i = 0; i < 360; i += 10) {
			rotated_filename.emplace_back(s.substr(0, s.find_last_of('.')) + "_r" + std::to_string(i) + ".bmp");
		}
	}
	this->_skin.ResetBitmap();
	this->_skin.LoadBitmapByString(rotated_filename, RGB(1, 11, 111));
}
CPoint Projectile::get_parabola(double angle, double speed, int time) {
	double dt = static_cast<double>(time) / 1000.0;
	static double pre_y = 100000000;
	speed = speed / (1000.0 / GAME_CYCLE_TIME) * 50; // 50 depends on game cycle time
	// since projectile using this function don't need target so we use it as a tmp
	double x = _target.x + speed * dt * cos(angle);
	double y = _target.y - speed * dt * sin(angle) + 0.5 * 980 * dt * dt;
	if (!_is_top && y > pre_y) {
		_is_top = true;
	}
	pre_y = y;
	return CPoint(static_cast<int>(x), static_cast<int>(y));
}


// deque<Projectile> Projectile::all_proj = {};
ObjPool<Projectile> Projectile::pool(PROJECTILE);
vector<reference_wrapper<Projectile>> Projectile::all_proj;
map <int, Projectile> Projectile::template_proj = {};