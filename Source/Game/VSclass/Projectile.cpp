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
Projectile::~Projectile() {};

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
void Projectile::collide_with_enemy(Enemy& 🥵) {
	clock_t now = clock();
	if (_is_over || !is_overlapped((*this), 🥵) || now - 🥵._last_time_got_hit_by_projectile[this->_type] < this->_hitbox_delay)
		return;
	🥵._is_stun = true;
	🥵._stun_speed = -1.0 * 🥵._speed * 🥵._kb * (this->_duration <= 0 ? 1 : this->_duration) * this->_knock_back;
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
	proj._hitbox_delay = hitbox_delay;
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
	for (Projectile& proj : Projectile::all_proj) {
		switch (proj._type) {
		case(WHIP):
			proj.WHIP_transition();
			break;
		case(MAGIC_MISSILE):
			proj.MAGIC_MISSILE_transition();
			break;
		case (KNIFE): case (THOUSAND):
			proj.KNIFE_transition();
			break;
		case (VAMPIRICA):
			proj.VAMPIRICA_transition();
			break;
		case (HOLY_MISSILE):
			proj.HOLY_MISSILE_transition();
			break;
		case (AXE):
			proj.AXE_transition();
			break;
		case (SCYTHE):
			proj.SCYTHE_transition();
			break;
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
void Projectile::WHIP_transition() {
	CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
	_position = player_pos + _offset;
}
void Projectile::MAGIC_MISSILE_transition() {
	if (!_is_start && clock() - _create_time - _delay < 0) {
		CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
		int min_dis = 1000000000;
		this->set_pos(player_pos);
		CPoint target = player_pos;
		QuadTree::VSPlain.query_nearest_enemy_pos(target, (VSObject*)(this), min_dis);
		this->set_target_vec((target != player_pos ? target - player_pos : CPoint(420, 69)));
		double rad = atan2(target.y - player_pos.y, target.x - player_pos.x);
		this->set_rotation(rad);
	}
	else {
		this->update_pos_by_vec();
	}
}
void Projectile::KNIFE_transition() {
	if (!_is_start && clock() - _create_time - _delay < 0) {
		CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
		CPoint p;
		GetCursorPos(&p);
		HWND targetWindow = FindWindow(NULL, GAME_TITLE);
		ScreenToClient(targetWindow, &p);
		p.x = p.x - VSObject::player_dx;
		p.y = p.y - VSObject::player_dy;
		this->set_target_vec(p - player_pos);
		_position = player_pos + _offset;
		double rad = atan2(p.y - player_pos.y, p.x - player_pos.x);
		this->set_rotation(rad);
	}
	else {
		this->update_pos_by_vec();
	}
}
void Projectile::VAMPIRICA_transition() {
	CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
	_position = player_pos + _offset;
}
void Projectile::HOLY_MISSILE_transition() {
	if (!_is_start && clock() - _create_time - _delay < 0) {
		CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
		int min_dis = 1000000000;
		this->set_pos(player_pos);
		CPoint target = (player_pos);
		QuadTree::VSPlain.query_nearest_enemy_pos(target, (VSObject*)(this), min_dis);
		this->set_target_vec((target != player_pos ? target - player_pos : _target_vec));
		double rad = atan2(target.y - player_pos.y, target.x - player_pos.x);
		this->set_rotation(rad);
	}
	else {
		this->update_pos_by_vec();
	}
}
void Projectile::AXE_transition() {
	int dt = clock() - _create_time - _delay;
	if (_is_start &&  dt >= 0) {
		this->set_pos(get_parabola(_angle, static_cast<double>(_speed), dt));
	}
}
void Projectile::SCYTHE_transition() {
	int dt = clock() - _create_time - _delay;
	if (_is_start && dt >= 0) {
		update_pos_by_vec();
	}
	else {
		set_pos(get_player_pos());
	}
}
void Projectile::set_rotation(double radien) {
	// angle += 2*acos(-1)
	int angle = static_cast<int>(radien * 180 / acos(-1));
	angle = (-angle + 360) % 360;
	int regular_angle = 10 * static_cast<int> (angle / 10.0);
	if (regular_angle == 0) return;
	this->_skin.SelectShowBitmap(regular_angle / 10);
	return;
	vector <string> rotated_filename;
	for (auto s : _file_name) {
		rotated_filename.emplace_back(s.substr(0, s.find_last_of('.')) + "_r" + std::to_string(regular_angle) + ".bmp");
	}
	this->_skin.ResetBitmap();
	this->_skin.LoadBitmapByString(rotated_filename, RGB(1, 11, 111));
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
	speed = speed / (1000.0 / GAME_CYCLE_TIME) * 100; // 100 depends on game cycle time
	CPoint player_pos = get_player_pos();
	double x = player_pos.x + speed * dt * cos(angle);
	double y = player_pos.y - speed * dt * sin(angle) + 0.5 * 980 * dt * dt;
	return CPoint(static_cast<int>(x), static_cast<int>(y));
}


// deque<Projectile> Projectile::all_proj = {};
ObjPool<Projectile> Projectile::pool(PROJECTILE);
vector<reference_wrapper<Projectile>> Projectile::all_proj;
map <int, Projectile> Projectile::template_proj = {};