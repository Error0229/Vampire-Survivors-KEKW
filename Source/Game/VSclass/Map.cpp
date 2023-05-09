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
	for (auto mp : _maps) {
		delete mp;
	}
}
void Map::load_map(vector<char*> filename, COLORREF color)
{
	VSObject::load_skin(filename);
	int len = filename.size();
	for (int i = 0; i < PADDING_NUM; i++) {
		_maps.emplace_back(new Map({ filename[i % len] }));
		_maps[i]->_show_enable = false;
	}
	_maps[0]->_show_enable = true;
	_maps[0]->set_pos(0, 0);
	_map_pos_set.insert({ 0,0 });
}
void Map::set_obstacle(int map_id)
{
	switch (map_id) {
	case 0: {
		vector<RECT> tmp = {
			{ 770, 1390, 260, 30 },
			{ 770, 1420, 30, 130 },
			{ 990, 1420, 30, 130 },
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
			{ 1700, 1802, 25, 20 },
			{ 1603, 1546, 25, 20 },
			{ 1733, 1545, 25, 20 },
			{ 1701, 1084, 25, 20 }
		};
		obs_all.clear();
		Obstacle obst;
		for (auto& r : tmp) {
			obst.set_base_pos(CPoint(r.left - 1024 + (r.right >> 1), r.top - 1024 + (r.bottom >> 1)));
			obst.set_width(r.right);
			obst.set_height(r.bottom);
			obs_all.emplace_back(obst);
		}
		break;
	}
	case 1: {

		break;
	}
	case 2: {
		break;
	}	
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