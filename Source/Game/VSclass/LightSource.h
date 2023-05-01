#pragma once

class LightSource : public VSObject {
    public:
    LightSource();
    ~LightSource();
    void show_skin(double factor = 1.0) override;
    void set_spawn(CPoint pos);
    void update_pos(CPoint pos) override;
    bool hurt(int damage);
    bool is_enable();

    friend class LightSourceFactory;

    private:
    clock_t _last_time_got_hit = -100000;
	vector <clock_t> _last_time_got_hit_by_projectile;
    int _hp_max, _hp;
    bool _is_enable;
    VSObject _hit_animation;
};


class LightSourceFactory {
    public:
    LightSourceFactory();
	~LightSourceFactory();
    int get_number_all();
	void init();
	void reset();
    void update(clock_t tick, CPoint player_pos, int luck);
    void spawn_lightsource(CPoint player_pos);
    static deque<LightSource*> light_sourse_all;

    private:
    static ObjPool<LightSource> pool;
    clock_t _last_t;
    const int MAX_SPAWN = 20;
};
