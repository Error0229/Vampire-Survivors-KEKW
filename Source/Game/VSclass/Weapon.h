#pragma once 
class Weapon : public VSObject {
public:
	Weapon();
	Weapon(int, vector<char*>&, vector<char*>&, int, float, float);
	~Weapon();
	void show_bullet();
	void update_bullet();
protected:
	VSObject* _base_bullet;
	set<VSObject*> _bullet_set;
	int _type, _level, _damage;
	float _speed, _range;
};
