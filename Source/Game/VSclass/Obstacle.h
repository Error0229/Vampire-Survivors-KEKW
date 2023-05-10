#pragma once
class Obstacle : public VSObject
{
public:
    Obstacle();
    Obstacle(CPoint pos, int hieght, int width);
    ~Obstacle();
    int get_height() override;
    int get_width() override;
    void set_height(int height);
    void set_width(int width);
    void set_base_pos(CPoint pos);
    CPoint get_base_pos();
private:
    int _height;
    int _width;
    CPoint _base_pos;
};

