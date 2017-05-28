#ifndef AFFINE_H
#define AFFINE_H
#include <QPoint>

class Affine
{
public:
    Affine();
    //void move(QPoint m);
    void reset();
    QPoint getPos(QPoint p);
    //void rotate(QPoint axis,double angle);
    void resize(QPoint axis, double sx, double sy);
    void rotate(QPoint axis, double theta);
    //direction=true:row shear;direcion=false:column shear
    void shear(bool direction, int ref, double sh);
public:
    double a[4];
    double b[2];
    void compound(double c[], double d[]);
};

#endif // AFFINE_H
