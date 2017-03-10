#ifndef BEZIER_H
#define BEZIER_H
#include <QVector>
#include <QLine>
#include <poly.h>
#include <shape.h>
#define close_bezier_pcs 8

class Bezier:public Shape
{
public:
    Bezier();
    void append(QPoint p2,QPoint c2);
    void clear();
    bool has_point(QPoint p);
    void move(QPoint m);
    void paint(QPainterPath &path);
    QRect getBorder();
private:
    static Poly B1,B2,B3,B4; //Four Bezier parameter
    //parameter curve keep all curves data
    //each element include a position point and a control point
    QVector<QLine> curve;
    int maxx,minx,maxy,miny;    //border of all curves
    QVector<QRect> border;  //border of each single curve
    QPoint p1,c1;   //temp parameter-accelerate calling function
    void get_zero_point(Poly &p,QVector<int> &extreme);
    void get_root(Poly &p,QVector<double>& root);
};

#endif // BEZIER_H
