#pragma once
#include "Enemy.h"
class Projectile : public VSObject
{
public:
	Projectile();
	Projectile(vector<char*> filename, COLORREF color = BLACK);
	~Projectile();
	void set_order(int);
	bool operator < (const Projectile& rhs) const;
	Projectile& operator = (const Projectile& rhs) {
		this->_position = rhs._position;
		this->_skin = rhs._skin;
		this->_order = rhs._order;
		return *this;
	}
	void collide_with_enemy(Enemy& ,int damage, int duration);
	int obj_type = PROJECTILE;

	static int order;
protected:
	int _order;
};

