#ifndef POLYGON_H
#define POLYGON_H
#include "vertex.h"

class Polygon
{
    Vertex *_v;
    int _size;
    void resize (void);
public:
    Polygon (void);
    Polygon (Polygon&);
    Polygon (Vertex* v);
    ~Polygon (void);
    Vertex *v(void);
    int size (void);
    Point point (void);
    Edge edge (void);
    Vertex *cw(void);
    Vertex *ccw (void);
    Vertex *neighbor (int rotation);
    Vertex *advance (int rotation);
    Vertex *setV (Vertex*);
    Vertex *insert (Point&);
    Vertex *insertt (Point);
    void remove (void);
    Polygon *split (Vertex*);
};

#endif // POLYGON_H
