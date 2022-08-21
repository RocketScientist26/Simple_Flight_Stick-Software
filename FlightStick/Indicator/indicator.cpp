#include "indicator.h"
#include <QPainter>
#include <math.h>

template<typename T>
T constrain(T Value, T Min, T Max){
  return (Value < Min)? Min : (Value > Max)? Max : Value;
}

Indicator::Indicator(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f), m_x(0), m_y(0){
}

void Indicator::setX(float value){
    m_x = constrain(value, -1.f, 1.f);
    qreal radius = ( m_bounds.width() - m_knopBounds.width() ) / 2;
    m_knopBounds.moveCenter(QPointF(m_bounds.center().x() + m_x * radius, m_knopBounds.center().y()));
    update();
}

void Indicator::setY(float value){
    m_y = constrain(value, -1.f, 1.f);
    qreal radius = ( m_bounds.width() - m_knopBounds.width() ) / 2;
    m_knopBounds.moveCenter(QPointF(m_knopBounds.center().x(), m_bounds.center().y() + m_y * radius));
    update();
}

void Indicator::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event)
    float size = qMin(rect().width(), rect().height());
    QPointF topleft;
    m_bounds = QRectF(topleft, QSize(size, size));
    m_knopBounds.setWidth(size * 0.3);
    m_knopBounds.setHeight(size*0.3);
    qreal radius = ( m_bounds.width() - m_knopBounds.width() ) / 2;
    m_knopBounds.moveCenter(QPointF(m_bounds.center().x() + m_x * radius, m_bounds.center().y() - m_y * radius));
}

void Indicator::paintEvent(QPaintEvent *event){
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //Background
    QRadialGradient gradient(m_bounds.center(), m_bounds.width()/2, m_bounds.center());
    gradient.setFocalRadius(m_bounds.width()*0.3);
    gradient.setCenterRadius(m_bounds.width()*0.7);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter.setPen(QPen(QBrush(Qt::gray), m_bounds.width()* 0.005));
    painter.setBrush(QBrush(gradient));
    painter.drawEllipse(m_bounds);

    //Crosshair
    painter.setPen(QPen(QBrush(Qt::gray), m_bounds.width()* 0.005));
    painter.drawLine(QPointF(m_bounds.left(), m_bounds.center().y()), QPointF(m_bounds.center().x() - m_bounds.width()*0.35, m_bounds.center().y()));
    painter.drawLine(QPointF(m_bounds.center().x() + m_bounds.width()*0.35, m_bounds.center().y()), QPointF(m_bounds.right(), m_bounds.center().y()));
    painter.drawLine(QPointF(m_bounds.center().x(), m_bounds.top()), QPointF(m_bounds.center().x(), m_bounds.center().y() - m_bounds.width()*0.35));
    painter.drawLine(QPointF(m_bounds.center().x(), m_bounds.center().y() + m_bounds.width()*0.35), QPointF(m_bounds.center().x(), m_bounds.bottom()));

    //Knob
    gradient = QRadialGradient(m_knopBounds.center(), m_knopBounds.width()/2, m_knopBounds.center());
    gradient.setColorAt(0, Qt::gray);
    gradient.setColorAt(1, Qt::darkGray);
    gradient.setFocalRadius(m_knopBounds.width()*0.2);
    gradient.setCenterRadius(m_knopBounds.width()*0.5);
    painter.setPen(QPen(QBrush(Qt::darkGray), m_bounds.width()*0.005));
    painter.setBrush(QBrush(gradient));
    painter.drawEllipse(m_knopBounds);
}
