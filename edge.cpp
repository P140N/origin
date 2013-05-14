#include "edge.h"
#include "point.h"
#include "float.h"

Edge::Edge(const Point &_org,const Point &_dest) :
    org(_org), dest(_dest)
{
}


Edge::Edge(void) :
    org (Point(0,0)), dest(Point(1,0))
{
}

Edge &Edge::rot(void)
{
    Point m = 0.5 * (org + dest);
    Point v = dest - org;
    Point n(v.y, -v.x);
    org = m.operator-(0.5 * n);
    dest = m + 0.5 * n;
    return *this;
}

Edge &Edge::flip(void)
{
    return rot().rot();
}

Point Edge::point(double t)
{
    return Point(org + t*(dest-org));
}

double dotProduct(const Point &p,const Point &q)
{
  return (p.x * q.x + p.y * q.y);
}


enum{CLOCKWISE,SKEW_NO_CROSS, SKEW_CROSS, PARALLEL,COLLINEAR, RIGHT, LEFT, BEYOND, BEHIND, FALSE, TRUE, ORIGIN, BETWEEN, DESTINATION,UNKNOWN, P_IS_INSIDE, Q_IS_INSIDE,SKEW};

int Edge::intersect(Edge e, double &t)
{
  Point a = org;
  Point b = dest;
  Point c = e.org;
  Point d = e.dest;
  Point n = Point((d-c).y, (c-d).x);
  double denom = dotProduct(n, b-a);
  if (denom ==0.0) {
    int aclass = org.classify(e);
    if ((aclass==LEFT) || (aclass==RIGHT))
      return PARALLEL;
    else return COLLINEAR;
  }
  double num = dotProduct(n, a-c);
  t = -num / denom;
  return SKEW;
}

int Edge::cross(Edge &e, double &t)
{
  double s;
  int crossType = e.intersect(*this, s);
  if ((crossType==COLLINEAR) || (crossType==PARALLEL))
    return crossType;
  if ((s < 0.0) || (s > 1.0))
    return SKEW_NO_CROSS;
  intersect(e, t);
  if ((0.0 <= t) && (t <= 1.0))
    return SKEW_CROSS;
  else
    return SKEW_NO_CROSS;
}



bool Edge::isVertical(void)
{
  return (org.x == dest.x);
}

double Edge::slope(void)
{
  if (org.x != dest.x)
    return (dest.y - org.y) / (dest.x - org.x);
  return DBL_MAX;
}

double Edge::y(double x)
{
    return slope() * (x - org.x +org.y);
}
