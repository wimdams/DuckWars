#ifndef SCAN_H
#define SCAN_H

#include <QGraphicsItem>
#include "config.h"

class Scan : public QGraphicsItem
{
public:
    Scan(int16_t angle = 0, int16_t width = 20, QGraphicsItem * parent = nullptr);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    uint16_t scan(uint16_t richting, uint16_t hoek);

protected:
    void advance(int step) override;
private:
    //uint16_t m_scanActive;
    uint16_t m_scanAngle;
    uint16_t m_scanWidth;
    QVector<QPointF> calculateScanBeam(void) const;
    int    m_visible;
};

#endif // SCAN_H
