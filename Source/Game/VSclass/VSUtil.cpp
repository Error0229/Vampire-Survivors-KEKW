#include "stdafx.h"
#include <vector>
#include <cstdint>
#include <random>
#include "../config.h"
#include "../../Library/gameutil.h"
#include "VSUtil.h"
#include <string>


int square(int x) {
    return x * x;
}

int fast_sqrt(int x)
{
    union { float f; int32_t x; } v;
    v.f = (float)x;
    // fast aprox sqrt
    //  assumes float is in IEEE 754 single precision format 
    //  assumes int is 32 bits
    //  b = exponent bias
    //  m = number of mantissa bits
    v.x -= 1 << 23; // subtract 2^m 
    v.x >>= 1;       // divide by 2
    v.x += 1 << 29; // add ((b + 1) / 2) * 2^m

    // convert to int
    return (int)((float)v.f);
}
bool is_overlapped_pure(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}
int poll(vector<double>& weights, bool handle_negtive)
{
    // handle negtive: set negtive weight to 0, return -1 if all weights are zero
    if (handle_negtive) {
        bool all_zero = true;
        for (auto& i : weights) {
            if (i < 0)
                i = 0;
            else if (i > 0)
                all_zero = false;
        }
        if (all_zero)
            return -1;
    }
    static random_device rd;
    static mt19937 gen(rd());
    discrete_distribution<> dist(weights.begin(), weights.end());
    return dist(gen);
}

VSTimer::VSTimer()
{
	start_ticks = 0;
	paused_ticks = 0;
	running = false;
}
VSTimer::~VSTimer()
{
}
void VSTimer::start()
{
    if (!running) {
	    running = true;
	    start_ticks = clock();
    }
}
void VSTimer::reset()
{
	running = false;
	start_ticks = 0;
	paused_ticks = 0;
}
void VSTimer::pause()
{
    if (running) {
		running = false;
		paused_ticks = clock() - start_ticks;
	}
}
void VSTimer::resume()
{
    if (!running) {
		running = true;
		start_ticks = clock() - paused_ticks;
		paused_ticks = 0;
	}
}
bool VSTimer::is_running()
{
	return running;
}
clock_t VSTimer::get_ticks()
{
	if (running)
		return clock() - start_ticks;
	else
		return paused_ticks;
}
int VSTimer::get_second()
{
	return get_ticks() / 1000;
}
string VSTimer::get_minute_string()
{
    string s = to_string(get_ticks()/60000);
    if (s.size() == 1)
		s = "0" + s;
    return s;
}
string VSTimer::get_second_string()
{
	string s = to_string(get_ticks()/1000%60);
	if (s.size() == 1)
		s = "0" + s;
	return s;
}
constexpr int w_size_x = (OPEN_AS_FULLSCREEN ? RESOLUTION_X : SIZE_X);
constexpr int w_size_y = (OPEN_AS_FULLSCREEN ? RESOLUTION_Y : SIZE_Y);