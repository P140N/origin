#include <QtGui/QApplication>
#include <QtOpenGL/QGLWidget>
#include <QDesktopWidget>
#include "GLWidget.h"
#include "Polygon.h"
#include "Point.h"
#include "MonotonePartition.h"
#include <list>
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

int main(int argc, char *argv[]){
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            list<Geometry::Polygon> polygons;
            list<Geometry::Polygon> triangles;

            Geometry::Polygon::ReadPolygonsFromFile(&polygons, argv[i]);
            Geometry::MonotonePartition::Triangulate(&polygons, &triangles);
            bool correctness = Geometry::MonotonePartition::CheckCorrectness(&polygons, &triangles);
            if (correctness == true)
                cout << "Test " << '"' << argv[i] << '"' << " passed" << endl;
            else
                cout << "Test " << '"' << argv[i] << '"' << " FAILED" << endl;
        }
    } else {
        QApplication app(argc, argv);
        GLWidget window;
        int windowWidth = 800;
        int windowHeight = 600;
        window.resize(windowWidth,windowHeight);
        QRect screen = app.desktop()->screenGeometry();
        window.move(screen.width()/2 - windowWidth/2,
                    screen.height()/2 - windowHeight/2);
        window.show();

        return app.exec();
    }
    return 0;
}
