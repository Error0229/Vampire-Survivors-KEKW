#pragma once
class Enemy : public VSObject{
public:
	Enemy(); 
	~Enemy();
	void set_level(int);
	void load_by_name(char*);
	void show_skin(double factor = 1.0) override;
	bool is_dead();
	void hurt(int damage);
private:
	// stats
	int _level, _speed, _health=1, _damage;
	int _drop, _range;
	VSObject _corpse;
};
