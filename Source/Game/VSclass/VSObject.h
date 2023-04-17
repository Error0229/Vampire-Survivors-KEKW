#pragma once
#include "VSUtil.h"
class VSObject
{
public:
	VSObject();
	VSObject(vector<char*> filename, COLORREF color = RGB(1, 11, 111));
	virtual ~VSObject();
	void load_skin(vector<char*> filename, COLORREF color= RGB(1, 11, 111));
	void load_skin(char* filename, COLORREF color = RGB(1, 11, 111));
	void load_skin(vector<string>& filename, COLORREF color = RGB(1, 11, 111));
	void load_animation(vector<char*> filename, COLORREF color = RGB(1, 11, 111));
	virtual void show_skin(double factor = 1.0);
	virtual void show_animation(double factor = 1.0);
	void unshow_skin();
	void set_default_direct(int dir);
	void set_animation(int delay, bool _once, int cooldown = 0);
	void set_selector(int);
	void set_scaler(double);
	void enable_animation();
	void disable_animation();
	void set_pos(CPoint);
	void set_pos(int, int);
	void set_pos(double, double);
	void set_target_vec(CPoint);
	void set_target_vec(int, int);
	void set_pool_id(int);
	void set_type(int);
	int get_type();
	int get_pool_id();
	virtual void set_speed(int);
	virtual void set_speed(double);
	void select_show_animation(int);
	void set_animation_frame(int);
	CPoint get_pos();
	virtual void update_pos(CPoint);
	virtual void update_pos();
	virtual void update_pos_by_vec(CPoint vec = {0,0});
	void set_is_mirror(bool);
	bool is_animation_done();
	bool is_animation();
	int get_direct();
	int get_height();
	int get_width();
	int get_animation_frame();
	void append_collide(VSObject&, double overlap_bound, double factor);
	void update_collide();
	void is_collide_with(VSObject* other);
	static int player_dx;
	static int player_dy; // every time player move should update these
	friend bool is_overlapped(VSObject&, VSObject&, double overlap_bound=1);
	static int distance(VSObject&, VSObject&);
	static int distance(CPoint&, CPoint&);
	friend class QuadTree; 
	int obj_type = VSOBJECT;
protected:
	game_framework::CMovingBitmap _skin;
	vector <game_framework::CMovingBitmap> _animations;
	int _selector = 0;
	CPoint _position;
	CPoint _target;
	CPoint _collision;
	CPoint _target_vec = {0,0};
	bool _is_mirror = 0;
	int _direct, _default_direct=LEFT;
	int _speed=0;
	int _pool_id, _type;
	double _scaler = 1;
	double _fx, _fy;
	int _file_size = 0;
	clock_t _animation_cycle_time;
};
CPoint get_player_pos();