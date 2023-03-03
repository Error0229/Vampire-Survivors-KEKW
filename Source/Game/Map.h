#pragma once
namespace game_framework {
	class Map {
	public:
		Map();
		~Map();
		void load_map();
		void show_map();
		void map_padding();
	private:
		int _x, _y;
		CMovingBitmap base_backgrounds;

	};
}