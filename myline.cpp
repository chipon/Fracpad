#include <myline.h>

Myline::Myline(){}

Myline::Myline(QPoint start){
    p2=p1=start;
}

bool Myline::has_point(QPoint p){
    if(!QRect(p1,p2).contains(p))
        return false;
    int det=(p.y()-p1.y())*(p1.x()-p2.x())-(p2.y()-p1.y())*(p.x()-p1.x());
    if(det<EPS_LINE)
        return true;
    return false;
}

void Myline::set_end_point(QPoint end){
    p2=end;
}

void Myline::move(QPoint m){
    p1+=m;
    p2+=m;
}

void Myline::paintBorder(QPainterPath &path){
    path.addRect(QRect(p1,p2));
}

void Myline::paint(QPainterPath &path){
    path.moveTo(p1);
    path.lineTo(p2);
}

void Myline::save(QDataStream &out){
    out<<p1.x()<<p1.y()<<p2.x()<<p2.y();
}

void Myline::load(QDataStream &in){
    int x,y;
    in>>x>>y;
    p1=QPoint(x,y);
    in>>x>>y;
    p2=QPoint(x,y);
}

//virtual void edit_point(QPoint p0,QPoint p1);
