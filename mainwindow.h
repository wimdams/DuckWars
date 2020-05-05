#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "config.h"
#include "duck.h"
#include "scan.h"
#include "bullet.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Angle_valueChanged(int value);
    void on_Speed_valueChanged(int value);
    void on_Health_valueChanged(int value);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_actionStart_triggered();

    void reloadSettings();
    void tcpClientNew();
    void tcpClientDisconnect(QTcpSocket *tcpClient);
    void tcpClientRecieveData(QTcpSocket *tcpClient);
private:
    Ui::MainWindow *ui;
    QGraphicsScene m_pond;
    QList<Duck *> m_ducks;
    QList<QColor> m_colors;
    //networking:
    QTcpServer *m_tcpServer;
    int m_serverPort;
    int m_colorCounter;
    QMap<QTcpSocket*, Duck*> m_duckMap;

};
#endif // MAINWINDOW_H
