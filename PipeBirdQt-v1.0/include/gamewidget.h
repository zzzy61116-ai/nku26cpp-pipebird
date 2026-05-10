#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>

#include "bird.h"
#include "pipe.h"

class GameWidget : public QWidget
{
public:
    GameWidget();

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *) override;

private:
    void updateGame();
    void spawnPipe();
    bool collision();

private:
    Bird bird;
    QVector<Pipe> pipes;
    QTimer timer;

    bool started;
    bool gameOver;

    int score;
    int bestScore;
    int spawnCounter;
};

#endif
