#ifndef PIPE_H
#define PIPE_H

#include <QPainter>
#include <QRectF>

class Pipe
{
public:
    Pipe(double x, double gapY, int gapHeight, double width, double speed);

    void update(double dt);
    void draw(QPainter &painter) const;

    QRectF topRect() const;
    QRectF bottomRect() const;
    bool offScreen() const;
    bool scored(double birdX);
    double right() const;

private:
    double m_x;
    double m_gapY;
    int m_gapHeight;
    double m_width;
    double m_speed;
    bool m_counted;
};

#endif
