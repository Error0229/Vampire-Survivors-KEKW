#pragma once
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
	CPoint get_base_pos();
	virtual void show();				// show on "fixed" pos on the screen
	virtual void show(CPoint);			// show on real pos on the map
	
	bool activate_hover;
protected:
	CPoint _base_pos;
};

class Icon : public Ui
{
public:
	Icon();
	Icon(int, int);
	Icon(CPoint);
	~Icon();
	void load_icon();
	void show();			// DONT USE
	void show(CPoint);		// DONT USE
	void show(int);
	void show(CPoint, int);
	static void load_filename();
	static vector<string> icon_filename;
private:

};