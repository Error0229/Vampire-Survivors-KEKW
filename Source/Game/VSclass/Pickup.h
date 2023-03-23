#pragma once
class Pickup : public VSObject{
public:
	Pickup();
	~Pickup();
	bool is_enable();
	virtual void spawn(CPoint, int);
	void despawn();
	virtual void show_skin(double factor = 1.0);
	int obj_type = PICKUP;
protected:
	int _type, _weight;
	bool _is_enable;
	static vector<Pickup> template_pickup;
};

class Xp : public Pickup {
public:
	Xp();
	~Xp();
	void spawn(CPoint, int);
	int get_xp_value();
	void show_skin(double factor = 1.0);
private:
	int _xp_value;
};

class Chest : public Pickup {
public:
	Chest();
	~Chest();
	void spawn(CPoint, int);
	bool get_can_evo();
private:
	bool _can_evo;
};