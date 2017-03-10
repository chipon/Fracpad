#include "mainwindow.h"
#include <QApplication>
#include <poly.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();
    /*int a[4]={1,3,2,0};
    Poly p1(3,a);
    int b[4]={1,2,1,3};
    Poly p2(3,b);
    Poly p3=p2.diff();
    for(int i=0;i<p3.power()+1;++i){
        qDebug()<<p3.get(i);
    }*/
}
