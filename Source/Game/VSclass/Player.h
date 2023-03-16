#pragma once
class Player :public VSObject{
public:
	Player();
	~Player();
	void update_pos(CPoint) override;
	void acquire_weapon(shared_ptr<Weapon>);
	void acquire_passive(shared_ptr<Passive>);
	void update_passive(shared_ptr<Passive>);
	void update_proj_pos();
	void level_up_passive(int);
	void show_proj_skin();
	void hurt(int damage);
	void show_skin(double factor = 1.0);
	void load_bleed();
	void pick_up_xp(int);
	int get_pickup_range();
private:
	VSObject _bleed_animation;
	vector<shared_ptr<Weapon>> _weapons;
	vector<shared_ptr<Passive>> _passives;
	vector<int> stats;
	int _might, _armor, _max_health, _recovery, _cooldown, _area, 
		_proj_speed, _duration, _amount, _move_speed, _magnet,
		_luck, _growth, _greed, _revival, _curse;
	int _hp;
	int _exp, _max_exp;
	int _level;
	int _weapon_type, _weapon_level;
	int _passive_type, _passive_level;
	int _direction;
	int _range;
	int _speed;	//this have same name with VSObject::speed, might change in the future
	int _money;
	int _reroll;
	bool _is_hurt = false;
};
