#pragma once
class Pickup : public VSObject{
public:
	Pickup();
	~Pickup();
	void load_xp();
	static void load_xp(vector<Pickup>&, int);
	void load_chest();
	void set_enable(bool enable=true);
	int get_xp_value();
	void spawn_xp(CPoint, int);
	void show_skin(double factor = 1.0);
	bool is_enable();
private:
	int _type, _level;
	int _xp_value;
	bool _is_enable = false;
};
