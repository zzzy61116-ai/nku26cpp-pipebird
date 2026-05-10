#include "pipe.h"

#include <QLinearGradient>

Pipe::Pipe(double x, double gapY, int gapHeight, double width, double speed)
    : m_x(x),
      m_gapY(gapY),
      m_gapHeight(gapHeight),
      m_width(width),
      m_speed(speed),
      m_counted(false)
{
}

void Pipe::update(double dt)
{
    m_x -= m_speed * dt;
}

void Pipe::draw(QPainter &painter) const
{
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient pipeGrad(m_x, 0, m_x + m_width, 0);
    pipeGrad.setColorAt(0.0, QColor(85, 205, 100));
    pipeGrad.setColorAt(0.55, QColor(55, 165, 70));
    pipeGrad.setColorAt(1.0, QColor(40, 125, 55));

    QBrush pipeBrush(pipeGrad);
    painter.setPen(Qt::NoPen);
    painter.setBrush(pipeBrush);

    QRectF top = topRect();
    QRectF bottom = bottomRect();

    painter.drawRoundedRect(top.adjusted(0, 0, 0, -2), 10, 10);
    painter.drawRoundedRect(bottom.adjusted(0, 2, 0, 0), 10, 10);

    painter.setBrush(QColor(35, 110, 48));
    painter.drawRect(QRectF(m_x - 4, top.bottom() - 20, m_width + 8, 20));
    painter.drawRect(QRectF(m_x - 4, bottom.top(), m_width + 8, 20));

    painter.restore();
}

QRectF Pipe::topRect() const
{
    return QRectF(m_x, 0.0, m_width, m_gapY);
}

QRectF Pipe::bottomRect() const
{
    return QRectF(m_x, m_gapY + m_gapHeight, m_width, 720.0);
}

bool Pipe::offScreen() const
{
    return m_x + m_width < 0.0;
}

bool Pipe::scored(double birdX)
{
    if (!m_counted && m_x + m_width < birdX) {
        m_counted = true;
        return true;
    }
    return false;
}

double Pipe::right() const
{
    return m_x + m_width;
}
