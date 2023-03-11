#pragma once
class Player :public VSObject{
public:
	Player();
	~Player();
	void update_pos(CPoint) override;
	void hurt(int damage);
	void show_skin(double factor = 1.0);
	void load_bleed();
private:
	VSObject _bleed_animation;
	vector<Weapon*> _weapons;
	vector<Passive*> _passives;
	int _hp=100, _max_hp;
	int _exp, _max_exp;
	int _level;
	int _attack, _defense;
	int _cd;
	int _weapon_type, _weapon_level;
	int _passive_type, _passive_level;
	int _pickups_range;
	int _direction;
	int _skill;
	int _skill_cd;
	int _skill_cd_max;
	int _range;
	int _might;
	int _speed;	
	int _recovery;
	int _aoe;
	int _projectile_amount;
	int _projectile_speed;
	int _duration;
	int _exp_buff;
	int _money_buff;
	int _money;
	int _lucky;
	int _revival;
	int _reroll;
	bool _is_hurt = false;
};
