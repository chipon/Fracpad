#include "affine.h"
#include <QtMath>
Affine::Affine()
{
    a[0]=a[3]=1;
    a[1]=a[2]=0;
    b[0]=b[1]=0;
}

//void Affine::move(QPoint m)
//{
//    double a[4]={1,0,0,1};
//    double b[2]={m.x(),m.y()};
//    this->compound(a,b);
//}

QPoint Affine::getPos(QPoint p)
{
    QPoint q;
    q.setX(qRound(a[0]*p.x()+a[1]*p.y()+b[0]));
    q.setY(qRound(a[2]*p.x()+a[3]*p.y()+b[1]));
    return q;
}

void Affine::resize(QPoint axis, double sx,double sy)
{
    a[0]=sx; a[1]=0; a[2]=0; a[3]=sy;
    b[0]=(1-sx)*axis.x();
    b[1]=(1-sy)*axis.y();
}

void Affine::rotate(QPoint axis, double theta)
{
    int x0=axis.x(),y0=axis.y();
    double sin=qSin(theta),cos=qCos(theta);
    double a[4]={cos,-sin,sin,cos};
    double b[2]={x0+y0*sin-x0*cos,y0-x0*sin-y0*cos};
}

void Affine::shear(bool direction, int ref, double sh)
{
    double a[4]={1,0,0,1};
    double b[2]={0,0};
    if(direction){
        a[1]=sh;
        b[0]=-sh*ref;
    }else{
        a[2]=sh;
        b[1]=-sh*ref;
    }
}

//void Affine::compound(double c[],double d[])
//{
//    a[0]=c[0]*a[0]+c[1]*a[2];
//    a[1]=c[0]*a[1]+c[1]*a[3];
//    a[2]=c[2]*a[0]+c[3]*a[2];
//    a[3]=c[2]*a[1]+c[3]*a[3];
//    b[0]=c[0]*b[0]+c[1]*b[1]+d[0];
//    b[1]=c[2]*b[0]+c[3]*b[1]+d[1];
//}
