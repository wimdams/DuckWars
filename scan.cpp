#include "scan.h"
#include "duck.h"

Scan::Scan(int16_t angle, int16_t width, QGraphicsItem * parent)
    : QGraphicsItem(parent)
{
    m_scanAngle=angle;
    m_scanWidth=width;
    this->setZValue(-1); //TODO dit lijkt niet te werken -> parent?
    m_visible = 0;
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
    QVector<QPointF> points;
    points << QPointF(20.0+0.0, 0.0)
            << QPointF(20.0+MAX_SCAN_DISTANCE, qTan(qDegreesToRadians(m_scanWidth/2.0))*MAX_SCAN_DISTANCE)
            << QPointF(20.0+MAX_SCAN_DISTANCE, -(qTan(qDegreesToRadians(m_scanWidth/2.0))*MAX_SCAN_DISTANCE))
    ;
    return points;
}

void Scan::paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void) option;
    (void) widget;
    if(m_visible){
        //Draw bounding rect in test mode:
        p->setPen(Qt::red);
        p->setBrush(Qt::NoBrush);
        //p->drawRect(boundingRect());

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
}

uint16_t Scan::scan(uint16_t angle, uint16_t width)
{
    m_scanAngle=angle;
    if(m_scanWidth != width){
        m_scanWidth=width;
        prepareGeometryChange();
    }
    setRotation(m_scanAngle);
    update();
    m_visible = 10;

    QList<QGraphicsItem *> items = collidingItems();
    QD << items;
    QPointF pos = mapToScene(this->pos());
    QD << pos << this->pos();
    uint32_t afstand = MAX_SCAN_DISTANCE;
    foreach (QGraphicsItem* whoami, items) {
        Duck * duck = qgraphicsitem_cast<Duck *>(whoami);
        if(duck){
            if(duck->isDead()){
                continue;
            }
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
    if(m_visible == 1) update();
    if(m_visible > 0) m_visible--;

}
