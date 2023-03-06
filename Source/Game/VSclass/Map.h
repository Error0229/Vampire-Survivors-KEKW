#pragma once
class Map :public VSObject {
public:
	Map();
	Map(vector<char*> filename);
	~Map();
	void load_map(vector<char*> filename, COLORREF color = RGB(255,255,255));
	void show_map();
	void map_padding();
private:
	
};
