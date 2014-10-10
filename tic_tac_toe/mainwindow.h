#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPainter>
#include <QMessageBox>
#include <QMainWindow>
#include <QMouseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int MAX_SIZE = 3;
    static const int PLAYER_NONE = 0;
    static const int PLAYER_H = 1;
    static const int PLAYER_C = 2;
    static const int COLOR_NONE = 0;
    static const int COLOR_W = 1;
    static const int COLOR_B = 2;
    static const int STATE_WAIT = 0;
    static const int STATE_PLAY = 1;
    static const int STATE_HOLD = 2;

    static const int AI_ACT = 10000;
    static const int AI_DEF = 1000;
    static const int AI_H_O = 100;
    static const int AI_C_O = 10;
    static const int AI_N = 0;


    int PLAYER_H_COLOR;
    int PLAYER_C_COLOR;
    int state;
    int player;

    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPoint point2pos(QPoint p) const;
    QPoint pos2point(QPoint p) const;
    void addPoint(QPoint p);
    void changeState();
    void changePlayer();
    int check();
    QPoint AI_check();
public slots:
    void mousePressEvent(QMouseEvent *event);
protected:
    void paintEvent(QPaintEvent *event);
private:
    void init();
    int map[MAX_SIZE][MAX_SIZE];
    int AI_HOW[MAX_SIZE][MAX_SIZE];
    int AI_CALC[MAX_SIZE][MAX_SIZE];

};

#endif // MAINWINDOW_H
