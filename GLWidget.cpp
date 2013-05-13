#include <QtGui/QMouseEvent>
#include <QFont>
#include <QTextStream>
#include <QMessageBox>
#include <list>
#include <iostream>
#include <fstream>
#include <assert.h>

#include "Point.h"
#include "Polygon.h"
#include "MonotonePartition.h"
#include "GLWidget.h"

using namespace std;
using namespace Geometry;

int windowWidth;
int windowHeight;
int worldWidth = 50;
int worldHeight;
Point worldPosition(0,0);
Point mousePosition(0,0);
list<Geometry::Polygon> polygons;
vector<Point> currentPolygon;
list<Geometry::Polygon> monotonePolygons;
list<Geometry::Polygon> triangles;
bool loopDraw = true;
bool drawPosibleNewEdge = true;
bool drawHelp = true;
QString clear;
QString deleteEdge;
QString failTest;
QString space;
QString enter;
QString save;
QString open;
QString lastEdge;
QString newEdge;
QString movement;
QString zoom;
QString help;
QString sExit;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);
    QTextStream(&lastEdge) << "L: enable\\disable draw last edge";
    QTextStream(&newEdge) << "N: enable\\disable draw possible new edge";
    QTextStream(&deleteEdge) << "D: delete last edge";
    QTextStream(&space) << "Space: complete current polygon";
    QTextStream(&enter) << "Enter: triangulate";
    QTextStream(&clear) << "C: clear triangulation";
    QTextStream(&save) << "S: save polygon to test.txt";
    QTextStream(&open) << "O: create polygon from test.txt";
    QTextStream(&failTest) << "F: create polygon from failTest.txt";
    QTextStream(&movement) << "Arrows: movement";
    QTextStream(&zoom) << "+\\-: zoom";
    QTextStream(&sExit) << "Esc: exit";
    QTextStream(&help) << "F1: enable\\disable draw help";
}

void GLWidget::initializeGL() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);
}

void GLWidget::resizeGL(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    worldHeight = ((double)h/w)*worldWidth;
    glOrtho(worldPosition.x, worldPosition.x + worldWidth,
            worldPosition.y, worldPosition.y + worldHeight,
            -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor4f(1,1,1,0.1);
    glBegin(GL_LINES);
    for (int i = 0; i < worldHeight; i++) {
        glVertex2d(worldPosition.x, worldPosition.y + i);
        glVertex2d(worldPosition.x + worldWidth, worldPosition.y + i);
    }
    for (int i = 0; i < worldWidth; i++) {
        glVertex2d(worldPosition.x + i, worldPosition.y + 0);
        glVertex2d(worldPosition.x + i, worldPosition.y + worldHeight);
    }
    glEnd();

    glColor4f(0,0,1,1);
    for (list<Geometry::Polygon>::iterator polygon = triangles.begin();
         polygon != triangles.end();
         polygon++) {
        int numPoints =  polygon->GetNumPoints();
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < numPoints; i++)
            glVertex2d((*polygon)[i].x,(*polygon)[i].y);
        glEnd();
    }

    glColor4f(1,0,0,1);
    glPointSize(5);
    for (list<Geometry::Polygon>::iterator polygon = polygons.begin();
         polygon != polygons.end();
         polygon++) {

        int numPoints = polygon->GetNumPoints();
        glBegin(GL_POINTS);
        for (int i = 0; i < numPoints; i++)
            glVertex2d((*polygon)[i].x, (*polygon)[i].y);
        glEnd();

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < numPoints; i++)
            glVertex2d((*polygon)[i].x, (*polygon)[i].y);
        glEnd();
    }

    glColor3f(1, 1, 0);
    for (list<Geometry::Polygon>::iterator monotonePolygon = monotonePolygons.begin();
         monotonePolygon != monotonePolygons.end();
         monotonePolygon++) {

        if (!monotonePolygon->Contain(mousePosition))
            continue;

        int numPoints = monotonePolygon->GetNumPoints();
        glBegin(GL_POINTS);
        for (int i = 0; i < numPoints; i++)
            glVertex2d((*monotonePolygon)[i].x, (*monotonePolygon)[i].y);
        glEnd();

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < numPoints; i++)
            glVertex2d((*monotonePolygon)[i].x, (*monotonePolygon)[i].y);
        glEnd();
    }
    glColor3f(1, 0, 0);

    loopDraw ? glBegin(GL_LINE_LOOP) : glBegin(GL_LINE_STRIP);
    for (unsigned int i = 0; i < currentPolygon.size(); i++)
        glVertex2d(currentPolygon[i].x, currentPolygon[i].y);
    glEnd();

    glBegin(GL_POINTS);
    for (unsigned int i = 0; i < currentPolygon.size(); i++)
        glVertex2d(currentPolygon[i].x, currentPolygon[i].y);
    glEnd();

    if (drawPosibleNewEdge && currentPolygon.size() != 0) {
        glBegin(GL_LINES);
        glVertex2d(currentPolygon[currentPolygon.size() - 1].x,
                   currentPolygon[currentPolygon.size() - 1].y);
        glVertex2d(mousePosition.x, mousePosition.y);
        glEnd();
    }

    glBegin(GL_POINTS);
    glVertex2d(mousePosition.x, mousePosition.y);
    glEnd();

    glColor4f(1,0,0,1);

    QString mousePos;
    QTextStream(&mousePos) << "(" << mousePosition.x << "," << mousePosition.y << ")";
    renderText(10, 20, mousePos);

    if (drawHelp) {
        renderText(10, 35, lastEdge);
        renderText(10, 50, newEdge);
        renderText(10, 65, deleteEdge);
        renderText(10, 80, space);
        renderText(10, 95, enter);
        renderText(10, 110, clear);
        renderText(10, 125, save);
        renderText(10, 140, open);
        renderText(10, 155, failTest);
        renderText(10, 170, movement);
        renderText(10, 185, zoom);
        renderText(10, 200, sExit);
        renderText(10, 215, help);
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    currentPolygon.push_back(mousePosition);
    repaint();
}
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    mousePosition.x = worldPosition.x + ((double)worldWidth/windowWidth)*event->x();
    mousePosition.y = worldPosition.y + (worldHeight - ((double)worldHeight/windowHeight)*event->y());
    repaint();
}

void GLWidget::keyPressEvent(QKeyEvent* event) {
    ofstream ofile;
    ifstream ifile;
    Geometry::Polygon newPolygon;
    switch(event->key()) {
    case Qt::Key_C:
        monotonePolygons.resize(0);
        triangles.resize(0);
        repaint();
        break;
    case Qt::Key_D:
        if (currentPolygon.size() != 0) {
            currentPolygon.pop_back();
            repaint();
        } else if (polygons.size() != 0) {
            currentPolygon = polygons.back().GetVectorPoints();
            polygons.pop_back();
        }
        break;
    case Qt::Key_F:
        polygons.resize(0);
        currentPolygon.resize(0);
        Geometry::Polygon::ReadPolygonsFromFile(&polygons, "failTest.txt");
        break;
    case Qt::Key_L:
        loopDraw = !loopDraw;
        repaint();
        break;
    case Qt::Key_N:
        drawPosibleNewEdge = !drawPosibleNewEdge;
        repaint();
        break;
    case Qt::Key_S:
        Geometry::Polygon::SavePolygonsToFile(&polygons, "test.txt");
        break;
    case Qt::Key_O:
        {
        polygons.resize(0);
        currentPolygon.resize(0);
        Geometry::Polygon::ReadPolygonsFromFile(&polygons, "test.txt");
        }
        break;
    case Qt::Key_Space:
        {
        Geometry::Polygon newPolygon(currentPolygon);
        if (polygons.size() == 0 && newPolygon.GetOrientation() == Geometry::Polygon::CW)
            newPolygon.InvertOrientation();
        else if (polygons.size() != 0 && newPolygon.GetOrientation() == Geometry::Polygon::CCW)
            newPolygon.InvertOrientation();

        polygons.push_back(newPolygon);
        currentPolygon.resize(0);
        repaint();
        }
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        {
        if (currentPolygon.size() != 0)
            break;
        /*
        monotonePolygons.clear();
        triangles.clear();
        Geometry::MonotonePartition::Triangulate(&polygons, &monotonePolygons, &triangles);
        bool correctness = Geometry::MonotonePartition::CheckCorrectness(&polygons, &triangles);
        if (correctness == false) {
            Geometry::Polygon::SavePolygonsToFile(&polygons, "failTest.txt");
            QMessageBox failMessage;
            failMessage.setText("Triangulation failed.\nTest was saved in failTest.txt");
            failMessage.exec();
        }*/
        int i = 1;

        currentPolygon[i];
        repaint();
        }
        break;
    case Qt::Key_Left:
        worldPosition.x -= 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Right:
        worldPosition.x += 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Down:
        worldPosition.y -= 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Up:
        worldPosition.y += 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Plus:
        worldWidth -= 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Minus:
        worldWidth += 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_F1:
        drawHelp = !drawHelp;
        repaint();
        break;
    default:
        event->ignore();
        break;
    }
}
