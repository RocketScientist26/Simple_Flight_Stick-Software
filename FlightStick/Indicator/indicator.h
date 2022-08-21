#ifndef INDICATOR_H
#define INDICATOR_H
#include <QWidget>

class Indicator : public QWidget{
    Q_OBJECT
    public:
        explicit Indicator(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    public slots:
        void setX(float value);
        void setY(float value);

    private:
        void resizeEvent(QResizeEvent *event) override;
        virtual void paintEvent(QPaintEvent *event) override;
        float m_x;
        float m_y;
        QRectF m_bounds;
        QRectF m_knopBounds;
};

#endif
