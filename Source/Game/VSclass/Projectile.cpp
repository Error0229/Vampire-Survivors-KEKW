#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Projectile.h"

Projectile::Projectile() {};
Projectile::~Projectile() {};
Projectile::Projectile(vector<char*> filename, COLORREF color)  {
	this->_skin.LoadBitmap(filename, color);
	this->_order = Projectile::order++;
};
void Projectile::set_order(int order) {
	this->_order = order;
}
bool Projectile::operator < (const Projectile& rhs) const {
	return this->_order < rhs._order;
}
void Projectile::💥_with_enemy(VSObject* enemy) {

}
int Projectile::order = 0;