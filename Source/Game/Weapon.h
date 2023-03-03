#pragma once 
namespace game_framework {
	class Weapon {
	public:
		Weapon();
		Weapon(CMovingBitmap, CMovingBitmap, int, int, int, int, int);
		~Weapon();
		void show_bullet();
	private:
		CMovingBitmap _skin;
		CMovingBitmap _bullet;
		int _type, _level, _damage, _speed, _range;
	};
}