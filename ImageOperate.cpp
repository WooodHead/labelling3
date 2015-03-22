#include <cmath>
#include <cstring>
#include <iostream>

#include <QtGui>
#include <QImage>
#include <QVector>

#include "ImageOperate.h"

//using std::string;
using std::iostream;


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

void getBinMask( cv::Mat& comMask, cv::Mat& binMask )
{
	if( comMask.empty() || comMask.type()!=CV_8UC1 )
		CV_Error( CV_StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)" );
	if( binMask.empty() || binMask.rows!=comMask.rows || binMask.cols!=comMask.cols )
		binMask.create( comMask.size(), CV_8UC1 );
	binMask = comMask & 1;
}

cv::Mat grabCutSeg( cv::Mat im, cv::Mat &mask, cv::Rect rect, int mode )
{
	cv::Mat bg, fg, binMask;

	if(mode == 0)
	{
		mask.create(im.size(), CV_8UC1);
		grabCut( im, mask, rect, bg, fg, 1, cv::GC_INIT_WITH_RECT );
		getBinMask(mask, binMask);
	}
	else if(mode == 1)
	{
		grabCut( im, mask, rect, bg, fg, 1, cv::GC_INIT_WITH_MASK );
		getBinMask(mask, binMask);
	}

	cv::Mat res;
	im.copyTo(res, binMask);

	return res;
}

QImage* IplImageToQImage( const IplImage* iplImage )
{
	if ( !CV_IS_IMAGE(iplImage) )
	{
		return NULL;
	}

	int w = iplImage->width;
	int h = iplImage->height;

	QImage* qImage = new QImage( w, h, QImage::Format_RGB32 );

	int row, col;
	for ( row = 0; row < h; ++row )
	{
		for ( col = 0; col < w; ++col )
		{
			CvScalar color = cvGet2D( iplImage, row, col );

			if ( iplImage->nChannels == 1 )
			{
				int val = color.val[0];
				qImage->setPixel( col, row, qRgb(val, val, val) );
			}
			else 
			{
				int r = color.val[2];
				int g = color.val[1];
				int b = color.val[0];
				qImage->setPixel( col, row, qRgb(r, g, b) );
			}
		}
	}

	if ( iplImage->origin != IPL_ORIGIN_TL )
	{
		*qImage = qImage->mirrored( false, true );
	}

	return qImage;
}

IplImage* QImageToIplImage( const QImage& qImage )
{
    if ( qImage.isNull() )
    {
        return NULL;
    }

    int w = qImage.width();
    int h = qImage.height();

    IplImage* pIplImage = cvCreateImage( cvSize(w, h), IPL_DEPTH_8U, 3);

    if ( !CV_IS_IMAGE(pIplImage) )
    {
        return NULL;
    }

    pIplImage->origin = IPL_ORIGIN_TL;

    int row;
    QRgb rgb;
    uchar* p_l;
    uchar* p_r = (uchar*)pIplImage->imageData;
    //if ( qImage.isGrayscale() )
    //{
    //	for (row = 0; row < h; row++)
    //	{
    //		p_l = p_r;
    //		for (int col = 0; col < w; col++)
    //		{
    //			*p_l = qGray(rgb);
    //			p_l++;
    //		}
    //		p_r += pIplImage->widthStep;
 //		}
    //}
 //  else
 //  {
       for (row = 0; row < h; row++)
       {
           p_l = p_r;
           for (int col = 0; col < w; col++)
           {
               rgb = qImage.pixel(col, row);
               *p_l =       qBlue(rgb);
               *(p_l+1) = qGreen(rgb);
               *(p_l+2) = qRed(rgb);
               p_l+=3;
           }
           p_r += pIplImage->widthStep;
       }
   //}

    //for ( row = 0; row < h; ++row )
    //{
    //	for ( col = 0; col < w; ++col )
    //	{
    //		QRgb rgb = qImage.pixel(col, row);

    //		if ( qImage.isGrayscale() )
    //		{
    //			cvSet2D( pIplImage, row, col, CV_RGB( qGray(rgb), 0, 0 ));
    //		}
    //		else
    //		{
    //			cvSet2D( pIplImage, row, col, CV_RGB(qRed(rgb), qGreen(rgb), qBlue(rgb)) );
    //		}
    //	}
    //}

    return pIplImage;
}
