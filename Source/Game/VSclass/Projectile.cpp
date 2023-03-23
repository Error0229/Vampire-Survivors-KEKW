#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "../config.h"
#include "VSObject.h"
#include "Projectile.h"
#include "Enemy.h"

Projectile::Projectile() {
	obj_type = PROJECTILE;
	_is_over = false;
};
Projectile::~Projectile() {};
Projectile::Projectile(vector<char*> filename, COLORREF color) {
	this->_skin.LoadBitmap(filename, color);
};

bool Projectile::operator < (const Projectile& rhs) const {
	return this->_type < rhs._type;
}
void Projectile::set_offset(CPoint os) {
	_offset = os;
}
void Projectile::collide_with_enemy(Enemy& 🥵) {
	if (!is_overlapped((*this), 🥵))
		return;
	🥵._is_stun = true;
	🥵._stun_speed = -1.0 * 🥵._speed * 🥵._kb * (this->_duration <= 0 ? 1 : this->_duration) * this->_knock_back;
	🥵._last_time_got_hit = clock();
	🥵.hurt(this->_damage);
}
void Projectile::create_projectile(Projectile proj, CPoint position, CPoint target_pos, int type, int delay, int damage, int speed, int duration, int pierce, int proj_interval, int hitbox_delay, int knock_back, int pool_limit, int chance, int criti_multi, int block_by_wall, bool is_mirror) {
	proj._position = position;
	proj._target = target_pos;
	proj._delay = delay;
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
	proj._type = type;
	proj._is_mirror = is_mirror;
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
	for (Projectile& proj: Projectile::all_proj) {
		switch (proj._type) {
		case(WHIP): 
			proj.WHIP_transition();
			break;
		case (VAMPIRICA):
			proj.VAMPIRICA_transition();
			break;
		}
	}
}
void Projectile::show_skin(double factor) {
	VSObject::show_skin(factor);
	if(this->_skin.IsAnimationDone() || clock() - this->_create_time - this->_delay >= this->_proj_interval)
		this->_is_over = true;
}
void Projectile::show() {
	int deq_size = static_cast<int> (all_proj.size());
	for (int i = 0; i < deq_size; i++) {
		if (clock() - all_proj.front()._create_time >= all_proj.front()._delay)
			all_proj.front().show_skin();
		if (!all_proj.front()._is_over) {
			all_proj.emplace_back(all_proj.front());
		}
		all_proj.pop_front();
	}
	if (clock() % 60000) {
		all_proj.shrink_to_fit(); // release memory
	}
}
void Projectile::WHIP_transition() {
	CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
	_position = player_pos + _offset;
}
void Projectile::VAMPIRICA_transition() {
	CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
	_position = player_pos + _offset;
}
deque<Projectile> Projectile::all_proj = {};