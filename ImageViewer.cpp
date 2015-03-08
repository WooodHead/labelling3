/****************************************************************************
**
** Copyright (C) 2005-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>
#include <opencv/highgui.h>

#include <opencv2/opencv.hpp>
#include <QSqlQuery>

#include "interfaces.h"
#include "ImageViewer.h"

#include "ImageOperate.h"
#include "Connection.h"

#include "imagecompletionui.h"


#define SAVERESULT 0
#define _DEBUG 0


ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    //setAttribute(Qt::WA_NoBackground);

    _ocvImage		= NULL;
    _srcOcvImage    = NULL;
    labelImage      = NULL;
    _displayImage	= new QImage(500, 500, QImage::Format_RGB32);

    _displayImage->fill(qRgb(128, 128, 128));
    _labelMapImage = new QImage(500, 500, QImage::Format_ARGB32);
    _labelMapImage->fill(qRgb(128, 128, 128));

    thickness		= 5;
    brushInterface  = 0;
    bPaintable		= false;
    lastPos			= QPoint(-1, -1);
    updateObjectCount(0);
    isEraser        = false;

    isUsePrior      = false;
    _result_labelImage = NULL;

    _bRectDrawing        = false;
    _bPolygonDrawing     = false;
    _bPolygonEndDrawing = false;
    _lineThickness       = 3;
    _lineColor           = Qt::yellow;

    m_method             = -1;
    _labelType           = -1;
    _seg_during          = 0;

    _bDone = false;

    this->setMouseTracking(true);
}

ImageViewer::~ImageViewer()
{
    deleteImage();
}

bool ImageViewer::openImage(const QString &fileName)
{
    //	FinishJob();
    filepath = fileName;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if ( !fileName.isEmpty() )
    {
        if ( _ocvImage != 0 )
        {
            cvReleaseImage ( &_ocvImage );
        }

        std::string sstr = fileName.toLocal8Bit().data();
        const char* csstr = sstr.c_str();
        IplImage* newImage = cvLoadImage( csstr, 1 );

        if ( newImage == 0 )
        {
            QMessageBox::warning( this, tr("ImageCompletion"), tr("无法打开文件 %1.\n").arg(fileName) );
            return false;
        }

        if (cv::max(newImage->width,newImage->height) > 800)
        {
            float scale = cv::max(newImage->width/800.0f, newImage->height/800.0f);
            _ocvImage = cvCreateImage(cvSize(newImage->width/scale,newImage->height/scale),8,3);
            cvResize(newImage,_ocvImage);
            cvReleaseImage(&newImage);
        }
        else if (newImage->width<=250||newImage->height<=250)
        {
            float scale = cv::min(newImage->width/300.0f, newImage->height/300.0f);
            _ocvImage = cvCreateImage(cvSize(newImage->width/scale,newImage->height/scale),8,3);
            cvResize(newImage,_ocvImage);
            cvReleaseImage(&newImage);
        }
        else
        {
            _ocvImage = newImage;
        }


        if(_srcOcvImage == NULL)
        {
            _srcOcvImage = cvCreateImage(cvSize(_ocvImage->width, _ocvImage->height), _ocvImage->depth, _ocvImage->nChannels);
            cvCopy(_ocvImage, _srcOcvImage);
        }

        _srcWidth = _ocvImage->width;
        _srcHeight = _ocvImage->height;

        QImage *image;

        _labelMapImage = new QImage(_ocvImage->width, _ocvImage->height, QImage::Format_ARGB32);
        QImage _alphaImage(_ocvImage->width, _ocvImage->height, QImage::Format_Indexed8);
        _alphaImage.fill(255);
        _labelMapImage->setAlphaChannel(_alphaImage);
        _labelMapImage->fill(qRgb(0, 0, 0));

        image = IplImageToQImage( _ocvImage );

        setImage( *image );
    }

    QApplication::restoreOverrideCursor();
    return true;
}

bool ImageViewer::deleteImage()
{

    if (CV_IS_IMAGE(_ocvImage))
    {
        cvReleaseImage(&_ocvImage);
        _ocvImage = NULL;
    }
    if (CV_IS_IMAGE(labelImage))
    {
        cvReleaseImage(&labelImage);
        labelImage = NULL;
    }
    if(CV_IS_IMAGE(_srcOcvImage))
    {
        cvReleaseImage(&_srcOcvImage);
        _srcOcvImage = NULL;
    }

    //	FinishJob();

    if(_displayImage) { delete _displayImage; _displayImage = NULL; };
    if(_labelMapImage) { delete _labelMapImage; _labelMapImage = NULL; }

    //////////////////////////////////////////////////////////////////////////
    _displayImage	= new QImage(500, 500, QImage::Format_RGB32);
    _displayImage->fill(qRgb(128, 128, 128));
    _labelMapImage = new QImage(500, 500, QImage::Format_ARGB32);
    _labelMapImage->fill(qRgb(128, 128, 128));
    //brushcolor		= Qt::blue;
    thickness		= 5;
    brushInterface = 0;
    bPaintable		= false;
    lastPos			= QPoint(-1, -1);
    updateObjectCount(0);
    isEraser = false;
    m_method = -1;
    isUsePrior = false;
    _result_labelImage = NULL;
    //////////////////////////////////////////////////////////////////////////
    update();

    return true;
}

bool ImageViewer::saveLabelMap(const QString &fileName, const char *fileFormat)
{
    bool flag;
    for (int i = 0; i < _labelMapImage->height(); i++)
    {
        for (int j = 0; j < _labelMapImage->width(); j++)
        {
            flag = false;
            for (int k = 0; k < 10; k++)
            {
                QRgb rgb = qRgb(colorTable[3*k], colorTable[3*k+1], colorTable[3*k+2]);
                QRgb rgb1 = _labelMapImage->pixel(j, i);
                if (rgb == rgb1)
                {
                    flag = true;
                }
            }
            if (!flag)
            {
                _labelMapImage->setPixel(j,i, qRgb(0, 0, 0));
            }
        }
    }
    return _labelMapImage->save(fileName, fileFormat);
}

bool ImageViewer::saveLabelledResult(QString path, QString ext)
{
    if(!_srcOcvImage) return false;
    if(!_displayImage) return false;
    if(!_result_labelImage) return false;

    return _result_labelImage->save(path, ext.toUtf8().constData());
}

bool ImageViewer::saveAsLabelledResult(QString &pathResult)
{
    if(!_srcOcvImage) return false;
    if(!_displayImage) return false;
    if(!_result_labelImage) return false;

    QFileDialog *fDlg = new QFileDialog(this);
    fDlg->setWindowTitle(tr("保存标注结果"));
    fDlg->setAcceptMode(QFileDialog::AcceptSave);
    fDlg->setFileMode(QFileDialog::AnyFile);
    fDlg->setViewMode(QFileDialog::Detail);
    fDlg->setDirectory(QDir::currentPath());

    fDlg->setFilter(tr("图像 (*.jpg *.png *.bmp *.jpeg)"));
    fDlg->setDefaultSuffix("png");

    if(fDlg->exec() == QDialog::Accepted)
    {
        QString filename = fDlg->selectedFiles()[0];
        if(_result_labelImage->save(filename))
        {
            pathResult = filename;
            return true;
        }
        else return false;
    }
    else return true;
}

void ImageViewer::saveUserLabels()
{
    // TODO
}

bool ImageViewer::saveMask(QString path, QString ext)
{
    if(_mask.empty()) return false;

    _mask = _mask * 255;
    IplImage* temp = new IplImage(_mask);
    return IplImageToQImage(temp)->save(path, ext.toUtf8().constData());
}

bool ImageViewer::saveAsMask(QString &pathMask)
{
    if(_mask.empty()) return false;

    QFileDialog *fDlg = new QFileDialog(this);
    fDlg->setWindowTitle(tr("保存掩码"));
    fDlg->setAcceptMode(QFileDialog::AcceptSave);
    fDlg->setFileMode(QFileDialog::AnyFile);
    fDlg->setViewMode(QFileDialog::Detail);
    fDlg->setDirectory(QDir::currentPath());

    fDlg->setFilter(tr("图像 (*.jpg *.png *.bmp *.jpeg)"));
    fDlg->setDefaultSuffix("bmp");

    if(fDlg->exec() == QDialog::Accepted)
    {
        QString filename = fDlg->selectedFiles()[0];
        if(m_method == 0)
        {
            if(saveLabelMap(filename))
            {
                pathMask = filename;
                return true;
            }
            else return false;
        }
        else
        {
            _mask = _mask * 255;
            IplImage* temp = new IplImage(_mask);
            QImage* temp2 = IplImageToQImage(temp);
            if(temp2->save(filename))
            {
                pathMask = filename;
                return true;
            }
            else return false;
        }
    }
    else
    {
        return true;
    }
}

bool ImageViewer::openLabelImage(const QString &fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if ( !fileName.isEmpty() )
    {
        if ( labelImage != 0 )
        {
            //*
            cvReleaseImage ( &labelImage );

            std::string sstr = fileName.toUtf8().data();
            const char* csstr = sstr.c_str();
            labelImage = cvLoadImage( csstr, 1 );
            //labelImage = cvLoadImage( fileName.toStdString().c_str(), 1 );

            if ( labelImage == 0 )
            {
                QMessageBox::warning( this, tr("ImageCompletion"), tr("Cannot read file %1.\n").arg(fileName) );
                return false;
            }
            //*/
        }

        if ( labelImage == 0 )
        {
            //*
            std::string sstr = fileName.toUtf8().data();
            const char* csstr = sstr.c_str();
            labelImage = cvLoadImage( csstr, 1 );
            //labelImage = cvLoadImage( fileName.toStdString().c_str(), 1 );

            if ( labelImage == 0 )
            {
                QMessageBox::warning( this, tr("ImageCompletion"), tr("Cannot read file %1.\n").arg(fileName) );
                return false;
            }
            //*/
        }
        _labelMapImage = IplImageToQImage( labelImage );
        update();
    }
    QApplication::restoreOverrideCursor();
    return true;
}

bool ImageViewer::saveImage(const QString &fileName, const char *fileFormat)
{
    return _displayImage->save(fileName, fileFormat);
}

bool ImageViewer::saveImage( const QString &fileName )
{
    return _displayImage->save(fileName);
}

void ImageViewer::setImage(const QImage &image)
{
    *_displayImage = image.convertToFormat(QImage::Format_RGB32);
    adjustSize();
    update();
    updateGeometry();
}

void ImageViewer::insertShape(const QPainterPath &path)
{
    pendingPath = path;
    setCursor(Qt::CrossCursor);
}

void ImageViewer::setBrushColor(const QColor &color)
{
    this->brushcolor = color;
}

void ImageViewer::setBrushWidth(int width)
{
    thickness = width;
    if (!bPaintable)
    {
        return;
    }
    updateLabelState();
}

void ImageViewer::setBrush(BrushInterface *brushInterface, const QString &brush)
{
    this->brushInterface = brushInterface;
    this->brush = brush;
}

QSize ImageViewer::sizeHint() const
{
    return _displayImage->size();
}

void ImageViewer::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), *_displayImage);

    if ( bPaintable )
    {
        QImage _alphaChannel(_displayImage->width(), _displayImage->height(), QImage::Format_Indexed8);
        _alphaChannel.fill(100);
        QImage labelMapImage(*_labelMapImage);
        labelMapImage.setAlphaChannel(_alphaChannel);
        painter.drawImage(QPoint(0, 0), labelMapImage);
    }

    if(_labelType == 0 && bPaintable)
    {
        setupPainter(painter);
    }
    else if(_labelType == 1 && _bRectDrawing && bPaintable)
    {
        setupOtherPainter(painter);
        painter.drawRect(_start_rect.x(), _start_rect.y(), _end_rect.x()-_start_rect.x(), _end_rect.y()-_start_rect.y());
    }
    else if(_labelType == 2 && bPaintable)
    {
        painter.setPen(QPen(Qt::red, 7));

        if( _bPolygonEndDrawing ) _polygonPointList.push_back(_polygonPointList.first());

        QPoint lastPt = _polygonPointList.first();
        painter.drawPoint(lastPt);
        foreach(QPoint curPt, _polygonPointList)
        {
            painter.setPen(QPen(Qt::red, 7));
            painter.drawPoint(curPt);

            painter.setPen(QPen(_lineColor, _lineThickness, Qt::SolidLine));
            painter.drawLine(lastPt, curPt);
            lastPt = curPt;
        }

        if( _bPolygonEndDrawing )
        {
            polygonLabelling();
            _bPolygonEndDrawing = false;
            bPaintable = false;
        }
    }
}

void ImageViewer::mousePressEvent(QMouseEvent *event)
{
    if ( bPaintable == true)
    {
        if (event->button() == Qt::LeftButton)
        {
            QPainter painter( _labelMapImage );
            if (!pendingPath.isEmpty())
            {
                setupPainter(painter);

                QRectF boundingRect = pendingPath.boundingRect();
                QLinearGradient gradient(boundingRect.topRight(), boundingRect.bottomLeft());

                gradient.setColorAt(0.0, QColor(brushcolor.red(), brushcolor.green(), brushcolor.blue(), 63));
                gradient.setColorAt(1.0, QColor(brushcolor.red(), brushcolor.green(), brushcolor.blue(), 191));

                painter.setBrush(gradient);
                painter.translate(event->pos() - boundingRect.center());
                painter.drawPath(pendingPath);

                pendingPath = QPainterPath();
                update();
            }
            else if(_labelType == 1)
            {
                setupOtherPainter(painter);
                _start_rect   = event->pos();
                _end_rect     = event->pos();
                _bRectDrawing = true;
                lastPos       = event->pos();
            }
            else if(_labelType == 2)
            {
                setupOtherPainter(painter);

                lastPos       = event->pos();

                if(_bPolygonDrawing == false)
                {
                    _polygonPointList.clear();
                    _polygonPointList_cv.clear();
                    _bPolygonEndDrawing = false;
                }

                _bPolygonDrawing = true;

                _polygonPointList.push_back(lastPos);
                _polygonPointList_cv.push_back(cv::Point(lastPos.x(), lastPos.y()));

                update();
            }
            else
            {
                if (brushInterface && _labelType == 0)
                {
                    QPainter painter( _labelMapImage );
                    setupPainter(painter);
                    QRect rect = brushInterface->mousePress(brush, painter, event->pos());
                    update(rect);
                }

                lastPos = event->pos();
            }
        }
    }
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if ( bPaintable == true )
    {
        if ((event->buttons() & Qt::LeftButton) && lastPos != QPoint(-1, -1) )
        {
            QPainter painter1( _labelMapImage );
            if (brushInterface && _labelType == 0)
            {
                setupPainter(painter1);
                QRect rect1 = brushInterface->mouseMove(brush, painter1, lastPos, event->pos());
                update(rect1);
            }
            else if(_labelType == 1 && _bRectDrawing)
            {
                setupOtherPainter(painter1);

                _end_rect = event->pos();
                update();
            }
            lastPos = event->pos();
        }
    }
}


void ImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if ( bPaintable == true )
    {
        if (event->button() == Qt::LeftButton && lastPos != QPoint(-1, -1))
        {
            //QPainter painter( _labelMapImage );
            if ( _labelType == 0 && brushInterface )
            {
                //setupPainter(painter);
                //QRect rect = brushInterface->mouseRelease(brush, painter, event->pos());
                //update(rect);

                // Excute
                _mask = getLabelMap(_labelMapImage);
                if(_mask.empty()) return;
                if( m_method != 0 ) return;
                QApplication::setOverrideCursor(Qt::WaitCursor);

                // Seg
                if(!_mask.empty())
                {
                    if(!CV_IS_IMAGE(_ocvImage)) return;
                    cv::Mat m(_ocvImage);
                    time_t start = clock();
                    cv::Mat res = grabCutSeg(m, _mask, cv::Rect(), 1);
                    time_t end = clock();

                    _seg_during = double(end-start) / CLOCKS_PER_SEC;
                    IplImage* temp = new IplImage(res);

                    if(_result_labelImage) { delete _result_labelImage; _result_labelImage = NULL; }
                    _result_labelImage = IplImageToQImage(temp);

                    setImage(*_result_labelImage);
                    bPaintable = false;
                    _mainWindow->uncheckStrikeOptions();
                }
            }
            else if(_labelType == 1 && _bRectDrawing)
            {
                _bRectDrawing = false;

                int x = cv::min(_start_rect.x(), _end_rect.x());
                int y = cv::min(_start_rect.y(), _end_rect.y());
                int w = abs(_start_rect.x()-_end_rect.x());
                int h = abs(_start_rect.y()-_end_rect.y());

                if(w == 0 || h == 0 ) return;

                cv::Rect roi = cv::Rect(x,y,w,h);

                if(!CV_IS_IMAGE(_ocvImage)) return;
                cv::Mat m(_ocvImage);

                time_t start = clock();
                cv::Mat res = grabCutSeg(m, _mask, roi, 0);
                time_t end = clock();

                _seg_during = double(end-start) / CLOCKS_PER_SEC;

                // mat -> iplimage -> qimage
                IplImage* temp = new IplImage(res);

                if (_result_labelImage) delete _result_labelImage;
                _result_labelImage = IplImageToQImage(temp);
                setImage(*_result_labelImage);

                _bRectDrawing = false;
                bPaintable = false;
            }

            lastPos = QPoint(-1, -1);
            QApplication::restoreOverrideCursor();
            _mainWindow->updateLog();
        }
    }
}

void ImageViewer::setupPainter(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(brushcolor, thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}


void ImageViewer::setupOtherPainter( QPainter &painter )
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(_lineColor, _lineThickness, Qt::SolidLine));
}


void ImageViewer::changeObjectLabeling(QTreeWidgetItem* curItem)
{
    isEraser = false;
    bPaintable = true;
    QString str = curItem->text(0);
    QString strb("background");
    QString strf("foreground");
    //curLabelingObject = str.toInt();
    if (str == "background")
    {
        curLabelingObject = 0;
    }
    else
    {
        curLabelingObject = 1;
    }

    QIcon icon = curItem->icon(1);
    QPixmap pixmap = icon.pixmap(16, 16);
    brushcolor = pixmap.toImage().pixel(0,0);
    updateLabelState();
}

void ImageViewer::changeObjectLabeling(int i)
{
    isEraser = false;
    bPaintable = true;
    curLabelingObject = i;
    brushcolor = qRgb(colorTable[3*i], colorTable[3*i+1], colorTable[3*i+2]);
    updateLabelState();
}

void ImageViewer::updateObjectCount(int count)
{
    objectCount = count+2;
}
QImage* ImageViewer::RunRegionCompetition()
{

    setCursor(Qt::ArrowCursor);

#ifdef SAVERESULT
    _labelMapImage->save(filepath+"_scribble.bmp");
#endif

    if (!(labelMap = getLabelMap(_labelMapImage, objectCount)))
    {
        return NULL;
    }
    char* result_labelmap;
    if (m_method == 1)
    {
        //		result_labelmap = Run(labelMap, objectCount,isUsePrior);
    }
    else if(m_method == 100)
    {
        //		result_labelmap = RunGraphcuts(_ocvImage, labelMap, objectCount);
    }
    else if(m_method == 1000)
    {
        //		result_labelmap = RunGeodesic(_ocvImage, labelMap, objectCount);
    }

#ifdef SAVERESULT
    saveGroundTruth(result_labelmap);
#endif

    if (_result_labelImage)
    {
        delete _result_labelImage;
    }

    _result_labelImage = setLabelMap(_ocvImage, result_labelmap);

    setImage( *_result_labelImage );

#ifdef SAVERESULT
    if (m_method == 0)
    {
        _result_labelImage->save(filepath+"_RCresult.jpg");
    }
    else if(m_method == 1)
    {
        _result_labelImage->save(filepath+"_GCresult.jpg");
    }
    else
    {
        _result_labelImage->save(filepath+"_Georesult.jpg");
    }

#endif
    delete[] labelMap;
    delete[] result_labelmap;

    return _result_labelImage;
} 

void ImageViewer::prepareFeature()
{
    //	prepareImageFeatures(_ocvImage);
}

void ImageViewer::setEraser()
{
    isEraser = true;
    brushcolor = qRgb(0, 0, 0);
    updateLabelState();
}

void ImageViewer::updateLabelState()
{
    QCursor cursor_eraser5(QPixmap(tr(":/new/prefix1/icons/cursor_eraser5.png")));
    QCursor cursor_eraser15(QPixmap(tr(":/new/prefix1/icons/cursor_eraser15.png")));
    QCursor cursor_eraser25(QPixmap(tr(":/new/prefix1/icons/cursor_eraser25.png")));

    QCursor cursor_f5(QPixmap(tr(":/new/prefix1/icons/cursor_f5.png")));
    QCursor cursor_f15(QPixmap(tr(":/new/prefix1/icons/cursor_f15.png")));
    QCursor cursor_f25(QPixmap(tr(":/new/prefix1/icons/cursor_f25.png")));

    QCursor cursor_b5(QPixmap(tr(":/new/prefix1/icons/cursor_b5.png")));
    QCursor cursor_b15(QPixmap(tr(":/new/prefix1/icons/cursor_b15.png")));
    QCursor cursor_b25(QPixmap(tr(":/new/prefix1/icons/cursor_b25.png")));

    if (isEraser)
    {
        switch (thickness)
        {
        case 5:
            setCursor(cursor_eraser5);
            break;
        case 15:
            setCursor(cursor_eraser15);
            break;
        case 30:
            setCursor(cursor_eraser25);
            break;
        default:
            break;
        }
    }
    else if(curLabelingObject==0)
    {
        switch (thickness)
        {
        case 5:
            setCursor(cursor_b5);
            break;
        case 15:
            setCursor(cursor_b15);
            break;
        case 30:
            setCursor(cursor_b25);
            break;
        default:
            break;
        }
    }
    else
    {
        switch (thickness)
        {
        case 5:
            setCursor(cursor_f5);
            break;
        case 15:
            setCursor(cursor_f15);
            break;
        case 30:
            setCursor(cursor_f25);
            break;
        default:
            break;
        }
    }

    update();
}

void ImageViewer::saveGroundTruth(char* label)
{
    QString cur_path = filepath;
    if (m_method==0)
    {
        cur_path = cur_path+"_rc.bmp";
    }
    else if(m_method == 1)
    {
        cur_path = cur_path+"_gc.bmp";
    }
    else
    {
        cur_path = cur_path+"_geo.bmp";
    }
    CvSize size = cvGetSize(_ocvImage);
    IplImage* groundTruthImage = cvCreateImage(size, 8, 3);
    uchar* p_l;
    uchar* p_r = (uchar*)groundTruthImage->imageData;
    char* _label = label;
    for (int rows = 0; rows < groundTruthImage->height; rows++)
    {
        p_l = p_r;
        for (int cols = 0; cols < groundTruthImage->width; cols++)
        {
            if (*_label == 0)
            {
                *(p_l++) = 23;
                *(p_l++) = 149;
                *(p_l++) = 100;
            }
            else
            {
                *(p_l++) = 80;
                *(p_l++) = 127;
                *(p_l++) = 255;
            }
            _label++;
        }
        p_r += groundTruthImage->widthStep;
    }

    std::string sstr = cur_path.toUtf8().data();
    const char* csstr = sstr.c_str();
    cvSaveImage(csstr, groundTruthImage);
    //cvSaveImage(cur_path.toStdString().c_str(), groundTruthImage);
    cvReleaseImage(&groundTruthImage);
}

void ImageViewer::imageScaling( double scaleFactor )
{
    if(_srcOcvImage == NULL) return;

    CvSize sz;
    sz.width = int(_srcWidth*scaleFactor)+1;
    sz.height = int(_srcHeight*scaleFactor)+1;

    if(_ocvImage) {cvReleaseImage(&_ocvImage); _ocvImage = NULL;}

    _ocvImage = cvCreateImage(sz, _srcOcvImage->depth, _srcOcvImage->nChannels);
    cvResize(_srcOcvImage, _ocvImage, CV_INTER_CUBIC );

    _labelMapImage = new QImage(_ocvImage->width, _ocvImage->height, QImage::Format_ARGB32);
    QImage _alphaImage(_ocvImage->width, _ocvImage->height, QImage::Format_Indexed8);
    _alphaImage.fill(255);
    _labelMapImage->setAlphaChannel(_alphaImage);
    _labelMapImage->fill(qRgb(0, 0, 0));

    QImage *image;
    image = IplImageToQImage( _ocvImage );

    setImage( *image );

    prepareFeature();
}

void ImageViewer::imageBrighting( double scaleFactor )
{
    if(_ocvImage == NULL || _srcOcvImage == NULL || _ocvImage->nChannels != 3 || _srcOcvImage->nChannels != 3 ) return;

    int x,y,i;
    float val;
    for (i = 0; i < 3; i++)
    {
        for (y = 0; y < _ocvImage->height; y++)
        {
            for (x = 0; x < _ocvImage->width; x++)
            {

                val = ((uchar*)(_ocvImage->imageData + _ocvImage->widthStep*y))[x*3+i];
                val += scaleFactor;
                if(val>255) val=255;
                if(val<0) val=0;
                ((uchar*)(_ocvImage->imageData + _ocvImage->widthStep*y))[x*3+i] = (uchar)val;
            }
        }
    }

    QImage *image;
    image = IplImageToQImage( _ocvImage );

    setImage( *image );
}

void ImageViewer::imageNotColor( double /*scaleFactor*/ )
{
    if(_ocvImage == NULL) return;

    CvSize sz = cvSize(_ocvImage->width, _ocvImage->height);

    IplImage* img = _ocvImage;

    _ocvImage = cvCreateImage(sz, img->depth, img->nChannels);
    cvNot( img, _ocvImage );

    if(img) {cvReleaseImage(&img); img = NULL;}

    QImage *image;
    image = IplImageToQImage( _ocvImage );

    setImage( *image );
}

void ImageViewer::mouseDoubleClickEvent( QMouseEvent */*event*/ )
{
    if(_bPolygonDrawing)
    {
        _bPolygonEndDrawing = true;
        _bPolygonDrawing = false;
        update();
    }
}

void ImageViewer::polygonLabelling()
{
    if(_bPolygonEndDrawing == false) return;

    if(!CV_IS_IMAGE(_ocvImage)) return;

    cv::Mat m(_ocvImage);
    cv::Mat res;
    _mask.create(m.size(), CV_8UC1);
    _mask.setTo(cv::GC_BGD);

    // Prepare for filling the polygon
    const cv::Point* elementPoints[1] = { &_polygonPointList_cv[0] };
    int npts = (int)_polygonPointList_cv.size();

    clock_t start = clock();
    if(m_method == 1) // polygon with grab cut
    {
        cv::fillPoly(_mask, elementPoints, &npts, 1, cv::GC_PR_FGD);
        res = grabCutSeg(m, _mask, cv::Rect(), 1);
    }
    else if(m_method == 2) // manual
    {
        // get mask
        cv::fillPoly(_mask, elementPoints, &npts, 1, cv::GC_FGD);
        m.copyTo(res, _mask);
    }
    clock_t end = clock();
    _seg_during = (double)(end-start) / CLOCKS_PER_SEC;

    IplImage* temp = new IplImage(res);

    if (_result_labelImage) delete _result_labelImage;
    _result_labelImage = IplImageToQImage(temp);
    setImage(*_result_labelImage);

    lastPos = QPoint(-1, -1);
    QApplication::restoreOverrideCursor();
    _mainWindow->updateLog();
}

void ImageViewer::redo()
{
    if(!_srcOcvImage || !_ocvImage || !_displayImage) return;

    if (CV_IS_IMAGE(labelImage))
    {
        cvReleaseImage(&labelImage);
        labelImage = NULL;
    }

    // reset
    if(_labelMapImage)
    {
        delete _labelMapImage;
        _labelMapImage = NULL;
    }

    thickness		= 5;
    bPaintable		= false;
    lastPos			= QPoint(-1, -1);
    updateObjectCount(0);
    isEraser   = false;
    m_method   = -1;
    isUsePrior = false;
    _result_labelImage = NULL;

    if(_srcOcvImage)
    {
        QImage *image = IplImageToQImage( _srcOcvImage );
        setImage(*image);

        _labelMapImage = new QImage(_ocvImage->width, _ocvImage->height, QImage::Format_ARGB32);
        QImage _alphaImage(_ocvImage->width, _ocvImage->height, QImage::Format_Indexed8);
        _alphaImage.fill(255);
        _labelMapImage->setAlphaChannel(_alphaImage);
        _labelMapImage->fill(qRgb(0, 0, 0));
    }

    QApplication::restoreOverrideCursor();
}

void ImageViewer::undo()
{
}

void ImageViewer::setDefaultCursor()
{
    setCursor(Qt::ArrowCursor);

    update();
}
