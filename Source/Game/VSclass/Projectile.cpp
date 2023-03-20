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
int Projectile::order = 0;