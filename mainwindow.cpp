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
    for(int i=0; i < MAX_COLORS; i++){
        QColor c;
        c.setHsv((int)((360/30.0)*i),255,255/((i%2)+1));
        m_colors << c;
    }
    m_colorCounter = 0;

    //De eendjes genereren
//    int i = 0, j = 0;
//    foreach (QColor color, m_colors) {
//        m_ducks.append(new Duck(color, "Wim"));
//        m_ducks.last()->setPos(i*100,j*100);
//        m_pond.addItem(m_ducks.last());
//        i++;
//        if(i > 5){
//            i=0;
//            j++;
//        }
//    }

    m_ducks.append(new Duck(m_colors.at((m_colorCounter++)%MAX_COLORS)));
    m_ducks.first()->setPos(4*100,4*100);
    m_ducks.first()->setPlayerName("A.I. One");
    m_pond.addItem(m_ducks.last());

    QTimer * t = new QTimer(this);
    t->setInterval(1000/33);
    connect(t,SIGNAL(timeout()),&m_pond,SLOT(advance()));
    t->start();

    m_tcpServer = new QTcpServer(this);
    reloadSettings();           //settings laden
    on_actionStart_triggered(); //start
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(tcpClientNew()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Angle_valueChanged(int value)
{
    m_ducks.first()->setAngle(value);
}

void MainWindow::on_Speed_valueChanged(int value)
{
    m_ducks.first()->setSpeed(value);
}

void MainWindow::on_Health_valueChanged(int value)
{
    m_ducks.first()->setHealth(value);
}

void MainWindow::on_pushButton_clicked()
{
    uint16_t afstand = m_ducks.first()->scan(ui->scanAngle->value(),ui->scanWidth->value());
    ui->shootAngle->setValue(ui->scanAngle->value());
    ui->shootDist->setValue(afstand);
}

void MainWindow::on_pushButton_2_clicked()
{
    m_ducks.first()->shoot(ui->shootAngle->value(),ui->shootDist->value());
}

void MainWindow::reloadSettings()
{
    QSettings settings("DNI", "ServerEmbSys");
    m_serverPort = settings.value("server/port", 64000U).toInt();
}

void MainWindow::tcpClientNew()
{
    QTcpSocket * tcpClient = m_tcpServer->nextPendingConnection();
    connect(tcpClient, &QTcpSocket::disconnected,[=]{tcpClientDisconnect(tcpClient);});
    connect(tcpClient, &QTcpSocket::readyRead, [=]{tcpClientRecieveData(tcpClient);});
    m_ducks.append(new Duck(m_colors.at((m_colorCounter++)%MAX_COLORS)));
    m_ducks.last()->setPos(4*100,4*100);
    m_pond.addItem(m_ducks.last());
    m_duckMap[tcpClient] = m_ducks.last();
}

void MainWindow::tcpClientDisconnect(QTcpSocket * tcpClient)
{
    QD << "Client closed connection";
    Duck * duck = m_duckMap[tcpClient];
    m_duckMap.remove(tcpClient);
    m_pond.removeItem(duck);
    //TODO remove bullet in duck!!! memory leak
    tcpClient->deleteLater();
}

void MainWindow::tcpClientRecieveData(QTcpSocket * tcpClient)
{
    QRegularExpressionMatch match;
    bool cmdExecuted = false;
    QString buffer = tcpClient->readAll();
    QD << "Client send data: " << buffer;
    if(buffer.contains("status\r\n")){
        Duck * duck = m_duckMap[tcpClient];
        QString reply("{posx:%1, posy:%2, health:%3, speed:%4, angle:%5}\r\n");
        reply = reply.arg(duck->pos().x()).arg(duck->pos().y()).arg(duck->health()).arg(duck->speed()).arg(duck->angle());
        tcpClient->write(reply.toLatin1());
        cmdExecuted = true;
    }
    if(buffer.contains(QRegularExpression("setName\\(([a-zA-Z0-9.-_&@#!*$%+=<>\\[\\]\\s]{1,30})\\)\\r\\n"),&match)){
        Duck * duck = m_duckMap[tcpClient];
        duck->setPlayerName(match.captured(1));
        QString reply("{result:ok}\r\n");
        tcpClient->write(reply.toLatin1());
        cmdExecuted = true;
    }
    if(buffer.contains(QRegularExpression("scan\\(([0-9]{1,3})\\)\\r\\n"),&match)){
        Duck * duck = m_duckMap[tcpClient];
        uint16_t afstand = duck->scan(match.captured(1).toInt(),20);
        QString reply("{range:%1}\r\n");
        reply = reply.arg(afstand);
        tcpClient->write(reply.toLatin1());
        cmdExecuted = true;
    }
    if(buffer.contains(QRegularExpression("swim\\(([0-9]{1,3})\\)\\r\\n"),&match)){
        Duck * duck = m_duckMap[tcpClient];
        duck->setAngle(match.captured(1).toInt());
        duck->setSpeed(25);
        QString reply("{result:ok}\r\n");
        tcpClient->write(reply.toLatin1());
        cmdExecuted = true;
    }
    if(buffer.contains(QRegularExpression("shoot\\(([0-9]{1,3}),([0-9]{1,3})\\)\\r\\n"),&match)){
        Duck * duck = m_duckMap[tcpClient];
        duck->shoot(match.captured(1).toInt(),match.captured(2).toInt());
        QString reply("{result:ok}\r\n");
        tcpClient->write(reply.toLatin1());
        cmdExecuted = true;
    }
    if(buffer.contains(QRegularExpression("setSpeed\\(([0-9]{1,3})\\)\\r\\n"),&match)){
        Duck * duck = m_duckMap[tcpClient];
        duck->setSpeed(match.captured(1).toInt());
        QString reply("{result:ok}\r\n");
        tcpClient->write(reply.toLatin1());
        cmdExecuted = true;
    }

    if(cmdExecuted == false){
        tcpClient->write("{result:malformed or wrong command}\r\n");
    }
}


void MainWindow::on_actionStart_triggered()
{
    if (!m_tcpServer->listen(QHostAddress::Any,m_serverPort)) {
        QMessageBox::critical(this, windowTitle(),
                              tr("Unable to start the server: %1.")
                              .arg(m_tcpServer->errorString()));

        /*ui->statusLabel->setText(tr("Unable to start the server: %1.")
                             .arg(m_tcpServer->errorString()));*/
        return;

    }

    /*ui->statusLabel->setText(tr("The server is running on port %1.\n"
                            "Run Your Client now.")
                         .arg(tcpServer->serverPort()));
    ui->debugConsole->appendPlainText(tr("The server is started and using port %1").arg(tcpServer->serverPort()));
    ui->actionStart->setDisabled(true);
    ui->actionStop->setDisabled(false);*/
}
