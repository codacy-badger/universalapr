#include <zpicturebox.h>
#include <QDebug>

ZPictureBox::ZPictureBox(QWidget *parent) :
    QLabel(parent)
{
    isOk=false;
    paintPen = new QPen(Qt::darkRed);
    paintPen->setWidth(2);
    rectangle = new QRectF(0,0,0,0);
}

void ZPictureBox::paintEvent(QPaintEvent*)
{
    if (isOk)
    {
        QPainter painter(this);
        painter.setPen(*paintPen);
        painter.drawRect(*rectangle);
        this->update();
    }
}

void ZPictureBox::mousePressEvent(QMouseEvent* event)
{
    startingPos = event->pos();
}

void ZPictureBox::mouseMoveEvent(QMouseEvent* event)
{
    if (isOk)
    {
        paintPen->setColor(Qt::darkRed);
        endingPos = event->pos();
        rectangle->setTopLeft(startingPos);
        rectangle->setBottomRight(endingPos);
    }
}

void ZPictureBox::mouseReleaseEvent(QMouseEvent*)
{
    paintPen->setColor(Qt::darkGreen);
}

void ZPictureBox::setOk(bool bl)
{
    isOk=bl;
}

const QRectF& ZPictureBox::getRect() const
{
    return *rectangle;
}

void ZPictureBox::setRect(const QRectF& rect)
{
    *rectangle = rect;
}
