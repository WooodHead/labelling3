#ifndef _BASEDATASTRUCTURE_H_
#define _BASEDATASTRUCTURE_H_

#include <vector>
#include <QImage>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <opencv2/opencv.hpp>

extern int colorTable[30];

cv::Mat getLabelMap(QImage* labelImage);
QImage* setMaskMap(IplImage* ocvImage, IplImage* mask);
bool isAllZero(CvScalar color);

class PointD
{
public:
	PointD( void )
	{
		// doing nothing
	}
	PointD( double x, double y )
	{
		_x = x;
		_y = y;
	}

	~PointD(){ }

	PointD( const PointD& point )
	{	
		_x = point._x;
		_y = point._y;
	}

	PointD& operator=( const PointD& point )
	{
		_x = point._x;
		_y = point._y;
		return *this;
	}

	friend PointD operator-( const PointD& point1, const PointD& point2 );

public:
	double _x;
	double _y;
};

PointD operator-( const PointD& point1, const PointD& point2 );

static inline double pabs(const PointD& point);

double minPointDistance( int& selectIndex, const PointD& point,std::vector<PointD> pointList );

# endif	

