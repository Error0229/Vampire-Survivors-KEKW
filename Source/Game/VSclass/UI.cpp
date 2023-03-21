#include "stdafx.h"
#include "../../Library/gameutil.h"
#include <fstream>
#include <sstream>
#include "VSObject.h"
#include "UI.h"
Button::Button() 
{
}
Button::~Button()
{
}
bool Button::is_hover(int mouse_x, int mouse_y) 
{
	return
	(
		_skin.Left() <= mouse_x &&
		_skin.Left() + _skin.Width() >= mouse_x &&
		_skin.Top() <= mouse_y &&
		_skin.Top() + _skin.Width()  >= mouse_y
	);
}
bool Button::is_hover(CPoint pos)
{
	return is_hover(pos.x, pos.y);
}

void Button::show_button() 
{
	VSObject::show_skin();
}

Icon::Icon()
{
	_is_loaded = false;
	if(icon_filename.size()==0)
		load_filename();
	
}
Icon::~Icon()
{
}
void Icon::show_icon(int item_id)
{
	if (!_is_loaded) {
		for(int i=0;i<(int)icon_filename.size(); i++)
			load_skin((char*)icon_filename[i].c_str(), (i<63)?BLACK:WHITE);
		_is_loaded = true;
	}
	set_selector(item_id);
	VSObject::show_skin();
}
void Icon::load_filename()
{
	ifstream file("resources/item_id.csv");
	string header, line;
	string number, id, old_id, filename, type;
	getline(file, header);
	char 🍆[200];
	while (getline(file, line)) {
		stringstream ss(line);
		getline(ss, number, ',');
		getline(ss, id, ',');
		getline(ss, old_id, ',');
		getline(ss, filename, ',');
		getline(ss, type, ',');
		memset(🍆, 0, sizeof(🍆));
		if (filename == "MISSING")
			sprintf(🍆, "resources/weapon/nightSword.bmp"); //placeholder
		else if (type != "passive")
			sprintf(🍆, "resources/weapon/%s.bmp", filename.c_str());
		else if (type == "passive")
			sprintf(🍆, "resources/passive/%s.bmp", filename.c_str());
		icon_filename.push_back(🍆);
	}
}
vector<string> Icon::icon_filename;