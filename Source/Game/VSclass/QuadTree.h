#pragma once
class QuadTree {
public:
    QuadTree();
    ~QuadTree();
    QuadTree(int x, int y, int w, int h, int max_objects, int max_levels, int level);
    vector<VSObject*> objects;
    QuadTree* children[4];
    int w, h;
    int _x, _y;
    int max_objects;
    int max_levels;
    int level;
    void split();
    void set_range(int, int, int, int);
    int get_quadrant(VSObject* object);
    void insert(VSObject* object);
    void clear();
    void query(vector<VSObject*>& return_objects, VSObject* object);
    void query_nearest_point(CPoint& result, VSObject* object, int min_distance);
};