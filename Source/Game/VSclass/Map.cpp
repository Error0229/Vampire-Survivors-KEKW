#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "../config.h"
#include "VSObject.h"
#include "Map.h"
constexpr int PADDING_NUM = 4;


Map::Map()
{
	obj_type = MAP;
}
Map::Map(vector<char*> filename) : VSObject(filename)
{
}
Map::~Map()
{
	for ( auto mp : _maps ) {
		delete mp;
	}
}
void Map::load_map(vector<char*> filename, COLORREF color)
{
	VSObject::load_skin(filename);
	int len = filename.size();
	for ( int i = 0; i < PADDING_NUM; i++ ) {
		_maps.emplace_back(new Map({ filename[ i % len ] }));
		_maps[ i ]->_show_enable = false;
	}
	_maps[ 0 ]->_show_enable = true;
	_maps[ 0 ]->set_pos(0, 0);
	_map_pos_set.insert({ 0,0 });
}
void Map::set_map_center(CPoint p) {
	_map_center = p;
}
void Map::show_map()
{
	for ( auto padd : _maps ) {
		if ( padd->_show_enable ) {
			padd->show_skin();
		}
	}
}
void Map::map_padding(CPoint player_pos)
{
	int dx[ 12 ] = { -1, -1, 0, 0, 1, 1, -1, -1, 0, 0, 1, 1 };
	int dy[ 12 ] = { 0, -1, -1, -1, -1, 0, 0, 1, 1, 1, 1, 0 };
	int ddx[ 4 ] = { -1, 1, -1, 1 };
	int ddy[ 4 ] = { -1, -1, 1, 1 };
	int w = this->get_width();
	int h = this->get_height();
	_map_center.x = w * lround(player_pos.x / static_cast<float>( w )); // all the problems come from here
	_map_center.y = h * lround(player_pos.y / static_cast<float>( h ));
	for ( int qr = 0; qr < 4; qr++ ) {
		if (!( ddx[ qr ] * ( player_pos.x - _map_center.x ) > 0 && ddy[ qr ] * ( player_pos.y - _map_center.y ) > 0 ) ){
			continue;
		}
		for ( auto pad : _maps ) {
			if ( !pad->_show_enable ) {
				continue;
			} 
			if ( pad->_position == _map_center )
				continue;
			bool in_range = false;
			for ( int i = qr * 3; i < qr * 3 + 3; i++ ) {
				CPoint p(_map_center.x + dx[ i ] * w, _map_center.y + dy[ i ] * h);
				if ( p == pad->_position ) {
					in_range = true;
				}
			}
			if ( !in_range ) {
				pad->_show_enable = false;
				_map_pos_set.erase({ pad->_position.x ,pad->_position.y });
			}
		}
		for ( int i = qr * 3; i < qr * 3 + 3; i++ ) {
			pair<int, int> p(_map_center.x + dx[ i ] * w, _map_center.y + dy[ i ] * h);
			if ( _map_pos_set.count(p) ) {
				continue;
			} 
			_map_pos_set.insert(p);
			for ( auto padd : _maps ) {
				if ( padd->_show_enable == false ) {
					padd->set_pos(p.first, p.second);
					padd->_show_enable = true;
					break;
				}
			}
		}
	}
}