#ifndef EDGE_H
#define EDGE_H
#include "point.h"

class Edge
{
public:
    Point org;
    Point dest;
    Edge(Point &_org, Point &_dest);
    Edge(void);
    Edge &rot(void);
    Edge &flip(void);
    Point point(double);
    int intersect(Edge&, double&);
    bool cross(Edge&, double&);
    bool isVertical(void);
    double slope(void);
    double y(double);

};

#endif // EDGE_H
