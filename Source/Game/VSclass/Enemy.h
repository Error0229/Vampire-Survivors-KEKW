#pragma once
class Enemy : public VSObject{
public:
	Enemy(); 
	~Enemy();
	void set_level(int);
private:
	// stats
	int _level, _speed, _health, _damage;
	int _drop, _range;
};
