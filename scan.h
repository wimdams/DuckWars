#ifndef SCAN_H
#define SCAN_H

#include <QGraphicsItem>
#include <QtWidgets>
#define QD qDebug() << __FILE__ << __LINE__

class Scan : public QGraphicsItem
{
public:
    Scan(int16_t angle, int16_t width, QGraphicsItem * parent);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
protected:
    void advance(int step) override;
private:
    //uint16_t m_scanActive;
    uint16_t m_scanAngle;
    uint16_t m_scanWidth;
};

#endif // SCAN_H
