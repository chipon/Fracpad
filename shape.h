#ifndef SHAPE_H
#define SHAPE_H
#include <QPoint>
#include <QRect>
#include <QPainterPath>
#include <QDataStream>

class Shape:public QObject
{
public:
    virtual void set_end_point(QPoint end){}
    virtual bool has_point(QPoint p)=0;
    virtual void move(QPoint m)=0;
    virtual void paint(QPainterPath &path)=0;
    virtual void paintBorder(QPainterPath &path)=0;
    virtual void save(QDataStream &out)=0;
    virtual void load(QDataStream &in)=0;
    //virtual void edit_point(QPoint p0,QPoint p1)=0;

    //from http://blog.csdn.net/freezgw1985/article/details/5511600
    friend QDataStream & operator << (QDataStream &dataStream, Shape &base);
    friend QDataStream & operator >> (QDataStream &dataStream, Shape **base);
};

#endif // SHAPE_H
