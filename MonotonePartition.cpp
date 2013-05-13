#include <stdio.h>
#include <string.h>
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>
#include <set>
#include "MonotonePartition.h"
#include "Point.h"
#include "Polygon.h"

using namespace std;

namespace Geometry {

bool MonotonePartition::IsConvex(const Point& p1, const Point& p2, const Point& p3) {
    long area;
    area = (p3.y - p1.y)*(p2.x - p1.x) - (p3.x - p1.x)*(p2.y - p1.y);
    if(area > 0)
        return 1;
    else
        return 0;
}

int MonotonePartition::PerformMonotonePartition(list<Polygon> *inPolygons, list<Polygon> *monotonePolygons) {
    list<Polygon>::iterator iter;
    MonotoneVertex *vertices;
    long i, numVertices, vIndex, vIndex2, newNumVertices, maxNumVertices;
    long polygonStartIndex, polygonEndIndex;
    Polygon *polygon;
    MonotoneVertex *v,*v2,*vPrev,*vNext;
    ScanLineEdge newEdge;
    bool error = false;

    numVertices = 0;
    for(iter = inPolygons->begin(); iter != inPolygons->end(); iter++) {
        numVertices += iter->GetNumPoints();
    }

    maxNumVertices = numVertices*3;
    vertices = new MonotoneVertex[maxNumVertices];
    newNumVertices = numVertices;

    polygonStartIndex = 0;
    for(iter = inPolygons->begin(); iter != inPolygons->end(); iter++) {
        polygon = &(*iter);
        polygonEndIndex = polygonStartIndex + polygon->GetNumPoints() - 1;
        for(i = 0; i < polygon->GetNumPoints(); i++) {
            vertices[i + polygonStartIndex].p = polygon->GetPoint(i);
            if(i == 0)
                vertices[i + polygonStartIndex].previous = polygonEndIndex;
            else
                vertices[i + polygonStartIndex].previous = i + polygonStartIndex - 1;
            if(i == polygon->GetNumPoints() - 1)
                vertices[i + polygonStartIndex].next = polygonStartIndex;
            else
                vertices[i + polygonStartIndex].next = i + polygonStartIndex + 1;
        }
        polygonStartIndex = polygonEndIndex + 1;
    }

    long *priority = new long[numVertices];
    for(i = 0;i < numVertices; i++)
        priority[i] = i;
    std::sort(priority, &(priority[numVertices]), VertexSorter(vertices));

    char *vertexTypes = new char[maxNumVertices];
    for(i = 0; i < numVertices; i++) {
        v = &(vertices[i]);
        vPrev = &(vertices[v->previous]);
        vNext = &(vertices[v->next]);

        if(Below(vPrev->p, v->p) && Below(vNext->p, v->p))
            if(IsConvex(vNext->p, vPrev->p,v->p))
                vertexTypes[i] = VERTEXTYPE_START;
            else
                vertexTypes[i] = VERTEXTYPE_SPLIT;
        else if(Below(v->p,vPrev->p)&&Below(v->p,vNext->p))
            if(IsConvex(vNext->p,vPrev->p,v->p))
                vertexTypes[i] = VERTEXTYPE_END;
            else
                vertexTypes[i] = VERTEXTYPE_MERGE;
        else
            vertexTypes[i] = VERTEXTYPE_REGULAR;
    }

    long *helpers = new long[maxNumVertices];

    set<ScanLineEdge> edgeTree;
    set<ScanLineEdge>::iterator *edgeTreeIterators,edgeIter;
    edgeTreeIterators = new set<ScanLineEdge>::iterator[maxNumVertices];
    pair<set<ScanLineEdge>::iterator,bool> edgeTreeRet;

    for(i = 0; i < numVertices; i++) {
        vIndex = priority[i];
        v = &(vertices[vIndex]);
        vIndex2 = vIndex;
        v2 = v;

        // Комментарии взяты из de Berg Computational Geometry: Algorithms and Applications
        switch(vertexTypes[vIndex]) {
            case VERTEXTYPE_START:
                //Insert ei in T and set helper(ei) to vi
                newEdge.p1 = v->p;
                newEdge.p2 = vertices[v->next].p;
                newEdge.index = vIndex;
                edgeTreeRet = edgeTree.insert(newEdge);
                edgeTreeIterators[vIndex] = edgeTreeRet.first;
                helpers[vIndex] = vIndex;
                break;

            case VERTEXTYPE_END:
                //if helper(ei-1) is a merge vertex
                if(vertexTypes[helpers[edgeTreeIterators[v->previous]->index]] == VERTEXTYPE_MERGE) {
                    //Insert the diagonal connecting vi to helper(ei-1) in D
                    AddDiagonal(vertices, &newNumVertices, vIndex, helpers[edgeTreeIterators[v->previous]->index]);
                    vertexTypes[newNumVertices - 2] = vertexTypes[vIndex];
                    edgeTreeIterators[newNumVertices - 2] = edgeTreeIterators[vIndex];
                    helpers[newNumVertices - 2] = helpers[vIndex];
                    vertexTypes[newNumVertices - 1] = vertexTypes[helpers[edgeTreeIterators[v->previous]->index]];
                    edgeTreeIterators[newNumVertices - 1] = edgeTreeIterators[helpers[edgeTreeIterators[v->previous]->index]];
                    helpers[newNumVertices - 1] = helpers[helpers[edgeTreeIterators[v->previous]->index]];
                }
                //Delete ei-1 from T
                edgeTree.erase(edgeTreeIterators[v->previous]);
                break;

            case VERTEXTYPE_SPLIT:
                //Search in T to find the edge ej directly left of vi
                newEdge.p1 = v->p;
                newEdge.p2 = v->p;
                edgeIter = edgeTree.lower_bound(newEdge);
                if(edgeIter == edgeTree.begin()) {
                    error = true;
                    break;
                }
                edgeIter--;
                //Insert the diagonal connecting vi to helper(ej) in D
                AddDiagonal(vertices, &newNumVertices, vIndex, helpers[edgeIter->index]);
                //AddDiagonal(vertices, &newNumVertices, vIndex, edgeIter->index);
                vertexTypes[newNumVertices - 2] = vertexTypes[vIndex];
                edgeTreeIterators[newNumVertices - 2] = edgeTreeIterators[vIndex];
                helpers[newNumVertices - 2] = helpers[vIndex];
                vertexTypes[newNumVertices - 1] = vertexTypes[helpers[edgeIter->index]];
                edgeTreeIterators[newNumVertices - 1] = edgeTreeIterators[helpers[edgeIter->index]];
                helpers[newNumVertices - 1] = helpers[helpers[edgeIter->index]];
                vIndex2 = newNumVertices - 2;
                v2 = &(vertices[vIndex2]);
                //helper(ej) = vi
                helpers[edgeIter->index] = vIndex;
                //Insert ei in T and set helper(ei) to vi
                newEdge.p1 = v2->p;
                newEdge.p2 = vertices[v2->next].p;
                newEdge.index = vIndex2;
                edgeTreeRet = edgeTree.insert(newEdge);
                edgeTreeIterators[vIndex2] = edgeTreeRet.first;
                helpers[vIndex2] = vIndex2;
                break;

            case VERTEXTYPE_MERGE:
                //if helper(ei-1) is a merge vertex
                if(vertexTypes[helpers[edgeTreeIterators[v->previous]->index]]==VERTEXTYPE_MERGE) {
                    //Insert the diagonal connecting vi to helper(ei-1) in D
                    AddDiagonal(vertices, &newNumVertices, vIndex, helpers[edgeTreeIterators[v->previous]->index]);
                    vertexTypes[newNumVertices - 2] = vertexTypes[vIndex];
                    edgeTreeIterators[newNumVertices - 2] = edgeTreeIterators[vIndex];
                    helpers[newNumVertices-2] = helpers[vIndex];
                    vertexTypes[newNumVertices - 1] = vertexTypes[helpers[edgeTreeIterators[v->previous]->index]];
                    edgeTreeIterators[newNumVertices - 1] = edgeTreeIterators[helpers[edgeTreeIterators[v->previous]->index]];
                    helpers[newNumVertices - 1] = helpers[helpers[edgeTreeIterators[v->previous]->index]];
                    vIndex2 = newNumVertices - 2;
                    v2 = &(vertices[vIndex2]);
                }
                //Delete ei-1 from T
                edgeTree.erase(edgeTreeIterators[v->previous]);
                //Search in T to find the edge e j directly left of vi
                newEdge.p1 = v->p;
                newEdge.p2 = v->p;
                edgeIter = edgeTree.lower_bound(newEdge);
                if(edgeIter == edgeTree.begin()) {
                    error = true;
                    break;
                }
                edgeIter--;
                //if helper(ej) is a merge vertex
                if(vertexTypes[helpers[edgeIter->index]]==VERTEXTYPE_MERGE) {
                    //Insert the diagonal connecting vi to helper(e j) in D
                    AddDiagonal(vertices, &newNumVertices, vIndex2, helpers[edgeIter->index]);
                    vertexTypes[newNumVertices - 2] = vertexTypes[vIndex2];
                    edgeTreeIterators[newNumVertices - 2] = edgeTreeIterators[vIndex2];
                    helpers[newNumVertices - 2] = helpers[vIndex2];
                    vertexTypes[newNumVertices - 1] = vertexTypes[helpers[edgeIter->index]];
                    edgeTreeIterators[newNumVertices - 1] = edgeTreeIterators[helpers[edgeIter->index]];
                    helpers[newNumVertices - 1] = helpers[helpers[edgeIter->index]];
                }
                //helper(ej) = vi
                helpers[edgeIter->index] = vIndex2;
                break;

            case VERTEXTYPE_REGULAR:
                //if the interior of P lies to the right of vi
                if(Below(v->p,vertices[v->previous].p)) {
                    //if helper(ei-1) is a merge vertex
                    if(vertexTypes[helpers[edgeTreeIterators[v->previous]->index]] == VERTEXTYPE_MERGE) {
                        //Insert the diagonal connecting vi to helper(ei-1) in D.
                        AddDiagonal(vertices,&newNumVertices,vIndex,helpers[edgeTreeIterators[v->previous]->index]);
                        vertexTypes[newNumVertices - 2] = vertexTypes[vIndex];
                        edgeTreeIterators[newNumVertices - 2] = edgeTreeIterators[vIndex];
                        helpers[newNumVertices - 2] = helpers[vIndex];
                        vertexTypes[newNumVertices-1] = vertexTypes[helpers[edgeTreeIterators[v->previous]->index]];
                        edgeTreeIterators[newNumVertices-1] = edgeTreeIterators[helpers[edgeTreeIterators[v->previous]->index]];
                        helpers[newNumVertices-1] = helpers[helpers[edgeTreeIterators[v->previous]->index]];
                        vIndex2 = newNumVertices - 2;
                        v2 = &(vertices[vIndex2]);
                    }
                    //Delete ei-1 from T
                    edgeTree.erase(edgeTreeIterators[v->previous]);
                    //Insert ei in T and set helper(ei) to vi
                    newEdge.p1 = v2->p;
                    newEdge.p2 = vertices[v2->next].p;
                    newEdge.index = vIndex2;
                    edgeTreeRet = edgeTree.insert(newEdge);
                    edgeTreeIterators[vIndex2] = edgeTreeRet.first;
                    helpers[vIndex2] = vIndex2;
                } else {
                    //Search in T to find the edge ej directly left of vi
                    newEdge.p1 = v->p;
                    newEdge.p2 = v->p;
                    edgeIter = edgeTree.lower_bound(newEdge);
                    if(edgeIter == edgeTree.begin()) {
                        error = true;
                        break;
                    }
                    edgeIter--;
                    //if helper(ej) is a merge vertex
                    if(vertexTypes[helpers[edgeIter->index]]==VERTEXTYPE_MERGE) {
                        //Insert the diagonal connecting vi to helper(ej) in D
                        AddDiagonal(vertices, &newNumVertices, vIndex,helpers[edgeIter->index]);
                        vertexTypes[newNumVertices - 2] = vertexTypes[vIndex];
                        edgeTreeIterators[newNumVertices - 2] = edgeTreeIterators[vIndex];
                        helpers[newNumVertices - 2] = helpers[vIndex];
                        vertexTypes[newNumVertices - 1] = vertexTypes[helpers[edgeIter->index]];
                        edgeTreeIterators[newNumVertices - 1] = edgeTreeIterators[helpers[edgeIter->index]];
                        helpers[newNumVertices-1] = helpers[helpers[edgeIter->index]];
                    }
                    //helper(ej) = vi
                    helpers[edgeIter->index] = vIndex;
                }
                break;
        }

        if(error) break;
    }

    char *used = new char[newNumVertices];
    memset(used, 0, newNumVertices*sizeof(char));

    if(!error) {
        long size;
        Polygon mPolygon;
        for(i=0; i < newNumVertices; i++) {
            if(used[i])
                continue;
            v = &(vertices[i]);
            vNext = &(vertices[v->next]);
            size = 1;
            while (vNext!=v) {
                vNext = &(vertices[vNext->next]);
                size++;
            }
            mPolygon.Init(size);
            v = &(vertices[i]);
            mPolygon[0] = v->p;
            vNext = &(vertices[v->next]);
            size = 1;
            used[i] = 1;
            used[v->next] = 1;
            while (vNext != v) {
                mPolygon[size] = vNext->p;
                used[vNext->next] = 1;
                vNext = &(vertices[vNext->next]);
                size++;
            }
            monotonePolygons->push_back(mPolygon);
        }
    }

    delete [] vertices;
    delete [] priority;
    delete [] vertexTypes;
    delete [] edgeTreeIterators;
    delete [] helpers;
    delete [] used;

    if(error) {
        return 0;
    } else {
        return 1;
    }
}

//Добавление диагонали в РСДС
void MonotonePartition::AddDiagonal(MonotoneVertex *vertices, long *numVertices, long index1, long index2) {
    long newIndex1,newIndex2;

    newIndex1 = *numVertices;
    (*numVertices)++;
    newIndex2 = *numVertices;
    (*numVertices)++;

    vertices[newIndex1].p = vertices[index1].p;
    vertices[newIndex2].p = vertices[index2].p;

    vertices[newIndex2].next = vertices[index2].next;
    vertices[newIndex1].next = vertices[index1].next;

    vertices[vertices[index2].next].previous = newIndex2;
    vertices[vertices[index1].next].previous = newIndex1;

    vertices[index1].next = newIndex2;
    vertices[newIndex2].previous = index1;

    vertices[index2].next = newIndex1;
    vertices[newIndex1].previous = index2;
}

bool MonotonePartition::Below(Point &p1, Point &p2) {
    if(p1.y < p2.y)
        return true;
    else if(p1.y == p2.y && p1.x < p2.x)
        return true;
    return false;
}

bool MonotonePartition::VertexSorter::operator() (long index1, long index2) {
    if(vertices[index1].p.y > vertices[index2].p.y)
        return true;
    else if(vertices[index1].p.y == vertices[index2].p.y &&
            vertices[index1].p.x > vertices[index2].p.x)
        return true;
    return false;
}

bool MonotonePartition::ScanLineEdge::operator < (const ScanLineEdge & other) const {
        if(other.p1.y == other.p2.y) {
            if(p1.y == p2.y) {
                if(p1.x < other.p1.x) return true;
                else return false;
            }
            if(MonotonePartition::IsConvex(p1,p2,other.p1)) return true;
            else return false;
        } else if(p1.y == p2.y) {
            if(MonotonePartition::IsConvex(other.p1,other.p2,p1)) return false;
            else return true;
        } else if(p1.y < other.p1.y) {
            if(MonotonePartition::IsConvex(other.p1,other.p2,p1)) return false;
            else return true;
        } else {
            if(MonotonePartition::IsConvex(p1,p2,other.p1)) return true;
            else return false;
        }
}

int MonotonePartition::TriangulateMonotone(Polygon *polygon, list<Polygon> *triangles) {
    long i, i2, j, topIndex, bottomIndex, leftIndex, rightIndex, vIndex;
    long numPoints;
    Polygon triangle;

    numPoints = polygon->GetNumPoints();
    Point* points = polygon->GetPoints();

    if(numPoints < 3)
        return 0;
    if(numPoints == 3) {
        triangles->push_back(*polygon);
        return 1;
    }

    topIndex = 0;
    bottomIndex=0;
    for(i = 1; i < numPoints; i++) {
        if(Below(points[i], points[bottomIndex]))
            bottomIndex = i;
        if(Below(points[topIndex],points[i]))
            topIndex = i;
    }

    //Проверка того, что полигон Y-монотонный
    i = topIndex;
    while(i != bottomIndex) {
        i2 = i + 1;
        if(i2 >= numPoints)
            i2 = 0;
        if(!Below(points[i2], points[i]))
            return 0;
        i = i2;
    }
    i = bottomIndex;
    while(i != topIndex) {
        i2 = i + 1;
        if(i2 >= numPoints)
            i2 = 0;
        if(!Below(points[i], points[i2]))
            return 0;
        i = i2;
    }

    char *vertextypes = new char[numPoints];
    long *priority = new long[numPoints];

    // Сливаем левую и правую цепь
    priority[0] = topIndex;
    vertextypes[topIndex] = 0;
    leftIndex = topIndex+1;
    if(leftIndex >= numPoints)
        leftIndex = 0;
    rightIndex = topIndex-1;
    if(rightIndex < 0)
        rightIndex = numPoints-1;

    for(i=1; i < numPoints - 1; i++) {
        if(leftIndex == bottomIndex) {
            priority[i] = rightIndex;
            rightIndex--;
            if(rightIndex < 0)
                rightIndex = numPoints-1;
            vertextypes[priority[i]] = -1;
        } else if(rightIndex == bottomIndex) {
            priority[i] = leftIndex;
            leftIndex++;
            if(leftIndex >= numPoints)
                leftIndex = 0;
            vertextypes[priority[i]] = 1;
        } else {
            if(Below(points[leftIndex], points[rightIndex])) {
                priority[i] = rightIndex;
                rightIndex--;
                if(rightIndex < 0)
                    rightIndex = numPoints - 1;
                vertextypes[priority[i]] = -1;
            } else {
                priority[i] = leftIndex;
                leftIndex++;
                if(leftIndex >= numPoints)
                    leftIndex = 0;
                vertextypes[priority[i]] = 1;
            }
        }
    }
    priority[i] = bottomIndex;
    vertextypes[bottomIndex] = 0;

    long *stack = new long[numPoints];
    long stackptr = 0;

    stack[0] = priority[0];
    stack[1] = priority[1];
    stackptr = 2;

    for(i=2; i < numPoints - 1; i++) {
        vIndex = priority[i];
        if(vertextypes[vIndex] != vertextypes[stack[stackptr - 1]]) {
            for(j = 0; j < stackptr - 1; j++) {
                if(vertextypes[vIndex] == 1) {
                    triangle.Triangle(points[stack[j + 1]], points[stack[j]], points[vIndex]);
                } else {
                    triangle.Triangle(points[stack[j]], points[stack[j + 1]], points[vIndex]);
                }
                triangles->push_back(triangle);
            }
            stack[0] = priority[i - 1];
            stack[1] = priority[i];
            stackptr = 2;
        } else {
            stackptr--;
            while(stackptr > 0) {
                if(vertextypes[vIndex] == 1) {
                    if(IsConvex(points[vIndex], points[stack[stackptr - 1]], points[stack[stackptr]])) {
                        triangle.Triangle(points[vIndex], points[stack[stackptr - 1]], points[stack[stackptr]]);
                        triangles->push_back(triangle);
                        stackptr--;
                    } else {
                        break;
                    }
                } else {
                    if(IsConvex(points[vIndex], points[stack[stackptr]] ,points[stack[stackptr-1]])) {
                        triangle.Triangle(points[vIndex], points[stack[stackptr]], points[stack[stackptr-1]]);
                        triangles->push_back(triangle);
                        stackptr--;
                    } else {
                        break;
                    }
                }
            }
            stackptr++;
            stack[stackptr] = vIndex;
            stackptr++;
        }
    }
    vIndex = priority[i];
    for(j=0; j < stackptr - 1; j++) {
        if(vertextypes[stack[j + 1]] == 1) {
            triangle.Triangle(points[stack[j]], points[stack[j+1]], points[vIndex]);
        } else {
            triangle.Triangle(points[stack[j+1]], points[stack[j]], points[vIndex]);
        }
        triangles->push_back(triangle);
    }

    delete [] priority;
    delete [] vertextypes;
    delete [] stack;

    return 1;
}

int MonotonePartition::Triangulate(std::list<Polygon> *inPolygons, std::list<Polygon> *triangles) {
    list<Polygon> monotonePolygons;
    list<Polygon>::iterator iter;

    if(!PerformMonotonePartition(inPolygons,&monotonePolygons))
        return 0;
    for(iter = monotonePolygons.begin(); iter != monotonePolygons.end();iter++)
        if(!TriangulateMonotone(&(*iter),triangles))
            return 0;
    return 1;
}

int MonotonePartition::Triangulate(list<Polygon> *inPolygons, list<Polygon> *monotonePolygons, list<Polygon> *triangles) {
    list<Polygon>::iterator iter;

    if(!PerformMonotonePartition(inPolygons,monotonePolygons))
        return 0;
    for(iter = monotonePolygons->begin(); iter != monotonePolygons->end();iter++)
        if(!TriangulateMonotone(&(*iter),triangles))
            return 0;
    return 1;
}

bool MonotonePartition::CheckCorrectness(std::list<Polygon> *polygons, std::list<Polygon> *triangles) {
    long area1 = polygons->front().TwiceArea();

    long area2 = 0;
    list<Geometry::Polygon>::iterator hole = polygons->begin();
    hole++;
    for (;hole != polygons->end(); hole++)
        area2 += hole->TwiceArea();
    for (list<Geometry::Polygon>::iterator triangle = triangles->begin();
         triangle != triangles->end();
         triangle++) {
        area2 += triangle->TwiceArea();
    }

    return area1 == area2;
}

}
