#include "scan.h"

Scan::Scan(int16_t angle = 0, int16_t width = 20, QGraphicsItem * parent = nullptr)
    : QGraphicsItem(parent)
{
    m_scanAngle=angle;
    m_scanWidth=width;
    this->setZValue(-1);
}

QRectF Scan::boundingRect() const
{
    //TODO: calculate the bounding rect based on the 3 points
    return QRectF(-130,-130,270,270);
}

QPainterPath Scan::shape() const
{
    QPainterPath path;
    QVector<QPointF> points;
    points  << QPointF(0.0, 0.0)
            << QPointF(100.0, qTan(qDegreesToRadians(m_scanWidth/2.0))*100)
            << QPointF(100.0, -(qTan(qDegreesToRadians(m_scanWidth/2.0))*100))
    ;
    QPolygonF driehoek(points);
    path.addPolygon(driehoek);
    return path;
}

void Scan::paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void) option;
    (void) widget;

    //Draw bounding rect in test mode:
    p->setPen(Qt::red);
    p->setBrush(Qt::NoBrush);
    p->drawRect(boundingRect());

    //draw scan lines
    p->save();
    p->rotate(m_scanAngle);
    p->translate(20,0);
    QLinearGradient scanGrad(QPointF(0, 0), QPointF(100, 0));
    scanGrad.setColorAt(0, QColor(255,255,255,200));
    scanGrad.setColorAt(1, QColor(255,255,255,24));
    QBrush b1(scanGrad);
    p->setBrush(b1);
    p->setPen(Qt::NoPen);
    QPointF points[3] = {
        QPointF(0.0, 0.0),
        QPointF(100.0, qTan(qDegreesToRadians(m_scanWidth/2.0))*100),
        QPointF(100.0, -(qTan(qDegreesToRadians(m_scanWidth/2.0))*100)),
    };
    p->drawPolygon(points,3);
    p->restore();
}

void Scan::advance(int step)
{
    //als step 0 is niets doen
    if (!step)
        return;
}
