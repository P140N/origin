#ifndef POINT_H
#define POINT_H
#include "edge.h"



class Point
{
public:
    double x;
    double y;
    Point(double _x = 0.0, double _y = 0.0);
    Point operator+ (const Point&);
    Point operator- (const Point&);
    friend Point operator* (double, Point&);
    double operator[](int);
    int operator== (Point&);
    int operator!= (Point&);
    int operator< (Point&);
    int operator> (Point&);
    int classify (Point&, Point&);
    int classify (Edge&);
    double polarAngle(void);
    double length (void);
    double distance(Edge&);
};


Point operator* (double s, const Point &p)
{
    return Point(s * p.x, s * p.y);
}



#endif // POINT_H
