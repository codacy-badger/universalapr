#include <mainwindow.h>
#include <ui_mainwindow.h>
#include <dialog.h>
#include <QDebug>
#include <QFileDialog>
#include <universalapr.h>
#include <QTimer>
#include <exception>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->copyrightLabel->setVisible(false);
    timer= new QTimer(this);
    group = new QParallelAnimationGroup;
    connect(timer, SIGNAL(timeout()), this, SLOT(detectPlate()));
    connect(group,SIGNAL(finished()),this,SLOT(animFinished()));

    pumaInit();
}

MainWindow::~MainWindow()
{
    pumaDone();
    delete ui;
}

void MainWindow::setMinArea(const QRectF mArea)
{
    minArea.setX(qAbs(mArea.x()));
    minArea.setY(qAbs(mArea.y()));
    minArea.setWidth(qAbs(mArea.width()));
    minArea.setHeight(qAbs(mArea.height()));
}

void MainWindow::setMaxArea(const QRectF mArea)
{
    maxArea.setX(qAbs(mArea.x()));
    maxArea.setY(qAbs(mArea.y()));
    maxArea.setWidth(qAbs(mArea.width()));
    maxArea.setHeight(qAbs(mArea.height()));
}

void MainWindow::setPicMinArea(const QRect mArea)
{
    picMinArea.setX(qAbs(mArea.x()));
    picMinArea.setY(qAbs(mArea.y()));
    picMinArea.setWidth(qAbs(mArea.width()));
    picMinArea.setHeight(qAbs(mArea.height()));
}

void MainWindow::setPicMaxArea(const QRect mArea)
{
    picMaxArea.setX(qAbs(mArea.x()));
    picMaxArea.setY(qAbs(mArea.y()));
    picMaxArea.setWidth(qAbs(mArea.width()));
    picMaxArea.setHeight(qAbs(mArea.height()));
}


void MainWindow::on_pushButton_clicked()
{
    Dialog *dialog = new Dialog;
    dialog->setStatus("Please select area of plate from the most remote vehicle picture.");
    if (dialog->exec() == QDialog::Rejected){
        delete dialog;
        return;
    }
    setMinArea(dialog->getRect());
    setPicMinArea(dialog->getRectPic());
    resolutionMin=(picMinArea.width()*picMinArea.height())/(minArea.width()*minArea.height());

    if (!maxArea.isEmpty() && !minArea.isEmpty())
        ui->pushButton_3->setEnabled(true);
    delete dialog;
}

void MainWindow::on_pushButton_2_clicked()
{
    Dialog *dialog = new Dialog;
    dialog->setStatus("Please select area of plate from the most close vehicle picture.");
    if (dialog->exec() == QDialog::Rejected){
        delete dialog;
        return;
    }

    setMaxArea(dialog->getRect());
    setPicMaxArea(dialog->getRectPic());
    resolutionMax=(picMaxArea.width()*picMaxArea.height())/(maxArea.width()*maxArea.height());
    if (!maxArea.isEmpty() && !minArea.isEmpty())
        ui->pushButton_3->setEnabled(true);
    delete dialog;
}

void MainWindow::on_pushButton_3_clicked()
{
    /*
    if (qAbs(resolutionMax)>RESOLUTION_MAX || qAbs(resolutionMin)<RESOLUTION_MIN)
        {
        qWarning("Çözünürlük aşımı");
        return;
        }
*/
    QStringList fileDialog = QFileDialog::getOpenFileNames(this,
                                                           tr("Resim seçin"),
                                                           "/home/",
                                                           tr("Resim Dosyası (*.png *.jpg *.bmp)"));

    if (fileDialog.isEmpty()){
        return;
    }

    fileNames = fileDialog;
    unsigned int fz=fileNames.size();
    Mat *localImages = new Mat[fz];
    for (unsigned int i=0;i<fz;i++)
        localImages[i]=imread(fileNames.at(i).toStdString());
    /*
    for (unsigned int i=0;i<fz;i++)
        if (localImages[i].cols * localImages[i].rows > ui->pictureLabel->width()*ui->pictureLabel->height())
            cv::resize(localImages[i], localImages[i], Size(ui->pictureLabel->width(),ui->pictureLabel->height()));
            */
    clearImages.clear();
    clearImages.reserve( fz );
    clearImages.insert( clearImages.begin(), localImages, localImages + fz );
    h=fz-1;
    delete[] localImages;
    ui->pushButton_4->setEnabled(true);
}

void MainWindow::on_pushButton_4_clicked()
{
    if (ui->logoLabel->x()!=10)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(ui->logoLabel, "geometry");
        animation->setDuration(800);
        animation->setStartValue(ui->logoLabel->frameGeometry());
        animation->setEndValue(QRect(10,450,101,111));
        group->addAnimation(animation);
        group->start();
    }
    else
        timer->start(ui->spinBox->value());
}

void MainWindow::detectPlate()
{
    try
    {
        if (h<0)
        {
            timer->stop();
            return;
        }
        vector<RotatedRect> filteredPlates;
        vector<Mat> croppedPlates;
        //Tüm cerceveler bulunuyor
        detectAllRects(clearImages.at(h),filteredPlates,200);
        //Cerceveler en-boy a göre filtreleniyor (plaka cercevesi icin)
        filterByAspectRatio(filteredPlates,filteredPlates,3,6);
        //Cerceveler pix alanına göre filtreleniyor (plaka cercevesini bulmak için)
        filterByArea(filteredPlates,filteredPlates,1000 /*minArea.width()*minArea.height()*/,9000/*maxArea.width()*maxArea.height()*/);
        //Bulunan cerceveler resimden tek tek koparılıp ayrı bir diziye atılıyor
        getNecessaryParts(clearImages.at(h),croppedPlates,filteredPlates);
        // Artık plaka cerceveleri ile işimiz bitti silebiliriz.
        filteredPlates.clear();

        vector <unsigned int> rectSize;
        //Tüm plaka boxları dolanılıp yazı boxları bulunuyor ve
        //Her plaka boxundaki yazı boxu adedi bir diziye atılıyor
        //Daha sonra ençok yazı boxuna sahip olan plaka boxu belirlenecek
        //Diğer plaka olma ihtimali düşük olan boxlar elenmiş olacak
        for (unsigned int i=0;i<croppedPlates.size();i++)
        {
            vector<RotatedRect> filteredRects;
            detectAllRects(croppedPlates.at(i),filteredRects,200);
            filterByAspectRatio(filteredRects,filteredRects,1,2);
            filterByArea(filteredRects,filteredRects,200/*(minArea.width()*minArea.height())/13*/,400/*(maxArea.width()*maxArea.height())/13*/);
            filterByHeightDistanceRatio(filteredRects,filteredRects);
            rectSize.push_back(filteredRects.size());
            filteredRects.clear();
        }
        //En çok yazı boxu olan rectin indexi bulunuyor
        const unsigned int indexOfMax = distance(rectSize.begin(), max_element(rectSize.begin(),rectSize.end()));


        // En çok yazı boxu olan rect plaka resmi üzerinde yeniden rectler bulunuyor
        //detectAllRects(croppedPlates.at(indexOfMax),filteredText,200);
        //filterByAspectRatio(filteredText,filteredText,1,3);
        //filterByArea(filteredText,filteredText,150,400);
        //filterByHeightDistanceRatio(filteredText,filteredText);
        // Yeniden bulunana rectler plaka resmi üstüne çiziliyor
        //printRects(croppedPlates.at(indexOfMax),filteredText, Scalar( 10, 10, 120 ));

        ui->pictureLabel->setPixmap(cvMatToQPixmap(clearImages.at(h)));
        ui->pictureLabel_2->setPixmap(cvMatToQPixmap(croppedPlates.at(indexOfMax)));

        Mat clearedLocal;
        QString outText;
        getClearImage(croppedPlates.at(indexOfMax),clearedLocal);
        getTextFromImage(clearedLocal, outText);
        ui->label_5->setText(outText);
        h--;

    }

    catch(exception& e)
    {
        qDebug() << e.what() << h;
    }
}

void MainWindow::animFinished()
{
    timer->start(ui->spinBox->value());
    ui->copyrightLabel->setVisible(true);
}
