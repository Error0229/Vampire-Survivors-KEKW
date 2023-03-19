#pragma once
class Button : public VSObject
{
public:
	Button();
	~Button();
	bool is_hover(int, int);
	bool is_hover(CPoint);
	void show_button();
private:

};

