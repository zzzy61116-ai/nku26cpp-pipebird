#include "gamewidget.h"

#include <QPainter>
#include <QKeyEvent>
#include <QDateTime>
#include <QLinearGradient>
#include <QPen>
#include <algorithm>
#include <cmath>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent),
      m_settings("PipeBirdQt", "PipeBirdQt"),
      m_selectedDifficulty(1),
      m_started(false),
      m_gameOver(false),
      m_score(0),
      m_bestScore(m_settings.value("bestScore", 0).toInt()),
      m_spawnCounter(0),
      m_frameCounter(0),
      m_width(480),
      m_height(720)
{
    setFixedSize(m_width, m_height);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_OpaquePaintEvent);

    m_difficulties = difficultyList();
    m_currentDifficulty = m_difficulties[m_selectedDifficulty];
    m_bird.setGravity(m_currentDifficulty.gravity);

    m_jumpSound.setSource(QUrl("qrc:/assets/sounds/jump.wav"));
    m_scoreSound.setSource(QUrl("qrc:/assets/sounds/score.wav"));
    m_hitSound.setSource(QUrl("qrc:/assets/sounds/hit.wav"));
    m_clickSound.setSource(QUrl("qrc:/assets/sounds/click.wav"));

    m_jumpSound.setVolume(0.35f);
    m_scoreSound.setVolume(0.35f);
    m_hitSound.setVolume(0.45f);
    m_clickSound.setVolume(0.20f);

    connect(&m_timer, &QTimer::timeout, this, &GameWidget::updateGame);
    m_timer.start(16);
}

int GameWidget::bestScore() const
{
    return m_bestScore;
}

void GameWidget::startGame(int difficultyIndex)
{
    playClick();

    if (difficultyIndex >= 0 && difficultyIndex < m_difficulties.size()) {
        m_selectedDifficulty = difficultyIndex;
    }

    m_currentDifficulty = m_difficulties[m_selectedDifficulty];
    m_bird.setGravity(m_currentDifficulty.gravity);

    resetRound();
    m_started = false;
    m_gameOver = false;
    update();
}

void GameWidget::resetRound()
{
    m_bird.reset();
    m_bird.setGravity(m_currentDifficulty.gravity);
    m_pipes.clear();
    m_coins.clear();
    m_score = 0;
    m_spawnCounter = 0;
    m_frameCounter = 0;
}

void GameWidget::spawnCoinForPipe(const Pipe &pipe)
{
    Coin coin;
    // 让金币出现在管道缺口内，并且从屏幕右侧进来时就能看见
    coin.x = m_width + 120.0;

    const double topH = pipe.topRect().height();
    const double minY = topH + 24.0;
    const double maxY = topH + m_currentDifficulty.gapHeight - 24.0;

    coin.y = topH + m_currentDifficulty.gapHeight * 0.5;
    coin.y += QRandomGenerator::global()->bounded(-20, 21);

    if (coin.y < minY) coin.y = minY;
    if (coin.y > maxY) coin.y = maxY;

    coin.speed = m_currentDifficulty.pipeSpeed;
    m_coins.append(coin);
}

void GameWidget::spawnPipe()
{
    const int groundHeight = 90;
    const int minGapY = 110;
    const int maxGapY = qMax(minGapY + 1, m_height - groundHeight - m_currentDifficulty.gapHeight - 70);

    const int gapY = QRandomGenerator::global()->bounded(minGapY, maxGapY + 1);
    m_pipes.append(Pipe(m_width + 20, gapY, m_currentDifficulty.gapHeight, 78, m_currentDifficulty.pipeSpeed));

    if (QRandomGenerator::global()->bounded(100) < 75) {
        spawnCoinForPipe(m_pipes.last());
    }
}

bool GameWidget::isColliding() const
{
    if (m_bird.y() < -10.0 || m_bird.y() > m_height - 90.0) {
        return true;
    }

    const QRectF birdRect = m_bird.rect();
    for (const Pipe &pipe : m_pipes) {
        if (birdRect.intersects(pipe.topRect()) || birdRect.intersects(pipe.bottomRect())) {
            return true;
        }
    }
    return false;
}

void GameWidget::finishGame()
{
    if (!m_gameOver) {
        m_gameOver = true;
        playHit();

        if (m_score > m_bestScore) {
            m_bestScore = m_score;
            m_settings.setValue("bestScore", m_bestScore);
            emit bestScoreChanged(m_bestScore);
        }
    }
}

void GameWidget::playJump()  { m_jumpSound.play(); }
void GameWidget::playScore() { m_scoreSound.play(); }
void GameWidget::playHit()   { m_hitSound.play(); }
void GameWidget::playClick()  { m_clickSound.play(); }

void GameWidget::updateGame()
{
    const double dt = 0.016;

    if (!m_started || m_gameOver) {
        update();
        return;
    }

    m_bird.update(dt);

    for (Pipe &pipe : m_pipes) {
        pipe.update(dt);
    }

    const QRectF birdRect = m_bird.rect();
    for (Coin &coin : m_coins) {
        coin.pulse += dt * 5.0;
        coin.x -= coin.speed * dt;
        const double bob = std::sin(coin.pulse) * 2.5;
        QRectF coinRect(coin.x - coin.radius, coin.y + bob - coin.radius,
                        coin.radius * 2.0, coin.radius * 2.0);

        if (!coin.collected && birdRect.intersects(coinRect)) {
            coin.collected = true;
            ++m_score;
            playScore();
        }
    }

    for (Pipe &pipe : m_pipes) {
        if (pipe.scored(130.0)) {
            ++m_score;
            playScore();
        }
    }

    m_pipes.erase(std::remove_if(m_pipes.begin(), m_pipes.end(),
                                 [](const Pipe &pipe) { return pipe.offScreen(); }),
                  m_pipes.end());

    m_coins.erase(std::remove_if(m_coins.begin(), m_coins.end(),
                                 [](const Coin &coin) { return coin.collected || coin.x < -40.0; }),
                  m_coins.end());

    ++m_spawnCounter;
    ++m_frameCounter;

    if (m_spawnCounter >= m_currentDifficulty.spawnIntervalFrames) {
        spawnPipe();
        m_spawnCounter = 0;
    }

    if (isColliding()) {
        finishGame();
    }

    update();
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    const int key = event->key();

    if (key == Qt::Key_Escape || key == Qt::Key_M) {
        playClick();
        emit backToMenuRequested();
        return;
    }

    if (key == Qt::Key_R && m_gameOver) {
        playClick();
        startGame(m_selectedDifficulty);
        m_started = true;
        return;
    }

    const bool flapKey = (key == Qt::Key_Space || key == Qt::Key_W || key == Qt::Key_Up);

    if (flapKey) {
        if (!m_started && !m_gameOver) {
            m_started = true;
            spawnPipe();
            playJump();
            m_bird.flap(m_currentDifficulty.flapStrength);
            return;
        }

        if (m_started && !m_gameOver) {
            playJump();
            m_bird.flap(m_currentDifficulty.flapStrength);
            return;
        }

        if (m_gameOver) {
            playClick();
            startGame(m_selectedDifficulty);
            m_started = true;
            return;
        }
    }

    QWidget::keyPressEvent(event);
}

void GameWidget::drawBackground(QPainter &painter)
{
    QLinearGradient sky(0, 0, 0, m_height);
    sky.setColorAt(0.0, QColor(67, 185, 255));
    sky.setColorAt(0.55, QColor(132, 228, 255));
    sky.setColorAt(1.0, QColor(233, 249, 255));

    painter.fillRect(rect(), sky);

    painter.setPen(Qt::NoPen);

    painter.setBrush(QColor(255, 235, 140));
    painter.drawEllipse(QPointF(390, 100), 52, 52);

    painter.setBrush(QColor(255, 255, 255, 190));
    painter.drawEllipse(QPointF(90, 120), 38, 24);
    painter.drawEllipse(QPointF(122, 108), 46, 30);
    painter.drawEllipse(QPointF(158, 120), 34, 20);

    painter.drawEllipse(QPointF(250, 75), 30, 18);
    painter.drawEllipse(QPointF(278, 66), 42, 26);
    painter.drawEllipse(QPointF(315, 76), 28, 18);

    painter.setBrush(QColor(114, 194, 122));
    QPolygon leftMountain;
    leftMountain << QPoint(0, 650) << QPoint(120, 380) << QPoint(250, 650);
    painter.drawPolygon(leftMountain);

    QPolygon rightMountain;
    rightMountain << QPoint(170, 650) << QPoint(350, 330) << QPoint(520, 650);
    painter.drawPolygon(rightMountain);
}

void GameWidget::drawGround(QPainter &painter)
{
    const int groundH = 90;
    QRect groundRect(0, m_height - groundH, m_width, groundH);
    painter.fillRect(groundRect, QColor(220, 190, 120));
    painter.fillRect(QRect(0, m_height - groundH, m_width, 14), QColor(168, 128, 70));

    painter.setPen(QPen(QColor(180, 145, 84), 2));
    for (int x = 0; x < m_width; x += 28) {
        painter.drawLine(x, m_height - 30, x + 14, m_height - 16);
    }
}

void GameWidget::drawCoin(QPainter &painter, const Coin &coin) const
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    const double bob = std::sin(coin.pulse) * 2.5;
    const double glow = 1.0 + std::sin(coin.pulse * 1.5) * 0.16;

    painter.translate(coin.x, coin.y + bob);
    painter.scale(glow, glow);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 236, 120, 80));
    painter.drawEllipse(QPointF(0, 0), coin.radius * 2.1, coin.radius * 2.1);

    QLinearGradient gold(-coin.radius, -coin.radius, coin.radius, coin.radius);
    gold.setColorAt(0.0, QColor(255, 250, 170));
    gold.setColorAt(0.45, QColor(255, 217, 70));
    gold.setColorAt(1.0, QColor(216, 156, 25));

    painter.setBrush(gold);
    painter.setPen(QPen(QColor(160, 110, 15), 1.2));
    painter.drawEllipse(QPointF(0, 0), coin.radius + 1.0, coin.radius + 1.0);

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(255, 245, 200), 1.2));
    painter.drawEllipse(QPointF(0, 0), coin.radius * 0.58, coin.radius * 0.58);

    QFont f("Microsoft YaHei UI", 11, QFont::Bold);
    painter.setFont(f);
    painter.setPen(QColor(120, 85, 20));
    painter.drawText(QRectF(-9, -11, 18, 22), Qt::AlignCenter, "¥");

    painter.restore();
}

void GameWidget::drawCoins(QPainter &painter)
{
    for (const Coin &coin : m_coins) {
        if (!coin.collected) {
            drawCoin(painter, coin);
        }
    }
}

void GameWidget::drawHud(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255, 125));
    painter.drawRoundedRect(QRect(122, 18, 236, 54), 18, 18);

    QFont scoreFont("Microsoft YaHei UI", 20, QFont::Bold);
    QFont infoFont("Microsoft YaHei UI", 11, QFont::Bold);

    painter.setPen(Qt::white);
    painter.setFont(scoreFont);
    painter.drawText(QRect(0, 24, m_width, 34), Qt::AlignCenter, QString("分数 %1").arg(m_score));

    painter.setFont(infoFont);
    painter.drawText(16, 28, QString("最高分：%1").arg(m_bestScore));
    painter.drawText(m_width - 120, 28, QString("难度：%1").arg(m_currentDifficulty.name));

    if (!m_started && !m_gameOver) {
        painter.setBrush(QColor(0, 0, 0, 95));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(QRect(56, 240, 368, 190), 26, 26);

        painter.setPen(Qt::white);
        QFont title("Microsoft YaHei UI", 30, QFont::Bold);
        painter.setFont(title);
        painter.drawText(QRect(0, 274, m_width, 40), Qt::AlignCenter, "管道鸟 Pro");

        QFont text("Microsoft YaHei UI", 13);
        painter.setFont(text);
        painter.drawText(QRect(0, 332, m_width, 78), Qt::AlignCenter,
                         "按 空格键 开始飞行\n躲避管道，冲击最高分");
    }

    if (m_gameOver) {
        painter.setBrush(QColor(0, 0, 0, 110));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(QRect(58, 238, 364, 194), 26, 26);

        painter.setPen(Qt::white);
        QFont over("Microsoft YaHei UI", 28, QFont::Bold);
        painter.setFont(over);
        painter.drawText(QRect(0, 270, m_width, 42), Qt::AlignCenter, "游戏结束");

        QFont text("Microsoft YaHei UI", 14);
        painter.setFont(text);
        painter.drawText(QRect(0, 328, m_width, 76), Qt::AlignCenter,
                         QString("最终得分：%1\n按 R 重新开始 或 按 M 返回菜单").arg(m_score));
    }
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    drawBackground(painter);

    for (const Pipe &pipe : m_pipes) {
        pipe.draw(painter);
    }

    drawCoins(painter);
    m_bird.draw(painter);
    drawGround(painter);
    drawHud(painter);
}
