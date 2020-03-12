#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pond.setSceneRect(0,0,500,500);
    ui->graphicsView->setScene(&m_pond);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    QPixmap bgPix(":/img/water.png");
    bgPix = bgPix.scaled(255,255);
    ui->graphicsView->setBackgroundBrush(bgPix);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //De kleuren genereren van de eendjes:
    for(int i=0; i < 30; i++){
        QColor c;
        c.setHsv((int)((360/30.0)*i),255,255/((i%2)+1));
        m_colors << c;
    }

    //De eendjes genereren
    int i = 0, j = 0;
    foreach (QColor color, m_colors) {
        m_ducks.append(new Duck(color, "Wim"));
        m_ducks.last()->setPos(i*100,j*100);
        m_pond.addItem(m_ducks.last());
        i++;
        if(i > 5){
            i=0;
            j++;
        }
    }

    QTimer * t = new QTimer(this);
    t->setInterval(1000/33);
    connect(t,SIGNAL(timeout()),&m_pond,SLOT(advance()));
    t->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Angle_valueChanged(int value)
{
    m_ducks.last()->setAngle(value);
}

void MainWindow::on_Speed_valueChanged(int value)
{
    m_ducks.last()->setSpeed(value);
}

void MainWindow::on_Health_valueChanged(int value)
{
    m_ducks.last()->setHealth(value);
}

void MainWindow::on_pushButton_clicked()
{
    uint16_t afstand = m_ducks.last()->scan(ui->scanAngle->value(),ui->scanWidth->value());
    ui->shootAngle->setValue(ui->scanAngle->value());
    ui->shootDist->setValue(afstand);

//    Scan *s =  new Scan(ui->scanOrient->value(),ui->scanHoek->value());
//    m_pond.addItem(s);
//    QList<QGraphicsItem *> items = s->collidingItems();
//    QD << items;
//    foreach (QGraphicsItem* whoami, items) {
//        Duck * duck = qgraphicsitem_cast<Duck *>(whoami);
//        if(duck){
//            QD << "Het is een eend" << duck;
//            if(duck->pos() != s->pos()){
//                QPointF v = duck->pos() - s->pos();
//                double trueLength = sqrt(pow(v.x(), 2) + pow(v.y(), 2));
//                QD << "Het is een andere eend op " << trueLength << " pixels van mij" ;
//            }else{
//                QD << "ik ben het zelf";
//            }
//        }
//    }


}
