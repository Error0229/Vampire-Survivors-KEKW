#pragma once 
class Weapon {
public:
	Weapon();
	Weapon(game_framework::CMovingBitmap, game_framework::CMovingBitmap, int, int, int, int, int);
	~Weapon();
	void show_bullet();
private:
	game_framework::CMovingBitmap _skin;
	game_framework::CMovingBitmap _bullet;
	int _type, _level, _damage, _speed, _range;
};
