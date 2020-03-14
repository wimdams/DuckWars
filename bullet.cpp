#include "bullet.h"
#include "duck.h"

Bullet::Bullet(int16_t angle, int16_t distance, QColor color, QGraphicsItem * parent)
    : QGraphicsItem(parent)
{
    m_shootAngle = angle;
    m_shootDistance = distance;
    m_bulletColor = color;
    m_visible = false;

//    m_sndEffect.setSource(QUrl("qrc:/snd/quack.wav"));
//    m_sndEffect.setLoopCount(1);
//    m_sndEffect.setVolume(0.5f);

}

void Bullet::paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void) option;
    (void) widget;

    if(m_visible){
//        //Draw bounding rect in test mode:
//        p->setPen(Qt::red);
//        p->setBrush(Qt::NoBrush);
//        p->drawRect(boundingRect());

//        QRadialGradient bulletGrad(0,0,12);
//        bulletGrad.setColorAt(0, QColor(210,105,30,200));
//        bulletGrad.setColorAt(1, QColor(0,0,0,255));
//        QBrush b1(bulletGrad);
        p->setBrush(m_bulletColor);
        p->setPen(Qt::black);
        p->drawEllipse(-6,-6,12,12);
    }
}

void Bullet::shoot(QPointF startPos, uint16_t angle, uint16_t distance)
{
    m_startPoint = startPos;
    setPos(startPos);
    m_shootAngle = angle;
    m_shootDistance = distance;
    m_visible = true;
}

QRectF Bullet::boundingRect() const
{
    return QRectF(-6,-6,12,12);
}

QPainterPath Bullet::shape() const
{
    QPainterPath path;
    path.addEllipse(-6,-6,12,12);
    return path;
}

void Bullet::advance(int step)
{
    //als step 0 is niets doen
    if (!step)
        return;
    if(m_visible){
        //berekenen naar waar we moeten bewegen en de stap zetten
        qreal angleInRad = qDegreesToRadians((qreal)m_shootAngle);
        setPos(mapToParent(qCos(angleInRad)*(11), qSin(angleInRad)*(11)));

        QPointF v = scenePos() - m_startPoint;
        double trueLength = sqrt(pow(v.x(), 2) + pow(v.y(), 2));

        if(m_shootDistance <= trueLength){
            QList<QGraphicsItem *> items = collidingItems(Qt::IntersectsItemBoundingRect);
            //QD << items;
            foreach (QGraphicsItem* whoami, items) {
                Duck * duck = qgraphicsitem_cast<Duck *>(whoami);
                if(duck){
                    //Je kan uzelf raken!!
                    if(!duck->isDead()){
                        duck->hit();
                        SndFx::playFX(QUACK);
                    }
                }
            }
            update();
            m_visible = false;
        }
    }
}
