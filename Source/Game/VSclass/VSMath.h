#pragma once
#define VSOM(x) ((x * 10) >> 10) 
// VS oprimizer
int square(int x);
int fast_sqrt(int x);
bool is_overlapped_pure(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
int poll(vector<double>& weights, bool handle_negtive=false);

class VSTimer {
public:
	VSTimer();
	~VSTimer();
	void start();
	void reset();
	void pause();
	void resume();
	bool is_running();
	clock_t get_ticks();
	int get_second();
	string get_minute_string();
	string get_second_string();
private:
	clock_t start_ticks;
	clock_t paused_ticks;
	bool running;
};;