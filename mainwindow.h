#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPoint>
#include <QLine>
#include <QPalette>
#include <QComboBox>
#include <QLineEdit>
#include <QStateMachine>
#include <QState>
#include <bezier.h>
#include <shape.h>
#include <multishapes.h>
#define RECT_SIZE 8
#define CENT_SIZE 8

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
//    void on_pushButton_clicked();
    void choose_shape();
    void draw_line();
    void draw_Bezier();
    void clear_screen();
private:
    Ui::MainWindow *ui;
    QLabel *pos;

    //dates
    QRect border;
    int closeBorder;    //determine which corner the mouse is closing
    Bezier *currentBezier;
    QPoint p1,p2,c1,c2,p3,p4,start,end,center;   //keep the temporary data
    Shape *currentShape,*currentChooseShape;
    Multishapes currentChooseShapes;
    QVector<Shape *> shapes;
    QVector<Bezier *> bezierLines;

    //states
    enum ChooseStates{Unchoose,ResizeChoose,RotateChoose};
    enum DrawStates{Pointer,Line,BezierCurve};
    enum BezierStates{Bezier1,Bezier2,Bezier3,Bezier4};
    enum MoveStates{None,Close1,Close2,Move,Choose1,Choose2,Escape1,Escape2,Resize,Shear,Rotate,MoveCenter};
    DrawStates currentDrawState;
    MoveStates currentMoveState;
    ChooseStates currentChooseState;
    BezierStates currentBezierState;

    QAction *drawLine,*drawBezier,*toPointer,*clear;
    QComboBox *cbMode;
    QLineEdit *iterTimes;
    QLabel *label;

    void paintBorder(QPainter &paint, QRect &border);
    void paintCenter(QPainter &p, QPoint &center);
    int getClosePosition(QPoint p);
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
