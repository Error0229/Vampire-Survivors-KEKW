#pragma once
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
	WEAPON,
	PROJECTILE,
	PICKUP,
	WALL,
	MAP
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
	VAMPIRICA = 32, HOLY_MISSLE, THOUSAND, SCYTHE,
	HEAVENSWORD, VESPERS, HELLFIRE, VORTEX, BORA,
	ROCHER, LOOP, SIRE, STIGRANGATTI, MANNAGGIA,
	TRAPANO2, MISSPELL2, CORRIDOR, SHROUD, TRIASSO2,
	TRIASSO3, GUNS3, SILF3, VENTO2, SOLES, CANDYBOX2
};
enum pickup_types {
	XP,
	COIN,
	COIN_BAG,
	Rich_Coin_Bag,
	ROSARY,
	NDUJA_FRITTA_TANTO,
	OROLOGION,
	VACUUM,
	FLOOR_CHICKEN,
	GILDED_CLOVER,
	LITTLE_COLVER,
	CHEST,
	GOLDEN_EGG
};
#define BLACK (RGB(0,0,0))
#define WHITE (RGB(255,255,255))
class VSObject
{
public:
	VSObject();
	VSObject(vector<char*> filename, COLORREF color = WHITE);
	virtual ~VSObject();
	void load_skin(vector<char*> filename, COLORREF color= RGB(255,255,255));
	void load_skin(char* filename, COLORREF color = RGB(255, 255, 255));
	void load_animation(vector<char*> filename, COLORREF color = RGB(255, 255, 255));
	virtual void show_skin(double factor = 1.0);
	virtual void show_animation(double factor = 1.0);
	void unshow_skin();
	void set_default_direct(int dir);
	void set_animation(int delay, bool _once, int cooldown = 0);
	void set_selector(int);
	void enable_animation();
	void set_pos(CPoint);
	void set_pos(int, int);
	void set_pos(double, double);
	void set_target_vec(CPoint);
	void set_target_vec(int, int);
	void set_speed(int);
	void set_speed(double);
	void select_show_animation(int);
	CPoint get_pos();
	virtual void update_pos(CPoint);
	virtual void update_pos();
	virtual void update_pos_by_vec(CPoint vec = {0,0});
	void set_is_mirror(bool);
	bool is_animation_done();
	int get_direct();
	int get_height();
	int get_width();
	void append_collide(VSObject&, double overlap_bound, double factor);
	void update_collide();
	void is_collide_with(VSObject* other);
	static int player_dx;
	static int player_dy; // every time player move should update these
	friend bool is_overlapped(VSObject&, VSObject&, double overlap_bound=1);
	static int distance(VSObject&, VSObject&);
	static int distance(CPoint&, CPoint&);
	friend class QuadTree; 
	int obj_type = VSOBJECT;
protected:
	game_framework::CMovingBitmap _skin;
	vector <game_framework::CMovingBitmap> _animations;
	int _selector = 0;
	CPoint _position;
	CPoint _target;
	CPoint _collision;
	CPoint _target_vec = {0,0};
	bool _is_mirror = 0;
	int _direct, _default_direct=LEFT;
	int _speed=0;
	double _fx, _fy;
};

