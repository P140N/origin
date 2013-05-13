#ifndef MYPOLY_H
#define MYPOLY_H

#include <vector>
#include <list>
#include "Point.h"

namespace Geometry {

class Polygon {
private:

    Point *points;
    long numPoints;

    enum {LEFT, ZERO, RIGHT};
    int GetLocation(Point& p1, Point& p2, Point& p3);

public:
    enum {CW, CCW};

    Polygon();
    Polygon(std::vector<Point> points);
    ~Polygon();

    Polygon(const Polygon &src);
    Polygon& operator=(Polygon &src);

    long GetNumPoints() {
        return numPoints;
    }

    Point& GetPoint(long i) {
        return points[i];
    }

    Point* GetPoints() {
        return points;
    }

    Point& operator[] (int i) {
        return points[i];
    }

    void Clear();

    void Init(long numPoints);

    void Triangle(Point &p1, Point &p2, Point &p3);

    void SetOrientation(int orientation);

    int GetOrientation();

    void InvertOrientation();

    long TwiceArea();

    long SignTwiceArea();

    bool Contain(Point p);

    std::vector<Point> GetVectorPoints();

    static void SavePolygonsToFile(std::list<Polygon>* polygons, const char* fileName);

    static void ReadPolygonsFromFile(std::list<Polygon>* polygons, const char* fileName);
};

}

#endif
