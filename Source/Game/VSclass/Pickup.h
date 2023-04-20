#pragma once
#include "ObjPool.h"
class Pickup : public VSObject{
public:
	Pickup();
	~Pickup();
	bool is_enable();
	virtual void spawn(CPoint);
	void despawn();
	virtual void show_skin(double factor = 1.0);
	int obj_type = PICKUP;
protected:
	int _weight;
	bool _is_enable;
	static vector<Pickup> template_pickup;
};

class Xp : public Pickup {
public:
	Xp();
	Xp(CPoint pos, int xp_value);
	~Xp();
	void spawn(CPoint, int);
	int get_xp_value();
	void show_skin(double factor = 1.0);
	static void init_XP();
	static void spawnXP(CPoint, int);
	static void show();
	static void reset_XP();
	static void update_XP_pos(int player_magnet);
	static deque<Xp*> xp_all;
	static ObjPool<Xp> pool;
private:
	int _xp_value;
	bool _is_moving = false;
};

class Chest : public Pickup {
public:
	Chest();
	~Chest();
	void spawn(CPoint, bool, int, int);
	bool get_can_evo();
	int get_upgrade_chance_0();
	int get_upgrade_chance_1();
	static void init_chest();
	static void spawnChest(CPoint, bool, int, int);
	static void show();
	static deque<Chest*> chest_all;
	static ObjPool<Chest> pool;
private:
	bool _can_evo;
	int _upgrade_chance_0;
	int _upgrade_chance_1;
};