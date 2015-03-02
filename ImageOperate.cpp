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


/*
QImage* CImgToQImage( CImg<unsigned char> *srcCImg )
{
	int imgWidth, imgHeight;
	int nChanels;

	imgWidth = srcCImg->dimx();
	imgHeight = srcCImg->dimy();

	nChanels = srcCImg->dim;

	QRgb pixelColor;
	int rgbArray[3];

	if ( nChanels == 1 )
	{
		QImage *retImage = new QImage( imgWidth, imgHeight, QImage::Format_RGB32 );

		for ( int row = 0; row < imgHeight; ++row )
		{
			for ( int col = 0; col < imgWidth; ++ col )
			{
				if ( srcCImg->data[ row * imgWidth + col ] == 0 )
				{
					pixelColor = qRgb( 0, 0, 0 );
					retImage->setPixel( col, row,  pixelColor );
				}
				else if ( srcCImg->data[ row * imgWidth + col ] == 255 )
				{
					pixelColor = qRgb( 255, 255, 255 );
					retImage->setPixel( col, row,  pixelColor );
				}
				
			}
		}

		return retImage;
	}
	
	else if ( nChanels == 3 )
	{
		QImage *retImage = new QImage( imgWidth, imgHeight, QImage::Format_RGB32);

		for ( int row = 0; row < imgHeight; ++row )
		{
			for ( int col = 0; col < imgWidth; ++ col )
			{
				for ( int k = 0; k < nChanels; ++k )
				{
					rgbArray[k] = srcCImg->data[ (row * imgWidth + col)*nChanels + k];
				}
				pixelColor = qRgb( rgbArray[0], rgbArray[1], rgbArray[2] );
				retImage->setPixel( col, row,  pixelColor );
			}
		}

		return retImage;
	}
	
	else 
	{
		QImage *retImage = new QImage;
		
		return retImage;
	}
}
*/

QImage* CombineTwoQImage( const QImage& lhsQImage, const QImage& rhsQImage, int widthGapSize = 20)
{
	int lhsWidth = lhsQImage.width();
	int rhsWidth = rhsQImage.width();
	int lhsHeight = lhsQImage.height();
	int rhsHeight = rhsQImage.height();

	int imgWidth =  lhsWidth + rhsWidth + widthGapSize;
	int imgHeight = max( lhsHeight, rhsHeight );

	QImage *retImage = new QImage( imgWidth, imgHeight,	QImage::Format_RGB32 );
	retImage->fill( 0 );

	for ( int row = 0; row < imgHeight; ++row )
	{
		for ( int col1 = 0; col1 < lhsWidth; ++col1 )
		{
			if ( row < lhsHeight )
			{
				retImage->setPixel( col1, row, lhsQImage.pixel( col1, row ) );
			}
		}

		for ( int col = 0; col < widthGapSize; ++col )
		{
			retImage->setPixel( lhsWidth+col, row, qRgb(255, 255, 255) );
		}

		for ( int col2 = 0; col2 < rhsWidth; ++col2 )
		{
			if ( row < rhsHeight )
			{
				retImage->setPixel( lhsWidth + col2 + widthGapSize, row, rhsQImage.pixel( col2, row ) );
			}
		}
	}

	return retImage;
}

inline int align(int size, int align)
{
	return (size + align - 1) & -align;
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

	int row, col;
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

/*
IplImage *QImageToIplImage(const QImage * qImage)
{
	int width = qImage->width();
	int height = qImage->height();

	CvSize Size;
	Size.height = height;
	Size.width = width;

	IplImage *charIplImageBuffer = cvCreateImage(Size, IPL_DEPTH_8U, 1);
	char *charTemp = (char *) charIplImageBuffer->imageData;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int index = y * width + x;
			charTemp[index] = (char) qGray(qImage->pixel(x, y));
		}
	}

	return charIplImageBuffer;
}

*/
/* Usage:
QImage *qi = IplImageToQImage(iplCurrentImage, &data);
QPixmap pix = QPixmap(*qi);
delete qi;
+
*/
/*

QImage *IplImageToQImage(const IplImage * iplImage, uchar **data, double mini, double maxi)
{
	uchar *qImageBuffer = NULL;

	int width = iplImage->width;

	/ * Note here that OpenCV image is stored so that each lined is
	32-bits aligned thus
	* explaining the necessity to "skip" the few last bytes of each
	line of OpenCV image buffer.
	* /
	int widthStep = iplImage->widthStep;
	int height = iplImage->height;

	switch (iplImage->depth)
	{
	case IPL_DEPTH_8U:
		if (iplImage->nChannels == 1)
		{
			/ * OpenCV image is stored with one byte grey pixel. We convert it
			to an 8 bit depth QImage.
			* /

			qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const uchar *iplImagePtr = (const uchar *) iplImage->imageData;

			for (int y = 0; y < height; y++)
			{
				// Copy line by line
				memcpy(QImagePtr, iplImagePtr, width);
				QImagePtr += width;
				iplImagePtr += widthStep;
			}

		}
		else if (iplImage->nChannels == 3)
		{
			/ * OpenCV image is stored with 3 byte color pixels (3 channels).
			We convert it to a 32 bit depth QImage.
			* /
			qImageBuffer = (uchar *) malloc(width*height*4*sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const uchar *iplImagePtr = (const uchar *) iplImage->imageData;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					// We cannot help but copy manually.
					QImagePtr[0] = iplImagePtr[0];
					QImagePtr[1] = iplImagePtr[1];
					QImagePtr[2] = iplImagePtr[2];
					QImagePtr[3] = 0;

					QImagePtr += 4;
					iplImagePtr += 3;
				}
				iplImagePtr += widthStep-3*width;
			}

		}
		else
		{
			qDebug("IplImageToQImage: image format is not supported : depth=8U and %d channels\n", iplImage->nChannels);
		}
		break;
	case IPL_DEPTH_16U:
		if (iplImage->nChannels == 1)
		{
			/ * OpenCV image is stored with 2 bytes grey pixel. We convert it
			to an 8 bit depth QImage.
			* /
			qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			//const uint16_t *iplImagePtr = (const uint16_t *);
			const unsigned int *iplImagePtr = (const unsigned int *)iplImage->imageData;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					// We take only the highest part of the 16 bit value. It is
					//similar to dividing by 256.
					*QImagePtr++ = ((*iplImagePtr++) >> 8);
				}
				iplImagePtr += widthStep/sizeof(unsigned int)-width;
			}
		}
		else
		{
			qDebug("IplImageToQImage: image format is not supported : depth=16U and %d channels\n", iplImage->nChannels);

		}
		break;
	case IPL_DEPTH_32F:
		if (iplImage->nChannels == 1)
		{
			/ * OpenCV image is stored with float (4 bytes) grey pixel. We
			convert it to an 8 bit depth QImage.
			* /
			qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const float *iplImagePtr = (const float *) iplImage->imageData;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					uchar p;
					float pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);
					if (pf < 0) p = 0;
					else if (pf > 255) p = 255;
					else p = (uchar) pf;

					*QImagePtr++ = p;
				}
				iplImagePtr += widthStep/sizeof(float)-width;
			}
		}
		else
		{
			qDebug("IplImageToQImage: image format is not supported : depth=32F and %d channels\n", iplImage->nChannels);
		}
		break;
	case IPL_DEPTH_64F:
		if (iplImage->nChannels == 1)
		{
			/ * OpenCV image is stored with double (8 bytes) grey pixel. We
			convert it to an 8 bit depth QImage.
			* /
			qImageBuffer = (uchar *) malloc(width*height*sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const double *iplImagePtr = (const double *) iplImage->imageData;
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					uchar p;
					double pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);

					if (pf < 0) p = 0;
					else if (pf > 255) p = 255;
					else p = (uchar) pf;

					*QImagePtr++ = p;
				}
				iplImagePtr += widthStep/sizeof(double)-width;
			}
		}
		else
		{
			qDebug("IplImageToQImage: image format is not supported : depth=64F and %d channels\n", iplImage->nChannels);
		}
		break;
	default:
		qDebug("IplImageToQImage: image format is not supported : depth=%d and %d channels\n", iplImage->depth, iplImage->nChannels);
	}

	QImage *qImage;
	QVector<QRgb> vcolorTable;
	if (iplImage->nChannels == 1)
	{
		// We should check who is going to destroy this allocation.
		QRgb *colorTable = new QRgb[256];
		for (int i = 0; i < 256; i++)
		{
			colorTable[i] = qRgb(i, i, i);
			vcolorTable[i] = colorTable[i];
		}
		qImage = new QImage(qImageBuffer, width, height, QImage::Format_Indexed8);
		qImage->setColorTable(vcolorTable);
	}
	else
	{
		qImage = new QImage(qImageBuffer, width, height, QImage::Format_RGB32);
	}
	*data = qImageBuffer;
	return qImage;
}

*/
