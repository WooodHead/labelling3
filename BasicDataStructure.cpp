
#include <cmath>
#include "BasicDataStructure.h"


PointD operator-( const PointD& point1, const PointD& point2 )
{
	PointD retPoint;

	retPoint._x = point1._x - point2._x;
	retPoint._y = point1._y - point2._y;

	return retPoint;
}


static inline double pabs(const PointD& point)
{
	return sqrt(point._x*point._x+point._y*point._y);
}


double minPointDistance( int& selectIndex, const PointD& point,std::vector<PointD> pointList )
{
	double minDistance = 1000;
	double r;

	int num = pointList.size();

	for ( int i = 0; i < num; ++i )
	{	
		r = pabs( point - pointList[i] );

		if ( r < minDistance )
		{
			selectIndex = i;
			minDistance = r;
		}

	}

	return minDistance;
}

int colorTable[30] = 
{
	100, 149, 23,//cornflowerblue
	255,127, 80,//coral
	127, 255, 0,//chartreuse
	144, 43, 226,//blueviolet
	85, 107, 47,//darkolivegreen
	200, 255, 200,//honeydew
	47, 79, 79,//darkslategray
	220, 20, 60,//crimson
	255, 146, 0,//darkorange
	134, 134,134//gray
};

char* getLabelMap(QImage* labelImage, int objNum)
{
	int height = labelImage->height();
	int width = labelImage->width();	
	int num[2] = {0, 0};
	char *labelMap = new char[height*width];
	memset(labelMap, -1, sizeof(char)*height*width);	

	char* _labelMap = labelMap;
	
	int R, G, B;
	QRgb cur_rgb; 
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
            cur_rgb = labelImage->pixel(j, i);
			R = qRed(cur_rgb);
			G = qGreen(cur_rgb);
			B = qBlue(cur_rgb);
			for (int k = 0; k < objNum; k++)
			{
				if (R==colorTable[3*k]&&G==colorTable[3*k+1]&&B==colorTable[3*k+2])
				{
					*_labelMap = k;
					num[k]++;
					break;
				}
			}
			_labelMap++;
		} 
	}
	if (num[0]<50||num[1]<50)
	{
		return NULL;
	}
	return labelMap;
}

cv::Mat getLabelMap(QImage* labelImage)
{
    if( !labelImage ) return cv::Mat();

    cv::Mat mask;
    mask.create(labelImage->height(), labelImage->width(), CV_8UC1);
    mask.setTo(cv::GC_PR_BGD);

    int R, G, B;
    QRgb curRGB;
    int num[2] = {0, 0};

    for(int i = 0; i < labelImage->height(); i++)
    {
        for(int j = 0; j < labelImage->width(); j++)
        {
            curRGB = labelImage->pixel(j, i);

            R = qRed(curRGB);
            G = qGreen(curRGB);
            B = qBlue(curRGB);

            if(R == colorTable[3] && G == colorTable[4] && B == colorTable[5]) // foreground
            {
                mask.at<uchar>(i, j) = cv::GC_FGD;
                num[0]++;
            }
            else if(R == colorTable[0] && G == colorTable[1] && B == colorTable[2]) // background
            {
                mask.at<uchar>(i, j) = cv::GC_BGD;
                num[1]++;
            }
        }
    }

    if(num[0] < 50 || num[1] < 50) return cv::Mat();

    return mask;
}



QImage* setLabelMap(IplImage* ocvImage, char* labelMap)
{

	int height = ocvImage->height;
	int width = ocvImage->width;
    //int widthStep = ocvImage->widthStep;
	QImage *retImage = new QImage( width, height, QImage::Format_RGB32 );
	
	char* _labelMap = labelMap;
    //char* _image = ocvImage->imageData;
	int R, G, B;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			CvScalar color = cvGet2D( ocvImage, y, x );
			if ( ocvImage->nChannels == 1 )
			{
				int val = color.val[0];
				R = 0.5 * val + 0.5 * colorTable[3 * (*_labelMap)];
				G = 0.5 * val + 0.5 * colorTable[3 * (*_labelMap)+1];
				B = 0.5 * val + 0.5 * colorTable[3 * (*_labelMap)+2];
				retImage->setPixel( x, y, qRgb(R, G, B) );
			}
			else 
			{
				R = color.val[2];
				G = color.val[1];
				B = color.val[0];
				R = 0.6 * R + 0.4 * colorTable[3 * (*_labelMap)];
				G = 0.6 * G + 0.4 * colorTable[3 * (*_labelMap)+1];
				B = 0.6 * B + 0.4 * colorTable[3 * (*_labelMap)+2];
				retImage->setPixel( x, y, qRgb(R, G, B) );
			}
			if(x>0 && *_labelMap != *(_labelMap-1)
				||y>0 && *_labelMap != *(_labelMap-width)
				||x<width-1 && *_labelMap != *(_labelMap+1)
				||y<height-1 && *_labelMap != *(_labelMap+width))
			{
				R = 0;
				G = 255;
				B = 255;
				retImage->setPixel( x, y, qRgb(R, G, B) );
			}			
			_labelMap++;
			//_image+=3;
		} 
		//_image+= widthStep-width*3 ;
	}
	return retImage;
}
