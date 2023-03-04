#pragma once
class Passive {
public:
	Passive();
	~Passive();
	void level_up();
private:
	int _level;
	int _type;
	game_framework::CMovingBitmap _skin;
};
