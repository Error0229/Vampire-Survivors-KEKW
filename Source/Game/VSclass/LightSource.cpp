#include "stdafx.h"
#include "../../Library/audio.h"
#include "../../Library/gameutil.h"
#include "VSObject.h"
#include "Ui.h"
#include "Enemy.h"
#include "Pickup.h"
#include "VSUtil.h"
#include "LightSource.h"

LightSource::LightSource()
{
    obj_type = LIGHTSOURCE;
    _type = LIGHTSOURCE;
    _last_time_got_hit_by_projectile.resize(100, -1000000);
	_last_time_got_hit = -1000000;
    _is_enable = false;
    _hp_max = 10;
    _hp = 0;
    load_skin({"Resources/Enemy/Brazier1.bmp", "Resources/Enemy/Brazier2.bmp", "Resources/Enemy/Brazier3.bmp"});
    _hit_animation.load_skin({ "Resources/hit_effect/Shockwave3.bmp" });
}

LightSource::~LightSource() 
{
}

void LightSource::show_skin(double factor)
{
    auto dt = clock() - _last_time_got_hit;
	if (dt < 120) {
		_hit_animation.set_pos(_position);
		_hit_animation.show_skin(factor + static_cast<double>(dt) / 1000.0 -0.3);
	}
	if ( !_is_enable )
		return;
	else
		VSObject::show_skin();
}

void LightSource::set_spawn(CPoint player_pos)
{
    set_animation(30, false);
    _is_enable = true;
	_hp = _hp_max;
    static vector<double> random_pos_weights(88, 1);
	int i = poll(random_pos_weights);
	if (i <= 21)
		_position = player_pos + CPoint(-440 + i * 40, -330);
	else if (i <= 43)
		_position = player_pos + CPoint(440, -330 + (i - 21) * 30);
	else if (i <= 65)
		_position = player_pos + CPoint(440 - (i - 43) * 40, 330);
	else
		_position = player_pos + CPoint(-440, 330 - (i - 65) * 30);
}

void LightSource::update_pos(CPoint pos)
{
    VSObject::update_pos(pos);
}

bool LightSource::hurt(int damage)
{
    Damage::damage_device()->add_damage(damage, _position);
    if(_is_enable){
        _hp -= damage;
        game_framework::CAudio::Instance()->Play(2, false);
        if (_hp <= 0) {
            // unshow_skin();
            _is_enable = false;
            _hp = 0;

            // SPAWN SOMETHING
            static vector<double> weights = {50, 10, 1, 1, 2, 12, 1};
            LightSourcePickup::spawn_lightsource_pickup(_position, poll(weights));
            
            return true;
        }
    }
    return false;
}

bool LightSource::is_enable()
{
    return _is_enable;
}



LightSourceFactory::LightSourceFactory()
{
}

LightSourceFactory::~LightSourceFactory()
{
}

int LightSourceFactory::get_number_all()
{
    return light_sourse_all.size();
}

void LightSourceFactory::init()
{
    LightSource 🔥;
    pool.add_obj(🔥, MAX_SPAWN);
}

void LightSourceFactory::reset()
{
    _last_t = -1;
    for (auto 🔥 : light_sourse_all) {
		pool.free_obj_ptr(🔥);
	}
}

void LightSourceFactory::update(clock_t tick, CPoint player_pos, int luck)
{
    // check exist
    bool is_clear = false;
    for(auto 🔥: light_sourse_all){
        if(!🔥->_is_enable){
            is_clear = true;
            pool.free_obj_ptr(🔥);
        }
    }
    if(is_clear){
        auto iter = remove_if(light_sourse_all.begin(), light_sourse_all.end(), [](LightSource* 🔥) {return !🔥->_is_enable; });
        light_sourse_all.erase(iter, light_sourse_all.end());
    }

    if(tick - _last_t < 1000)
        return;
    _last_t = tick;
    if ((int)light_sourse_all.size() >= MAX_SPAWN)
        return;

    //spawn new
    vector<double> weights(2, 0);
    weights[1] = max(0.1 * luck / 100, 0.5);
    weights[0] = 1 - weights[1];
    if (poll(weights)) {
        spawn_lightsource(player_pos);
    }
}

void LightSourceFactory::spawn_lightsource(CPoint player_pos)
{
    auto 🔥 = pool.get_obj_ptr(LIGHTSOURCE);
    🔥->set_spawn(player_pos);
    light_sourse_all.push_back(🔥);
}

ObjPool<LightSource> LightSourceFactory::pool(LIGHTSOURCE);
deque<LightSource*> LightSourceFactory::light_sourse_all;