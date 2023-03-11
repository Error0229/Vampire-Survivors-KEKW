#pragma once
class Projectile : public VSObject
{
public:
	Projectile();
	Projectile(vector<char*> filename, COLORREF color = BLACK);
	~Projectile();
	void set_order(int);
	bool operator < (const Projectile& rhs) const;
	static int order;
protected:
	int _order;
};

