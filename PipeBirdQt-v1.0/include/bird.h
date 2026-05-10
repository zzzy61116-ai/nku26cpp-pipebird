#ifndef BIRD_H
#define BIRD_H

#include <QPainter>
#include <QRectF>

class Bird
{
public:
    Bird();

    void reset();
    void flap();
    void update(double dt);
    void draw(QPainter &painter) const;

    QRectF rect() const;
    double y() const;

private:
    double m_x;
    double m_y;
    double m_velocity;
    double m_radius;
};

#endif
