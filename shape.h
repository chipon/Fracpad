#ifndef SHAPE_H
#define SHAPE_H
#include <QPoint>
#include <QRect>
#include <QPainterPath>
#include <QDataStream>
class Shape:public QObject
{
public:
    Shape();
    virtual bool has_point(QPoint p);
    virtual void move(QPoint m);
    virtual void paint(QPainterPath &path);
    virtual QRect getBorder();
    virtual void save(QDataStream &dataStream);
    virtual void load(QDataStream &dataStream);
    virtual void edit_point(QPoint p0,QPoint p1);
    friend QDataStream & operator << (QDataStream &dataStream, Shape &base);
    friend QDataStream & operator >> (QDataStream &dataStream, Shape &base);
};

#endif // SHAPE_H