#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Projectile.h"
#include "Enemy.h"

Projectile::Projectile() {};
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
void Projectile::collide_with_enemy(Enemy& e, int 💥, int 😄) {
	if (!is_overlapped((*this), e))
		return;
	int dis = distance(_target, this->_position);
	int dx = -static_cast<int>(e._speed * e._kb * 😄 * (e._target.x - e._position.x) / dis);
	int dy = -static_cast<int>(e._speed * e._kb * 😄 * (e._target.y - e._position.y) / dis);
	e._position.x += dx + (dx > 0);
	e._position.y += dy + (dy > 0);
}
int Projectile::order = 0;