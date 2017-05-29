#ifndef MULTISHAPES_H
#define MULTISHAPES_H
#include <QVector>
#include <shape.h>
#include <bezier.h>
#include <myline.h>

class Multishapes:public Shape
{
public:
    QRect getBorder();
    void append(Shape *s);
    bool has_point(QPoint p);
    void move(QPoint m);
    void paint(QPainter &paint);
    void save(QDataStream &out);
    void load(QDataStream &in);
    void clear();
    bool isEmpty();
    void split(QVector<Shape *> &v,int index);

    void resize(QPoint axis, QPoint mov);
    void rotate(QPoint axis, QPoint start, QPoint end);
    //direction=true:row shear;direcion=false:column shear
    void shear(bool direction, int ref, double sh);
    void updateData();
    void setCenter(QPoint c);
//    virtual void set_end_point(QPoint end){}
//    virtual bool has_point(QPoint p)=0;
//    virtual void move(QPoint m)=0;
//    virtual void paint(QPainter &paint)=0;
//    virtual void paintBorder(QPainter &paint)=0;
//    virtual void save(QDataStream &out)=0;
//    virtual void load(QDataStream &in)=0;
private:
    //Affine af;
    QRect border;
    QVector<Shape *> shapes;
    QVector<Shape *>::iterator it;
};

#endif // MULTISHAPES_H
