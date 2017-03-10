#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPoint>
#include <QLine>
#include <QStateMachine>
#include <QState>
#include <bezier.h>
#include <shape.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
//    void EscPressed();
//    void MousePressed();
//    void mouseReleased();
private slots:
    //void on_pushButton_clicked();
    void choose_shape();
    void draw_line();
    void draw_Bezier();
    void clear_screen();
private:
    Ui::MainWindow *ui;
    QLabel *pos;
    QRect border;
    bool flag1,flag2,flag3;
    bool flag4; //记录贝塞尔曲线是否被靠近
    int order;

    //dates
    QPoint p1,p2,c1,c2;
    Shape currentShape;
    Bezier currentBezier;
    QVector<Shape > shapes;
    QVector<QLine> lines;
    QVector<Bezier> bezierLines;

    //states
    enum DrawStates{Choose,Line,BezierCurve};
    enum BezierStates{Bezier1,Bezier2,Bezier3,Bezier4};
    enum MoveStates{None,Move,Resize,Rotate};
    DrawStates currentDrawState;
    MoveStates currentMoveState;
    BezierStates currentBezierState;

    QAction *drawLine,*drawBezier,*toPointer,*clear;
    //QPainterPath path;

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H