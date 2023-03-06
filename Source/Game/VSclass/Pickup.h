#pragma once
class Pickups : public VSObject{
public:
	Pickups();
	~Pickups();
private:
	int _type, _level;
};
