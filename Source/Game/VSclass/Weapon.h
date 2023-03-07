#pragma once 
class Weapon : public VSObject {
public:
	Weapon();
	Weapon(int, vector<char*>&, vector<char*>&, int, float, float);
	~Weapon();
	void show_proj();
	void update_proj();
protected:
	VSObject* _base_proj;
	set<VSObject*> _proj_set;
	int _type, _level, _damage;
	float _speed, _range;
};
