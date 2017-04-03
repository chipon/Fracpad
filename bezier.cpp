#include "bezier.h"
#include <QtMath>
#include <QtDebug>

Poly Bezier::B1=Poly(1,-3,3,-1);
Poly Bezier::B2=Poly(0,-3,-6,3);
Poly Bezier::B3=Poly(0,0,3,-3);
Poly Bezier::B4=Poly(0,0,0,1);

Bezier::Bezier(){
    int a[4]={1,-3,3,-1};
    B1=Poly(3,a);
    a[0]=0;a[1]=3;a[2]=-6;a[3]=3;
    B2=Poly(3,a);
    a[0]=0;a[1]=0;a[2]=3;a[3]=-3;
    B3=Poly(3,a);
    a[0]=0;a[1]=0;a[2]=0;a[3]=1;
    B4=Poly(3,a);
}

void Bezier::save(QDataStream &out){
    QVector<QLine>::iterator it;
    out<<curve.size();
    for(it=curve.begin();it!=curve.end();it++){
        out<<it->p1().x()<<it->p1().y()<<it->p2().x()<<it->p2().y();
    }
}

void Bezier::load(QDataStream &in){
    QLine line;
    int count,x,y;
    in>>count;
    for(int i=0;i<count;++i){
        in>>x>>y;//<<line.p1.y()<<line.p2.x()<<line.p2.y();
        line.setP1(QPoint(x,y));
        in>>x>>y;//<<line.p1.y()<<line.p2.x()<<line.p2.y();
        line.setP2(QPoint(x,y));
        curve.append(line);
    }
}

void Bezier::append(QPoint p2,QPoint c2){
    if(curve.empty()){
        maxx=minx=p2.x();
        maxy=miny=p2.y();
        curve.append(QLine(p2,c2));
        return;
    }
    QVector<int> extremeX,extremeY;
    QVector<int>::iterator it;
    extremeX.append(p2.x());
    extremeY.append(p2.y());

    p1=curve.last().p1();
    c1=curve.last().p2();
    Poly x=(B1*p1.x())+(B2*c1.x())+(B3*c2.x())+(B4*p2.x());
    Poly y=(B1*p1.y())+(B2*c1.y())+(B3*c2.y())+(B4*p2.y());
    get_zero_point(x,extremeX);
    get_zero_point(y,extremeY);

    for(it=extremeX.begin();it!=extremeX.end();it++){
        maxx=qMax(maxx,*it);
        minx=qMin(minx,*it);
    }
    for(it=extremeY.begin();it!=extremeY.end();it++){
        maxy=qMax(maxy,*it);
        miny=qMin(miny,*it);
    }

    int maxx1,minx1,maxy1,miny1;
    maxx1=minx1=p1.x();
    maxy1=miny1=p1.y();
    for(it=extremeX.begin();it!=extremeX.end();it++){
        maxx1=qMax(maxx1,*it);
        minx1=qMin(minx1,*it);
    }
    for(it=extremeY.begin();it!=extremeY.end();it++){
        maxy1=qMax(maxy1,*it);
        miny1=qMin(miny1,*it);
    }
    border.append(QRect(QPoint(minx1,miny1),QPoint(maxx1,maxy1)));
    maxx=qMax(maxx,maxx1);
    minx=qMin(minx,minx1);
    maxy=qMax(maxy,maxy1);
    miny=qMin(miny,miny1);

    curve.append(QLine(p2,c2));
}

void Bezier::clear(){
    curve.clear();
}

bool Bezier::has_point(QPoint p){
    if(p.x()<minx || p.x()>maxx || p.y()<miny ||p.y()>maxy)
        return false;
    QVector<int> y;
    QVector<int>::iterator ity;
    QVector<QRect>::iterator it;
    QVector<QLine>::iterator itor;
    QVector<double> u;
    QVector<double>::iterator itu;
    Poly xu,yu;
    for(it=border.begin(),itor=curve.begin();it!=border.end();it++){
        if(it->contains(p)){
            p1=itor->p1();
            c1=itor->p2();
            itor++;
            xu=(B1*p1.x())+(B2*c1.x())+(B3*itor->p2().x())+(B4*itor->p1().x());
            yu=(B1*p1.y())+(B2*c1.y())+(B3*itor->p2().y())+(B4*itor->p1().y());
            xu[0]-=p.x();
            u.clear();
            get_root(xu,u);
            for(itu=u.begin();itu!=u.end();itu++){
                if((*itu)>=0 && (*itu)<=1)
                    y.append((int)(yu.value(*itu)));
            }
        }
    }
    for(ity=y.begin();ity!=y.end();ity++){
        if(abs((*ity)-p.y())<close_bezier_pcs){
            return true;
        }
    }
    return false;
}

void Bezier::move(QPoint m){
    QVector<QLine>::iterator it;
    for(it=curve.begin();it!=curve.end();it++){
        it->setP1(it->p1()+m);
        it->setP2(it->p2()+m);
    }
    minx+=m.x();
    maxx+=m.x();
    miny+=m.y();
    maxy+=m.y();
}

void Bezier::paint(QPainterPath &path){
    QVector<QLine>::iterator it;
    if(curve.size()>=2){
        for(it=curve.begin();;){
            p1=it->p1();
            c1=it->p2();
            it++;
            if(it==curve.end()) break;
            path.moveTo(p1);
            path.cubicTo(c1,it->p2(),it->p1());
        }
    }
}

void Bezier::paintBorder(QPainterPath &path){
    path.addRect(QRect(QPoint(minx,miny),QPoint(maxx,maxy)));
}

void Bezier::get_zero_point(Poly &p,QVector<int> &extreme){
    Poly pd=p.diff();
    int a=pd[2],b=pd[1],c=pd[0];
    int det=b*b-4*a*c;
    if(det<0) return;
    double u,k;
    if(det==0){
        u=-b/(2*a);
        if(u>=0 && u<=1){
            //qDebug()<<'1'<<u<<(int)(p.value(u));
            extreme.append((int)(p.value(u)));
        }
    }else if(det>0){
        k=qSqrt(det);
        u=(-b+k)/(2*a);
        if(u>=0 && u<=1){
            //qDebug()<<'2'<<u<<(int)(p.value(u));
            extreme.append((int)(p.value(u)));
        }
        u=(-b-k)/(2*a);
        if(u>=0 && u<=1){
            //qDebug()<<'3'<<u<<(int)(p.value(u));
            extreme.append((int)(p.value(u)));
        }
    }
}
void Bezier::get_root(Poly &p,QVector<double>& root){
    int a=p[3],b=p[2],c=p[1],d=p[0];
    double A,B,C,det;
    A=b*b-3*a*c;
    B=b*c-9*a*d;
    C=c*c-3*b*d;
    det=B*B-4*A*C;
    if(A==0&&B==0){
        if(c!=0)
            root.append(-3*d/c);
        else
            root.append(0);
    }else if(det>0){
        double y1,y2;
        int g1,g2;
        g1=1;
        g2=1;
        y1=A*b+3*a*(-1*B+pow((B*B-4*A*C),0.5))/2;
        y2=A*b+3*a*(-1*B-pow((B*B-4*A*C),0.5))/2;
        if(y1<0){
            y1=y1*(-1);
            g1=-1;
        }
        if(y2<0){
            y2=y2*(-1);
            g2=-1;
        }
        root.append((-1*b-(g1*pow(y1,1.0/3.0)+g2*pow(y2,1.0/3.0)))/(3*a));
    }else if(det==0){
        double k;
        k=A/B;
        double x1,x2;
        x1=-1*b/a+k;
        x2=-k/2;
        root.append(x1);
        root.append(x2);
    }else{
        double t,angle,x1,x2,x3;
        t=(2*A*b-3*a*B)/(2*pow(A,1.5));
        angle=acos(t);
        x1=(-1*b-2*pow(A,0.5)*cos(angle/3.0))/(3.0*a);
        x2=(-1*b+pow(A,0.5)*(cos(angle/3.0)+pow(3.0,0.5)*sin(angle/3.0)))/(3.0*a);
        x3=(-1*b+pow(A,0.5)*(cos(angle/3.0)-pow(3.0,0.5)*sin(angle/3.0)))/(3.0*a);
        root.append(x1);
        root.append(x2);
        root.append(x3);
    }
}
