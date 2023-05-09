#pragma once
#include "../config.h"
#define VSOM(x) ((x * 10) >> 10) 
// VS oprimizer
int square(int x);
int fast_sqrt(int x);
bool is_overlapped_pure(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
int poll(vector<double>& weights, bool handle_negtive = false);
int hit_border(int bx, int by, int w, int h, int obj_x, int obj_y);
CPoint get_ellipse_point(CPoint center, int long_r, int short_r, int count, int n);
class VSTimer{
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
	void add_time(clock_t);
	string get_minute_string();
	string get_second_string();
private:
	clock_t start_ticks;
	clock_t paused_ticks;
	bool running;
};
#define VS_ASSERT(boolexp,str)											\
		if (!(boolexp)) {													\
			int id;															\
			char s[300]="";													\
			sprintf(s,"Game fatal error:\n\n%s\n\nFile: %s\n\nLine: %d"		\
				"\n\n(Press Retry to debug the application, "				\
				"if it is executed in debug mode.)"							\
				"\n(Press Cancel otherwise.)",								\
				 str , __FILE__,__LINE__);									\
			id = AfxMessageBox(s, MB_RETRYCANCEL);							\
																			\
			if (id == IDCANCEL)												\
				exit(1);													\
			AfxDebugBreak();												\
		}
enum direction { UP, DOWN, LEFT, RIGHT };
enum VSObject_types {
	VSOBJECT = 499,
	PLAYER,
	PASSIVE,
	ENEMY,
	LIGHTSOURCE,
	WEAPON,
	PROJECTILE,
	PICKUP,
	WALL,
	MAP,
	DAMAGE,
	UI,
	OBSTACLE
};
enum weapon_names {
	WHIP = 0, MAGIC_MISSILE, KNIFE, AXE, CROSS, HOLYBOOK,
	FIREBALL, GARLIC, HOLYWATER, DIAMOND, LIGHTNING,
	PENTAGRAM, SILF, SILF2, GUNS, GUNS2, GATTI, SONG,
	TRAPING, LANCET, LAUREL, VENTO, BONE, CHERRY,
	CART2, FLOWER, LAROBBA, JUBILEE, TRIASSO1, CANDYBOX,
	VICTORY, MISSPELL
};
enum evolution_weapon_names {
	VAMPIRICA = 32, HOLY_MISSILE, THOUSAND, SCYTHE,
	HEAVENSWORD, VESPERS, HELLFIRE, VORTEX, BORA,
	ROCHER, LOOP, SIRE, STIGRANGATTI, MANNAGGIA,
	TRAPANO2, MISSPELL2, CORRIDOR, SHROUD, TRIASSO2,
	TRIASSO3, GUNS3, SILF3, VENTO2, SOLES, CANDYBOX2
};
enum pickup_types {
	COIN,
	COIN_BAG,
	RICH_COIN_BAG,
	ROSARY,
	VACUUM,
	CHICKEN,
	LITTLE_CLOVER,
	XP,
	CHEST,
	LSPICKUP
};

#define BLACK (RGB(0,0,0))
#define WHITE (RGB(255,255,255))
const double MATH_PI = 3.1415926;
constexpr int w_size_x = (OPEN_AS_FULLSCREEN ? RESOLUTION_X : SIZE_X);
constexpr int w_size_y = (OPEN_AS_FULLSCREEN ? RESOLUTION_Y : SIZE_Y);
extern int MAP_ID; // by using extern, can avoid static variable copy problem, and don't need getter and setter
extern int GOLD_NUM;
extern int KILL_NUM;