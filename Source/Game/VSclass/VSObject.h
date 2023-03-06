#pragma once
class VSObject
{
public:
	VSObject();
	virtual ~VSObject();
	void load_skin(vector<char*> filename, COLORREF color);
	void show_skin();
	void set_pos(CPoint);
	void set_pos(int, int);
	int get_height();
	int get_width();
	CPoint get_pos();
	void move_toward(CPoint, float);
	static int player_dx;
	static int player_dy; // every time player move should update these
	friend bool is_overlapped(VSObject&, VSObject&);
private:
	game_framework::CMovingBitmap _skin;
	CPoint _position;
};

