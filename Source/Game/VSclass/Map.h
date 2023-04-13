#pragma once
class Map :public VSObject{
public:
	Map();
	Map(vector<char*> filename);
	~Map();
	void load_map(vector<char*> filename, COLORREF color = RGB(255,255,255));
	void show_map();
	void set_map_center(CPoint);
	void map_padding(CPoint);
private:
	bool _show_enable = false;
	CPoint _map_center;
	set<pair<int, int>> _map_pos_set;
	deque <Map*> _maps;
};
