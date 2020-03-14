#ifndef BULLET_H
#define BULLET_H

#include <QSoundEffect>
#include <QGraphicsItem>
#include <QtWidgets>
#define QD qDebug() << __FILE__ << __LINE__

class Bullet : public QGraphicsItem
{
public:
    Bullet(int16_t angle = 0, int16_t distance = 100, QColor color = Qt::yellow, QGraphicsItem * parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void shoot(QPointF startPos, uint16_t angle, uint16_t distance);

protected:
    void advance(int step) override;
private:
    //uint16_t m_scanActive;
    uint16_t m_shootAngle;
    uint16_t m_shootDistance;
    QColor m_bulletColor;
    QPointF m_startPoint;
    bool    m_visible;
    QSoundEffect m_sndEffect;
};

#endif // BULLET_H
