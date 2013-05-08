#include "vertex.h"

Vertex::Vertex(double x, double y) : Point(x, y)
{

}

Vertex::Vertex(Point &p): Point(p)
{
}

Vertex *Vertex::cw(void)
{
    return (Vertex*)_next;
}

Vertex *Vertex::ccw(void)
{
    return (Vertex*)_prev;
}

Vertex *Vertex::neighbor(int rotation)
{
    return ((rotation == CLOCKWISE) ? cw() : ccw());
}

Point Vertex::point(void)
{
    return  *((Point*)this);
}

Vertex *Vertex::insert(Vertex *v)
{
    return (Vertex *) (Node::insert (v));
}

Vertex *Vertex::remove(void)
{
    return (Vertex *) (Node::remove ());
}

void Vertex::splice(Vertex *b)
{
    Node::splice (b);
}

Vertex *Vertex::split(Vertex *b)
{
    Vertex *bp = b->ccw()->insert(new Vertex(b->point()));
      insert(new Vertex(point()));
                           // занесение ар после текущей вершины
      splice(bр);
      return bp;
}


