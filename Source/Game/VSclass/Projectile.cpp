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
	this->_order = Projectile::order++;
};
void Projectile::set_order(int order) {
	this->_order = order;
}
bool Projectile::operator < (const Projectile& rhs) const {
	return this->_order < rhs._order;
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
void Projectile::create_projectile(int player_x, int player_y, int type, int damage, int speed, int amount, int duration, int pierce, int proj_interval, int hitbox_delay, int knock_back, int pool_limit, int chance, int criti_multi, int block_by_wall) {
	Projectile p = Projectile();
	p._type = type;
	p._damage = damage;
	p._speed = speed;
	p._amount = amount;
	p._duration = duration;
	p._pierce = pierce;
	p._proj_interval = proj_interval;
	p._hitbox_delay = hitbox_delay;
	p._knock_back = knock_back;
	p._pool_limit = pool_limit;
	p._chance = chance;
	p._crit_multi = criti_multi;
	p._block_by_wall = block_by_wall;
}
void Projectile::create_projectile(Projectile p) {
	Projectile::_all_proj.push_back(p);
}

vector<Projectile> Projectile::_all_proj = {};
int Projectile::order = 0;