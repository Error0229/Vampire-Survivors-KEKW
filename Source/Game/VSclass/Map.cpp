#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "../config.h"
#include "VSObject.h"
#include "Obstacle.h"
#include "Map.h"
constexpr int PADDING_NUM = 4;


Map::Map()
{
	obj_type = MAP;
}
Map::Map(vector<char*> filename) : VSObject(filename)
{
	obj_type = MAP;
}
Map::~Map()
{
}
void Map::load_map(vector<char*> filename, COLORREF color)
{
	VSObject::load_skin(filename);
	int len = filename.size();
	for (int i = 0; i < PADDING_NUM; i++) {
		_maps.push_back(make_shared<Map>(Map({ filename[i % len] })));
		_maps[i]->_show_enable = false;
	}
	_maps[0]->_show_enable = true;
	_maps[0]->set_pos(0, 0);
	_map_pos_set.insert({ 0,0 });
}
void Map::set_obstacle(int map_id)
{
	vector<RECT> tmp;
	switch (map_id) {
	case 0: {
		tmp = {
			{ 770, 1390, 260, 30 },
			{ 770, 1420, 30, 130 },
			{ 990, 1420, 40, 130 },
			{ 770, 1520, 100, 30 },
			{ 930, 1520, 100, 30 },
			{ 130, 170, 25, 20 },
			{ 355, 170, 25, 20 },
			{ 425, 170, 25, 20 },
			{ 485, 75, 25, 20 },
			{ 455, 330, 25, 20 },
			{ 295, 80, 25, 20 },
			{ 230, 80, 25, 20 },
			{ 197, 80, 25, 20 },
			{ 230, 365, 25, 20 },
			{ 550, 135, 25, 20 },
			{ 290, 200, 25, 20 },
			{ 1478, 266, 25, 20 },
			{ 1769, 522, 50, 20 },
			{ 1863, 522, 50, 20 },
			{ 1959, 523, 50, 20 },
			{ 1376, 965, 25, 20 },
			{ 1414, 985, 25, 20 },
			{ 1640, 967, 50, 40 },
			//{ 1700, 1802, 25, 20 },
			{ 1603, 1546, 25, 20 },
			{ 1733, 1545, 25, 20 },
			{ 1701, 1084, 25, 20 }
		};
		break;
	}
	case 1: {
		tmp = {
			{0, 0, 1375, 240},
			{1537, 0, 411, 240},
			{0, 688, 2048, 240},
			{224, 368, 31, 63},
			{256, 336, 95 ,64},
			{352, 369, 63, 62},
			{272, 400, 63, 64},
			{293, 463, 20 ,10},
			{673, 239, 66, 34},
			{865, 239, 66, 34},
			{800, 256, 33, 30},
			{1664, 608, 33, 30},
			{1280, 238, 95, 34},
			{1536, 238, 95, 34},
			{1376, 0, 160, 175},
			{1423, 174, 66, 15}
		};
		break;
	}
	case 2: {
		tmp = {
			{400, 1090, 30, 1958},
			{1170, 1090, 30, 1958},

			{335, 1090, 65, 30},
			{1200, 1090, 65, 30},

			{304, 866, 30, 224},
			{1266, 866, 30, 244},

			{300, 220, 194, 720},
			{1106, 220, 194, 720},

			{400, 2040, 190, 8},
			{1010, 2040, 190, 8},

			{400, 5, 190, 295},
			{1010, 5, 190, 295},
			
			{500, 785, 25, 37},
			{1075, 785, 25, 37},

			{435, 1520, 25, 66},
			{1140, 1520, 25, 66},
			
			{690, 1520, 25, 66},
			{885, 1520, 25, 66},
			
			{720, 1500, 160, 100},
			
			{1150, 950, 68, 24},

			{500, 600, 25, 37},
			{1075, 600, 25, 37},

			{495, 580, 65, 20},
			{1040, 580, 65, 20},

			{495, 480, 65, 110},
			{1040, 480, 65, 110},

			{755, 480, 90, 110},

			{590, 224, 65, 12},
			{945, 224, 65, 12},

			{590, 32, 65, 12},
			{945, 32, 65, 12},
		};
		break;
	}	
	default: {
		break;
	}
	}
	obs_all.clear();
	Obstacle obst;
	for (auto& r : tmp) {
		obst.set_base_pos(CPoint(r.left - (get_width() >> 1) + (r.right >> 1), r.top - (get_height() >> 1) + (r.bottom >> 1)));
		obst.set_width(r.right);
		obst.set_height(r.bottom);
		obs_all.emplace_back(obst);
	}
}
void Map::set_map_center(CPoint p) {
	_map_center = p;
}
void Map::show_map()
{
	for (auto padd : _maps) {
		if (padd->_show_enable) {
			padd->show_skin();
		}
	}
}
void Map::map_padding(CPoint player_pos)
{
	int dx[12] = { -1, -1, 0, 0, 1, 1, -1, -1, 0, 0, 1, 1 };
	int dy[12] = { 0, -1, -1, -1, -1, 0, 0, 1, 1, 1, 1, 0 };
	int ddx[4] = { -1, 1, -1, 1 };
	int ddy[4] = { -1, -1, 1, 1 };
	int w = this->get_width();
	int h = this->get_height();
	_map_center.x = w * lround(player_pos.x / static_cast<float>(w)); // all the problems come from here
	_map_center.y = h * lround(player_pos.y / static_cast<float>(h));
	for (int qr = 0; qr < 4; qr++) {
		if (!(ddx[qr] * (player_pos.x - _map_center.x) > 0 && ddy[qr] * (player_pos.y - _map_center.y) > 0)) {
			continue;
		}
		for (auto pad : _maps) {
			if (!pad->_show_enable) {
				continue;
			}
			if (pad->_position == _map_center)
				continue;
			bool in_range = false;
			for (int i = qr * 3; i < qr * 3 + 3; i++) {
				CPoint p(_map_center.x + dx[i] * w, _map_center.y + dy[i] * h);
				if (p == pad->_position) {
					in_range = true;
				}
			}
			if (!in_range) {
				pad->_show_enable = false;
				_map_pos_set.erase({ pad->_position.x ,pad->_position.y });
			}
		}
		for (int i = qr * 3; i < qr * 3 + 3; i++) {
			pair<int, int> p(_map_center.x + dx[i] * w, _map_center.y + dy[i] * h);
			if (_map_pos_set.count(p)) {
				continue;
			}
			_map_pos_set.insert(p);
			for (auto padd : _maps) {
				if (padd->_show_enable == false) {
					padd->set_pos(p.first, p.second);
					padd->_show_enable = true;
					break;
				}
			}
		}
	}
	for (auto& obs : obs_all) {
		obs.set_pos(obs.get_base_pos().x + _map_center.x, obs.get_base_pos().y + _map_center.y);
	}
}
vector <Obstacle> Map::obs_all;