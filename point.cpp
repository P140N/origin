#include "point.h"

Point::Point(double _x, double _y):
    x(_x), y(_y)
{
}

Point Point::operator +(Point &p)
{
    return (x + p.x, y + p.y);
}

Point Point::operator -(Point &p)
{
    return (x - p.x, y - p.y);
}

Point Point::operator *(double, Point &p)
{
    return point (s * p.x, s * p.y);
}

double Point::operator [](int i)
{
    return (i == 0) ? x : y;
}

int Point::operator ==(Point &p)
{
    return (x == p.x) && (y == p.y);
}

int Point::operator !=(Point &p)
{
    return !(*this == p);
}

int Point::operator <(Point &p)
{
    return ((x < p.x) || ((x == p.x) && (y<p.y)));
}

int Point::operator >(Point &)
{
    return ((x>p.x) || ((x == p.x) && (y > p.y)));
}

int Point::classify(Point &p0, Point &pl)
{
    Point p2 = *this;
      Point a = p1 - pO;
      Point b = p2 - pO;
      double sa = a. x * b.y - b.x * a.y;
      if (sa > 0.0)
        return LEFT;
      if (sa < 0.0)
        return RIGHT;
      if ((a.x * b.x < 0.0) || (a.y * b.y < 0.0))
        return BEHIND;
      if (a.length() < b.length())
        return BEYOND;
      if (pO == p2)
        return ORIGIN;
      if (p1 == p2)
        return DESTINATION;
      return BETWEEN;
}

int Point::classify(Edge &)
{
    return classify(e.org, e.dest);
}

double Point::polarAngle(void)
{
    if ((x == 0.0 ) && (y == 0.0))
        return -1.0;
      if (x == 0.0)
        return ((y > 0.0) ? 90 : 270);
      double theta = atan(y/x);                    // в радианах
      theta *= 360 / (2 * 3.1415926);            // перевод в градусы
      if (x > 0.0)                                 // 1 и 4 квадранты
        return ((y >= 0.0) ? theta : 360 + theta);
      else                                         // 2 и З квадранты
          return (180 + theta);
}

double Point::length(void)
{
    return sqrt(x*x + y*y);
}


