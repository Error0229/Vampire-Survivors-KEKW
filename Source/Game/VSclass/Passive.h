#pragma once
class Passive : public VSObject {
public:
	Passive();
	Passive(int);
	~Passive();
	void level_up();
private:
	int _level, _stacking;
	int _type, _rarity,  _max_level;
	vector<int> _effect_by_level;
	vector<int> _alt_effect;
	vector<string> _level_up_msg;
	string _name;
};
enum passive_type {
	POWER = 0, ARMOR, MAXHEALTH, REGEN, COOLDOWN,
	AREA, SPEED, DURATION, AMOUNT, MOVESPEED, 
	MAGNET, LUCK, GROWTH, GREED, REVIVAL, CURSE, 
	SILVER, GOLD, pLEFT, pRIGHT, PANDORA
};
enum stacking {
	Additive = 1, Multiplicative 
};