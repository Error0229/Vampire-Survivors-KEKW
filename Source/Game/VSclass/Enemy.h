#pragma once
class Enemy : public VSObject{
public:
	Enemy(); 
	~Enemy();
	void set_level(int);
	void set_stat(int level=1, int health=1, int damage=1, int speed=3000);
	void load_by_name(char*);
	void show_skin(double factor = 1.0) override;
	bool is_dead();
	void hurt(int damage);
private:
	// stats
	int _level, _health=1, _damage;
	int _drop, _range;
	VSObject _corpse;
};
