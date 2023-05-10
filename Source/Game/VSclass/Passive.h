#pragma once
class Passive : public VSObject {
public:
	Passive();
	Passive(int);
	~Passive();
	void level_up();
	int get_type();
	int get_effect();
	static int get_effect(int type);
	int get_level();
	bool is_max_level();
	int get_max_level();
	string get_level_up_msg(bool is_new=false);
	int get_rarity();
	void upgrade();
	static void init();
	static void upgrade(int type);
	static deque<Passive> all_passive;
	static vector<Passive> base_passive;
	static int passive_count();
private:
	int _level, _stacking;
	int _type, _rarity,  _max_level;
	vector<int> _effect_by_level;
	vector<string> _level_up_msg;
};
enum passive_type {
	POWER = 63, ARMOR, MAXHEALTH, REGEN, COOLDOWN,
	AREA, SPEED, DURATION, AMOUNT, MOVESPEED, 
	MAGNET, LUCK, GROWTH, GREED, REVIVAL, CURSE, 
	SILVER, GOLD, pLEFT, pRIGHT, PANDORA
};
enum stacking_type {
	Additive = 1, Multiplicative 
};