#pragma once
class Enemy : public VSObject{
public:
	Enemy(); 
	~Enemy();
	void set_level(int);
	void set_stat(int, int, int, int);
	void load_by_name(char*);
	void show_skin(double factor = 1.0) override;
	void set_enable(bool enable=true);
	bool is_dead();
	void hurt(int damage);
	bool is_enable();
	//void resolve_collide(VSObject&);
	friend void load_enemy_type(vector<Enemy>& vec, char* name, int n, int level = 1, int health = 10, int damage = 1, int speed = 150);

private:
	// stats
	int _level, _health, _damage;
	int _drop, _range;
	bool _is_enable=true; //this name is not good
	VSObject _death_animation;
};
