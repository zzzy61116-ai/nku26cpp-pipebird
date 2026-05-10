#ifndef PIPE_H
#define PIPE_H

#include <QPainter>
#include <QRectF>

class Pipe
{
public:
    Pipe(double x,double gapY,double gapHeight);

    void update(double dt);
    void draw(QPainter &painter) const;

    QRectF topRect() const;
    QRectF bottomRect() const;

    bool offScreen() const;

    bool counted;
    double right() const;

private:
    double m_x;
    double m_gapY;
    double m_gapHeight;
};

#endif
