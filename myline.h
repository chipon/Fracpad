#ifndef LINE_H
#define LINE_H
#include <QPoint>
#include <QDataStream>
#include <shape.h>
#define EPS_LINE 1100
#define EPS_VERTICAL 3
#define EPS_HORIZON 3

class Myline:public Shape
{
public:
//    virtual void setPoint(QPoint end)=0;
//    virtual bool has_point(QPoint p)=0;
//    virtual void move(QPoint m)=0;
//    virtual void paint(QPainterPath &path)=0;
//    virtual void paintBorder(QPainterPath &path)=0;
    Myline();
    Myline(QPoint start);
    bool has_point(QPoint p);
    void set_end_point(QPoint end);
    void move(QPoint m);
    void paintBorder(QPainterPath &path);
    void paint(QPainterPath &path);
    void save(QDataStream &out);
    void load(QDataStream &in);
    //virtual void edit_point(QPoint p0,QPoint p1);
private:
    QPoint p1,p2;
    bool f1,f2; //accelerate checking whether the point is closed to the line
};

#endif // LINE_H
