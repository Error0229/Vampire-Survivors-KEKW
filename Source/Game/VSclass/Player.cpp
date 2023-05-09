#include "stdafx.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Weapon.h"
#include "Passive.h"
#include "Player.h"
#include <string>
#include <fstream>
#include <sstream>
using namespace game_framework;
map<string, Player> Player::template_player;
Player::Player() {
	_type = PLAYER;
	obj_type = PLAYER;
};
Player::~Player() = default;
Player::Player(string name) {
	*this = template_player[name];
	init_stats();
	acquire_weapon(_weapon_type);
}
void Player::init_stats() {
	static array<int, 16> power_up = {5, 1, 1, 1, 3, 5, 10, 15, 1, 5, 25, 10, 3, 10, 1, 10};
	ifstream fin("save/save_data.csv");
	vector <int> data;
	string line, token;
	getline(fin, line); // header
	getline(fin, line);
	stringstream ss(line);
	getline(ss, token, ',');
	while (getline(ss, token, ',')) {
		data.push_back(stoi(token));
	}
	auto f = [=](int x) noexcept -> int {return data[x] * power_up[x];};
	fin.close();
	obj_type = PLAYER;
	_coef_might = 100 + f(0); 
	_armor = f(1);
	_coef_max_health = 100 + f(2);
	_recovery = static_cast<double>(f(3)) / 10;
	_coef_cooldown = 100 + f(4); 
	_coef_area = 100 + f(5);
	_coef_proj_speed = 100 + f(6);
	_coef_duration = 100 + f(7);
	_amount = f(8);
	_coef_move_speed = 100 + f(9); 
	_coef_magnet = 100 + f(10);
	_coef_luck = 100 + f(11);
	_coef_growth = 100 + f(12);
	_coef_greed = 100 + f(13);
	_revival = f(14);
	_coef_curse = 100 + f(15);
	_reroll = 0;
	_exp = 0;
	_base_speed = 300;
	_base_max_health = 150;
	_base_magnet = 30;
	_max_exp = 5;
	_level = 1;
	_max_health = 150;
	_hp = _max_health;

	_base_stats = {
		{"speed", _speed},
		{"might", _coef_might},
		{"armor", _armor},
		{"max_health", _coef_max_health},
		{"recovery", _recovery},
		{"cooldown", _coef_cooldown},
		{"area", _coef_area},
		{"proj_speed", _coef_proj_speed},
		{"duration", _coef_duration},
		{"amount", _amount},
		{"move_speed", _coef_move_speed},
		{"magnet", _coef_magnet},
		{"luck", _coef_luck},
		{"growth", _coef_growth},
		{"greed", _coef_greed},
		{"revival", _revival},
		{"curse", _coef_curse }
	};
	update_all_passive_effect();
}
void Player::init_player() {

	ifstream file("source/game/VSclass/player_data.csv");
	string header, line, skin_file, name, token;
	getline(file, header); // no use
	while (getline(file, line)) {
		Player p;
		int cnt;
		vector <string> skin_files;
		stringstream ss(line);
		getline(ss, skin_file, ',');
		getline(ss, token, ',');
		cnt = stoi(token);
		getline(ss, name, ',');
		getline(ss, token, ',');
		p._weapon_type = stoi(token);
		p._name = name;
		for (int i = 1; i <= cnt; i++) {
			skin_files.push_back("resources/character/" + skin_file + "_0" +to_string(i) + ".bmp");
		}
		p.load_skin(skin_files);
		template_player[p._name] = p;
	}
}
void Player::update_pos(CPoint target) {
	CPoint pos = _position;
	VSObject::update_pos(target);
	VSObject::player_dx -= _position.x - pos.x ;
	VSObject::player_dy -= _position.y - pos.y ;
}
void Player::load_bleed() {
	_bleed_animation.load_skin({ "resources/character/Blood1.bmp", "resources/character/Blood2.bmp", "resources/character/Blood3.bmp" });
	_bleed_animation.load_mirror_skin();
	_bleed_animation.enable_animation();
}
void Player::show_skin(double factor) {
	
	VSObject::show_skin(factor);
	if (_is_hurt && clock() - _last_time_got_hit < 300) {
		_bleed_animation.set_pos(_position);
		_bleed_animation.show_skin();
		// _is_hurt = false;
	}
	else {
		_bleed_animation.unshow_skin();
	}
}
void Player::hurt(int damage) {
	if (clock() - _last_time_got_hit < 250) {
		return;
	}
	_last_time_got_hit = clock();
	_hp -= damage;
	_is_hurt = true;
	_bleed_animation.set_animation(100, true);	//set_animation everytime because unshow will make the animation ended
	_bleed_animation.set_animation_frame(0);
	_bleed_animation.enable_animation();
	if (_hp < 0) {
		_hp = 0;
	}
}
void Player::set_speed(int s){
	_speed = static_cast<int>(s * _coef_move_speed / 100.0);
};
void Player::set_speed(double s) {
	set_speed(static_cast<int> (s));
};
void Player::acquire_weapon(Weapon& weapon) {
	Weapon::all_weapon.push_back(weapon);
	Weapon::update_all_weapon_stats(_coef_might, _coef_cooldown, _coef_proj_speed, _coef_duration, _amount, _coef_area);
}
void Player::acquire_weapon(int weapon_id) {
	Weapon::all_weapon.push_back(Weapon::_base_weapon[weapon_id]);
	Weapon::update_all_weapon_stats(_coef_might, _coef_cooldown, _coef_proj_speed, _coef_duration, _amount, _coef_area);
}
void Player::acquire_passive(Passive& passive) {
	Passive::all_passive.push_back(passive);
	update_all_passive_effect();
}
void Player::acquire_passive(int passive_id) {
	Passive::all_passive.push_back(Passive(passive_id));
	update_all_passive_effect();
}
void Player::update_all_passive_effect() {
	_coef_might = any_cast<int>(_base_stats["might"]) + Passive::get_effect(POWER) + Passive::get_effect(PANDORA);
	_armor = any_cast<int>(_base_stats["armor"]) + Passive::get_effect(ARMOR);
	_coef_max_health = any_cast<int>(_base_stats["max_health"]) + Passive::get_effect(MAXHEALTH) + Passive::get_effect(pLEFT);
	_recovery = any_cast<double>(_base_stats["recovery"]) + Passive::get_effect(REGEN)/100.0 + Passive::get_effect(pLEFT) / 500.0;
	_coef_cooldown = any_cast<int>(_base_stats["cooldown"]) + Passive::get_effect(COOLDOWN);
	_coef_area = any_cast<int>(_base_stats["area"]) + Passive::get_effect(AREA) + Passive::get_effect(SILVER) + Passive::get_effect(PANDORA);
	_coef_proj_speed = any_cast<int>(_base_stats["proj_speed"]) + Passive::get_effect(SPEED) + Passive::get_effect(PANDORA);
	_coef_duration = any_cast<int>(_base_stats["duration"]) + Passive::get_effect(DURATION) + Passive::get_effect(SILVER) + Passive::get_effect(PANDORA);
	_amount = any_cast<int>(_base_stats["amount"]) + Passive::get_effect(AMOUNT);
	_coef_move_speed = any_cast<int>(_base_stats["move_speed"]) + Passive::get_effect(MOVESPEED);
	_coef_magnet = any_cast<int>(_base_stats["magnet"]) + Passive::get_effect(MAGNET);
	_coef_luck = any_cast<int>(_base_stats["luck"]) + Passive::get_effect(LUCK);
	_coef_growth = any_cast<int>(_base_stats["growth"]) + Passive::get_effect(GROWTH);
	_coef_greed = any_cast<int>(_base_stats["greed"]) + Passive::get_effect(GREED);
	_revival = any_cast<int>(_base_stats["revival"]) + Passive::get_effect(REVIVAL);
	_coef_curse = any_cast<int>(_base_stats["curse"]) + Passive::get_effect(CURSE) + Passive::get_effect(GOLD) + Passive::get_effect(pRIGHT);
	Weapon::update_all_weapon_stats(_coef_might, _coef_cooldown, _coef_proj_speed, _coef_duration, _amount, _coef_area); // should this be here ? 
	_speed = static_cast<int>(_base_speed * _coef_move_speed / 100.0);
	_max_health = static_cast<int>(_base_max_health * _coef_max_health / 100.0);
	_magnet = static_cast<int>(_base_magnet * _coef_magnet / 100.0);
}

void Player::level_up_passive(Passive& p) {
	VS_ASSERT(!p.is_max_level(), "level up passive above max level.");
	p.level_up();
}
void Player::level_up_passive(int passive_id) {
	Passive::upgrade(passive_id);
	update_all_passive_effect();
}

void Player::pick_up_xp(int xp_value)
{
	_exp += xp_value;
}
bool Player::apply_level_up()
{
	VS_ASSERT(_exp >= _max_exp, "attemp to lvl up when xp < max_xp");
	_level++;
	_exp -= _max_exp;

	if (_level < 20)
		_max_exp += 10;
	else if (_level < 40)
		_max_exp += 13;
	else
		_max_exp += 16;

	if (_level == 20) {
		_max_exp += 600;
		_base_stats["growth"] = any_cast<int>(_base_stats["growth"]) + 100;
	}
	else if (_level == 40) {
		_max_exp += 2400;
		_base_stats["growth"] = any_cast<int>(_base_stats["growth"]) + 100;
	}
	else if (_level == 21 || _level == 41) {
		_base_stats["growth"] = any_cast<int>(_base_stats["growth"]) - 100;
	}

	_coef_growth = any_cast<int>(_base_stats["growth"]) + Passive::get_effect(GROWTH);
	return (_exp >= _max_exp);
}
int Player::get_pickup_range()
{
	return _magnet;
}
int Player::get_level()
{
	return _level;
}
int Player::get_exp_percent()
{
	return (_exp < _max_exp) ? (_exp * 100 / _max_exp):(100);
}
int Player::get_move_speed()
{
	return _speed;
}
int Player::get_magnet()
{
	return _magnet;
}
int Player::get_luck()
{
	return _coef_luck;
}
int Player::get_curse()
{
	return _coef_curse;
}
int Player::get_duration() {
	return _coef_duration;
}
int Player::get_revival() {
	return _revival;
}
int Player::get_greed() {
	return _coef_greed;
}
void Player::revive() {
	_hp = _max_health;
	_revival -= 1;
}
vector<stat_struct> Player::get_stats_string()
{
	vector<stat_struct> stats;
	stats.push_back({ MAXHEALTH,"Max Health",	to_string(_max_health) });
	string 💕 = to_string(_recovery);
	💕 = 💕.substr(0, 💕.find(".") + 3);
	stats.push_back({ REGEN,	"Recovery",		💕 }); // 1:double
	stats.push_back({ ARMOR,	"Armor",		stat_to_string(_armor, false) });
	stats.push_back({ MOVESPEED,"Move Speed",	stat_to_string(_coef_move_speed)});
	stats.push_back({ POWER,	"Might",		stat_to_string(_coef_might) });
	stats.push_back({ SPEED,	"Speed",		stat_to_string(_coef_proj_speed) });
	stats.push_back({ DURATION,	"Duration",		stat_to_string(_coef_duration) });
	stats.push_back({ AREA,		"Area", 		stat_to_string(_coef_area) });
	stats.push_back({ COOLDOWN, "Cooldown",		stat_to_string(_coef_cooldown) });
	stats.push_back({ AMOUNT,	"Amount",		stat_to_string(_amount, false) });
	stats.push_back({ REVIVAL,	"Revival",		stat_to_string(_revival, false) });
	stats.push_back({ MAGNET,	"Magnet",		to_string(_magnet) });
	stats.push_back({ LUCK,		"Luck",			stat_to_string(_coef_luck) });
	stats.push_back({ GROWTH,	"Growth",		stat_to_string(_coef_growth) });
	stats.push_back({ GREED,	"Greed",		stat_to_string(_coef_greed) });
	stats.push_back({ CURSE,	"Curse",		stat_to_string(_coef_curse) });
	//Reroll
	//Skip
	//Banish
	return stats;
}
int Player::get_hp_percent()
{
	return (_hp < _max_health) ? (_hp * 100 / _max_health) : (100);
}
bool Player::is_hurt()
{
	return _is_hurt;
}
bool Player::have(int type) {
	for (auto& i : Weapon::all_weapon) {
		if (i.get_type() == type) {
			return true;
		}
	} 
	for (auto& i : Passive::all_passive) {
		if (i.get_type() == type) {
			return true;
		}
	}
	return false;
}
void Player::obtain_item(int type)
{
	bool is_own = false;
	if (type < 32) {
		//weapon
		if (this->have(type)) {
			Weapon::upgrade(type);
		}
		else {
			acquire_weapon(type);
		}
	}
	else if (type < 63) {
		Weapon::evolution(type);
	}
	else {
		//passive
		if (this->have(type)) {
			level_up_passive(type);
		}
		else {
			acquire_passive(type);
		}
	}
}

bool Player::all_max()
{
	for (auto& i : Weapon::all_weapon) {
		if (!i.is_max_level())
			return false;
	}
	for (auto& i : Passive::all_passive) {
		if (!i.is_max_level())
			return false;
	}
	return true;
}
bool Player::full_inv()
{
	return Weapon::weapon_count() >= 6 && Passive::all_passive.size() >= 6;
}
string Player::stat_to_string(int val, bool percent)
{
	string 🍆_;
	if (percent) {
		if(val >= 100)
			🍆_ = "+" + to_string(val - 100) + "%";
		else
			🍆_ = to_string(val - 100) + "%";
	}
	else if(val >= 0)
		🍆_ = "+" + to_string(val);
	else
		🍆_ = to_string(val);
	return 🍆_;
}
void Player::regen(double amount)
{
	static double 💕 = 0;
	static time_t last_t = clock();
	if (_hp >= _max_health)
		return;
	if (amount == 0) {
		if (clock() - last_t >= 1000) {
			💕 += _recovery;
			last_t = clock();
		}
	}
	else {
		💕 += amount;
	}
	if (💕 >= 1) {
		_hp += (int)💕;
		💕 -= (int)💕;
		if (_hp >= _max_health) {
			_hp = _max_health;
			💕 = 0;
		}
	}
}

void Player::increase_luck(int amount)
{
	_coef_luck += amount;
}