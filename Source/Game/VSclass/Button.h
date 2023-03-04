#pragma once
class Button
{
public:
	Button();
	~Button();
	void is_hover();
	void is_press();
	void set_pos(int, int);
	void show_button();
private:
	game_framework::CMovingBitmap _skin;
	int _x, _y;

};

