#include "multishapes.h"
#include <QDebug>
#include <QtMath>
void Multishapes::append(Shape *s)
{
    if(shapes.empty())
        border=s->getBorder();
    else{
        border=border.united(s->getBorder());
    }
    shapes.append(s);
    center=border.center();
}

bool Multishapes::has_point(QPoint p)
{
    for(it=shapes.begin();it!=shapes.end();it++){
        if((*it)->has_point(p)){
            return true;
        }
    }
    return false;
}

void Multishapes::move(QPoint m)
{
    for(it=shapes.begin();it!=shapes.end();it++){
        (*it)->move(m);
    }
    border.moveTo(border.topLeft()+m);
    center=border.center();
}

void Multishapes::paint(QPainter &paint)
{
    for(it=shapes.begin();it!=shapes.end();it++){
        (*it)->paint(paint);
    }
}

QRect Multishapes::getBorder()
{
    return border;
}

void Multishapes::save(QDataStream &out)
{
    for(it=shapes.begin();it!=shapes.end();it++){
        out << QString((*it)->metaObject()->className());
        (*it)->save(out);
    }
}

void Multishapes::load(QDataStream &in)
{
    QString str;
    Shape *s;
    for(it=shapes.begin();it!=shapes.end();it++){
        in >> str;
        if (str == QString("Bezier")){   //根据类名字符串生成相应类的对象{
            s = new Bezier();
            s->load(in);
            shapes.append(s);
        }else if (str == QString("Myline")){
            s = new Myline();
            s->load(in);
            shapes.append(s);
        }
    }
}

void Multishapes::clear()
{
    shapes.clear();
}

bool Multishapes::isEmpty()
{
    return shapes.isEmpty();
}

void Multishapes::split(QVector<Shape *> &v, int index)
{
    v.remove(index);
    for(it=shapes.begin();it!=shapes.end();it++){
        v.append(*it);
    }
}

void Multishapes::resize(QPoint axis, QPoint mov)
{
    double sx=1+mov.x()/(double)(border.width());
    double sy=1+mov.y()/(double)(border.height());
    af.resize(axis,sx,sy);
    for(it=shapes.begin();it!=shapes.end();it++){
        (*it)->resize(axis,mov);
    }
    //this->pre_resize(axis,mov);
    updateData();
}

void Multishapes::rotate(QPoint axis,QPoint start, QPoint end)
{
    double theta1=qAtan2(start.x()-axis.x(),axis.y()-start.y());
    double theta2=qAtan2(end.x()-axis.x(),axis.y()-end.y());
    //qDebug()<<theta2<<theta1;
    af.rotate(axis,theta2-theta1);
    for(it=shapes.begin();it!=shapes.end();it++){
        (*it)->rotate(axis,start,end);
    }
    updateData();
}

void Multishapes::shear(bool direction, int ref, double sh)
{
    af.shear(direction,ref,sh);
    for(it=shapes.begin();it!=shapes.end();it++){
        (*it)->shear(direction,ref,sh);
    }
    updateData();
}

void Multishapes::updateData()
{
    it=shapes.begin();
    border=(*it)->getBorder();
    for(it++;it!=shapes.end();it++){
        border=border.united((*it)->getBorder());
    }
    center=af.getPos(center);
}

void Multishapes::setCenter(QPoint c)
{
    center=c;
    for(it=shapes.begin();it!=shapes.end();it++){
        (*it)->setCenter(center);
    }
}
