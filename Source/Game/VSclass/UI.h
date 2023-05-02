#pragma once
#include "ObjPool.h"
class Ui : public VSObject
{
public:
	Ui();
	Ui(int, int);
	Ui(CPoint);
	~Ui();
	bool is_hover(int, int);
	bool is_hover(CPoint);
	void set_base_pos(CPoint);
	void set_base_pos(int, int);
	void start();
	void reset();
	bool done();
	CPoint get_base_pos();
	virtual void show(double factor=1.0);				// show on "fixed" pos on the screen
	virtual void show(CPoint, double factor=1.0);			// show on real pos on the map
	
	bool activate_hover;
protected:
	clock_t _animation_start_time = -1;
	CPoint _base_pos;
	bool _is_start = false;
};

class Icon : public Ui
{
public:
	Icon();
	Icon(int, int);
	Icon(CPoint);
	~Icon();
	void load_icon();
	void show(double factor = 1.0);				// DONT USE
	void show(CPoint, double factor = 1.0);		// DONT USE
	void show(int, double factor = 1.0);
	void show(CPoint, int, double factor = 1.0);
	static void load_filename();
	static vector<string> icon_filename;
	static vector<string> icon_name;
	static bool file_loaded;
private:
};
class Damage : public Ui {
public:
	static Damage* damage_device();
	void add_damage(int, CPoint);
	void update();
	void show_damage();
	void init();
	~Damage() = default;
private:
	Damage();
	Damage(int, CPoint);
	static Damage dmg;
	static vector<Damage*> all_dmg;
	static ObjPool<Damage> pool;
	int _number;
	int _number_width; 
	vector<VSObject> num_bmp;
};