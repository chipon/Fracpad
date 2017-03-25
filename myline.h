#ifndef LINE_H
#define LINE_H
#include <shape.h>
#include <QPoint>
#define EPS_LINE 4
class Myline:public Shape
{
public:
    Myline(QPoint start){
        p2=p1=start;
    }
    bool has_point(QPoint p){
        if(!QRect(p1,p2).contains(p))
            return false;
        int det=(p.y()-p1.y())*(p1.x()-p2.x())-(p2.y()-p1.y())*(p.x()-p1.x());
        if(det<EPS_LINE)
            return true;
        return false;
    }

    void setPoint(QPoint end){
        p2=end;
    }

    void move(QPoint m){
        p1+=m;
        p2+=m;
    }

    void paintBorder(QPainterPath &path){
        path.addRect(QRect(p1,p2));
    }

    void paint(QPainterPath &path){
        path.moveTo(p1);
        path.lineTo(p2);
    }

    void save(QDataStream &dataStream){

    }

    void load(QDataStream &dataStream){

    }

    //virtual void edit_point(QPoint p0,QPoint p1);
    //friend QDataStream & operator << (QDataStream &dataStream, Shape &base);
    //friend QDataStream & operator >> (QDataStream &dataStream, Shape &base);
private:
    QPoint p1,p2;
};

#endif // LINE_H
