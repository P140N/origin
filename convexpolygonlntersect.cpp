#include "convexpolygonlntersect.h"

convexPolygonlntersect::convexPolygonlntersect()
{

}

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
    if (crossType = SKEW_CROSS) {
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
    }else if ( (crossType - COLLINEAR)&& (pclass != BEHIND) && (qclass != BEHIND))
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
      if ((inflag == Q_IS_INSIDE) ((inflag == UNKNOWN) && (pclass == LEFT)))
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
