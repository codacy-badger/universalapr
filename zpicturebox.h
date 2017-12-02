#ifndef ZPICTUREBOX_H
#define ZPICTUREBOX_H

#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

class ZPictureBox : public QLabel
{
        Q_OBJECT
    public:
        explicit ZPictureBox(QWidget *parent = 0);
        void setOk(bool bl);
        const QRectF& getRect() const;
        void setRect(const QRectF& rect);
        QPoint startingPos, endingPos;

    private:
        bool isOk;
        QPen* paintPen;
        QRectF* rectangle;
        void paintEvent(QPaintEvent *);

    protected:
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* );

};

#endif // ZPICTUREBOX_H
