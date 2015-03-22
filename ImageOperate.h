#ifndef IMAGE_FORMAT_CONVERT_H
#define IMAGE_FORMAT_CONVERT_H

#include <list>

#include <opencv/cv.h>

class QImage;
using std::list;

QImage* IplImageToQImage( const IplImage* iplImage );

void getBinMask( cv::Mat& comMask, cv::Mat& binMask );
cv::Mat grabCutSeg( cv::Mat im, cv::Mat& mask, cv::Rect rect, int mode );
IplImage* QImageToIplImage( const QImage& qImage );


#endif
