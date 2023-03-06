#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSMath.h"
#include "VSObject.h"
VSObject::VSObject()
{
	_speed = 0;
}
VSObject::VSObject(vector<char*> filename) :VSObject()
{
	this->load_skin(filename);
}
VSObject::~VSObject()
{
}
void VSObject::load_skin(vector<char*> filename, COLORREF color)
{
	this->_skin.LoadBitmap(filename, color);
}
void VSObject::show_skin()
{
	this->_skin.SetTopLeft(this->_position.x - (this->_skin.Width()>>1) + player_dx, this->_position.y - (this->_skin.Height()>>1) + player_dy);
	this->_skin.ShowBitmap();
}
void VSObject::set_pos(CPoint pos)
{
	this->_position = pos;
}
void VSObject::set_pos(int x, int y)
{
	this->_position.x = x;
	this->_position.y = y;
}

void VSObject::update_pos(CPoint target)
{
	this->_target = target;
	this->update_pos();
}
void VSObject::update_pos()
{
	// have a speed and moving in a 2d plane
	this->_position.x += VSOM(this->_speed * (this->_target.x - this->_position.x) / fast_sqrt(square(this->_target.x - this->_position.x) + square(this->_target.y - this->_position.y)));
	this->_position.y += VSOM(this->_speed * (this->_target.y - this->_position.y) / fast_sqrt(square(this->_target.x - this->_position.x) + square(this->_target.y - this->_position.y)));
	

}
CPoint VSObject::get_pos()
{
	return this->_position;
}
bool is_overlapped(VSObject& obj1, VSObject& obj2)
{
	return
		(
		obj1._position.x <= obj2._position.x + obj2._skin.Width() &&
		obj1._position.x + obj1._skin.Width() >= obj2._position.x &&
		obj1._position.y <= obj2._position.y + obj2._skin.Height() &&
		obj1._position.y + obj1._skin.Height() >= obj2._position.y
	);
}
int VSObject::get_height()
{
	return this->_skin.Height();
}
int VSObject::get_width()
{
	return this->_skin.Width();
}
int VSObject::player_dx = 400;
int VSObject::player_dy = 300;