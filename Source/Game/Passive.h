#pragma once
namespace game_framework {
	class Passive {
	public:
		Passive();
		~Passive();
		void levelup();
	private:
		int _level;
		int _type;
		CMovingBitmap skin;
	};
}