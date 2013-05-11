#include "point.h"
#include "math.h"

Point::Point(double _x, double _y):
    x(_x), y(_y)
{
}

Point Point::operator+ (Point &p)
{
    return Point(x + p.x, y + p.y);
}

Point Point::operator- (Point &p)
{
    return Point(x - p.x, y - p.y);
}

Point Point::operator* (double s)
{
    return Point(s * this ->x, y * this->y);
}

/*
Point Point::operator [] (int i)
{
    return (i == 0) ? x : y;
}
*/

int Point::operator== (Point &p)
{
  return (x == p.x) && (y == p.y);
}

int Point::operator!= (Point &p)
{
  return !(*this == p);
}

int Point::operator< (Point &p)
{
  return ((x < p.x) || ((x == p.x) && (y<p.y)));
}

int Point::operator> (Point &p)
{
  return ((x>p.x) || ((x == p.x) && (y > p.y)));
}


enum {LEFT,  RIGHT,  BEYOND,  BEHIND, BETWEEN, ORIGIN, DESTINATION};

int Point::classify(Point &p0, Point &p1)
{
  Point p2 = *this;
  Point a = p1 - p0;
  Point b = p2 - p0;
  double sa = a. x * b.y - b.x * a.y;
  if (sa > 0.0)
    return LEFT;
  if (sa < 0.0)
    return RIGHT;
  if ((a.x * b.x < 0.0) || (a.y * b.y < 0.0))
    return BEHIND;
  if (a.length() < b.length())
    return BEYOND;
  if (p0 == p2)
    return ORIGIN;
  if (p1 == p2)
    return DESTINATION;
  return BETWEEN;
}

/*
int Point::classify(Edge &e)
{
  return classify(e.org, e.dest);
}

*/

double Point::polarAngle(void)
{
  if ((x == 0.0) && (y == 0.0))
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

Point Point::length(void)
{
    return sqrt(x*x + y*y);
}