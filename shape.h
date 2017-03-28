#ifndef SHAPE_H
#define SHAPE_H
#include <QPoint>
#include <QRect>
#include <QPainterPath>
#include <QDataStream>
class Shape//:public QObject
{
public:
    virtual void set_end_point(QPoint end){}
    virtual bool has_point(QPoint p)=0;
    virtual void move(QPoint m)=0;
    virtual void paint(QPainterPath &path)=0;
    virtual void paintBorder(QPainterPath &path)=0;
    virtual void save(QDataStream &dataStream);
    virtual void load(QDataStream &dataStream);
    virtual void edit_point(QPoint p0,QPoint p1);
    friend QDataStream & operator << (QDataStream &dataStream, Shape &base){
        dataStream << QString(base.metaObject()->className());//将对象的类名写入流中
        base.save(dataStream);
        return dataStream;
    }

    friend QDataStream & operator >> (QDataStream &dataStream, Shape &base){
        QString str;
        dataStream >> str;
        *base = NULL;
//        if (str == QString("CKDrawLine"))   //根据类名字符串生成相应类的对象
//        {
//            *drawBase = new CKDrawLine;
//            (*drawBase)->load(dataStream);
//            return dataStream;
//        }

//        if (str == QString("CKDrawRectangle"))
//        {
//            *drawBase = new CKDrawRectangle;
//            (*drawBase)->load(dataStream);
//            return dataStream;
//        }
        return dataStream;
    }
};

#endif // SHAPE_H
