#ifndef BIRD_H
#define BIRD_H

#include <QPainter>
#include <QPixmap>
#include <QVector>
#include <QRectF>

class Bird
{
public:
    Bird();

    void reset();
    void flap(double strength);
    void update(double dt);
    void draw(QPainter &painter) const;

    QRectF rect() const;
    double y() const;
    double velocity() const;
    void setGravity(double gravity);

private:
    void loadSprites();

private:
    double m_x;
    double m_y;
    double m_velocity;
    double m_gravity;
    double m_radius;

    QVector<QPixmap> m_frames;
    int m_frameIndex;
    double m_animClock;
};

#endif
