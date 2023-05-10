#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Obstacle.h"
Obstacle::Obstacle() 
{
	obj_type = OBSTACLE;
}
Obstacle::Obstacle(CPoint pos, int w, int h) : _width(w), _height(h) {
	obj_type = OBSTACLE;
	_position = pos;
}
Obstacle::~Obstacle() = default;
int Obstacle::get_height()
{
	return _height;
}
int Obstacle::get_width()
{
	return _width;
}
void Obstacle::set_height(int height)
{
	_height = height;
}
void Obstacle::set_width(int width)
{
	_width = width;
}
void Obstacle::set_base_pos(CPoint pos)
{
	_base_pos = pos;
}
CPoint Obstacle::get_base_pos()
{
	return _base_pos;
}