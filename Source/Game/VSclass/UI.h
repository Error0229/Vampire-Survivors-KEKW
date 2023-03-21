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

class Icon : public VSObject
{
public:
	Icon();
	~Icon();
	void show_icon(int);
	static void load_filename();
	static vector<string> icon_filename;
private:
	bool _is_loaded;
};