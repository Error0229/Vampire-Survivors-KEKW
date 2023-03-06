#pragma once
class Button
{
public:
	Button();
	~Button();
	bool is_hover(int, int);
	void set_pos(int, int);
	void show_button();

	//temporary
	game_framework::CMovingBitmap& get_skin();
private:
	game_framework::CMovingBitmap _skin;
	int _x, _y;

};

