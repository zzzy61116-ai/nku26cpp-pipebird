#include "pipe.h"

Pipe::Pipe(double x,double gapY,double gapHeight)
{
    m_x = x;
    m_gapY = gapY;
    m_gapHeight = gapHeight;
    counted = false;
}

void Pipe::update(double dt)
{
    m_x -= 220 * dt;
}

void Pipe::draw(QPainter &painter) const
{
    painter.setPen(Qt::NoPen);

    painter.setBrush(QColor(76,185,80));

    painter.drawRect(topRect());
    painter.drawRect(bottomRect());

    painter.setBrush(QColor(50,140,55));

    painter.drawRect(QRectF(m_x,topRect().bottom()-16,80,16));
    painter.drawRect(QRectF(m_x,bottomRect().top(),80,16));
}

QRectF Pipe::topRect() const
{
    return QRectF(m_x,0,80,m_gapY);
}

QRectF Pipe::bottomRect() const
{
    return QRectF(m_x,m_gapY+m_gapHeight,80,720);
}

bool Pipe::offScreen() const
{
    return m_x+80 < 0;
}

double Pipe::right() const
{
    return m_x+80;
}
