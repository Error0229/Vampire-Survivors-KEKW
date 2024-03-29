#pragma once
class Map :public VSObject{
public:
	Map();
	Map(vector<char*> filename);
	~Map();
	void load_map(vector<char*> filename, COLORREF color = RGB(255,255,255));
	void show_map();
	void set_map_center(CPoint);
	void set_obstacle(int);
	void map_padding(CPoint);
	static vector <Obstacle> obs_all;
private:
	bool _show_enable = false;
	CPoint _map_center;
	set<pair<int, int>> _map_pos_set;
	deque <shared_ptr<Map>> _maps;
};
