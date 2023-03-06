#pragma once
class VSObject
{
public:
	VSObject();
	VSObject(vector<char*> filename);
	virtual ~VSObject();
	void load_skin(vector<char*> filename, COLORREF color= RGB(255,255,255));
	void show_skin();
	void set_pos(CPoint);
	void set_pos(int, int);
	CPoint get_pos();
	void update_pos(CPoint);
	void update_pos();
	int get_height();
	int get_width();
	static int player_dx;
	static int player_dy; // every time player move should update these
	friend bool is_overlapped(VSObject&, VSObject&);
protected:
	game_framework::CMovingBitmap _skin;
	CPoint _position;
	CPoint _target;
	int _speed;
};

