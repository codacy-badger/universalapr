#ifndef UNIVERSALAPR_H
#define UNIVERSALAPR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <cmath>

using namespace cv;
using namespace std;

void getNecessaryParts(const Mat& src,
                       vector<Mat>& dst,
                       const vector<RotatedRect>& rects);
void filterByHeightDistanceRatio(const vector<RotatedRect>& src,
                                 vector<RotatedRect>& dst,
                                 const float ratioMin=0.8,
                                 const float ratioMax=3.2);
void filterByArea(const vector<RotatedRect>& src,
                  vector<RotatedRect>& dst,
                  const register unsigned short areaMin=1000,
                  const register unsigned short areaMax=9000);
void filterByAspectRatio(const vector<RotatedRect>& src,
                         vector<RotatedRect>& dst,
                         register const unsigned char ratioMin=3,
                         register const unsigned char ratioMax=6);
void detectAllRects(const Mat& picture,
                    vector<RotatedRect>& rect,
                    const unsigned char cannyFactor=200);
inline void detectRect(const Mat& src_gray,
                       vector<RotatedRect>& rect,
                       const unsigned char cannyFactor=200);
void printRects(Mat& img,
                const vector<RotatedRect>& rects,
                const Scalar& color,
                register const unsigned char lineLenth=2);

#endif // UNIVERSALAPR_H
