#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QSettings>
#include <QSoundEffect>
#include <QRandomGenerator>
#include "bird.h"
#include "pipe.h"
#include "difficulty.h"

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);

    void startGame(int difficultyIndex);
    int bestScore() const;

signals:
    void backToMenuRequested();
    void bestScoreChanged(int score);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateGame();

private:
    struct Coin
    {
        double x = 0.0;
        double y = 0.0;
        double radius = 11.0;
        bool collected = false;
        double pulse = 0.0;
        double speed = 0.0;
    };

private:
    void resetRound();
    void spawnPipe();
    void spawnCoinForPipe(const Pipe &pipe);
    bool isColliding() const;
    void finishGame();
    void playJump();
    void playScore();
    void playHit();
    void playClick();
    void drawBackground(QPainter &painter);
    void drawGround(QPainter &painter);
    void drawHud(QPainter &painter);
    void drawCoins(QPainter &painter);
    void drawCoin(QPainter &painter, const Coin &coin) const;

private:
    Bird m_bird;
    QVector<Pipe> m_pipes;
    QVector<Coin> m_coins;
    QTimer m_timer;
    QSettings m_settings;

    QSoundEffect m_jumpSound;
    QSoundEffect m_scoreSound;
    QSoundEffect m_hitSound;
    QSoundEffect m_clickSound;

    QVector<DifficultyConfig> m_difficulties;
    int m_selectedDifficulty;
    DifficultyConfig m_currentDifficulty;

    bool m_started;
    bool m_gameOver;
    int m_score;
    int m_bestScore;
    int m_spawnCounter;
    int m_frameCounter;
    int m_width;
    int m_height;
};

#endif
