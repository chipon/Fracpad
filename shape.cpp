class Bezier;
class Myline;
#include <bezier.h>
#include <myline.h>

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
