#include <QDebug>
#include <QPainter>
#include <QColor>
#include <QSizeF>
#include <QRectF>
#include <QVariantAnimation>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QFontInfo>
#include <QFontMetrics>
#include <QMarginsF>

#include "RoundGaugeGraphicsObject.h"

RoundGaugeGraphicsObject::RoundGaugeGraphicsObject(const QRectF &rect, QGraphicsItem *parent) :
    QGraphicsObject(parent),
    mRect(rect),
    mMinValue(0.0),
    mMaxValue(200.0),
    mValue(100.0),
    mGaugeValue(mValue),
    mStartAngle(210*16),
    mAngleSpan(-240*16),
    mDialAnimation(new QVariantAnimation(this)),
    mGlowRingAnimation(new QVariantAnimation(this))
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    setAcceptHoverEvents(true);

    mGlowRingAnimation->setDuration(1500);
    mGlowRingAnimation->setLoopCount(-1);

    mValueChannelPen.setWidth(0);

    mRatio = static_cast<qreal>(mAngleSpan)/(mMaxValue-mMinValue);

    connect(mDialAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(handleDialAnimationValueChanged(QVariant)));
    connect(mGlowRingAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(handleGlowRingColorChanged(QVariant)));
}

QRectF RoundGaugeGraphicsObject::boundingRect() const
{
    return mRect + QMarginsF(5, 5, 5, 5);
}

void RoundGaugeGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();

    //
    // Draw outline
    //
    QPen outlinePen(mOuterRingColor);
    outlinePen.setWidthF(2.5);
    painter->setPen(outlinePen);
    painter->drawEllipse(mRect+QMarginsF(5, 5, 5, 5));

    //
    // Draw empty channel
    //
    painter->setPen(mValueChannelPen);
    painter->setBrush(mValueChannelBrush);
    painter->drawPie(mRect, mStartAngle, mAngleSpan);

    //
    // Draw value bar
    //
    painter->setPen(mValuePen);
    painter->setBrush(mValueBrush);
    qreal newSpan = (mGaugeValue-mMinValue)*mRatio;
    painter->drawPie(mRect, mStartAngle, newSpan);

    //
    // Draw glow ring
    //
    if ( mGlowRingEnabled )
    {
        painter->setBrush(mGlowRingColor);
        painter->setPen(mGlowRingColor);

        QSizeF glowRingSize(mRect.width()*0.75, mRect.height()*0.75);
        QRectF glowRingEllipse(QPointF(0.0, 0.0), glowRingSize);
        glowRingEllipse.moveCenter(mRect.center());
        glowRingEllipse = glowRingEllipse;
        painter->drawEllipse(glowRingEllipse);
    }

    //
    // Draw center
    //
    painter->setPen(QColor("black"));
    painter->setBrush(QColor("black"));
    QSizeF centerSize(mRect.width()*0.70, mRect.height()*0.70);
    QRectF centerEllipse(QPointF(0.0, 0.0), centerSize);
    centerEllipse.moveCenter(mRect.center());
    painter->drawEllipse(centerEllipse);

    //
    // Draw value text
    //
    QString valueStr = QString("%L1").arg(mValue, 2, 'f', 2);
    QFont font = painter->font();
    font.setBold(true);
    font.setFixedPitch(true);
    font.setPointSize(24);
    painter->setFont(font);

    painter->setPen(mFontColor);
    painter->drawText(mRect, Qt::AlignCenter, valueStr);

    painter->restore();
}

qreal RoundGaugeGraphicsObject::value() const
{
    return mValue;
}

qreal RoundGaugeGraphicsObject::minValue() const
{
    return mMinValue;
}

qreal RoundGaugeGraphicsObject::maxValue() const
{
    return mMaxValue;
}

QColor RoundGaugeGraphicsObject::valueColor() const
{
    return mValueColor;
}

QColor RoundGaugeGraphicsObject::glowRingColor() const
{
    return mGlowRingColor;
}

bool RoundGaugeGraphicsObject::glowRingEnabled() const
{
    return mGlowRingEnabled;
}

QColor RoundGaugeGraphicsObject::fontColor() const
{
    return mFontColor;
}

QColor RoundGaugeGraphicsObject::outerRingColor() const
{
    return mOuterRingColor;
}

void RoundGaugeGraphicsObject::setRange(qreal min, qreal max)
{
   mMinValue = min;
   mMaxValue = max;
   mRatio = static_cast<qreal>(mAngleSpan)/(mMaxValue-mMinValue);

   update();
}

void RoundGaugeGraphicsObject::setValue(qreal value)
{
    mDialAnimation->setStartValue(mGaugeValue);
    mDialAnimation->setEndValue(value);
    mDialAnimation->setDuration(500);
    mDialAnimation->start();

    mValue = value;
}

void RoundGaugeGraphicsObject::setMinValue(qreal value)
{
    if ( value == mMinValue )
        return;

    mMinValue = value;
    mRatio = static_cast<qreal>(mAngleSpan)/(mMaxValue-mMinValue);

    update();

    emit minValueChanged(mMinValue);
}

void RoundGaugeGraphicsObject::setMaxValue(qreal value)
{
    if ( value == mMaxValue )
        return;

    mMaxValue = value;
    mRatio = static_cast<qreal>(mAngleSpan)/(mMaxValue-mMinValue);

    update();

    emit maxValueChanged(mMaxValue);
}

void RoundGaugeGraphicsObject::setValueColor(const QColor &color)
{
    if ( mValueColor == color )
        return;

    mValueColor = color;
    mValueBrush.setColor(mValueColor);
    mValuePen.setColor(mValueColor);

    update();

    emit valueColorChanged(mValueColor);
}

void RoundGaugeGraphicsObject::setGlowRingColor(const QColor &color)
{
    mGlowRingColor = color;

    QAbstractAnimation::State currState = mGlowRingAnimation->state();

    if ( currState == QAbstractAnimation::Running )
        mGlowRingAnimation->stop();

    mGlowRingAnimation->setKeyValueAt(0, QColor("black"));
    mGlowRingAnimation->setKeyValueAt(0.5, mGlowRingColor);
    mGlowRingAnimation->setKeyValueAt(1, QColor("black"));

    if ( currState == QAbstractAnimation::Running )
        mGlowRingAnimation->start();

    update();
}

void RoundGaugeGraphicsObject::setGlowRingEnabled(bool enabled)
{
    mGlowRingEnabled = enabled;

    if ( enabled )
        mGlowRingAnimation->start();
    else
        mGlowRingAnimation->stop();

    update();

    emit glowRingEnabledChanged(mGlowRingEnabled);
}

void RoundGaugeGraphicsObject::setFontColor(const QColor &color)
{
    if ( mFontColor == color )
        return;

    mFontColor = color;

    update();

    emit fontColorChanged(mFontColor);

}

void RoundGaugeGraphicsObject::setOuterRingColor(const QColor &color)
{
    if ( color == mOuterRingColor )
        return;

    mOuterRingColor = color;

    update();

    emit outerRingColorChanged(mOuterRingColor);
}

void RoundGaugeGraphicsObject::handleDialAnimationValueChanged(const QVariant &newValue)
{
    mGaugeValue = newValue.toDouble();

    update();
}

void RoundGaugeGraphicsObject::handleGlowRingColorChanged(const QVariant &newValue)
{
    mGlowRingColor = newValue.value<QColor>();

    update();
}

int RoundGaugeGraphicsObject::type() const
{
    return QGraphicsItem::UserType + 1;
}
