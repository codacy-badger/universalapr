#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Picture Area / Minimum Plate Area
// Yani bütük resim alanı plaka bölgesinin alanının kaç katı büyük
// Burada onun alt ve üst sınırlarını ayarladık
#define RESOLUTION_MAX 305.0
#define RESOLUTION_MIN 3.0

#include <QMainWindow>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <asmopencv.h>
#include <pumaocr.h>
#include <zclear.h>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

using namespace ASM;
using namespace std;
using namespace cv;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        void setMinArea(const QRectF mArea);
        void setMaxArea(const QRectF mArea);
        void setPicMinArea(const QRect mArea);
        void setPicMaxArea(const QRect mArea);

    private slots:
        void on_pushButton_clicked();
        void on_pushButton_2_clicked();
        void on_pushButton_3_clicked();
        void on_pushButton_4_clicked();
        void detectPlate();
        void animFinished();

    private:
        Ui::MainWindow *ui;
        QRectF minArea, maxArea, picMinArea, picMaxArea;
        float resolutionMax, resolutionMin;
        QStringList fileNames;
        vector<Mat> clearImages;
        QTimer *timer ;
        QParallelAnimationGroup*  group;
        int h;

};

#endif // MAINWINDOW_H
