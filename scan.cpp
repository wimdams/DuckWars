#include "scan.h"
#include "duck.h"

Scan::Scan(int16_t angle = 0, int16_t width = 20, QGraphicsItem * parent = nullptr)
    : QGraphicsItem(parent)
{
    m_scanAngle=angle;
    m_scanWidth=width;
    this->setZValue(-1); //dit lijkt niet te werken -> parent?
}

QRectF Scan::boundingRect() const
{
    int32_t xMax = INT32_MIN, yMax = INT32_MIN;
    int32_t xMin = INT32_MAX, yMin = INT32_MAX;

    QVector<QPointF> points = calculateScanBeam();
    //Calculate the min and max x,y points
    foreach (QPointF point, points) {
        if(point.x() > xMax) xMax = point.x();
        if(point.y() > yMax) yMax = point.y();
        if(point.x() < xMin) xMin = point.x();
        if(point.y() < yMin) yMin = point.y();
    }

    return QRectF(xMin,yMin,xMax-xMin,yMax-yMin);
}

QPainterPath Scan::shape() const
{
    QPainterPath path;
    QPolygonF driehoek(calculateScanBeam());
    path.addPolygon(driehoek);
    return path;
}

QVector<QPointF> Scan::calculateScanBeam(void) const {
//    QVector<QPointF> points1, points2;
//    points1 << QPointF(0.0, 0.0)
//            << QPointF(100.0, qTan(qDegreesToRadians(m_scanWidth/2.0))*100)
//            << QPointF(100.0, -(qTan(qDegreesToRadians(m_scanWidth/2.0))*100))
//    ;
//    //2D rotation about a point: https://academo.org/demos/rotation-about-point/
//    qreal angle = qDegreesToRadians((double)m_scanAngle);
//    qreal translateXvalue = 20 * qCos(angle);
//    qreal translateYvalue = 20 * qSin(angle);
//    foreach (QPointF point, points1) {
//        qreal x = translateXvalue + ((point.x() * qCos(angle)) - (point.y() * qSin(angle)));
//        qreal y = translateYvalue + ((point.y() * qCos(angle)) - (point.x() * qSin(angle)));
//        points2 << QPointF(x,y);
//    }

//    return points2;

    QVector<QPointF> points;
    points << QPointF(20.0+0.0, 0.0)
            << QPointF(20.0+100.0, qTan(qDegreesToRadians(m_scanWidth/2.0))*100)
            << QPointF(20.0+100.0, -(qTan(qDegreesToRadians(m_scanWidth/2.0))*100))
    ;
    return points;
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
    QLinearGradient scanGrad(QPointF(0, 0), QPointF(100, 0));
    scanGrad.setColorAt(0, QColor(255,255,255,200));
    scanGrad.setColorAt(1, QColor(255,255,255,24));
    QBrush b1(scanGrad);
    p->setBrush(b1);
    p->setPen(Qt::NoPen);
    QPolygonF driehoek(calculateScanBeam());
    p->drawPolygon(driehoek);

}

uint16_t Scan::scan(uint16_t angle, uint16_t width)
{
    m_scanAngle=angle;
    if(m_scanWidth != width){
        m_scanWidth=width;
        prepareGeometryChange();
        update();
    }

    setRotation(m_scanAngle);

    QList<QGraphicsItem *> items = collidingItems();
    QD << items;
    QPointF pos = mapToScene(this->pos());
    QD << pos << this->pos();
    uint32_t afstand = 100;
    foreach (QGraphicsItem* whoami, items) {
        Duck * duck = qgraphicsitem_cast<Duck *>(whoami);
        if(duck){

            if(duck->pos() != pos){
                QPointF v = duck->pos() - pos;
                double trueLength = sqrt(pow(v.x(), 2) + pow(v.y(), 2));
                QD << "Het is een andere eend op " << trueLength << " pixels van mij" ;
                if(afstand > trueLength){
                    afstand = trueLength;
                }
            }else{
                QD << "ik ben het zelf";
            }
        }
    }


    return afstand;
}

void Scan::advance(int step)
{
    //als step 0 is niets doen
    if (!step)
        return;

    //TODO: beam laten faden
}
