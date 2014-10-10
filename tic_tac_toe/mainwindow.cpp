#include "mainwindow.h"
#define DEBUG

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), player(PLAYER_NONE), state(STATE_HOLD)
{
    resize(480,320);
    init();
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    for(int i = 0; i <= MAX_SIZE; ++i)
    {
        p.drawLine(10 + i * 100, 10, 10 + i * 100, 310);
        p.drawLine(10, 10 + i * 100, 310, 10 + i * 100);
    }

    QBrush b;
    b.setStyle(Qt::SolidPattern);

    for(int i = 0; i < MAX_SIZE; ++i)
    {
        for(int j = 0; j < MAX_SIZE; ++j)
        {
            if(map[i][j] == PLAYER_NONE)
            {
                continue;
            }
            else if(map[i][j] == PLAYER_C)
            {
                if(PLAYER_C_COLOR == COLOR_B)
                {
                    b.setColor(Qt::black);
                }else if(PLAYER_C_COLOR == COLOR_W){
                    b.setColor(Qt::white);
                }else{
                    continue;
                }
            }else{
                if(PLAYER_H_COLOR == COLOR_B)
                {
                    b.setColor(Qt::black);
                }else if (PLAYER_H_COLOR == COLOR_W){
                    b.setColor(Qt::white);
                }else{
                    continue;
                }
            }
            p.setBrush(b);
            p.drawEllipse(point2pos(QPoint(i, j)), 45, 45);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint p = event->pos();
    if(state == STATE_PLAY) addPoint(pos2point(p));
    p = AI_check();
    if(p.x() != -1 && p.y() != -1 && state == STATE_WAIT) addPoint(p);
}

QPoint MainWindow::point2pos(QPoint p) const
{
    return QPoint(10 + p.x() * 100 + 50, 10 + p.y() * 100 + 50);
}

QPoint MainWindow::pos2point(QPoint p) const
{
    return QPoint((p.x() - 10) / 100, (p.y() - 10) / 100);
}

void MainWindow::addPoint(QPoint p)
{
    if(map[p.x()][p.y()] == PLAYER_NONE)
    {
       map[p.x()][p.y()] = player;
       int result = check();
       if(result != PLAYER_NONE)
       {
            QMessageBox::information(0,"","end");
            state = STATE_HOLD;
            return;
       }
       changePlayer();
       changeState();
    }
    update();

}

 void MainWindow::init()
 {
    memset(map, PLAYER_NONE, sizeof(map));
    player = PLAYER_NONE;
    state = STATE_HOLD;
    PLAYER_C_COLOR = PLAYER_H_COLOR = COLOR_NONE;

    AI_HOW[0][0] = AI_HOW[2][2] = AI_HOW[0][2] = AI_HOW[2][0] = 3;
    AI_HOW[0][1] = AI_HOW[1][0] = AI_HOW[1][2] = AI_HOW[2][1] = 2;
    AI_HOW[1][1] = 4;

/////////////////////////////////
#ifdef DEBUG
    player = PLAYER_H;
    PLAYER_C_COLOR = COLOR_W;
    PLAYER_H_COLOR = COLOR_B;
    state = STATE_PLAY;

#else
    player = PLAYER_C;
    PLAYER_C_COLOR = COLOR_B;
    PLAYER_H_COLOR = COLOR_W;
    state = STATE_WAIT;

    QPoint p = AI_check();
    addPoint(p);

#endif
}

void MainWindow::changeState()
{
    if(state == STATE_PLAY)
    {
        state = STATE_WAIT;
    }else if(state == STATE_WAIT){
        state = STATE_PLAY;
    }
}

void MainWindow::changePlayer()
{
    if(player == PLAYER_C)
    {
        player = PLAYER_H;
    }else if(player == PLAYER_H){
        player = PLAYER_C;
    }
}

int MainWindow::check()
{
    int num = 0;
    for(int i = 0; i < MAX_SIZE; ++i)
    {
        for(int j = 0; j < MAX_SIZE; ++j)
        {
            if(map[i][j] == PLAYER_NONE) ++num;
        }
    }
    if(num == 0){
        if(PLAYER_C_COLOR == COLOR_W)
        {
            return PLAYER_C;
        }else{
            return PLAYER_H;
        }
    }
   int color = map[1][1];
   if(color == COLOR_NONE) return PLAYER_NONE;
   if(color == map[0][0] && color == map[2][2]) return player;
   if(color == map[2][0] && color == map[0][2]) return player;
   if(color == map[1][0] && color == map[1][2]) return player;
   if(color == map[0][1] && color == map[2][1]) return player;
   color = map[0][0];
   if(color == COLOR_NONE) return PLAYER_NONE;
   if(color == map[1][0] && color == map[2][0]) return player;
   if(color == map[0][1] && color == map[0][2]) return player;
   color = map[2][2];
   if(color == COLOR_NONE) return PLAYER_NONE;
   if(color == map[0][2] && color == map[1][2]) return player;
   if(color == map[2][0] && color == map[2][1]) return player;
   return PLAYER_NONE;
}

QPoint MainWindow::AI_check()
{
    memset(AI_CALC,0,sizeof(AI_CALC));
    for(int i = 0; i < MAX_SIZE; ++i)
    {
        for(int j = 0; j < MAX_SIZE; ++j)
        {
           if(map[i][j] != PLAYER_NONE)
           {
                continue;
           }else{

                if((map[0][j] + map[1][j] + map[2][j]) == PLAYER_C * 2 && (map[0][j] * map[1][j] * map[2][j]) == 0 && ((map[0][j] - 1) * (map[1][j] -1) * (map[2][j] - 1)) == -1)
                    AI_CALC[i][j] = AI_HOW[i][j] + AI_ACT + AI_CALC[i][j];
                if((map[i][0] + map[i][1] + map[i][2]) == PLAYER_C * 2 && (map[i][0] * map[i][1] * map[i][2]) == 0 && ((map[i][0] - 1) * (map[i][1] -1) * (map[i][2] - 1)) == -1)
                    AI_CALC[i][j] = AI_HOW[i][j] + AI_ACT + AI_CALC[i][j];

                if((map[0][j] + map[1][j] + map[2][j]) == PLAYER_H * 2 && (map[0][j] * map[1][j] * map[2][j]) == 0 && ((map[0][j] - 1) * (map[1][j] -1) * (map[2][j] - 1)) == 0)
                    AI_CALC[i][j] = AI_HOW[i][j] + AI_DEF + AI_CALC[i][j];
                if((map[i][0] + map[i][1] + map[i][2]) == PLAYER_H * 2 && (map[i][0] * map[i][1] * map[i][2]) == 0 && ((map[i][0] - 1) * (map[i][1] -1) * (map[i][2] - 1)) == 0)
                    AI_CALC[i][j] = AI_HOW[i][j] + AI_DEF + AI_CALC[i][j];

                if((map[0][j] + map[1][j] + map[2][j]) == PLAYER_H && (map[0][j] * map[1][j] * map[2][j]) == 0 && ((map[0][j] - 1) * (map[1][j] -1) * (map[2][j] - 1)) == 0)
                    AI_CALC[i][j] = AI_HOW[i][j] + AI_H_O + AI_CALC[i][j];
                if((map[i][0] + map[i][1] + map[i][2]) == PLAYER_H && (map[i][0] * map[i][1] * map[i][2]) == 0 && ((map[i][0] - 1) * (map[i][1] -1) * (map[i][2] - 1)) == 0)
                    AI_CALC[i][j] = AI_HOW[i][j] + AI_H_O + AI_CALC[i][j];

                if((map[0][j] + map[1][j] + map[2][j]) == PLAYER_C && (map[0][j] * map[1][j] * map[2][j]) == 0 && ((map[0][j] - 1) * (map[1][j] -1) * (map[2][j] - 1)) == 1)
                    AI_CALC[i][j] = AI_HOW[i][j] + AI_C_O + AI_CALC[i][j];
                if((map[i][0] + map[i][1] + map[i][2]) == PLAYER_C && (map[i][0] * map[i][1] * map[i][2]) == 0 && ((map[i][0] - 1) * (map[i][1] -1) * (map[i][2] - 1)) == 1)
                    AI_CALC[i][j] = AI_HOW[i][j] + AI_C_O + AI_CALC[i][j];

                if((map[0][j] + map[1][j] + map[2][j]) == PLAYER_NONE && (map[0][j] * map[1][j] * map[2][j]) == 0 && ((map[0][j] - 1) * (map[1][j] -1) * (map[2][j] - 1)) == -1)
                    AI_CALC[i][j] = AI_HOW[i][j] + AI_N + AI_CALC[i][j];
                if((map[i][0] + map[i][1] + map[i][2]) == PLAYER_NONE && (map[i][0] * map[i][1] * map[i][2]) == 0 && ((map[i][0] - 1) * (map[i][1] -1) * (map[i][2] - 1)) == -1)
                    AI_CALC[i][j] = AI_HOW[i][j] + AI_N + AI_CALC[i][j];



                ///////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////

                if((i == 0 && j == 0) || (i == 1 && j == 1) || (i == 2 && j == 2))
                {
                    if((map[0][0] + map[1][1] + map[2][2]) == PLAYER_C * 2 && (map[0][0] * map[1][1] * map[2][2]) == 0 && ((map[0][0] - 1) * (map[1][1] -1) * (map[2][2] - 1)) == -1)
                        AI_CALC[i][j] = AI_HOW[i][j] + AI_ACT + AI_CALC[i][j];

                    if((map[0][0] + map[1][1] + map[2][2]) == PLAYER_H * 2 && (map[0][0] * map[1][1] * map[2][2]) == 0 && ((map[0][0] - 1) * (map[1][1] -1) * (map[2][2] - 1)) == 0)
                        AI_CALC[i][j] = AI_HOW[i][j] + AI_DEF + AI_CALC[i][j];

                    if((map[0][0] + map[1][1] + map[2][2]) == PLAYER_H && (map[0][0] * map[1][1] * map[2][2]) == 0 && ((map[0][0] - 1) * (map[1][1] -1) * (map[2][2] - 1)) == 0)
                        AI_CALC[i][j] = AI_HOW[i][j] + AI_H_O + AI_CALC[i][j];

                    if((map[0][0] + map[1][1] + map[2][2]) == PLAYER_C && (map[0][0] * map[1][1] * map[2][2]) == 0 && ((map[0][0] - 1) * (map[1][1] -1) * (map[2][2] - 1)) == 1)
                        AI_CALC[i][j] = AI_HOW[i][j] + AI_C_O + AI_CALC[i][j];

                    if((map[0][0] + map[1][1] + map[2][2]) == PLAYER_NONE && (map[0][0] * map[1][1] * map[2][2]) == 0 && ((map[0][0] - 1) * (map[1][1] -1) * (map[2][2] - 1)) == -1)
                        AI_CALC[i][j] = AI_HOW[i][j] + AI_N + AI_CALC[i][j];

                }

                ////////////////////////////////////////////////////////////////////
                ////////////////////////////////////////////////////////////////////

                if((i == 2 && j == 0) || (i == 1 && j == 1) || (i == 0 && j == 2))
                {
                    if((map[2][0] + map[1][1] + map[0][2]) == PLAYER_C * 2 && (map[2][0] * map[1][1] * map[0][2]) == 0 && ((map[2][0] - 1) * (map[1][1] -1) * (map[0][2] - 1)) == -1)
                        AI_CALC[i][j] = AI_HOW[i][j] + AI_ACT + AI_CALC[i][j];

                    if((map[2][0] + map[1][1] + map[0][2]) == PLAYER_H * 2 && (map[2][0] * map[1][1] * map[0][2]) == 0 && ((map[2][0] - 1) * (map[1][1] -1) * (map[0][2] - 1)) == 0)
                        AI_CALC[i][j] = AI_HOW[i][j] + AI_DEF + AI_CALC[i][j];

                    if((map[2][0] + map[1][1] + map[0][2]) == PLAYER_H && (map[2][0] * map[1][1] * map[0][2]) == 0 && ((map[2][0] - 1) * (map[1][1] -1) * (map[0][2] - 1)) == 0)
                        AI_CALC[i][j] = AI_HOW[i][j] + AI_H_O + AI_CALC[i][j];

                    if((map[2][0] + map[1][1] + map[0][2]) == PLAYER_C && (map[2][0] * map[1][1] * map[0][2]) == 0 && ((map[2][0] - 1) * (map[1][1] -1) * (map[0][2] - 1)) == 1)
                        AI_CALC[i][j] = AI_HOW[i][j] + AI_C_O + AI_CALC[i][j];

                    if((map[2][0] + map[1][1] + map[0][2]) == PLAYER_NONE && (map[2][0] * map[1][1] * map[0][2]) == 0 && ((map[2][0] - 1) * (map[1][1] -1) * (map[0][2] - 1)) == -1)
                        AI_CALC[i][j] = AI_HOW[i][j] + AI_N + AI_CALC[i][j];

                }

           }
        }
    }
    int m_max = -1;
    int maxx = -1;
    int maxy = -1;
    for(int i = 0; i < MAX_SIZE; ++i)
    {
        for(int j = 0; j < MAX_SIZE; ++j)
        {
            if(map[i][j] == PLAYER_NONE && m_max < AI_CALC[i][j])
            {
                m_max = AI_CALC[i][j];
                maxx = i;
                maxy = j;
            }
        }
    }
    return QPoint(maxx, maxy);
}
