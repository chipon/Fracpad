#include <myline.h>
#include <QDebug>
#include <QtMath>

Myline::Myline(){}

Myline::Myline(QPoint start){
    p2=p1=center=start;
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
    QPoint topLeft=QPoint(qMin(p1.x(),p2.x()),qMin(p1.y(),p2.y()));
    QPoint buttomRight=QPoint(qMax(p1.x(),p2.x()),qMax(p1.y(),p2.y()));
    border=QRect(topLeft,buttomRight);
    center=border.center();
}

void Myline::move(QPoint m){
    p1+=m;
    p2+=m;
    border.moveTo(border.topLeft()+m);
    center=border.center();
}

QRect Myline::getBorder(){
    return border;
}

void Myline::paint(QPainter &paint){
    paint.drawLine(p1,p2);
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

void Myline::resize(QPoint axis, QPoint mov)
{
    double sx=1+mov.x()/(double)(border.width());
    double sy=1+mov.y()/(double)(border.height());
    af.resize(axis,sx,sy);
//    this->pre_resize(axis,mov);
    updateData();
}

void Myline::rotate(QPoint axis,QPoint start, QPoint end)
{
    double theta1=qAtan2(start.x()-axis.x(),axis.y()-start.y());
    double theta2=qAtan2(end.x()-axis.x(),axis.y()-end.y());
    //qDebug()<<theta2<<theta1;
    af.rotate(axis,theta2-theta1);
    //qDebug()<<af.a[0]<<af.a[1]<<af.a[2]<<af.a[3]<<af.b[0]<<af.b[1];
    updateData();
}

void Myline::shear(bool direction, int ref, double sh)
{
    //qDebug()<<af.a[0]<<af.a[1]<<af.a[2]<<af.a[3]<<af.b[0]<<af.b[1];
    af.shear(direction,ref,sh);
    updateData();
}

void Myline::updateData()
{
    p1=af.getPos(p1);
    p2=af.getPos(p2);
    QPoint topLeft=QPoint(qMin(p1.x(),p2.x()),qMin(p1.y(),p2.y()));
    QPoint buttomRight=QPoint(qMax(p1.x(),p2.x()),qMax(p1.y(),p2.y()));
    border=QRect(topLeft,buttomRight);
    center=af.getPos(center);
}
//virtual void edit_point(QPoint p0,QPoint p1);
