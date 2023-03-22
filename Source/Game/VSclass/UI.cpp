#include "stdafx.h"
#include "../../Library/gameutil.h"
#include <fstream>
#include <sstream>
#include "VSObject.h"
#include "UI.h"
#include "../config.h"

Ui::Ui(CPoint base_pos)
{
	_base_pos = base_pos;
	activate_hover = false;
}
Ui::Ui(int, int)
{
	_base_pos = CPoint(0, 0);
	activate_hover = false;
}
Ui::Ui()
{
	_base_pos = CPoint(0, 0);
	activate_hover = false;
}
Ui::~Ui()
{
}
bool Ui::is_hover(int mouse_x, int mouse_y)
{
	if (!activate_hover)
		return false;
	int dx = abs(mouse_x - _position.x);
	int dy = abs(mouse_y - _position.y);
	return dx <= (get_width() >> 1) && dy <= (get_height() >> 1);
}
bool Ui::is_hover(CPoint pos)
{
	return is_hover(pos.x, pos.y);
}
void Ui::set_base_pos(CPoint pos)
{
	_base_pos = pos;
}
void Ui::set_base_pos(int x, int y)
{
	_base_pos = CPoint(x, y);
}
CPoint Ui::get_base_pos()
{
	return _base_pos;
}
void Ui::show()
{
	CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
	_position = _base_pos + player_pos;
	VSObject::show_skin();
}
void Ui::show(CPoint pos)
{
	_position = pos;
	VSObject::show_skin();
}

Icon::Icon(CPoint base_pos) :Ui(base_pos)
{
}
Icon::Icon(int x, int y) :Ui(x, y)
{
}
Icon::Icon() :Ui()
{
}
Icon::~Icon()
{
}
void Icon::load_icon()
{
	VS_ASSERT(icon_filename.size()!=0, "please icon filename before loading icon");
	for (int i = 0; i < (int)icon_filename.size(); i++)
		load_skin((char*)icon_filename[i].c_str(), (i < 63) ? BLACK : WHITE);
}
void Icon::show()
{
	VS_ASSERT(false, "please use show_icon with item_id");
}
void Icon::show(CPoint pos)
{
	VS_ASSERT(false, "please use show_icon with item_id");
}
void Icon::show(int item_id)
{
	set_selector(item_id);
	Ui::show();
}
void Icon::show(CPoint pos, int item_id)
{
	set_selector(item_id);
	Ui::show(pos);
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