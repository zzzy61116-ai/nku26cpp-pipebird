#include "gamewidget.h"

#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <algorithm>

GameWidget::GameWidget()
{
    setFixedSize(480,720);

    started = false;
    gameOver = false;

    score = 0;
    bestScore = 0;

    spawnCounter = 0;

    connect(&timer,&QTimer::timeout,this,&GameWidget::updateGame);

    timer.start(16);
}

void GameWidget::spawnPipe()
{
    int gapY = QRandomGenerator::global()->bounded(120,420);

    pipes.append(Pipe(width()+20,gapY,180));
}

bool GameWidget::collision()
{
    QRectF birdRect = bird.rect();

    if (bird.y() > height()-90 || bird.y() < 0)
        return true;

    for (auto &pipe : pipes)
    {
        if (birdRect.intersects(pipe.topRect()) ||
            birdRect.intersects(pipe.bottomRect()))
            return true;
    }

    return false;
}

void GameWidget::updateGame()
{
    if (started && !gameOver)
    {
        bird.update(0.016);

        for (auto &pipe : pipes)
            pipe.update(0.016);

        for (auto &pipe : pipes)
        {
            if (!pipe.counted && pipe.right() < 120)
            {
                pipe.counted = true;
                score++;

                if (score > bestScore)
                    bestScore = score;
            }
        }

        pipes.erase(
            std::remove_if(
                pipes.begin(),
                pipes.end(),
                [](Pipe &p){ return p.offScreen(); }),
            pipes.end());

        spawnCounter++;

        if (spawnCounter >= 90)
        {
            spawnPipe();
            spawnCounter = 0;
        }

        if (collision())
            gameOver = true;
    }

    update();
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
    {
        if (!started)
            started = true;

        if (gameOver)
        {
            pipes.clear();
            bird.reset();
            score = 0;
            gameOver = false;
        }

        bird.flap();
    }

    if (event->key() == Qt::Key_R)
    {
        pipes.clear();
        bird.reset();
        score = 0;
        gameOver = false;
        started = false;
    }
}

void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient sky(0,0,0,height());

    sky.setColorAt(0,QColor(60,180,255));
    sky.setColorAt(1,QColor(230,245,255));

    painter.fillRect(rect(),sky);

    painter.setPen(Qt::NoPen);

    painter.setBrush(QColor(255,240,120));
    painter.drawEllipse(QPointF(390,110),55,55);

    painter.setBrush(QColor(255,255,255,180));

    painter.drawEllipse(QPointF(100,110),50,25);
    painter.drawEllipse(QPointF(140,100),60,32);
    painter.drawEllipse(QPointF(180,110),45,22);

    painter.setBrush(QColor(110,190,120));

    QPolygon mountain;
    mountain << QPoint(0,620)
             << QPoint(120,360)
             << QPoint(260,620);

    painter.drawPolygon(mountain);

    QPolygon mountain2;
    mountain2 << QPoint(180,620)
              << QPoint(340,300)
              << QPoint(520,620);

    painter.drawPolygon(mountain2);

    for (auto &pipe : pipes)
        pipe.draw(painter);

    bird.draw(painter);

    painter.fillRect(QRect(0,height()-90,width(),90),
                     QColor(210,180,100));

    painter.fillRect(QRect(0,height()-90,width(),12),
                     QColor(150,110,60));

    painter.setBrush(QColor(255,255,255,120));
    painter.drawRoundedRect(QRect(125,18,230,55),18,18);

    painter.setPen(Qt::white);

    QFont scoreFont("Arial",20,QFont::Bold);
    painter.setFont(scoreFont);

    painter.drawText(QRect(0,28,width(),30),
                     Qt::AlignCenter,
                     QString("SCORE %1").arg(score));

    QFont small("Arial",11,QFont::Bold);
    painter.setFont(small);

    painter.drawText(16,28,
                     QString("BEST: %1").arg(bestScore));

    if (!started)
    {
        painter.setBrush(QColor(0,0,0,100));
        painter.setPen(Qt::NoPen);

        painter.drawRoundedRect(QRect(70,240,340,180),25,25);

        painter.setPen(Qt::white);

        QFont title("Arial",30,QFont::Bold);
        painter.setFont(title);

        painter.drawText(QRect(0,275,width(),40),
                         Qt::AlignCenter,
                         "PIPE BIRD");

        QFont info("Arial",13);
        painter.setFont(info);

        painter.drawText(QRect(0,335,width(),70),
                         Qt::AlignCenter,
                         "Press SPACE to Start");
    }

    if (gameOver)
    {
        painter.setBrush(QColor(0,0,0,110));
        painter.setPen(Qt::NoPen);

        painter.drawRoundedRect(QRect(80,250,320,170),25,25);

        painter.setPen(Qt::white);

        QFont over("Arial",28,QFont::Bold);
        painter.setFont(over);

        painter.drawText(QRect(0,285,width(),40),
                         Qt::AlignCenter,
                         "GAME OVER");

        QFont text("Arial",14);
        painter.setFont(text);

        painter.drawText(QRect(0,340,width(),60),
                         Qt::AlignCenter,
                         QString("Final Score: %1\nPress SPACE Again")
                         .arg(score));
    }
}
