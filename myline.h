#ifndef LINE_H
#define LINE_H
#include <shape.h>
#include <QPoint>
class Myline:public Shape
{
public:
    Myline(QPoint start):Shape(start){
        p1=start;
        p2=end;
    }
    bool has_point(QPoint p){
        if()
        if(p.x()<p1.x() || p.x()>p1.y() || p.y()<miny ||p.y()>maxy)
            return false;
    }

    void setPoint(QPoint end){
        p2=end;
    }

    QRect getBorder(){
        return QRect(p1,p2);
    }

    virtual void move(QPoint m);
    virtual void paint(QPainterPath &path);
    virtual QRect getBorder();
    virtual void save(QDataStream &dataStream);
    virtual void load(QDataStream &dataStream);
    virtual void edit_point(QPoint p0,QPoint p1);
    friend QDataStream & operator << (QDataStream &dataStream, Shape &base);
    friend QDataStream & operator >> (QDataStream &dataStream, Shape &base);
private:
    QPoint p1,p2;
};

#endif // LINE_H
