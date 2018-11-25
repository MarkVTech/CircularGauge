#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QPushButton;

namespace Ui {
class MainWindow;
}

class RoundGaugeGraphicsObject;
class NeedleMeterGraphicsObject;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* mScene;

    RoundGaugeGraphicsObject* mRoundGauge;
    NeedleMeterGraphicsObject* mNeedleMeter;

    void setAGaugeColor(QPushButton* button);
};

#endif // MAINWINDOW_H
