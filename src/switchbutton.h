#ifndef _SWITCH_BUTTON_H_
#define _SWITCH_BUTTON_H_

#include "cocr.h"

#include <QWidget>
#include <QMouseEvent>
#include <QAbstractButton>
#include <QPainter>
#include <QTimer>
#include <QColor>

class SwitchButton : public QAbstractButton {
Q_OBJECT
public:
    explicit SwitchButton(QWidget *parent = nullptr);

    ~SwitchButton();

protected:
    void paintEvent(QPaintEvent *) override;

    void mousePressEvent(QMouseEvent *) override;

    void mouseReleaseEvent(QMouseEvent *) override;

    void resizeEvent(QResizeEvent *e) override;

private:
    void updateValue();

    void drawSlider(QPainter &painter);

    void drawBackGround(QPainter &painter);

    int mStep, mEndX, mStartX, m_space;
    bool animationEnabled;
public:
    bool isAnimationEnabled() const;

    void setAnimationEnabled(bool animationEnabled = true);

private:
    QTimer *mTimer;
    const QColor mRoundButtonColorOn = Qt::white,
            mRoundButtonColorOff = Qt::white,
            mBGColorOn = Qt::lightGray,
            mBgColorOff = Qt::darkGray,
            mTextColor = Qt::black;
    const QString mTextOn = "绘制", mTextOff = "拖动";
};

#endif
