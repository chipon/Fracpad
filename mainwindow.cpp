#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "attrdialog.h"
#include <QDebug>
#include <QToolButton>
#include <QPainter>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <cmath>
#include <myline.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pos=new QLabel();
    this->statusBar()->addWidget(pos);

    toPointer=new QAction();
    toPointer->setToolTip("选择");
    toPointer->setIcon(QIcon(":/icons/pointer.ico"));
    connect(toPointer,SIGNAL(triggered(bool)),this,SLOT(choose_shape()));
    ui->toolBar->addAction(toPointer);

    drawLine=new QAction();
    drawLine->setToolTip("直线");
    drawLine->setIcon(QIcon(":/icons/line.ico"));
    connect(drawLine,SIGNAL(triggered(bool)),this,SLOT(draw_line()));
    ui->toolBar->addAction(drawLine);

    drawBezier=new QAction();
    drawBezier->setToolTip("贝塞尔曲线");
    drawBezier->setIcon(QIcon(":/icons/Bezier_Curve.ico"));
    connect(drawBezier,SIGNAL(triggered(bool)),this,SLOT(draw_Bezier()));
    ui->toolBar->addAction(drawBezier);

    clear=new QAction();
    clear->setToolTip("清除");
    clear->setIcon(QIcon(":/icons/clear.ico"));
    connect(clear,SIGNAL(triggered(bool)),this,SLOT(clear_screen()));
    ui->toolBar->addAction(clear);

    currentBezier=nullptr;
    currentShape=nullptr;
    currentMoveState=None;
    currentDrawState=Pointer;
    currentBezierState=Bezier1;
    this->centralWidget()->setMouseTracking(true);
    this->setMouseTracking(true);
    //QDesktopWidget* desktopWidget = QApplication::desktop();
    //QRect clientRect = desktopWidget->();    //用户可用窗口大小
    //QRect r = desktopWidget->availableGeometry();  //应用程序可用窗口大小
    //qDebug()<<(r.height()-this->statusBar()->height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::choose_shape()
{
    currentDrawState=Pointer;
}

void MainWindow::draw_line()
{
    currentDrawState=Line;
}

void MainWindow::draw_Bezier()
{
    currentDrawState=BezierCurve;
}

void MainWindow::clear_screen()
{
    switch (currentDrawState) {
    case BezierCurve:
        currentBezierState=Bezier1;
        update();
        bezierLines.clear();
        currentBezier->clear();
        update();
        break;
    default:
        break;
    }
}

//void MainWindow::on_pushButton_clicked()
//{
//    AttrDialog ad(this);
//    ad.exec();
//    //ui->label->setText(ad.getValue());
//    //qDebug()<<ad.getValue();
//    //qDebug("%s",qPrintable(ad.getValue()));
//}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QString s("位置：x=");
    s+=QString::number(event->x());
    s+=" y=";
    s+=QString::number(event->y());
    pos->setText(s);
    Shape *p;
    Bezier *b;

    if(currentDrawState==Pointer){
        switch (currentMoveState) {
        case Choose:
            currentMoveState=None;
        case None:
            foreach (p, shapes) {
                if(p->has_point(event->pos())){
                    currentMoveState=Choose;
                    currentShape=p;
                    break;
                }
            }
            foreach (b, bezierLines) {
                if(b->has_point(event->pos())){
                    currentMoveState=Choose;
                    currentShape=b;
                    break;
                }
            }
            if(currentMoveState==Choose){
                this->setCursor(Qt::DragMoveCursor);
            }else{
                this->setCursor(Qt::ArrowCursor);
            }
            break;
        case Move:
            end=event->pos();
            foreach (p, currentChooseShapes) {
                p->move(end-start);
            }
            update();
            start=end;
            break;
        default:
            break;
        }
    }else if(currentDrawState==BezierCurve){
        switch (currentBezierState) {
        case Bezier2:
            c1=event->pos();
            update();
            break;
        case Bezier3:
            p2=c2=event->pos();
            update();
            break;
        case Bezier4:
            c2=event->pos();
            update();
            break;
        default:
            break;
        }
    }else{
        if(currentShape!=nullptr){
            currentShape->set_end_point(event->pos());
            update();
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    switch (currentDrawState) {
    case Pointer:
        if(currentMoveState==None){
            currentChooseShapes.clear();
        }else if(currentMoveState==Choose){
            currentMoveState=Move;
            start=event->pos();
        }
        break;
    case Line:
        if(currentShape==nullptr)
            currentShape=new Myline(event->pos());
        break;
    case BezierCurve:
        if(currentBezier==nullptr)
            currentBezier=new Bezier();
        switch (currentBezierState) {
        case Bezier1:
            p1=c1=event->pos();
            currentBezierState=Bezier2;
            break;
        case Bezier3:
            p2=c2=event->pos();
            currentBezierState=Bezier4;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    switch (currentDrawState) {
    case Pointer:
        end=event->pos();
        switch (currentMoveState) {
        case Move:
            currentShape->move(end-start);
            currentChooseShapes.append(currentShape);
            currentMoveState=None;
            update();
            break;
        default:
            break;
        }
        //qDebug()<<p1<<p2;
        break;
    case Line:
        currentShape->set_end_point(event->pos());
        shapes.append(currentShape);
        currentShape=nullptr;
        break;
    case BezierCurve:
        switch (currentBezierState) {
        case Bezier2:
            p2=c1=event->pos();
            //update();
            currentBezierState=Bezier3;
            currentBezier->append(p1,c1);
            break;
        case Bezier4:
            c2=event->pos();
            //update();
            currentBezier->append(p2,c2);
            p1=p2;
            c1=c2;
            currentBezierState=Bezier3;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(currentDrawState==BezierCurve){
        if(event->key()==Qt::Key_Escape){
            currentBezierState=Bezier1;
            bezierLines.append(currentBezier);
            //currentBezier->clear();
            currentBezier=nullptr;
            update();
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    if(currentBezier!=nullptr)
        currentBezier->paint(path);
    Shape *p,*b;
    QVector<Shape *>::iterator it1;
    QVector<Bezier *>::iterator it2;

    if(currentDrawState==Pointer){
        foreach (p, currentChooseShapes) {
           p->paintBorder(path);
        }
    }else if(currentDrawState==BezierCurve){
        switch (currentBezierState) {
        case Bezier4:
            path.moveTo(p2);
            path.lineTo(c2);
        case Bezier3:
            path.moveTo(p1);
            path.cubicTo(c1,c2,p2);
        case Bezier2:
            path.moveTo(p1);
            path.lineTo(c1);
        default:
            break;
        }
    }else if(currentShape!=nullptr){
        currentShape->paint(path);
    }
    foreach (p, shapes) {
        p->paint(path);
    }
    foreach (b, bezierLines) {
        b->paint(path);
    }
    paint.drawPath(path);
}
