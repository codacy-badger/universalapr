#ifndef ZCLEAR
#define ZCLEAR
#include <QStack>
#include <QVector>
#include <QList>
#include <QImage>
#include <QPoint>
#include <QRgb>
#include <QDebug>
#include <QFile>
#include <QtMath>
#include <QtAlgorithms>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>

using namespace cv;
struct ZPart
{
    public:
        unsigned short int maxLength;
        unsigned char color;
        QPoint centralPixel;
        QVector<uint> edgePixels;
        QVector<QPoint> pixels;
};
static void Mat2QImage(const Mat& src, QImage* dst)
{
    dst= new QImage((const uchar *) src.data, src.cols, src.rows, src.step, QImage::Format_Indexed8);
    dst->bits(); // enforce deep copy, see documentation
}
static void QImage2Mat(QImage const& src, Mat* dst)
{
    QImage temp = src.copy();
    dst = new Mat(temp.height(),temp.width(),CV_8UC3,(uchar*)temp.bits(),temp.bytesPerLine());
    cvtColor(*dst, *dst,CV_BGR2RGB);
}
static void Trash(const Mat& src, Mat& dst)
{
    // Smooth to picture
    //bilateralFilter(src,dst, 30, 30*2, 30/2);
    // Convert to grayscale
    cvtColor( src, dst, CV_BGR2GRAY );
    // Apply Histogram Equalization (Grayscale fix to normal)
    //equalizeHist( dst, dst );
    //Tresholding picture (Trash to Black-White)
    adaptiveThreshold( dst, dst, 255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,17, 9 );
}
static void detectParts(Mat& src, QVector<ZPart>& parts, const uint x=0, const uint y=0)
{
    ZPart part;
    parts.append(part);
    QStack<uint> stack; /* Piksel surme icin stack */
    parts.last().pixels.append(QPoint(x,y)); /* Part a ilk pixelini ekledik */
    parts.last().color=src.at<uchar>(y,x); /* Part a rengini atadik ilk pikselinden */
    stack.push(0); /* ilk pikselimizin indexini stack a da koyduk */
    src.at<uchar>(y,x)=127; /* İlk piksele ayagimizi bastik ve rengini gri yaptik, artik baslayabiliriz */

    while (!stack.isEmpty())
    {
        if (src.at<uchar>(parts.last().pixels.at(stack.top()).y(),parts.last().pixels.at(stack.top()).x()+1)==parts.last().color &&
            parts.last().pixels.at(stack.top()).x()+1<src.cols)
        {
            parts.last().pixels.append(QPoint(parts.last().pixels.at(stack.top()).x()+1,parts.last().pixels.at(stack.top()).y()));
            stack.push(parts.last().pixels.size()-1);
            src.at<uchar>(parts.last().pixels.last().y(),parts.last().pixels.last().x())=127;
        }

        else if (src.at<uchar>(parts.last().pixels.at(stack.top()).y()+1,parts.last().pixels.at(stack.top()).x())==parts.last().color &&
                 parts.last().pixels.at(stack.top()).y()+1<src.rows)
        {
            parts.last().pixels.append(QPoint(parts.last().pixels.at(stack.top()).x(),parts.last().pixels.at(stack.top()).y()+1));
            stack.push(parts.last().pixels.size()-1);
            src.at<uchar>(parts.last().pixels.last().y(),parts.last().pixels.last().x())=127;
        }

        else if (src.at<uchar>(parts.last().pixels.at(stack.top()).y(),parts.last().pixels.at(stack.top()).x()-1)==parts.last().color &&
                 parts.last().pixels.at(stack.top()).x()-1>=0)
        {
            parts.last().pixels.append(QPoint(parts.last().pixels.at(stack.top()).x()-1,parts.last().pixels.at(stack.top()).y()));
            stack.push(parts.last().pixels.size()-1);
            src.at<uchar>(parts.last().pixels.last().y(),parts.last().pixels.last().x())=127;
        }

        else if (src.at<uchar>(parts.last().pixels.at(stack.top()).y()-1,parts.last().pixels.at(stack.top()).x())==parts.last().color &&
                 parts.last().pixels.at(stack.top()).y()-1>=0)
        {
            parts.last().pixels.append(QPoint(parts.last().pixels.at(stack.top()).x(),parts.last().pixels.at(stack.top()).y()-1));
            stack.push(parts.last().pixels.size()-1);
            src.at<uchar>(parts.last().pixels.last().y(),parts.last().pixels.last().x())=127;
        }
        else
        {
            if (!stack.isEmpty())
                stack.pop();
            else
                break;
        }
    }

    for (int i=0;i<src.cols;i++)
        for (int j=0;j<src.rows;j++)
            if (src.at<uchar>(j,i)!=127)
                detectParts(src,parts,i,j);
}
static void detectCentralPix(QVector<ZPart>& parts)
{
    /* Belli bir buyuklukten fazla pixeli olan parcalar bulunuyor */
#define PIX_UP_LIMIT 300
#define PIX_DOWN_LIMIT 30
    QVector<uint> indexOfDel;
    for (int j=0;j<parts.size();j++)
        if (parts.at(j).pixels.size()>PIX_UP_LIMIT || parts.at(j).pixels.size()<PIX_DOWN_LIMIT)
            indexOfDel.append(j);
    /* Belli bir buyuklukten fazla pixeli olan parcalar siliniyor */
    for (int k=0;k<indexOfDel.size();k++)
    {
        parts.remove(indexOfDel.at(k)-k);
    }
    indexOfDel.clear();

    /* Tum parcalar gezilerek merkezi noktalari bulunuyor */
    int xTop, yTop;
    for (int j=0;j<parts.size();j++)
    {
        xTop=0;
        yTop=0;
        for (int i=0;i<parts.at(j).pixels.size();i++)
        {
            xTop += parts.at(j).pixels.at(i).x();
            yTop += parts.at(j).pixels.at(i).y();
        }
        parts[j].centralPixel.setX(xTop/(parts.at(j).pixels.size()));
        parts[j].centralPixel.setY(yTop/(parts.at(j).pixels.size()));
    }
}
static void detectLength(QVector<ZPart>& parts)
{
    for (int j=0;j<parts.size();j++)
    {
        /* Parcanin tum pixelleri buffera kopyalandi */
        QVector<QPoint> pixList;
        for (int z=0;z<parts.at(j).pixels.size();z++)
            pixList.append(parts.at(j).pixels.at(z));
        /* Tum pixellerin karsilikli hipotenusleri burada tutlacak */
        QList<uint> hipotenus;
        for (int k=0;k<parts.at(j).pixels.size();k++)
        {
            pixList.removeFirst();
            for (int l=0;l<pixList.size();l++)
            {
                hipotenus.append(qSqrt(qPow(parts.at(j).pixels.at(k).x()-pixList.at(l).x(), 2) +
                                       qPow(parts.at(j).pixels.at(k).y()-pixList.at(l).y(), 2) ) );
            }
        }
        qSort(hipotenus);
        parts[j].maxLength=hipotenus.last();
        hipotenus.clear();
    }

    /* Belli bir uzunluktan fazla pixeli olan parcalar bulunuyor*/
#define LENGTH_UP_LIMIT 80
#define LENGTH_DOWN_LIMIT 8
    QVector<uint> indexOfDel;
    for (int j=0;j<parts.size();j++)
        if (parts.at(j).maxLength>LENGTH_UP_LIMIT || parts.at(j).maxLength<LENGTH_DOWN_LIMIT)
            indexOfDel.append(j);
    /* Belli bir uzunluktan fazla pixeli olan parcalar siliniyor*/
    for (int k=0;k<indexOfDel.size();k++)
    {
        parts.remove(indexOfDel.at(k)-k);
    }
    indexOfDel.clear();
}
static void lengthFilter(QVector<ZPart>& parts)
{
    /* Parcalarin maxLength leri arasinda bir oranlama yapiyor
     * Ayni zamanda aralarinda da belli aradan fazla mesafe varsa da kontrol et
     * Filtreye uyan parcalarin indexi listeye aktariliyor */

    QList<uint> indexOfStatic;
    for (int j=0;j<parts.size();j++)
        for (int i=0;i<parts.size();i++)
        {
            if (i!=j)
                if (qSqrt(qPow( parts.at(j).centralPixel.x()-parts.at(i).centralPixel.x() ,2)+qPow( parts.at(j).centralPixel.y()-parts.at(i).centralPixel.y() ,2))<(parts.at(j).maxLength*1.2))
                    if ( ((float)(parts.at(j).maxLength)/(parts.at(i).maxLength)) > 0.8 &&
                         ((float)(parts.at(j).maxLength)/(parts.at(i).maxLength)) < 1.2 )
                        indexOfStatic.append(j);
        }

    /* Listedeki duplicate elemanlar siliniyor */
    QSet<uint> set=indexOfStatic.toSet();
    indexOfStatic.clear();
    indexOfStatic = set.toList();

    /* Silinmeyenler cikarilip silinecekler belirleniyor */
    QVector<uint> indexOfDel;
    for (int k=0;k<parts.size();k++)
        if (indexOfStatic.indexOf(k)==-1)
        {
            indexOfDel.append(k);
        }
    indexOfStatic.clear();

    /* Silinecekler siliniyor */
    for (int k=0;k<indexOfDel.size();k++)
    {
        parts.remove(indexOfDel.at(k)-k);
    }
    indexOfDel.clear();
}

static void getClearImage(const Mat& src, Mat& dst)
{
    QVector<ZPart> parteDePorte;
    int x=1,y=1;
    Trash(src,dst);
    detectParts(dst,parteDePorte,x,y);
    detectCentralPix(parteDePorte);
    detectLength(parteDePorte);
    lengthFilter(parteDePorte);
    /* parcalar cizdiriliyor*/
    for (int j=0;j<parteDePorte.size();j++)
        for (int i=0;i<parteDePorte.at(j).pixels.size();i++)
            dst.at<uchar>(parteDePorte.at(j).pixels.at(i).y(),parteDePorte.at(j).pixels.at(i).x())=0;
    threshold( dst, dst, 30, 255, THRESH_BINARY );
    parteDePorte.clear();
}

#endif // ZCLEAR
