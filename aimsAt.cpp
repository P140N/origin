#include "edge.h"


enum{COLLINEAR, RIGHT, LEFT, BEYOND};

bool aimsAt(Edge &a, Edge &b, int aclass, int crossType)
{
    Point va = a.dest - a.org;
    Point vb = b.dest - b.org;

    if (crossType != COLLINEAR) {
        if  ( (va.x * vb.y) >= (vb.x * va.y) )
          return (aclass != RIGHT);
        else
          return (aclass != LEFT);
      } else {
        return (aclass != BEYOND);
      }
}
