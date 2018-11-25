#ifndef ROUNDGAUGEGRAPHICSOBJECT_H
#define ROUNDGAUGEGRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QRectF>
#include <QRadialGradient>
#include <QBrush>
#include <QPen>
#include <QColor>

class QVariantAnimation;
//class QPropertyAnimation;

class RoundGaugeGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(qreal minValue READ minValue WRITE setMinValue NOTIFY minValueChanged)
    Q_PROPERTY(qreal maxValue READ maxValue WRITE setMaxValue NOTIFY maxValueChanged)
    Q_PROPERTY(QColor valueColor READ valueColor WRITE setValueColor NOTIFY valueColorChanged)
    Q_PROPERTY(QColor glowRingColor READ glowRingColor WRITE setGlowRingColor NOTIFY glowRingColorChanged)
    Q_PROPERTY(bool enableGlowRing READ glowRingEnabled WRITE setGlowRingEnabled NOTIFY glowRingEnabledChanged)
    Q_PROPERTY(QColor fontColor READ fontColor WRITE setFontColor NOTIFY fontColorChanged)
    Q_PROPERTY(QColor outerRingColor READ outerRingColor WRITE setOuterRingColor NOTIFY outerRingColorChanged)

public:
    RoundGaugeGraphicsObject(const QRectF& rect, QGraphicsItem* parent = nullptr);

public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    qreal value() const;
    qreal minValue() const;
    qreal maxValue() const;
    QColor valueColor() const;
    QColor glowRingColor() const;
    bool glowRingEnabled() const;
    QColor fontColor() const;
    QColor outerRingColor() const;

public slots:
    void setValue(qreal value);
    void setMinValue(qreal value);
    void setMaxValue(qreal value);
    void setValueColor(const QColor& color);
    void setGlowRingColor(const QColor& color);
    void setGlowRingEnabled(bool enabled=true);
    void setFontColor(const QColor& color);
    void setOuterRingColor(const QColor& color);

    void setRange(qreal min, qreal max);


private slots:
    void handleDialAnimationValueChanged(const QVariant& newValue);
    void handleGlowRingColorChanged(const QVariant &newValue);

signals:
    void valueChanged(qreal newValue);
    void minValueChanged(qreal newValue);
    void maxValueChanged(qreal newValue);
    void valueColorChanged(const QColor& newColor);
    void glowRingColorChanged(const QColor& newColor);
    void fontColorChanged(const QColor& newColor);
    void glowRingEnabledChanged(bool enabled);
    void outerRingColorChanged(const QColor& newColor);

private:
    QRectF mRect;
    qreal mPenWidth;
    qreal mMinValue, mMaxValue;
    qreal mRatio;
    qreal mValue;
    qreal mGaugeValue;
    int mStartAngle, mAngleSpan;

    //QRadialGradient mQrg;
    QBrush mBrush;
    QColor mGlowRingColor = QColor("green");
    QColor mValueColor = QColor("white");
    QColor mFontColor = QColor("white");
    bool mGlowRingEnabled = false;
    QColor mOuterRingColor = QColor("white");

    QVariantAnimation* mDialAnimation;
    QVariantAnimation* mGlowRingAnimation;

    QPen mValueChannelPen=QColor(100, 100, 100, 50);
    QBrush mValueChannelBrush=QColor(100, 100, 100, 50);

    QPen mValuePen=QColor("white");
    QBrush mValueBrush=QColor("white");


    // QGraphicsItem interface
public:
    int type() const override;

//protected:
    //QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // ROUNDGAUGE_H
