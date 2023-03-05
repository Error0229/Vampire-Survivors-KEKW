#pragma once
class Pickups{
public:
	Pickups();
	~Pickups();
	void set_pos(int, int);
private:
	int _type, _level;
	int _x, _y;
};
