#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Projectile.h"
#include "Enemy.h"

Projectile::Projectile() {
	obj_type = PROJECTILE;
};
Projectile::~Projectile() {};
Projectile::Projectile(vector<char*> filename, COLORREF color) {
	this->_skin.LoadBitmap(filename, color);
};

bool Projectile::operator < (const Projectile& rhs) const {
	return this->_type < rhs._type;
}
void Projectile::set_player_origin_pos(CPoint pos) {
	_origin_player_pos = pos;
}
void Projectile::collide_with_enemy(Enemy& e, int 💥, int 😄, int 😵) {
	// 💥 = damage
	// 😄 = duration
	if (!is_overlapped((*this), e))
		return;
	if (😄 < 0)
		😄 = 1;
	e._is_stun = true;
	e._stun_speed = -1.0 * e._speed * e._kb * 😄 * 😵;
	e._last_time_got_hit = clock();
	e.hurt(💥);
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
	Projectile::_all_proj.push_back(proj);
}
void Projectile::create_projectile(Projectile p) {
	Projectile::_all_proj.push_back(p);
}
void Projectile::set_delay(int delay) {
	_delay = delay;
}
void Projectile::set_create_time(clock_t time) {
	_create_time = time; 
}
void Projectile::update_projectile_position() {
	for (Projectile& proj: Projectile::_all_proj) {
		switch (proj._type) {
		case(WHIP): 
			proj.WHIP_transition();
			break;
			
		}
	}
}
void Projectile::WHIP_transition() {
	CPoint player_pos = { -VSObject::player_dx, -VSObject::player_dy };
	_position = player_pos + (_position - _origin_player_pos);
}

deque<Projectile> Projectile::_all_proj = {};