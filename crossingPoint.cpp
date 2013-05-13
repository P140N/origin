#include "edge.h"

#define EPSILON2 1E-10

enum{SKEW_NO_CROSS, SKEW_CROSS, COLLINEAR, PARALLEL};

int crossingPoint(Edge &e, Edge &f, Point &p)
{
  double s, t;
  int classe = e.intersect(f,s);
  if ((classe == COLLINEAR) || (classe == PARALLEL))
      return classe;
  double lene = (e.dest-e.org).length();
  if ((s < -EPSILON2*lene) || (s > 1.0+EPSILON2*lene))
      return SKEW_NO_CROSS;
  f.intersect(e, t);
  double lenf = (f.org-f.dest).length();
  if ((-EPSILON2+lenf <= t) && (t <= 1.0+EPSILON2*lenf)) {
      if (t <= EPSILON2*lenf) p = f.org;
      else if (t >= 1.0-EPSILON2*lenf) p = f.dest;
      else if (s <= EPSILON2*lene) p = e.org;
      else if (a >= 1.0-EPSILON2*lene) p = e.dest;
      else p = f.point(t);
        return SKEW_CROSS;
    } else
      return SKEW_NO_CROSS;
}
