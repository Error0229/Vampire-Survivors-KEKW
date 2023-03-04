#pragma once
class Enemy {
public:
	Enemy(); 
	~Enemy();
	void set_pos(int, int);
	void set_level(int);
private:
	// stats
	game_framework::CMovingBitmap _skin;
	int _level, _speed, _health, _damage;
	int _drop;
	int _x, _y;
};
