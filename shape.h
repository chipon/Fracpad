#ifndef SHAPE_H
#define SHAPE_H
#include <QPoint>
#include <QRect>
#include <affine.h>
#include <QPainter>
#include <QDataStream>
#define CORNER_EPS 0.1

class Shape:public QObject
{
public:
    virtual void set_end_point(QPoint end){}
    virtual bool has_point(QPoint p)=0;
    virtual void move(QPoint m)=0;
    virtual void paint(QPainter &paint)=0;
    virtual QRect getBorder()=0;
    virtual void save(QDataStream &out)=0;
    virtual void load(QDataStream &in)=0;

    virtual void updateData()=0;
    virtual void resize(QPoint axis, QPoint mov)=0;
    virtual void rotate(QPoint axis, QPoint start, QPoint end)=0;
    //direction=true:row shear;direcion=false:column shear
    virtual void shear(bool direction, int ref, double sh)=0;
    //virtual void edit_point(QPoint p0,QPoint p1)=0;

//    void pre_resize(QPoint axis, QPoint mov);
//    void pre_rotate(QPoint axis, QPoint start, QPoint end);
//    //direction=true:row shear;direcion=false:column shear
//    void pre_shear(bool direction, int ref, double sh);

    //from http://blog.csdn.net/freezgw1985/article/details/5511600
    friend QDataStream & operator << (QDataStream &dataStream, Shape &base);
    friend QDataStream & operator >> (QDataStream &dataStream, Shape **base);
protected:
//    Affine af;
};

#endif // SHAPE_H
