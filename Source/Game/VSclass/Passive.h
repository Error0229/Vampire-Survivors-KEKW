#pragma once
class Passive : public VSObject {
public:
	Passive();
	~Passive();
	void level_up();
private:
	int _level;
	int _type;
};
