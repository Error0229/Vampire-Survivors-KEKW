#include "stdafx.h"
#include "../config.h"
#include "../../Library/gameutil.h"
#include "VSUtil.h"
#include "VSObject.h"
#include "QuadTree.h"

QuadTree::QuadTree() = default;
QuadTree::~QuadTree() = default;
QuadTree::QuadTree(int x, int y, int w, int h, int max_objects, int max_levels, int level) : _x(x) , _y(y), w(w), h(h), max_objects(max_objects), max_levels(max_levels), level(level), objects({})
{
    for (int i = 0; i < 4; i++) {
        this->children[i] = nullptr;
    }
}
void QuadTree::set_range(int x, int y, int w, int h) {
    _x = x;
    _y = y;
    this->w = w;
    this->h = h;
}
void QuadTree::insert(VSObject* obj)
{
    if (obj->get_pos().x + obj->get_width() < this->_x || obj->get_pos().x > this->_x + this->w || obj->get_pos().y + obj->get_height() < this->_y || obj->get_pos().y > this->_y + this->h)
        return;
    auto objects_size = static_cast<int> (this->objects.size());
    if ( objects_size < this->max_objects || this->level == this->max_levels) {
        this->objects.push_back(obj);
        return;
    }
    else {
        if (this->children[0] == nullptr)
            this->split();
        int index = this->get_quadrant(obj);
        if (index != -1) {
            this->children[index]->insert(obj);
            return;
        }
        else {
            this->objects.push_back(obj);
            return;
        }
    }
}
int QuadTree::get_quadrant(VSObject* obj)
{
    int index = -1;
    int vertical_midpoint = this->_x + (this->w >> 1);
    int horizontal_midpoint = this->_y + (this->h >> 1);
    bool top_quadrant = (obj->_position.y + obj->get_height() < horizontal_midpoint && obj->_position.y > this->_y);
    bool bottom_quadrant = (obj->_position.y > horizontal_midpoint && obj->_position.y + obj->get_height() < this->_y + this->h);
    if (obj->get_pos().x + obj->get_width() < vertical_midpoint && obj->get_pos().x > this->_x) {
        if (top_quadrant) {
            index = 0;
        }
        else if (bottom_quadrant) {
            index = 2;
        }
    }
    else if (obj->get_pos().x > vertical_midpoint && obj->get_pos().x + obj->get_width() < this->_x + this->w) {
        if (top_quadrant) {
            index = 1;
        }
        else if (bottom_quadrant) {
            index = 3;
        }
    }
    return index;
}
void QuadTree::split()
{
    int subWidth = (this->w >> 1);
    int subHeight = (this->h >> 1);
    this->children[0] = new QuadTree(_x, _y, subWidth, subHeight, this->max_objects, this->max_levels, this->level + 1);
    this->children[1] = new QuadTree(_x + subWidth, _y, subWidth, subHeight, this->max_objects, this->max_levels, this->level + 1);
    this->children[2] = new QuadTree(_x, _y + subHeight, subWidth, subHeight, this->max_objects, this->max_levels, this->level + 1);
    this->children[3] = new QuadTree(_x + subWidth, _y + subHeight, subWidth, subHeight, this->max_objects, this->max_levels, this->level + 1);
}
void QuadTree::query(vector<VSObject*>& result, VSObject* q)
{
    int index = this->get_quadrant(q);
    if (index != -1 && this->children[0] != nullptr) {
        this->children[index]->query(result, q);
    }
    else {
        if (this->children[0] != nullptr) {

            for (int i = 0; i < 4; i++) {
                if (is_overlapped_pure(this->children[i]->_x, this->children[i]->_y, this->children[i]->w, this->children[i]->h, q->_position.x, q->_position.y, q->get_width(), q->get_height())) {
                    this->children[i]->query(result, q);
                }
            }
        }
    }
    for (VSObject *obj : objects) {
        if (is_overlapped(*(obj), *q)) {
            result.push_back(obj);
        }
    }
}
void QuadTree::query_by_type(vector<VSObject*>& result, VSObject* q, int type)
{
    int index = this->get_quadrant(q);
    if (index != -1 && this->children[0] != nullptr) {
        this->children[index]->query_by_type(result, q, type);
    }
    else {
        if (this->children[0] != nullptr) {
            for (int i = 0; i < 4; i++) {
                if (is_overlapped_pure(this->children[i]->_x, this->children[i]->_y, this->children[i]->w, this->children[i]->h, q->_position.x, q->_position.y, q->get_width(), q->get_height())) {
                    this->children[i]->query_by_type(result, q, type);
                }
            }
        }
    }
    for (VSObject* obj : objects) {
        if (obj->obj_type != type)
            continue;
        if (is_overlapped(obj, q)) {
            result.push_back(obj);
        }
    }
}
void QuadTree::query_nearest_enemy_pos(CPoint& result, VSObject* q, int& min_distance) {
    if (this->children[0] != nullptr) {
        for (int i = 0; i < 4; i++) {
            this->children[i]->query_nearest_enemy_pos(result, q, min_distance);
        }
    }
    for (VSObject* obj : objects) {
        if (obj->obj_type != ENEMY)
            continue;
        int dis = VSObject::distance(obj->get_pos(), q->get_pos());
        if (dis < min_distance) {
            min_distance = dis;
            result = obj->get_pos();
        }
    }
}
void QuadTree::clear()
{
    this->objects.clear();
    for (int i = 0; i < 4; i++) {
        if (this->children[i] != nullptr) {
            this->children[i]->clear();
            delete this->children[i];
            this->children[i] = nullptr;
        }
    }
}
QuadTree QuadTree::VSPlain = QuadTree(-VSObject::player_dx, -VSObject::player_dy, (OPEN_AS_FULLSCREEN ? RESOLUTION_X : SIZE_X), (OPEN_AS_FULLSCREEN ? RESOLUTION_Y : SIZE_Y), 6, 10, 0);