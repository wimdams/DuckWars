#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "duck.h"
#include "scan.h"

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

private:
    Ui::MainWindow *ui;
    QGraphicsScene m_pond;
    QList<Duck *> m_ducks;
    QList<QColor> m_colors;
};
#endif // MAINWINDOW_H
