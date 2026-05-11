#include "bird.h"

#include <QtMath>

Bird::Bird()
    : m_x(130.0),
      m_y(320.0),
      m_velocity(0.0),
      m_gravity(920.0),
      m_radius(20.0),
      m_frameIndex(0),
      m_animClock(0.0)
{
    loadSprites();
}

void Bird::loadSprites()
{
    m_frames.clear();
    m_frames << QPixmap(":/assets/sprites/bird_0.png")
             << QPixmap(":/assets/sprites/bird_1.png")
             << QPixmap(":/assets/sprites/bird_2.png");
}

void Bird::reset()
{
    m_x = 130.0;
    m_y = 320.0;
    m_velocity = 0.0;
    m_frameIndex = 1;
    m_animClock = 0.0;
}

void Bird::flap(double strength)
{
    m_velocity = strength;
}

void Bird::update(double dt)
{
    m_velocity += m_gravity * dt;
    m_y += m_velocity * dt;

    m_animClock += dt;
    if (m_animClock >= 0.09) {
        m_animClock = 0.0;
        if (!m_frames.isEmpty()) {
            m_frameIndex = (m_frameIndex + 1) % m_frames.size();
        }
    }
}

void Bird::draw(QPainter &painter) const
{
    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.translate(m_x, m_y);

    double angle = qBound(-28.0, m_velocity * 0.08, 65.0);
    painter.rotate(angle);

    if (!m_frames.isEmpty() && !m_frames[m_frameIndex].isNull()) {
        const QPixmap &pix = m_frames[m_frameIndex];
        painter.drawPixmap(QRectF(-24, -24, 48, 48), pix, QRectF(0, 0, pix.width(), pix.height()));
    } else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 214, 60));
        painter.drawEllipse(QPointF(0, 0), m_radius, m_radius);
    }

    painter.restore();
}

QRectF Bird::rect() const
{
    return QRectF(m_x - 20.0, m_y - 20.0, 40.0, 40.0);
}

double Bird::y() const
{
    return m_y;
}

double Bird::velocity() const
{
    return m_velocity;
}

void Bird::setGravity(double gravity)
{
    m_gravity = gravity;
}
