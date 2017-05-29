class Bezier;
class Myline;
#include <bezier.h>
#include <myline.h>
#include <QtMath>

QDataStream & operator << (QDataStream &dataStream, Shape &base){
    dataStream << QString(base.metaObject()->className());//将对象的类名写入流中
    base.save(dataStream);
    return dataStream;
}
QDataStream & operator >> (QDataStream &dataStream, Shape **base){
    QString str;
    dataStream >> str;
    *base = NULL;
    if (str == QString("Bezier"))   //根据类名字符串生成相应类的对象
    {
        (*base) = new Bezier();
        (*base)->load(dataStream);
        return dataStream;
    }
    if (str == QString("Myline"))
    {
        (*base) = new Myline();
        (*base)->load(dataStream);
        return dataStream;
    }
    return dataStream;
}

//void Shape::pre_resize(QPoint axis, QPoint mov)
//{
//    double sx=1+mov.x()/(float)(this->getBorder().width());
//    double sy=1+mov.y()/(float)(this->getBorder().height());
//    af.resize(axis,sx,sy);
//    this->updateData();
//}

//void Shape::pre_rotate(QPoint axis, QPoint start, QPoint end)
//{
//    double theta1=qAtan2(start.x()-axis.x(),start.y()-axis.y());
//    double theta2=qAtan2(end.x()-axis.x(),end.y()-axis.y());
//    af.rotate(axis,theta2-theta1);
//    this->updateData();

//}

//void Shape::pre_shear(bool direction, int ref, double sh)
//{
//    af.shear(direction,ref,sh);
//    this->updateData();
//}

QPoint Shape::getCenter()
{
    return center;
}

void Shape::setCenter(QPoint c)
{
    center=c;
}

void Shape::movCenter(QPoint c)
{
    center+=c;
}
