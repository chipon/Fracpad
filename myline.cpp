#include <myline.h>
#include <QDebug>
#include <QtMath>

Myline::Myline(){}

Myline::Myline(QPoint start){
    p2=p1=start;
}

bool Myline::has_point(QPoint p){
    if(f1) return qAbs(p.x()-p1.x())<EPS_HORIZON && (p.y()-p1.y())*(p2.y()-p.y())>=0;
    if(f2) return qAbs(p.y()-p1.y())<EPS_VERTICAL && (p.x()-p1.x())*(p2.x()-p.x())>=0;
    if(!QRect(p1,p2).contains(p))
        return false;
    int det=(p.y()-p1.y())*(p2.x()-p1.x())-(p2.y()-p1.y())*(p.x()-p1.x());
    if(qAbs(det)<EPS_LINE)
        return true;
    return false;
}

void Myline::set_end_point(QPoint end){
    p2=end;
    f1=qAbs(p1.x()-p2.x())<EPS_HORIZON;
    f2=qAbs(p1.y()-p2.y())<EPS_VERTICAL;
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
    f1=qAbs(p1.x()-p2.x())<EPS_HORIZON;
    f2=qAbs(p1.y()-p2.y())<EPS_VERTICAL;
}

//virtual void edit_point(QPoint p0,QPoint p1);
