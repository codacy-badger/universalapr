#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <zpicturebox.h>
#include <zlayout.h>
#include <QRectF>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
        Q_OBJECT

    public:
        explicit Dialog(QWidget *parent = 0);
        ~Dialog();
        void resizeEvent(QResizeEvent*);
        void paintEvent(QPaintEvent *);
        void setStatus(const QString& stat) const;
        const QRectF getRect() const;
        const QRect getRectPic() const;


    private slots:
        void on_browseButton_clicked();
        void on_cancelButton_clicked();
        void on_okButton_clicked();

    private:
        ZLayout* lay;
        Ui::Dialog *ui;
        QString* fileName;
        QPixmap* clearImage;

    signals:
        void resEvent(); /* Name is not important */
};

#endif // DIALOG_H
