#include "Polygon.h"
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <list>
using namespace std;

namespace Geometry {

Polygon::Polygon() {
    numPoints = 0;
    points = NULL;
}

Polygon::Polygon(vector<Point> points) {
    this->points = NULL;
    Init(points.size());
    for (int i = 0; i < numPoints; i++)
        this->points[i] = points[i];
}

Polygon::~Polygon() {
    if (points)
        delete [] points;
}

void Polygon::Clear() {
    if(points) delete [] points;
    numPoints = 0;
    points = NULL;
}

void Polygon::Init(long numPoints) {
    Clear();
    this->numPoints = numPoints;
    points = new Point[numPoints];
}

void Polygon::Triangle(Point &p1, Point &p2, Point &p3) {
    Init(3);
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
}

Polygon::Polygon(const Polygon &src) {
    numPoints = src.numPoints;
    points = new Point[numPoints];
    memcpy(points, src.points, numPoints*sizeof(Point));
}

Polygon& Polygon::operator=(Polygon &src) {
    Clear();
    numPoints = src.numPoints;
    points = new Point[numPoints];
    memcpy(points, src.points, numPoints*sizeof(Point));
    return *this;
}

void Polygon::SetOrientation(int orientation) {
    int Polygonorientation = GetOrientation();
    if(Polygonorientation != 0 &&
       Polygonorientation != orientation) {
        InvertOrientation();
    }
}

int Polygon::GetOrientation() {
    long area = SignTwiceArea();
    if (area < 0)
        return CCW;
    if (area > 0)
        return CW;
    return 0;
}

void Polygon::InvertOrientation() {
    long i;
    Point *invpoints;

    invpoints = new Point[numPoints];
    for(i=0;i<numPoints;i++) {
        invpoints[i] = points[numPoints-i-1];
    }

    delete [] points;
    points = invpoints;
}

long Polygon::TwiceArea() {
    return abs(SignTwiceArea());
}

// возвращает удвоенную площадь полигона со знаком
long Polygon::SignTwiceArea() {
    long area = 0;
    for (int i = 0; i < numPoints; i++) {
        Point a(points[i]);
        Point b;
        if (i != numPoints - 1)
            b = points[i + 1];
        else
            b = points[0];

        area += (a.x + b.x)*(a.y - b.y);
    }
    return area;
}

// луч пускается горизонтально вправо
bool Polygon::Contain(Point p) {
    bool inside = false;
    for (int i = 0; i < numPoints; i++) {
        Point a(points[i]);
        Point b;
        if (i != numPoints - 1)
            b = points[i + 1];
        else
            b = points[0];

        int location = GetLocation(a, b, p);
        if (location == ZERO &&
             ((a.x <= p.x && p.x <= b.x) || (b.x <= p.x && p.x <= a.x)) &&
             ((a.y <= p.y && p.y <= b.y) || (b.y <= p.y && p.y <= a.y))
           )
            return true;
        else if ((location == LEFT && a.y < p.y && p.y <= b.y) ||
                 (location == RIGHT && b.y < p.y && p.y <= a.y))
                inside = !inside;
    }
    return inside;
}

int Polygon::GetLocation(Point &p1, Point &p2, Point &p3) {
    long area = (p3.x - p1.x)*(p2.y - p1.y) - (p2.x - p1.x)*(p3.y - p1.y);

    if (area > 0)
        return LEFT;
    else if (area < 0)
        return RIGHT;
    else
        return ZERO;
}

vector<Point> Polygon::GetVectorPoints() {
    vector<Point> points;
    for (int i = 0; i < numPoints; i++)
        points.push_back(this->points[i]);
    return points;
}

void Polygon::SavePolygonsToFile(std::list<Polygon>* polygons, const char* fileName) {
    ofstream ofile;
    ofile.open(fileName);
    ofile << polygons->size() << endl;
    for (list<Geometry::Polygon>::iterator polygon = polygons->begin(); polygon != polygons->end(); polygon++) {
        ofile << polygon->GetNumPoints() << endl;
        for (int i = 0; i < polygon->GetNumPoints(); i++)
            ofile << polygon->GetPoint(i).x << " " << polygon->GetPoint(i).y << endl;
    }
    ofile.close();
}

void Polygon::ReadPolygonsFromFile(list<Polygon> *polygons, const char *fileName) {
    int numPolygons;
    int numPoints;
    ifstream ifile;

    ifile.open(fileName);
    ifile >> numPolygons;
    for (int i = 0; i < numPolygons; i++) {
        ifile >> numPoints;
        Polygon polygon;
        polygon.Init(numPoints);
        for (int i = 0; i < numPoints; i++) {
            ifile >> polygon.GetPoint(i).x;
            ifile >> polygon.GetPoint(i).y;
        }
        if (polygons->size() == 0 && polygon.GetOrientation() == CW)
            polygon.InvertOrientation();
        else if (polygons->size() > 0 && polygon.GetOrientation() == CCW)
            polygon.InvertOrientation();
        polygons->push_back(polygon);
    }
}

}
