#ifndef IMAGE_FORMAT_CONVERT_H
#define IMAGE_FORMAT_CONVERT_H

#include <list>

//#include "CImg.h"
#include <opencv/cv.h>

class QImage;
//using cimg_library::CImg;
using std::list;

QImage* IplImageToQImage( const IplImage* iplImage );

IplImage* QImageToIplImage( const QImage& qImage );

//QImage*	CImgToQImage( CImg<unsigned char>*srcCImg );
//
//QImage*	CombineTwoQImage( const QImage& lhsQImage, const QImage& rhsQImage, int widthGapSize );

void getBinMask( cv::Mat& comMask, cv::Mat& binMask );
cv::Mat grabCutSeg( cv::Mat im, cv::Mat& mask, cv::Rect rect, int mode );


#endif
