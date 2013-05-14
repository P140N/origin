#include "polygon.h"
#include <cstddef>
#include "edge.h"

Polygon::Polygon(void) :
  _v(NULL) , _size(0)
{
}

enum {CLOCKWISE, COUNTER_CLOCKWISE };

Polygon::Polygon(Polygon &p)
{
    _size = p._size;
    if (_size == 0)
    {
        _v = NULL;
    }else{
        _v = new Vertex (p.point());
        for (int i = 1; i < _size; i++)
        {
            p.advance(CLOCKWISE);
            _v = _v ->insert(new Vertex(p.point()));
        }
        p.advance(CLOCKWISE);
        _v = _v->cw();

    }
}


Polygon::Polygon (Vertex *v):
  _v(v)
{
  resize();
}

void Polygon::resize (void)
{
  if (_v == NULL)
    _size = 0;
  else {
    Vertex *v = _v->cw();
    for (_size = 1; v != _v; ++_size, v = v->cw());
  }
}

Polygon::~Polygon (void)
{
  if (_v) {
    Vertex *w = _v->cw();
    while (_v != w) {
      delete w->remove();
      w = _v->cw();
    }
    delete _v;
  }
}

Vertex *Polygon::v(void)
{
  return _v;
}

int Polygon::size(void)
{
  return _size;
}


Point Polygon::point (void)
{
  return _v->point();
}

Edge Polygon::edge(void)
{
  return Edge (point(), _v->cw()->point());
}

Vertex *Polygon::cw(void)
{
  return _v->cw();
}

Vertex *Polygon::ccw(void)
{
  return _v->ccw();
}

Vertex *Polygon::neighbor(int rotation)
{
  return _v->neighbor(rotation);
}

Vertex *Polygon::advance(int rotation)
{
  return _v = _v->neighbor(rotation);
}


Vertex *Polygon::setV(Vertex *v)
{
  return _v = v;
}


Vertex *Polygon::insert(Point p)
{
  if (_size++ == 0)
    _v = new Vertex(p);
  else
    _v = _v->insert(new Vertex(p));
  return _v;
}



void Polygon::remove(void)
{
  Vertex *v = _v;
  _v = (--_size == 0) ? NULL : _v->ccw();
  delete v->remove();
}


Polygon *Polygon::split (Vertex *b)
{
  Vertex *bp = _v->split(b);
  resize();
  return new Polygon(bp);
}
