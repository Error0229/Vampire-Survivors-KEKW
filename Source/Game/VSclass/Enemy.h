#pragma once
class Enemy : public VSObject{
public:
	Enemy(); 
	~Enemy();
	void set_level(int);
	void set_stat(int, int, int, int, int);
	void load_by_name(char*);
	void show_skin(double factor = 1.0) override;
	void set_enable(bool enable=true);
	bool is_dead();
	bool hurt(int damage); //this will return true if the enemy die from this damage, otherwise false
	bool is_enable();
	int get_xp_value();
	//void resolve_collide(VSObject&);
	friend void load_enemy_type(vector<Enemy>& vec, char* name, int n, int level = 1, int health = 10, int damage = 1, int speed = 150, int xp_value = 1);

private:
	// stats
	int _level, _health, _damage;
	int _xp_value=10, _range;
	bool _is_enable=true; //this name is not good
	VSObject _death_animation;
};
