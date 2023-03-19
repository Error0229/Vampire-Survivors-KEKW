#pragma once
enum pickup_type {
	XP,
	CHEST
};	
class Pickup : public VSObject{
public:
	Pickup();
	~Pickup();
	int get_xp_value();
	void spawn_xp(CPoint, int);
	void spawn_chest(CPoint, bool can_evo=false);
	void despawn();
	void show_skin(double factor = 1.0);
	bool is_enable();
	int obj_type = PICKUP;
	static void load_template_pickup();
	static Pickup get_template_pickup(int type);
private:
	void spawn(CPoint);
	int _type, _level;
	int _xp_value;
	bool _is_enable, _can_evo;
	static vector<Pickup> template_pickup;
};
