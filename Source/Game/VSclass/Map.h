#pragma once
class Map {
public:
	Map();
	~Map();
	void load_map();
	void show_map();
	void map_padding();
private:
	int _x, _y;
	game_framework::CMovingBitmap base_backgrounds;

};
