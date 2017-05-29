#ifndef BEZIER_H
#define BEZIER_H
#include <QVector>
#include <QLine>
#include <poly.h>
#include <shape.h>
#include <affine.h>
#define close_bezier_pcs 10

class Bezier:public Shape
{
public:
    Bezier();
    void append(QPoint p2,QPoint c2);
    void clear();
    bool has_point(QPoint p);
    void move(QPoint m);
    void paint(QPainter &paint);
    QRect getBorder();
    int close_corner(QPoint p);
    void save(QDataStream &out);
    void load(QDataStream &in);

    void resize(QPoint axis, QPoint mov);
    void rotate(QPoint axis, QPoint start, QPoint end);
    void shear(bool direction, int ref, double sh);
private:
    //Affine af;
    static Poly B1,B2,B3,B4; //Four Bezier parameter
    //parameter curve keep all curves data
    //each element include a position point and a control point
    QVector<QLine> curve;
    int maxx,minx,maxy,miny;    //border of all curves
    QVector<QRect> border;  //border of each single curve
    QPoint p1,c1;   //temp parameter-accelerate calling function
    void get_zero_point(Poly &p,QVector<int> &extreme);
    void get_root(Poly &p,QVector<double>& root);
    void updateData();
};

#endif // BEZIER_H
