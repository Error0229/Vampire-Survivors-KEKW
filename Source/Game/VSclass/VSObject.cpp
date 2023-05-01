#include "stdafx.h"
#include "../config.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "VSUtil.h"

VSObject::VSObject()
{
	_collision = CPoint(0, 0);
	_is_mirror = 0;
	_speed = 0;
	_fx = 0;
	_fy = 0;
}
VSObject::VSObject(vector<char*> filename, COLORREF color) :VSObject()
{
	this->load_skin(filename, color);
	for (auto s : filename) {
		_file_name.push_back(s);
	}
}
VSObject::~VSObject()
{
	//VSObject::unshow_skin();
}
void VSObject::load_skin(char* filename, COLORREF color)
{
	_file_size = 1;
	this->_skin.LoadBitmap(filename, color);
	_file_name.push_back(filename);
}
void VSObject::load_skin(vector<char*> filename, COLORREF color)
{
	_file_size = static_cast<int> (filename.size());
	this->_skin.LoadBitmap(filename, color);
	for (auto s : filename) {
		_file_name.push_back(s);
	}
}
void VSObject::load_skin(vector<string>& filename, COLORREF color) {
	_file_size = static_cast<int> (filename.size());
	_skin.LoadBitmapByString(filename, color);
	_file_name = filename;
}
void VSObject::load_mirror_skin() {
	vector<string> tmp;
	for (auto &s : _file_name) {
		tmp.emplace_back(s.substr(0, s.find_last_of(".")) + "_m" + s.substr(s.find_last_of(".")));
	}
	_m_skin.LoadBitmapByString(tmp, RGB(1, 11, 111));
	_m_skin.SyncMirror(_skin);
	mirror_loaded = true;
}
void VSObject::load_animation(vector<char*> filename, COLORREF color)
{
	this->_animations.push_back(game_framework::CMovingBitmap());
	this->_animations.back().LoadBitmap(filename, color);
}
void VSObject::show_skin(double factor)
{
	_scaler = factor;
	
	_skin.SetTopLeft(this->_position.x - (get_width() >> 1) + player_dx, this->_position.y - (this->get_height() >> 1) + player_dy);
	if (mirror_loaded) {
		if (_is_mirror) {
			_m_skin.SetTopLeft(_position.x - (get_width() >> 1) + player_dx, _position.y - (get_height() >> 1) + player_dy);
		}
		if (!last_mirror && _is_mirror) {
			_m_skin.SyncMirror(_skin);
		}
		else if (last_mirror && !_is_mirror) {
			_skin.SyncMirror(_m_skin);
		}
	}
	if (!mirror_loaded || !_is_mirror) {
		_skin.ShowBitmap(factor);
	}
	else if (_is_mirror) {
		_m_skin.ShowBitmap(factor);
	}
	last_mirror = _is_mirror;
}
void VSObject::show_animation(double factor)
{
	this->_animations[this->_selector].SetTopLeft(this->_position.x - (this->_animations[this->_selector].Width() >> 1) + player_dx, this->_position.y - (this->_animations[this->_selector].Height() >> 1) + player_dy);
	this->_animations[this->_selector].ShowBitmap(factor, _is_mirror);
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
	_skin.SetAnimation(delay, _once, cooldown);
	if(mirror_loaded)
		_m_skin.SetAnimation(delay, _once, cooldown);
	_animation_cycle_time = delay * (_file_size+2) ; // ?
}
void VSObject::set_selector(int selector)
{
	_skin.SelectShowBitmap(selector);
	if (mirror_loaded)
		_m_skin.SelectShowBitmap(selector);
}
void VSObject::enable_animation()
{
	_skin.EnableAnimation();
	if (mirror_loaded)
		_m_skin.EnableAnimation();
}
void VSObject::disable_animation()
{
	this->_skin.DisableAnimation();
	if (mirror_loaded)
		_m_skin.DisableAnimation();
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
void VSObject::set_pos(double x, double y) {
	this->_position.x = static_cast<int>(x);
	this->_position.y = static_cast<int>(y);
}
void VSObject::set_target_vec(CPoint v) {
	_target_vec = v;
}
void VSObject::set_target_vec(int dx, int dy) {
	_target_vec = CPoint{ dx, dy };
}
void VSObject::set_speed(double speed) {
	this->_speed = static_cast<int>(speed);
}
void VSObject::set_speed(int speed)
{
	this->_speed = speed;
}
void VSObject::select_show_animation(int index)
{
	this->_selector = index;
}
void VSObject::set_type(int type) {
	_type = type;
}
void VSObject::set_animation_frame(int i) {
	if (mirror_loaded) {
		_m_skin.SelectShowBitmap(i);
	}
	_skin.SelectShowBitmap(i);
}
int VSObject::get_type() {
	return _type;
}
void VSObject::update_pos(CPoint target)
{
	this->_target = target;
	this->update_pos();
}
int VSObject::distance(VSObject& obj1, VSObject& obj2)
{
	return fast_sqrt(square(obj1._position.x - obj2._position.x) + square(obj1._position.y - obj2._position.y));
}
int VSObject::distance(CPoint& p1, CPoint& p2)
{
	return fast_sqrt(square(p1.x - p2.x) + square(p1.y - p2.y));
}
void VSObject::set_pool_id(int id) {
	_pool_id = id;
}
int VSObject::get_pool_id() {
	return _pool_id;
}
void VSObject::update_pos()
{
	// have a speed and moving in a 2d plane
	_target_vec = _target - _position;
	this->_direct = (this->_target.x > this->_position.x) ? RIGHT : LEFT;
	this->_is_mirror = (_direct != _default_direct);
	update_pos_by_vec(_target_vec);
	return;
	int dis = distance(_target, this->_position);
	if (dis < 1) return;
	int dx = VSOM(this->_speed * square(this->_target.x - this->_position.x) / dis);
	int dy = VSOM(this->_speed * square(this->_target.y - this->_position.y) / dis);
	_fx += (_speed * (double)square(this->_target.x - this->_position.x) / (double)dis / 100) - (double)dx;
	_fy += (_speed * (double)square(this->_target.y - this->_position.y) / (double)dis / 100) - (double)dy;
	if (abs(_fx) > 1) {
		dx += static_cast<int>(_fx);
		_fx -= static_cast<int>(_fx);
	}
	if (abs(_fy) > 1) {
		dy += static_cast<int>(_fy);
		_fy -= static_cast<int>(_fy);
	}
	this->_position.x += dx;
	this->_position.y += dy;
}
void VSObject::update_pos_by_vec(CPoint vec) {
	double speed = static_cast<double>(_speed) / (1000.0 / GAME_CYCLE_TIME);
	if (_target_vec == CPoint{ 0, 0 })
		return;
	if (vec != CPoint{ 0, 0 })
		_target_vec = vec;
	//this->_direct = (this->_target.x > this->_position.x) ? RIGHT : LEFT;
	//this->_is_mirror = (_direct != _default_direct);
	double dis = static_cast<double>((square(_target_vec.x) + square(_target_vec.y)));
	double vx = (_target_vec.x > 0 ? 1.0 : -1.0) * speed * square(_target_vec.x) / dis;
	double vy = (_target_vec.y > 0 ? 1.0 : -1.0) * speed * square(_target_vec.y) / dis;
	int dx = static_cast<int> (vx);
	int dy = static_cast<int> (vy);
	_fx += vx - (double)dx;
	_fy += vy - (double)dy;
	if (fabs(_fx) > 1) {
		dx += static_cast<int>(_fx);
		_fx -= static_cast<int>(_fx);
	}
	if (fabs(_fy) > 1) {
		dy += static_cast<int>(_fy);
		_fy -= static_cast<int>(_fy);
	}
	this->_position.x += dx;
	this->_position.y += dy;

}
CPoint VSObject::get_pos()
{
	return this->_position;
}
bool is_overlapped(VSObject& obj1, VSObject& obj2, double overlap_bound)
{
	int dx = abs(obj1._position.x - obj2._position.x);
	int dy = abs(obj1._position.y - obj2._position.y);
	return
		(
			dx < ((int)((obj1.get_width() + obj2.get_width()) >> 1) * overlap_bound) &&
			dy < ((int)((obj1.get_height() + obj2.get_height()) >> 1) * overlap_bound)
		);
}
int VSObject::get_height()
{
	return static_cast<int>(static_cast<double>(this->_skin.Height()) * _scaler);
}
int VSObject::get_width()
{
	return static_cast<int>(static_cast<double>(this->_skin.Width()) * _scaler);
}
bool VSObject::is_animation() {
	bool k = false;
	if(mirror_loaded && _is_mirror)
		k = _m_skin.IsAnimation();
	return k ||_skin.IsAnimation();
}
bool VSObject::is_animation_done()
{
	bool k = false;
	if (mirror_loaded && _is_mirror)
		k = _m_skin.IsAnimationDone();
	return k || _skin.IsAnimationDone();
}

void VSObject::set_is_mirror(bool is_mirror)
{
	this->_is_mirror = is_mirror;
}
void VSObject::set_scaler(double scaler)
{
	this->_scaler = scaler;
}
int VSObject::get_direct()
{
	return this->_default_direct;
}
int VSObject::get_animation_frame()
{
	return this->_skin.GetSelectShowBitmap();
}

void VSObject::append_collide(VSObject& other, double overlap_bound, double factor) {
	// compute what will happen when this obj collide with other, add the result to variable _collision
	// after many tests, i think constant factor is good enough
	// the greater factor is, the farer this obj get push
	if ((_position == other._position) || !is_overlapped(*this, other, overlap_bound))
		return;
	double raw_ratio;
	double dx = (double)abs(_position.x - other._position.x);
	double dy = (double)abs(_position.y - other._position.y);
	if (dx > dy) {
		double w = (double)((other.get_width() >> 1) + (get_width() >> 1)) * overlap_bound;
		raw_ratio = (w - dx) / dx;
	}
	else {
		double h = (double)((other.get_height() >> 1) + (get_height() >> 1)) * overlap_bound;
		raw_ratio = (h - dy) / dy;
	}
	_collision.x += (int)((_position.x - other._position.x) * raw_ratio * factor);
	_collision.y += (int)((_position.y - other._position.y) * raw_ratio * factor);
}
void VSObject::is_collide_with(VSObject* other) {

}
void VSObject::update_collide()
{
	// apply the collision to _position
	// maight integrated into update_pos in the future if we need the real vector implentment
	this->_position += _collision;
	_collision = { 0, 0 };
}
CPoint get_player_pos() {
	return CPoint((w_size_x >> 1) - VSObject::player_dx, (w_size_y >> 1) - VSObject::player_dy);
}

int VSObject::player_dx = (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1);
int VSObject::player_dy = (OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1);