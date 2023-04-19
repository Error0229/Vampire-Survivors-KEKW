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
void Ui::show(double factor)
{
	CPoint player_pos = { (OPEN_AS_FULLSCREEN ? RESOLUTION_X >> 1 : SIZE_X >> 1) - VSObject::player_dx,(OPEN_AS_FULLSCREEN ? RESOLUTION_Y >> 1 : SIZE_Y >> 1) - VSObject::player_dy };
	_position = _base_pos + player_pos;
	VSObject::show_skin(factor);
}
void Ui::show(CPoint pos, double factor)
{
	_position = pos;
	VSObject::show_skin(factor);
}
void Ui::start() {
	if (!_is_start) {
		_animation_start_time = clock();
		_is_start = true;
	}
}
bool Ui::done()
{
	return ((clock() - _animation_start_time) > _animation_cycle_time);
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
		load_skin((char*)icon_filename[i].c_str());
}
void Icon::show(double factor)
{
	VS_ASSERT(false, "please use show_icon with item_id");
}
void Icon::show(CPoint pos, double factor)
{
	VS_ASSERT(false, "please use show_icon with item_id");
}
void Icon::show(int item_id, double factor)
{
	set_selector(item_id);
	Ui::show(factor);
}
void Icon::show(CPoint pos, int item_id, double factor)
{
	set_selector(item_id);
	Ui::show(pos, factor);
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
vector<Damage*> Damage::all_dmg;
ObjPool<Damage> Damage::pool(DAMAGE);
Damage Damage::dmg;
Damage* Damage::damage_device() {
	return &dmg;
}
Damage::Damage() {
	_type = DAMAGE;
}
void Damage::init() {
	pool.add_obj(Damage(0, { 0,0 }), 2000);
}
Damage::Damage(int damage, CPoint pos) :_number(damage) {
	_position = pos;
	VSObject tmp({ "resources/damage/0.bmp","resources/damage/1.bmp","resources/damage/2.bmp","resources/damage/3.bmp","resources/damage/4.bmp","resources/damage/5.bmp","resources/damage/6.bmp","resources/damage/7.bmp","resources/damage/8.bmp","resources/damage/9.bmp" });
	tmp.set_speed(25);
	tmp.set_target_vec(0, -100);
	_number_width = tmp.get_width();
	tmp.set_selector(-1);
	for (int i = 0; i < 4; i++) {
		num_bmp.push_back(tmp);
	}
	_animation_cycle_time = 500;
	_type = DAMAGE;
}
void Damage::add_damage(int num, CPoint pos) {
	Damage* ptr = pool.get_obj_ptr(DAMAGE);
	ptr->_number = num ;
	string s = to_string(num);
	if (num >= 100) {
		ptr->num_bmp[0].set_selector(s[0] - '0');
		ptr->num_bmp[0].set_pos(pos.x - ptr->_number_width - 1, pos.y);
		ptr->num_bmp[1].set_selector(s[1] - '0');
		ptr->num_bmp[1].set_pos(pos.x, pos.y);
		ptr->num_bmp[2].set_selector(s[2] - '0');
		ptr->num_bmp[2].set_pos(pos.x + ptr->_number_width + 1, pos.y);
	}
	else if (num >= 10) {
		ptr->num_bmp[0].set_selector(s[0] - '0');
		ptr->num_bmp[0].set_pos(pos.x - ptr->_number_width - 1, pos.y);
		ptr->num_bmp[1].set_selector(s[1] - '0');
		ptr->num_bmp[1].set_pos(pos.x, pos.y);
	} 
	else {
		ptr->num_bmp[0].set_selector(num);
		ptr->num_bmp[0].set_pos(pos);
	}
	ptr->start();
	all_dmg.push_back(ptr);
}
void Damage::update() {
	for (auto ptr : all_dmg) {
		for(auto & bmp : ptr->num_bmp)
			bmp.update_pos_by_vec();
	}
}
void Damage::show_damage() {
	for (int i = 0; i < (int)all_dmg.size(); i++) {
		if (all_dmg[i]->done()) {
			pool.free_obj_ptr(all_dmg[i]);
		}
		else {
			for (auto& bmp : all_dmg[i]->num_bmp) {
				if(bmp.get_animation_frame() != -1)
					bmp.show_skin();
			}
		}
	}
	auto iter = remove_if(all_dmg.begin(), all_dmg.end(), [](Damage* d) {
		return d->done();
	});
	all_dmg.erase(iter, all_dmg.end());
}
