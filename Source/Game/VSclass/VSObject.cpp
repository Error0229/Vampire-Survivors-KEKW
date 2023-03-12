#include "stdafx.h"
#include "../config.h"
#include "../../Library/gameutil.h"
#include "VSMath.h"
#include "VSObject.h"

VSObject::VSObject()
{
	_is_mirror = 0;
	_speed = 0;
}
VSObject::VSObject(vector<char*> filename, COLORREF color) :VSObject()
{
	this->load_skin(filename);
}
VSObject::~VSObject()
{
	VSObject::unshow_skin();
}
void VSObject::load_skin(char* filename, COLORREF color)
{
	this->_skin.LoadBitmap(filename, color);
}
void VSObject::load_skin(vector<char*> filename, COLORREF color)
{
	this->_skin.LoadBitmap(filename, color);
}
void VSObject::show_skin(double factor) 
{
	this->_skin.SetTopLeft(this->_position.x - (this->_skin.Width() >> 1) + player_dx, this->_position.y - (this->_skin.Height() >> 1) + player_dy);
	this->_skin.ShowBitmap(factor, _is_mirror);
}
void VSObject::unshow_skin()
{
	this->_skin.UnshowBitmap();
}
void VSObject::set_default_direct(int dir) 
{
	this->_default_direct = dir;
}
void VSObject::set_animation(int delay, bool _once, int cooldown)
{
	this->_skin.SetAnimation(delay, _once, cooldown);
}
void VSObject::enable_animation()
{
	this->_skin.ToggleAnimation();
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
void VSObject::set_speed(int speed)
{
	this->_speed = speed;
}
void VSObject::update_pos(CPoint target)
{
	this->_target = target;
	this->update_pos();
}
int distance(VSObject& obj1, VSObject& obj2)
{
	return fast_sqrt(square(obj1._position.x - obj2._position.x) + square(obj1._position.y - obj2._position.y));
}
int distance(CPoint& p1, CPoint& p2)
{
	return fast_sqrt(square(p1.x - p2.x) + square(p1.y - p2.y));
}
void VSObject::update_pos()
{
	// have a speed and moving in a 2d plane
	int dis = distance(_target, this->_position);
	if (dis < 1) return;
	this->_direct = (this->_target.x > this->_position.x) ? RIGHT : LEFT;
	this->_is_mirror = (_direct != _default_direct);
	int dx = VSOM(this->_speed * ( this->_target.x - this->_position.x ) / dis);
	int dy = VSOM(this->_speed * ( this->_target.y - this->_position.y ) / dis);
	this->_position.x += dx + ( dx > 0 );
	this->_position.y += dy + ( dy > 0 );

}
CPoint VSObject::get_pos()
{
	return this->_position;
}
bool is_overlapped(VSObject& obj1, VSObject& obj2)
{
	int dx = abs(obj1._position.x - obj2._position.x);
	int dy = abs(obj1._position.y - obj2._position.y);
	return 
		(
			dx < ((int)(obj1._skin.Width() + obj2._skin.Width()) >> 1) &&
			dy < ((int)(obj1._skin.Height() + obj2._skin.Height()) >> 1)
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

bool VSObject::is_animation_done()
{
	return this->_skin.IsAnimationDone();
}

void VSObject::set_is_mirror(bool is_mirror)
{
	this->_is_mirror = is_mirror;
}
int VSObject::get_direct()
{
	return this->_default_direct;
}

int VSObject::player_dx = 400;
int VSObject::player_dy = 300;