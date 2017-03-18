#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "attrdialog.h"
#include <QDebug>
#include <QToolButton>
#include <QPainter>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <cmath>

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

    flag1=flag2=flag3=flag4=false;
    currentMoveState=None;
    currentDrawState=Choose;
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
    currentDrawState=Choose;
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
        currentBezier.clear();
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
//    ui->label->setText(ad.getValue());
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
    QVector<Bezier>::iterator it;

    switch (currentDrawState) {
    case Choose:
        switch (currentMoveState) {
        case Move:
            currentBezier.move(p2-p1);
            update();
            break;
        default:
            break;
        }
        if(bezierLines.empty()) break;
        flag4=false;
        for(it=bezierLines.begin(),order=0;it!=bezierLines.end();it++,order++){
            if(it->has_point(event->pos())){
                flag4=true;
                break;
            }
        }
        if(flag4){
            this->setCursor(Qt::DragMoveCursor);
        }else{
            this->setCursor(Qt::ArrowCursor);
        }
        break;
    case Line:
        if(flag1){
            p2=event->pos();
            update();
        }
        break;
    case BezierCurve:
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
        break;
    default:
        break;
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    switch (currentDrawState) {
    case Choose:
        if(!flag4) break;
        currentMoveState=Move;
        bezierLines[order].move(p2-p1);
        p1=event->pos();
        break;
    case Line:
        flag1=true;
        p1=event->pos();
        break;
    case BezierCurve:
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
    case Choose:
        if(currentBezierState!=Bezier5) break;
        p2=event->pos();
        qDebug()<<p1<<p2;
        currentBezier.move(p2-p1);
        update();
        break;
    case Line:
        flag1=false;
        lines.append(QLine(p1,p2));
        break;
    case BezierCurve:
        switch (currentBezierState) {
        case Bezier2:
            p2=c1=event->pos();
            //update();
            currentBezierState=Bezier3;
            currentBezier.append(p1,c1);
            break;
        case Bezier4:
            c2=event->pos();
            //update();
            currentBezier.append(p2,c2);
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
            currentBezier.clear();
            flag3=true;
            border=currentBezier.getBorder();
            update();
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    currentBezier.paint(path);
    if(currentDrawState==BezierCurve){
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
    }else if(currentDrawState==Line){
        path.moveTo(p1);
        path.lineTo(p2);
    }
    QVector<Bezier>::iterator it;
    QVector<QLine>::iterator itor;
    for(it=bezierLines.begin();it!=bezierLines.end();it++){
        it->paint(path);
    }
    for(itor=lines.begin();itor!=lines.end();++itor){
        paint.drawLine(itor->p1(),itor->p2());
    }
    if(flag3){
        paint.drawRect(border);
    }
    paint.drawpol(border);
    paint.drawPath(path);
}
