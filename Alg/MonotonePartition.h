#ifndef MONOTONEPARTITION_H
#define MONOTONEPARTITION_H

#include <list>
#include "Polygon.h"

namespace Geometry {

class MonotonePartition {
private:
    enum {VERTEXTYPE_REGULAR,
          VERTEXTYPE_START,
          VERTEXTYPE_END,
          VERTEXTYPE_SPLIT,
          VERTEXTYPE_MERGE};

    // Вершина, входящая в РСДС
    struct MonotoneVertex {
        Point p;
        long previous;
        long next;
    };

    class VertexSorter{
        MonotoneVertex *vertices;
    public:
        VertexSorter(MonotoneVertex *v) : vertices(v) {}
        bool operator() (long index1, long index2);
    };

    // ребро, пересекающее заметающую прямую
    struct ScanLineEdge {
        long index;
        Point p1;
        Point p2;

        bool operator< (const ScanLineEdge & other) const;
    };

    static bool IsConvex(const Point& p1, const Point& p2, const Point& p3);

    static bool Below(Point &p1, Point &p2);
    static void AddDiagonal(MonotoneVertex *vertices, long *numVertices, long index1, long index2);

    //Разбиение на Y-монотонные полигоны
    static int PerformMonotonePartition(std::list<Polygon> *polygons, std::list<Polygon> *monotonePolygons);
    //Триангуляция Y-монотонного полигона
    static int TriangulateMonotone(Polygon *polygon, std::list<Polygon> *triangles);

public:
    static int Triangulate(std::list<Polygon> *polygons, std::list<Polygon> *triangles);
    static int Triangulate(std::list<Polygon> *polygons, std::list<Polygon> *monotonePolygons, std::list<Polygon> *triangles);
    static bool CheckCorrectness(std::list<Polygon> *polygons, std::list<Polygon> *triangles);
};

}

#endif // MONOTONEPARTITION_H
