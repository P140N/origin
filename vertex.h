#ifndef VERTEX_H
#define VERTEX_H
#include "point.h"
#include "node.h"

class Vertex: public Node, public Point
{
public:
    Vertex(double x, double y);
    Vertex(Point&);
    Vertex *cw(void);
    Vertex *ccw(void);
    Vertex *neighbor(int rotation);
    Point point(void);
    Vertex *insert(Vertex*);
    Vertex *remove(void);
    void splice(Vertex*);
    Vertex *split(Vertex*);
    //friend class Polygon;
};

#endif // VERTEX_H
