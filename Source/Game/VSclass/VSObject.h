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
class VSObject
{
public:
	VSObject();
	VSObject(vector<char*> filename);
	virtual ~VSObject();
	void load_skin(vector<char*> filename, COLORREF color= RGB(255,255,255));
	void load_skin(char* filename, COLORREF color = RGB(255, 255, 255));
	virtual void show_skin(double factor = 1.0);
	void unshow_skin();
	void set_default_direct(int dir);
	void set_animation(int delay, bool _once);
	void enable_animation();
	void set_pos(CPoint);
	void set_pos(int, int);
	void set_speed(int);
	void set_is_mirror(bool);
	bool is_animation_done();
	CPoint get_pos();
	virtual void update_pos(CPoint);
	virtual void update_pos();
	int get_height();
	int get_width();

	static int player_dx;
	static int player_dy; // every time player move should update these
	friend bool is_overlapped(VSObject&, VSObject&);
	friend int distance(VSObject&, VSObject&);
	friend int distance(CPoint&, CPoint&);
protected:
	game_framework::CMovingBitmap _skin;
	CPoint _position;
	CPoint _target;
	bool _is_mirror = 0;
	int _direct, _default_direct=LEFT;
	int _speed;
};

