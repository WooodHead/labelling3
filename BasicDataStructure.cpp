
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

QImage* setMaskMap(IplImage* ocvImage, IplImage* mask)
{
    int height = ocvImage->height;
    int width = ocvImage->width;
    QImage *retImage = new QImage( width, height, QImage::Format_RGB32 );

    int R, G, B;

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            CvScalar color1 = cvGet2D(ocvImage, y, x);
            CvScalar color2 = cvGet2D(mask, y, x);

            if(ocvImage->nChannels == 3)
            {
                R = 0.6 * color1.val[2] + 0.4 * color2.val[2];
                G = 0.6 * color1.val[1] + 0.4 * color2.val[1];
                B = 0.6 * color1.val[0] + 0.4 * color2.val[0];
            }
            if(x > 0 && x < width-1 && y > 0 && y < height-1 && (color2.val[2] != 0 || color2.val[1] != 0 || color2.val[0] != 0))
            {
                CvScalar color3 = cvGet2D(mask, y, x-1);
                CvScalar color4 = cvGet2D(mask, y, x+1);
                CvScalar color5 = cvGet2D(mask, y-1, x);
                CvScalar color6 = cvGet2D(mask, y+1, x);

                if(isAllZero(color3) || isAllZero(color4) || isAllZero(color5) || isAllZero(color6))
                {
                    R = 0;
                    G = 255;
                    B = 255;
                }
            }
            retImage->setPixel(x, y, qRgb(R, G, B));
        }
    }

    return retImage;
}

bool isAllZero(CvScalar color)
{
    if(color.val[2] == 0 && color.val[1] == 0 && color.val[1] == 0)
        return true;
    else return false;
}
