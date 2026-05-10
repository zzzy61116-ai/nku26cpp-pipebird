#include "bird.h"

Bird::Bird()
{
    reset();
}

void Bird::reset()
{
    m_x = 120;
    m_y = 300;
    m_velocity = 0;
    m_radius = 20;
}

void Bird::flap()
{
    m_velocity = -320;
}

void Bird::update(double dt)
{
    m_velocity += 900 * dt;
    m_y += m_velocity * dt;
}

void Bird::draw(QPainter &painter) const
{
    painter.setPen(Qt::NoPen);

    painter.setBrush(QColor(255,220,70));
    painter.drawEllipse(QPointF(m_x,m_y),m_radius,m_radius);

    painter.setBrush(Qt::white);
    painter.drawEllipse(QPointF(m_x+6,m_y-5),4,4);

    painter.setBrush(Qt::black);
    painter.drawEllipse(QPointF(m_x+7,m_y-5),1.5,1.5);

    painter.setBrush(QColor(255,150,0));

    QPolygonF beak;
    beak << QPointF(m_x+18,m_y)
         << QPointF(m_x+28,m_y+4)
         << QPointF(m_x+18,m_y+8);

    painter.drawPolygon(beak);
}

QRectF Bird::rect() const
{
    return QRectF(m_x-m_radius,m_y-m_radius,m_radius*2,m_radius*2);
}

double Bird::y() const
{
    return m_y;
}
