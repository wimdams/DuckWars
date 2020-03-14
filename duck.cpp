#include "duck.h"

Duck::Duck(QColor color, QString playerName)
{
    m_color = color;
    m_playerName = playerName;
    m_health = 10;
    m_angle = 0;
    m_speed = 0;
    m_scanActive = 0;
    m_scanHoek = 20;
    m_scanRichting = 0;
    m_boundingRect = QRectF(-30,-30,60,70);
    m_scan = new Scan(0,20,this);
    m_scan->setZValue(-1);
    m_bullet = new Bullet(0,200,m_color);
}

QRectF Duck::boundingRect() const
{
    return m_boundingRect;
}

QPainterPath Duck::shape() const
{
    QPainterPath path;
    //enkel ellipse van de body (sorry geen headshots)
    path.addEllipse(-20,-20,40,40);
    return path;
}

void Duck::paint(QPainter *p, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void) option;
    (void) widget;

    //Draw bounding rect in test mode:
    //p->setPen(Qt::red);
    //p->setBrush(Qt::NoBrush);
    //p->drawRect(boundingRect());

    //Health bar
    p->setPen(Qt::NoPen);
    p->setBrush(Qt::green);
    p->drawRect(-27,-20,4,40);
    p->setBrush(Qt::red);
    p->drawRect(-27,-20,4,(10 - m_health) * 4); //4 pixels per health point

    //Player Name
    p->setPen(Qt::white);
    p->drawText(0,25+10,m_playerName);

    //Gradient for body
    QLinearGradient bodyGrad(QPointF(0, 0), QPointF(30, 30));
    bodyGrad.setColorAt(0, m_color);
    bodyGrad.setColorAt(1, m_color.darker(400));
    //Draw body
    QBrush b2(bodyGrad);
    p->setBrush(b2);
    QPen pen(Qt::black);
    pen.setWidth(2);
    p->setPen(pen);
    p->drawEllipse(-20,-20,40,40);
    //Draw dot on back of body
    p->setPen(Qt::NoPen);
    p->setBrush(QColor(0,0,0,50));
    p->drawEllipse(-7,-7,14,14);
    //Draw Head
    p->save();
    p->rotate(m_angle);
    p->translate(24,0);
    p->setBrush(m_color);
    p->setPen(pen);
    p->drawEllipse(-20,-10,20,20);
    //Draw eyes
    p->setBrush(Qt::black);
    p->drawChord(-12, -6, 6, 5, -45 * 16, 135 * 16);
    p->drawChord(-12, 2, 6, 5, 45 * 16, -135 * 16);
    //Draw beak
    p->setBrush(QColor(255,113,0));
    pen.setWidth(1);
    p->setPen(pen);
    p->drawRect(-4,-4,10,8);
    //p->drawLine(0,0,0,20);
    //p->drawLine(0,0,20,0);
    p->restore();


//    p->setPen(Qt::darkRed);
//    p->drawLine(0,0,0,75);
//    p->drawLine(0,0,75,0);
}

int Duck::type() const
{
    // Enable the use of qgraphicsitem_cast with this item.
    return Type;
}

void Duck::setAngle(uint16_t angle)
{
    while(angle > 360){
        angle = angle - 360;
    }
    m_angle = angle;
    update();
}

uint16_t Duck::angle()
{
    return m_angle;
}

void Duck::setSpeed(uint16_t speed)
{
    if(speed > 100){
       speed = 100;
    }
    m_speed = speed;
}

uint16_t Duck::speed()
{
    return m_speed;
}

void Duck::setHealth(uint16_t health)
{
    if(health > 10){
        health = 10;
    }
    m_health = health;
    update();
}

uint16_t Duck::health()
{
    return m_health;
}

void Duck::hit()
{
    if(health()!=0) {
        setHealth(health()-1);
    }
}

uint16_t Duck::scan(uint16_t richting, uint16_t hoek)
{
    return m_scan->scan(richting, hoek);
}

void Duck::shoot(uint16_t angle, uint16_t distance)
{
    if(!m_bullet->scene()){
        this->scene()->addItem(m_bullet);
    }
    m_bullet->shoot(scenePos(), angle, distance);
}

void Duck::advance(int step)
{
    //als step 0 is niets doen
    if (!step)
        return;
    //berekenen naar waar we moeten bewegen en de stap zetten
    qreal angleInRad = qDegreesToRadians((qreal)m_angle);
    setPos(mapToParent(qCos(angleInRad)*(m_speed/10), qSin(angleInRad)*(m_speed/10)));
}
