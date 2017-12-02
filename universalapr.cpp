#include <universalapr.h>

void getNecessaryParts(const Mat& src,
                       vector<Mat>& dst,
                       const vector<RotatedRect>& rects)
{

    Mat* dstLocal, * crpLocal;
    unsigned int sy=0;
    unsigned int rz=rects.size();
    unsigned int *indexOfStatic = new unsigned int[rz];

    for (unsigned int i=0;i<rz;i++)
    {
        //cols sat�r sayisi, x de satiri belirtir
        if ( rects.at(i).boundingRect().br().x <= src.cols && rects.at(i).boundingRect().tl().x >= 0 &&
             rects.at(i).boundingRect().br().y <= src.rows && rects.at(i).boundingRect().tl().y >= 0 )
        {
            indexOfStatic[sy]=i;
            sy++;
        }
    }

    // Plakalar�n boundingBoxlar� ��kart�l�yor
    dstLocal = new Mat[sy];
    for (unsigned int j=0;j<sy;j++)
        dstLocal[j]=src(rects.at(indexOfStatic[j]).boundingRect());
    // �lgili boundingBoxlarda sadece plaka �ercevesi Bulunup k�rp�l�yor
    crpLocal = new Mat[sy];
    Mat M, rotated;
    float angle;
    Size rs;
    Point2f newPt;
    RotatedRect rect;
    for (unsigned int g=0;g<sy;g++)
    {
        newPt = Point2f(rects.at(indexOfStatic[g]).center.x - rects.at(indexOfStatic[g]).boundingRect().tl().x,
                        rects.at(indexOfStatic[g]).center.y - rects.at(indexOfStatic[g]).boundingRect().tl().y);
        rect = RotatedRect( newPt, rects.at(indexOfStatic[g]).size, rects.at(indexOfStatic[g]).angle );
        angle = rect.angle;
        rs = rect.size;
        if (rect.angle < -45.)
        {
            angle += 90.0;
            swap(rs.width, rs.height);
        }
        M = getRotationMatrix2D(rect.center, angle, 1.0);
        warpAffine(dstLocal[g], rotated, M, dstLocal[g].size(), INTER_CUBIC);
        getRectSubPix(rotated, rs, rect.center, crpLocal[g]);
    }

    dst.clear();
    dst.reserve( sy );
    dst.insert( dst.begin(), crpLocal, crpLocal + sy );

    delete[] indexOfStatic;
    delete[] dstLocal;
    delete[] crpLocal;
}

void filterByHeightDistanceRatio(const vector<RotatedRect>& src,
                                 vector<RotatedRect>& dst,
                                 const float ratioMin,
                                 const float ratioMax)
{
    if (&src==&dst)
    {
        bool test;
        unsigned int sy=0;
        unsigned int sz=src.size();
        unsigned int *indexOfDel = new unsigned int[sz];

        for( unsigned int i = 0; i<src.size(); i++ )
        {
            test=false;
            for( unsigned int j = 0; j< src.size(); j++ )
            {
                float heighti = ((float)(src.at(i).size.width)/(src.at(i).size.height) < 1) ?
                                    (float)(src.at(i).size.height):
                                    (float)(src.at(i).size.width);
                float heightj = ((float)(src.at(j).size.width)/(src.at(j).size.height) < 1) ?
                                    (float)(src.at(j).size.height):
                                    (float)(src.at(j).size.width);
                float height = (heighti+heightj)/2;

                float differenceOfCenters = sqrtf( powf(src.at(i).center.x - src.at(j).center.x , 2 ) +
                                                   powf(src.at(i).center.y - src.at(j).center.y , 2) );

                if (height/differenceOfCenters <= ratioMax && height/differenceOfCenters >= ratioMin)
                {
                    test=true;
                    break;
                }
            }

            if (test==false)
            {
                indexOfDel[sy]=i;
                sy++;
            }
        }
        for (unsigned int j=0;j<sy;j++)
            dst.erase(dst.begin()+indexOfDel[j]-j);

        delete[] indexOfDel;
    }
    else
        for( unsigned int i = 0; i<src.size(); i++ )
        {
            for( unsigned int j = 0; j< src.size(); j++ )
            {
                float heighti = ((float)(src.at(i).size.width)/(src.at(i).size.height) < 1) ?
                                    (float)(src.at(i).size.height):
                                    (float)(src.at(i).size.width);
                float heightj = ((float)(src.at(j).size.width)/(src.at(j).size.height) < 1) ?
                                    (float)(src.at(j).size.height):
                                    (float)(src.at(j).size.width);
                float height = (heighti+heightj)/2;

                float differenceOfCenters = sqrtf( powf(src.at(i).center.x - src.at(j).center.x , 2 ) +
                                                   powf(src.at(i).center.y - src.at(j).center.y , 2) );
                if (height/differenceOfCenters <= ratioMax && height/differenceOfCenters >= ratioMin)
                {
                    bool test=false;
                    for (unsigned int c=0; c<dst.size();c++)
                    {
                        if (dst.at(c).center==src.at(i).center && dst.at(c).size == src.at(i).size)
                            test=true;
                    }
                    if (!test && i!=j)
                        dst.push_back(src.at(i));
                }
            }
        }
}

void filterByArea(const vector<RotatedRect>& src,
                  vector<RotatedRect>& dst,
                  const unsigned short areaMin,
                  const unsigned short areaMax)
{
    unsigned int sz = src.size();
    float area;
    unsigned int sy=0;
    unsigned int *indexOfStatic = new unsigned int[sz];
    RotatedRect *dstLocal;
    if (&src==&dst)
    {
        for( unsigned int i = 0; i< sz; i++ )
        {
            area = src.at(i).size.width * src.at(i).size.height;
            if( area<=areaMax && area>=areaMin )
            {
                indexOfStatic[sy]=i;
                sy++;
            }
        }
        dstLocal=new RotatedRect[sy];
        for (unsigned int j=0;j<sy;j++)
            dstLocal[j]=src.at(indexOfStatic[j]);
        dst.clear();
        dst.reserve( sy );
        dst.insert( dst.begin(), dstLocal, dstLocal + sy );
    }
    else
    {
        for( unsigned int i = 0; i< sz; i++ )
        {
            area = src.at(i).size.width * src.at(i).size.height;
            if ( area<=areaMax && area>=areaMin )
            {
                indexOfStatic[sy]=i;
                sy++;
            }
        }
        dstLocal=new RotatedRect[sy];
        for (unsigned int j=0;j<sy;j++)
            dstLocal[j]=src.at(indexOfStatic[j]);
        dst.reserve( dst.size() + sy );
        dst.insert( dst.end(), dstLocal, dstLocal + sy );
    }

    delete[] indexOfStatic;
    delete[] dstLocal;
}

void filterByAspectRatio(const vector<RotatedRect>& src,
                         vector<RotatedRect>& dst,
                         const unsigned char ratioMin,
                         const unsigned char ratioMax)
{

    unsigned int sz = src.size();
    float aspect_ratio;
    unsigned int *indexOfStatic = new unsigned int[sz];
    RotatedRect *dstLocal;
    unsigned int sy=0;
    if (&src==&dst)
    {
        for( unsigned int i = 0; i< sz; i++ )
        {
            aspect_ratio = ((float)(src.at(i).size.width)/(src.at(i).size.height) < 1) ?
                               (float)(src.at(i).size.height)/(src.at(i).size.width) :
                               (float)(src.at(i).size.width)/(src.at(i).size.height);
            if( aspect_ratio<=ratioMax && aspect_ratio>=ratioMin )
            {
                indexOfStatic[sy]=i;
                sy++;
            }
        }
        dstLocal=new RotatedRect[sy];
        for (unsigned int j=0;j<sy;j++)
            dstLocal[j]=src.at(indexOfStatic[j]);
        dst.clear();
        dst.reserve( sy );
        dst.insert( dst.begin(), dstLocal, dstLocal + sy );
    }
    else
    {
        for( unsigned int i = 0; i< sz; i++ )
        {
            aspect_ratio = ((float)(src.at(i).size.width)/(src.at(i).size.height) < 1) ?
                               (float)(src.at(i).size.height)/(src.at(i).size.width) :
                               (float)(src.at(i).size.width)/(src.at(i).size.height);
            if ( aspect_ratio<=ratioMax && aspect_ratio>=ratioMin )
            {
                indexOfStatic[sy]=i;
                sy++;
            }
        }
        dstLocal = new RotatedRect[sy];
        for (unsigned int j=0;j<sy;j++)
            dstLocal[j]=src.at(indexOfStatic[j]);
        dst.reserve( dst.size() + sy );
        dst.insert( dst.end(), dstLocal, dstLocal + sy );
    }

    delete[] indexOfStatic;
    delete[] dstLocal;
}


void detectAllRects(const Mat& picture,
                    vector<RotatedRect>& rect,
                    const unsigned char cannyFactor)
{
    Mat src_gray, trashed;
    cvtColor( picture, src_gray, CV_BGR2GRAY );

    adaptiveThreshold( src_gray, trashed, 255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,17,0);
    detectRect(trashed, rect, cannyFactor);

    adaptiveThreshold( src_gray, trashed, 255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,51,0);
    detectRect(trashed, rect, cannyFactor);

    threshold( src_gray, trashed, 30, 255, THRESH_BINARY );
    detectRect(trashed, rect, cannyFactor);

    threshold( src_gray, trashed, 60, 255, THRESH_BINARY );
    detectRect(trashed, rect, cannyFactor);

    threshold( src_gray, trashed, 90, 255, THRESH_BINARY );
    detectRect(trashed, rect, cannyFactor);

    threshold( src_gray, trashed, 120, 255, THRESH_BINARY );
    detectRect(trashed, rect, cannyFactor);

    threshold( src_gray, trashed, 150, 255, THRESH_BINARY );
    detectRect(trashed, rect, cannyFactor);

    /////////////////////////////////////////////////////////

    equalizeHist( src_gray, trashed );
    threshold( trashed, trashed, 30, 255, THRESH_BINARY );
    detectRect(trashed, rect, cannyFactor);

    equalizeHist( src_gray, trashed );
    threshold( trashed, trashed, 60, 255, THRESH_BINARY );
    detectRect(trashed, rect, cannyFactor);

    equalizeHist( src_gray, trashed );
    threshold( trashed, trashed, 90, 255, THRESH_BINARY );
    detectRect(trashed, rect, cannyFactor);

    equalizeHist( src_gray, trashed );
    threshold( trashed, trashed, 120, 255, THRESH_BINARY );
    detectRect(trashed, rect, cannyFactor);

    equalizeHist( src_gray, trashed );
    threshold( trashed, trashed, 150, 255, THRESH_BINARY );
    detectRect(trashed, rect, cannyFactor);
}

inline void detectRect(const Mat& src_gray,
                       vector<RotatedRect>& rect,
                       const unsigned char cannyFactor)
{
    vector< vector<Point> > contours;
    Mat canny_output;
    Canny( src_gray, canny_output, (int)cannyFactor, (int)cannyFactor*2);
    findContours( canny_output, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    unsigned int cs=contours.size();
    RotatedRect *rectLocal=new RotatedRect[cs];
    for( unsigned int i = 0; i < cs; i++ )
        rectLocal[i]= minAreaRect( Mat(contours[i]) ) ;
    // Vector kapasitesi cs kadar arttiriliyor
    rect.reserve( rect.size() + cs );
    // yerel rect dizisi vectorun sonuna ekleniyor
    rect.insert( rect.end(), rectLocal, rectLocal+cs );
    delete[] rectLocal;
}

void printRects(Mat& img,
                const vector<RotatedRect>& rects,
                const Scalar& color,
                const unsigned char lineLenth)
{
    Point2f rect_points[4];
    unsigned int rs = rects.size();
    for(unsigned int i = 0; i< rs; i++ )
    {
        rects[i].points( rect_points );
        for( unsigned int j = 0; j < 4; j++ )
            line( img , rect_points[j], rect_points[(j+1)%4], color, (int)lineLenth, 8 );
    }
}
