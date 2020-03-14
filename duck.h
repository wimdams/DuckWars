#ifndef DUCK_H
#define DUCK_H

#include <QGraphicsItem>
#include "scan.h"
#include "bullet.h"
#include <QtWidgets>
#define QD qDebug() << __FILE__ << __LINE__

class Duck : public QGraphicsItem
{
public:
    Duck(QColor color, QString playerName);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    enum { Type = UserType + 1 };
    int type() const override;

    //setters & getters
    void setAngle(uint16_t angle);
    uint16_t angle();
    void setSpeed(uint16_t speed);
    uint16_t speed();
    void setHealth(uint16_t health);
    uint16_t health();
    void hit();
    uint16_t scan(uint16_t richting, uint16_t hoek);
    void shoot(uint16_t angle, uint16_t distance);

protected:
    void advance(int step) override;
private:
    QColor m_color;
    QString m_playerName;
    uint16_t m_angle;
    uint16_t m_speed;
    uint16_t m_health;
    uint16_t m_scanActive;
    uint16_t m_scanRichting;
    uint16_t m_scanHoek;
    QRectF m_boundingRect;
    Scan * m_scan;
    Bullet * m_bullet;
};

#endif // DUCK_H
