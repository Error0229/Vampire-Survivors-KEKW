#pragma once
namespace game_framework {
	class Enemy {
	public:
		Enemy(); 
		~Enemy();
		void set_pos(int, int);
		void set_level(int);
	private:
		// stats
		CMovingBitmap skin;
		int _level, _speed, _health, _damage;
		int _drop;
		int _x, _y;
	};
}