#include "colorbutton.h"
#include <QColorDialog>
#include <QPainter>

using namespace GUI;

SelectColorButton::SelectColorButton(QWidget *parent) : m_color(Qt::white)
{
    setFixedSize(QSize{80,80});
    connect(this, SIGNAL(clicked()), this, SLOT(changeColor()));
}

void SelectColorButton::changeColor()
{
    QColor newColor = QColorDialog::getColor(m_color, parentWidget());
    if (newColor != m_color)
    {
        setColor(newColor);
        emit onColorChanged();
    }
}

void SelectColorButton::setColor(const QColor &color)
{
    m_color = color;
    update();
}

QColor SelectColorButton::getColor() const
{
    return m_color;
}

void SelectColorButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    
    auto r = rect().adjusted(2,2,-2,-2);

    painter.setPen(Qt::black);
    painter.setBrush(m_color);
    painter.drawRect(r);
}
