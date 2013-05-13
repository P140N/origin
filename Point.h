#ifndef POINT_H
#define POINT_H

namespace Geometry {

struct Point {
    int x;
    int y;

    Point() {}

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    Point operator + (Point& p) {
        Point r;
        r.x = x + p.x;
        r.y = y + p.y;
        return r;
    }

    Point operator - (Point& p) {
        Point r;
        r.x = x - p.x;
        r.y = y - p.y;
        return r;
    }

    Point operator * (int i) {
        Point r;
        r.x = x*i;
        r.y = y*i;
        return r;
    }

    Point operator / (int i) {
        Point r;
        r.x = x/i;
        r.y = y/i;
        return r;
    }

    bool operator == (Point& p) {
        if (x == p.x && y == p.y)
            return true;
        else
            return false;
    }

    bool operator != (Point& p) {
        if(x == p.x && y == p.y)
            return false;
        else
            return true;
    }
};

}

#endif // POINT_H
