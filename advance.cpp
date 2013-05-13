#include "polygon.h"

enum{CLOCKWISE};


void advance (Polygon &A, Polygon &R, int inside)
{
  A. advance (CLOCKWISE);
  if (inside && (R.point() != A.point ()))
    R.insert (A.point());
}
