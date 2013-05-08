#ifndef POINT_H
#define POINT_H

class Point
{

public:
  double x;
  double y;

  Point(double _x = 0.0, double _y =0.0);

  Point operator+(Point&);
  Point operator-(Point&);
  friend Point operator* (double, Point&);

  // возвращает координату х, если в качестве индекса
  // координаты указано значение О, или координату у при индексе 1
  double operator[] (int);

  // одинаковы ли точки ?
  int operator== (Point&);
  int operator!= (Point&);

  // лексикографический порядок отношений, точка а < точки b,
  // если либо а.х < b.х, либо a.х = b.x и а.у < b.у.
  int operator< (Point&);
  int operator> (Point&);

  int classify(Point&, Point&);
    int classify(Edge&);  // ребро вместо пары точек

    // Угол точки в полярной системе координат
    // возвращает -1, если точка = (0, 0)
    double polarAngle(void);

    double length(void);

    double distance(Edge&);
  };

#endif // POINT_H
