#include "point.h"
#include "math.h"
#include "edge.h"

Point::Point(double _x, double _y):
    x(_x), y(_y)
{
}

Point Point::operator+ (const Point &p)
{
    return Point(x + p.x, y + p.y);
}

Point Point::operator- (const Point &p)
{
    return Point(x - p.x, y - p.y);
}

Point operator* (double s,  Point p)
{
    return Point(s * p.x, s * p.y);
}

double Point::operator [] (int i)
{
    return (i == 0) ? x : y;
}


int Point::operator== (Point p)
{
  return (x == p.x) && (y == p.y);
}

int Point::operator!= (Point p)
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


int Point::classify(Edge e)
{
  return classify(e.org, e.dest);
}



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

double Point::length(void)
{
    return sqrt(x*x + y*y);
}

double Point::distance(Edge &e)
{
  Edge ab = e;
  ab.flip().rot();             // поворот ab на 90 градусов
                               // против часовой стрелки
  Point n(ab.dest -  ab.org);
                               // n = вектор, перпендикулярный ребру е
  n = (1.0 / n.length()) * n;
                               // нормализация вектора n
  Edge f(*this, *this + n);
                              // ребро f = n позиционируется
                              // на текущей точке
  double t;                   // t = расстоянию со знаком
  f.intersect(e, t);          // вдоль вектора f до точки,
                              // в которой ребро f пересекает ребро е
  return t;
}
