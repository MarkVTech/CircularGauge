#include <functional>

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QSlider>
#include <QColorDialog>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "RoundGaugeGraphicsObject.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mScene = new QGraphicsScene(0, 0, 640, 480);
    mScene->setBackgroundBrush(QBrush("black"));
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setScene(mScene);

    mRoundGauge = new RoundGaugeGraphicsObject(QRectF(50, 50, 200, 200));
    mScene->addItem(mRoundGauge);
    mRoundGauge->setPos(QPointF(250, 0));
    mRoundGauge->setGlowRingColor(QColor("magenta"));
    mRoundGauge->setValueColor(QColor("purple"));
    mRoundGauge->setFontColor(QColor("yellow"));

    mRoundGauge->setValue(ui->doubleSpinBox->value());
    mRoundGauge->setRange(ui->doubleSpinBox->minimum(), ui->doubleSpinBox->maximum());

    using namespace std::placeholders;

    connect(ui->valueColorButton, &QPushButton::clicked,
            [=]() { setAGaugeColor(ui->valueColorButton); }
    );

    connect(ui->glowRingColorButton, &QPushButton::clicked,
            [=]() { setAGaugeColor(ui->glowRingColorButton); }
    );

    connect(ui->fontColorButton, &QPushButton::clicked,
            [=]() { setAGaugeColor(ui->fontColorButton); }
    );

    connect(ui->outerRingColorButton, &QPushButton::clicked,
            [=]() { setAGaugeColor(ui->outerRingColorButton); }
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    mRoundGauge->setValue(ui->doubleSpinBox->value());
    mRoundGauge->setGlowRingEnabled(ui->glowRingEnabledCheck->checkState());
}

void MainWindow::setAGaugeColor(QPushButton *button)
{
    QColor color = QColorDialog::getColor();
    //QString qss = QString("color: rgba(%1, %2, %3, %4)").
            //arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
    //button->setStyleSheet(qss);

    if ( button->objectName() == "glowRingColorButton" )
        mRoundGauge->setGlowRingColor(color);
    else if ( button->objectName() == "fontColorButton" )
        mRoundGauge->setFontColor(color);
    else if ( button->objectName() == "valueColorButton" )
        mRoundGauge->setValueColor(color);
    else if ( button->objectName() == "outerRingColorButton" )
        mRoundGauge->setOuterRingColor(color);
}
