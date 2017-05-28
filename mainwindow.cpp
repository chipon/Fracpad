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
#include <QPixmap>

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

    //ui->mainToolBar->setLayout(QLayout());
    label=new QLabel("模式:");
    ui->mainToolBar->addWidget(label);
    cbMode=new QComboBox();
    cbMode->addItem("编辑模式");
    cbMode->addItem("单次生成");
    cbMode->addItem("多次生成");
    cbMode->setCurrentIndex(0);
    ui->mainToolBar->addWidget(cbMode);

    label=new QLabel("迭代次数:");
    ui->mainToolBar->addWidget(label);
    iterTimes=new QLineEdit();
    iterTimes->setText("5");
    iterTimes->setMaximumWidth(50);
    ui->mainToolBar->addWidget(iterTimes);

//    label=new QLabel("颜色");
//    ui->mainToolBar->addWidget(label);

    currentBezier=nullptr;
    currentShape=nullptr;
    currentChooseShape=nullptr;
    currentMoveState=None;
    currentDrawState=Pointer;
    currentBezierState=Bezier1;
    currentChooseState=Unchoose;
    this->centralWidget()->setMouseTracking(true);
    this->setMouseTracking(true);
//    QPoint po1(100,100),po2(200,200);
//    po1=po1-po2;
//    qDebug()<<po1.x()<<po1.y();
//    QVector<int> set;
//    QVector<int>::iterator it;
//    set.append(1);
//    set.append(2);
//    set.append(3);
//    set.append(4);
//    set.append(5);
//    for(it=set.begin();it!=set.end();it++){
//        if((*it)==3){
//            (*it)=6;
//        }
//    }
//    for(it=set.begin();it!=set.end();it++){
//        qDebug()<<(*it);
//    }
//    QRect rt(QPoint(100,100),QPoint(200,200));
//    rt.adjust(-10,-10,10,10);
//    qDebug()<<rt;
//    QPixmap pm=QPixmap(":/icons/Clockwise.png");
//    QSize size(16, 16);
//    this->setCursor(QCursor(pm.scaled(size,Qt::KeepAspectRatio)));
    //this->setAttribute(Qt::WA_TranslucentBackground, true);
    //this->centralWidget()->setAutoFillBackground(true);
    //this->centralWidget()->setBackgroundRole(pal.ColorRole);
    //this->centralWidget()->setPalette(pal);
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

//return 0:TopLeft,1:TopRight,2:ButtomLeft,3:ButtomRight
//return 4:TopCentre,5:LeftCentre,6:RightCentre,7:ButtomCentre
int MainWindow::getClosePosition(QPoint p)
{
    QVector<QPoint> borders;
    border.adjust(-RECT_SIZE/2,-RECT_SIZE/2,RECT_SIZE/2,RECT_SIZE/2);
    borders.append(border.topLeft());
    borders.append(border.topRight());
    borders.append(border.bottomLeft());
    borders.append(border.bottomRight());

    borders.append(QPoint((border.left()+border.right())/2,border.top()));
    borders.append(QPoint((border.left()+border.right())/2,border.bottom()));
    borders.append(QPoint(border.left(),(border.top()+border.bottom())/2));
    borders.append(QPoint(border.right(),(border.top()+border.bottom())/2));

    for(int i=0;i<8;++i){
        if(qAbs(p.x()-borders[i].x())<=RECT_SIZE/2 && qAbs(p.y()-borders[i].y())<=RECT_SIZE/2)
            return i;
    }
    return -1;
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
    QPoint axis,move;

    if(currentDrawState==Pointer){
        switch (currentMoveState) {
        case Close1:
            currentMoveState=None;
        case None:
            foreach (p, shapes) {
                if(p->has_point(event->pos())){
                    currentMoveState=Close1;
                    currentChooseShape=p;
                    break;
                }
            }
            foreach (b, bezierLines) {
                if(b->has_point(event->pos())){
                    currentMoveState=Close1;
                    currentChooseShape=b;
                    break;
                }
            }
            if(currentMoveState==Close1){
                this->setCursor(Qt::DragMoveCursor);
            }else{
                this->setCursor(Qt::ArrowCursor);
            }
            break;
        case Move:
            end=event->pos();
            currentChooseShapes.move(end-start);
            update();
            start=end;
            break;
        case Choose1:
            if(!currentChooseShapes.has_point(event->pos())){
                currentMoveState=Escape1;
                this->setCursor(Qt::ArrowCursor);
            }
            break;
        case Choose2:
            if(!currentChooseShapes.has_point(event->pos())){
                currentMoveState=Escape2;
                this->setCursor(Qt::ArrowCursor);
            }
            break;
        case Escape1:
            if((closeBorder=getClosePosition(event->pos()))>=0){
                if(closeBorder==0 || closeBorder==3)
                    this->setCursor(Qt::SizeFDiagCursor);
                else if(closeBorder==1 || closeBorder==2)
                    this->setCursor(Qt::SizeBDiagCursor);
                else if(closeBorder==4 || closeBorder==5)
                    this->setCursor(Qt::SizeVerCursor);
                else
                    this->setCursor(Qt::SizeHorCursor);
            }else{
                if(currentChooseShapes.has_point(event->pos())){
                    currentMoveState=Choose1;
                    this->setCursor(Qt::DragMoveCursor);
                }else{
                    this->setCursor(Qt::ArrowCursor);
                }
            }
            break;
        case Escape2:
            if((closeBorder=getClosePosition(event->pos()))>=0){
                if(closeBorder==0 || closeBorder==3)
                    this->setCursor(Qt::SizeBDiagCursor);
                else if(closeBorder==1 || closeBorder==2)
                    this->setCursor(Qt::SizeFDiagCursor);
                else if(closeBorder==4 || closeBorder==5)
                    this->setCursor(Qt::SizeHorCursor);
                else
                    this->setCursor(Qt::SizeVerCursor);
            }else{
                if(currentChooseShapes.has_point(event->pos())){
                    currentMoveState=Choose2;
                    this->setCursor(Qt::DragMoveCursor);
                }else{
                    this->setCursor(Qt::ArrowCursor);
                }
            }
            break;
        case Resize:
            if(closeBorder==0){
                axis=border.bottomRight();
                mov=start-event->pos();
            }else if(closeBorder==1){
                axis=border.bottomLeft();
                mov=event->pos()-start;
                mov.setY(-mov.y());
            }else if(closeBorder==2){
                axis=border.topRight();
                mov=event->pos()-start;
                mov.setX(-mov.x());
            }else if(closeBorder==3){
                axis=border.topLeft();
                mov=event->pos()-start;
            }else if(closeBorder==4){
                axis=QPoint((border.left()+border.right())/2,border.bottom());
                mov.setX(0);
                mov.setY(start.y()-event->pos().y());
            }else if(closeBorder==5){
                axis=QPoint((border.left()+border.right())/2,border.top());
                mov.setX(0);
                mov.setY(event->pos().y()-start.y());
            }else if(closeBorder==6){
                axis=QPoint(border.right(),(border.top()+border.bottom())/2);
                mov.setX(start.x()-event->pos().x());
                mov.setY(0);
            }else if(closeBorder==7){
                axis=QPoint(border.left(),(border.top()+border.bottom())/2);
                mov.setX(event->pos().x()-start.x());
                mov.setY(0);
            }
            //qDebug()<<"1"<<axis<<mov;
            currentChooseShapes.resize(axis,mov);
            update();
            start=event->pos();
            break;
        case Rotate:
            axis=border.center();
            end=event->pos();
            currentChooseShapes.rotate(axis,start,end);
            update();
            start=end;
            break;
        case Shear:
            axis=border.center();
            end=event->pos();
            if(closeBorder==4){
                currentChooseShapes.shear(true,axis.y(),(end.x()-start.x())/(double)(border.top()-axis.y()));
            }else if(closeBorder==5){
                currentChooseShapes.shear(true,axis.y(),(end.x()-start.x())/(double)(border.bottom()-axis.y()));
            }else if(closeBorder==6){
                currentChooseShapes.shear(false,axis.x(),(end.y()-start.y())/(double)(border.left()-axis.x()));
            }else{
                currentChooseShapes.shear(false,axis.x(),(end.y()-start.y())/(double)(border.right()-axis.x()));
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
        if(currentMoveState==Close1){
            currentChooseShapes.clear();
            currentChooseShapes.append(currentChooseShape);
            currentMoveState=Move;
            start=event->pos();
        }else if(currentMoveState==Choose1){
            currentChooseState=ResizeChoose;
            currentMoveState=Move;
            start=event->pos();
        }else if(currentMoveState==Choose2){
            currentChooseState=RotateChoose;
            currentMoveState=Move;
            start=event->pos();
        }else if(currentMoveState==Escape1){
            if(closeBorder<0){  //空白处单击...
                currentChooseShapes.clear();
                currentMoveState=None;
                currentChooseState=Unchoose;
                update();
            }else{
                currentMoveState=Resize;
                start=event->pos();
            }
        }else if(currentMoveState==Escape2){
            if(closeBorder<0){  //空白处单击...
                currentChooseShapes.clear();
                currentMoveState=None;
                currentChooseState=Unchoose;
                update();
            }else if(closeBorder<4){
                currentMoveState=Rotate;
                start=event->pos();
            }else{
                currentMoveState=Shear;
                start=event->pos();
            }
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
            if(currentChooseState==Unchoose || currentChooseState==RotateChoose){
                currentChooseState=ResizeChoose;
                currentMoveState=Choose1;
            }else if(currentChooseState==ResizeChoose){
                currentChooseState=RotateChoose;
                currentMoveState=Choose2;
            }else if(currentMoveState==Rotate){
                currentMoveState=Escape1;
            }else if(currentMoveState==Shear || currentMoveState==Rotate){
                currentMoveState=Escape2;
            }
            if(end!=start)
                currentChooseShapes.move(end-start);
            update();
            break;
        case Resize:
            currentMoveState=Escape1;
            this->setCursor(Qt::ArrowCursor);
            break;
        case Rotate:
            currentMoveState=Escape2;
            this->setCursor(Qt::ArrowCursor);
            break;
        case Shear:
            currentMoveState=Escape2;
            this->setCursor(Qt::ArrowCursor);
            break;
        default:
            break;
        }
        //qDebug()<<start<<end;
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

    paint.setBrush(Qt::white);
    paint.drawRect(0,0, this->width(), this->height());
    paint.setBrush(Qt::NoBrush);

    if(currentBezier!=nullptr)
        currentBezier->paint(paint);
    Shape *p,*b;

    if(!currentChooseShapes.isEmpty()){
        border=currentChooseShapes.getBorder();
        p1=border.topLeft();
        p2=border.topRight();
        p3=border.bottomLeft();
        p4=border.bottomRight();
        paint.setBrush(Qt::black);

        paint.drawRect(QRect(p1-QPoint(RECT_SIZE,RECT_SIZE),p1));
        paint.drawRect(QRect(p2-QPoint(0,RECT_SIZE),QSize(RECT_SIZE,RECT_SIZE)));
        paint.drawRect(QRect(p3-QPoint(RECT_SIZE,0),QSize(RECT_SIZE,RECT_SIZE)));
        paint.drawRect(QRect(p4,QSize(RECT_SIZE,RECT_SIZE)));

        paint.drawRect(QRect(QPoint((p1.x()+p2.x())/2-RECT_SIZE/2,border.top()-RECT_SIZE),QSize(RECT_SIZE,RECT_SIZE)));
        paint.drawRect(QRect(QPoint((p1.x()+p2.x())/2-RECT_SIZE/2,border.bottom()),QSize(RECT_SIZE,RECT_SIZE)));
        paint.drawRect(QRect(QPoint(p1.x()-RECT_SIZE,(p1.y()+p3.y())/2-RECT_SIZE/2),QSize(RECT_SIZE,RECT_SIZE)));
        paint.drawRect(QRect(QPoint(p2.x(),(p1.y()+p3.y())/2-RECT_SIZE/2),QSize(RECT_SIZE,RECT_SIZE)));
        paint.setBrush(Qt::NoBrush);

        paint.setPen(QPen(Qt::DashLine));
        border.adjust(-RECT_SIZE/2,-RECT_SIZE/2,RECT_SIZE/2,RECT_SIZE/2);
        paint.drawRect(border);
        paint.setPen(QPen(Qt::SolidLine));

        if(currentMoveState==Escape2){
            //
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
        currentShape->paint(paint);
    }
    foreach (p, shapes) {
        p->paint(paint);
    }
    foreach (b, bezierLines) {
        b->paint(paint);
    }
    paint.drawPath(path);
}
