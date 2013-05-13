#include "polygon.h"
#include "edge.h"

enum{CLOCKWISE};
enum{SKEW_NO_CROSS, SKEW_CROSS, PARALLEL};
enum{COLLINEAR, RIGHT, LEFT, BEYOND, BEHIND, FALSE};

enum { UNKNOWN, P_IS_INSIDE, Q_IS_INSIDE};

void advance (Polygon &A, Polygon &R, int inside)
{
  A. advance (CLOCKWISE);
  if (inside && (R.point() != A.point ()))
    R.insert (A.point());
}


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

#define EPSILON2 1E-10



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
      else if (s >= 1.0-EPSILON2*lene) p = e.dest;
      else p = f.point(t);
        return SKEW_CROSS;
    } else
      return SKEW_NO_CROSS;
}



#include "edge.h"
#include "polygon.h"




Polygon *convexPolygonlntersect(Polygon &P, Polygon &Q)
{
  Polygon *R;
  Point iPnt, startPnt;
  int inflag = UNKNOWN;
  int phase = 1;
  int maxItns = 2 * (P.size() + Q.size());
                                 // начало цикла for
  for (int i = 1; (i<=maxItns) || (phase==2); i++) {
    Edge p = P.edge();
    Edge q = Q.edge();
    int pclass = p.dest.classify (q);
    int qclass = q.dest.classify (p);
    int crossType = crossingPoint (p, q, iPnt);
    if (crossType == SKEW_CROSS) {
      if (phase==1) {
        phase = 2;
        R = new Polygon;
        R->insert(iPnt);
        startPnt = iPnt;
      } else if (iPnt != R->point() ) {
        if (iPnt != startPnt)
          R->insert(iPnt);
        else
          return R;
      }
      if (pclass==RIGHT) inflag = P_IS_INSIDE;
      else if (qclass=RIGHT) inflag = Q_IS_INSIDE;
      else inflag = UNKNOWN;
    } else if ( (crossType == COLLINEAR) && (pclass != BEHIND) && (qclass != BEHIND) )
      inflag = UNKNOWN;
    bool pAIMSq = aimsAt(p, q, pclass, crossType);
    bool qAIMSp = aimsAt(q, p, qclass, crossType);
    if (pAIMSq && qAIMSp) {
      if ( (inflag == Q_IS_INSIDE) || ( (inflag == UNKNOWN)  && (pclass == LEFT)))
        advance(P, *R, FALSE);
      else
        advance (Q, *R, FALSE);
    } else if (pAIMSq) {
      advance (P, *R, inflag == P_IS_INSIDE);
    } else if (qAIMSp) {
      advance (Q, *R, inflag == Q_IS_INSIDE);
    } else {
      if ((inflag == Q_IS_INSIDE) || (inflag == UNKNOWN) && (pclass == LEFT))
        advance ( P , *R , FALSE );
      else
        advance (Q, *R, FALSE);
    }
  }
                                      // конец цикла for
  if (pointInConvexPolygon ( P.point (), Q) )
    return new Polygon (P);
  else if (pointInConvexPolygon (Q.point(), P) )
    return new Polygon(Q);
  return new Polygon;
}
