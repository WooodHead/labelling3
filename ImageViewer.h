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

#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QColor>
#include <QImage>
#include <QPainterPath>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QtGui/QMainWindow>
#include <QGraphicsScene>
#include <QWidgetAction>
#include <opencv/cv.h>
#include "ui_ButtomWindow.h"
#include "BasicDataStructure.h"
#include "buttom.h"

class BrushInterface;
class ImageCompletionUI;

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = 0);
	~ImageViewer();

    bool			openImage(const QString &fileName);
	bool			openLabelImage(const QString &fileName);
	bool			deleteImage();

    bool			saveImage(const QString &fileName, const char *fileFormat);
	bool			saveImage(const QString &fileName);

	bool			saveAsImage( const QString &fileName );
    void			setImage(const QImage &image);
    void			insertShape(const QPainterPath &path);
    void			setBrushColor(const QColor &color);
    void			setBrushWidth(int width);
    void			setBrush(BrushInterface *brushInterface, const QString &brush);

    QImage			image() const { return *_displayImage; }
	IplImage*		getOCVImage() { return _ocvImage; }

	QColor			brushColor() const { return brushcolor; }
    int				brushWidth() const { return thickness; }
    QSize			sizeHint() const;

	void			setPaintable( bool flag ) { bPaintable = flag; }
    //////////////////////////////////////////////////////////////////////////
	// Region Competition Operations
	/////////////////////////////////////////////////////////////////////////
	QImage*			RunRegionCompetition();
	bool			saveLabelMap(const QString &fileName, const char *fileFormat = "BMP");
    bool            saveLabelledResult(QString wholefilename, QString ext);
    bool            saveAsLabelledResult();
    void            saveUserLabels();
    bool            saveMask(QString path, QString ext);
    bool            saveAsMask();
	
	//bool			openLabelImage(const QString &fileName);

public:
	void			imageScaling( double scaleFactor );
	void			imageBrighting( double scaleFactor );
    void			imageNotColor(double  );
	void            setLineColor(QColor color) { _lineColor = color; }
	QColor          getLineColor() const { return _lineColor; }
	int				getLineThickness() const { return _lineThickness; }
	void			setLineThickness(int val) { _lineThickness = val; }

protected:
    void			paintEvent(QPaintEvent *event);
    void			mousePressEvent(QMouseEvent *event);
    void			mouseMoveEvent(QMouseEvent *event);
    void			mouseReleaseEvent(QMouseEvent *event);
	void			mouseDoubleClickEvent(QMouseEvent *event);

private:
    void			setupPainter(QPainter &painter);
	void			setupOtherPainter(QPainter &painter);

	void			prepareFeature();
	void			saveGroundTruth(char* label);

	void			polygonLabelling();

    QImage		    *_displayImage;
	QImage			*_labelMapImage;
	QImage          *_result_labelImage;
	QString			curFilename;
	
	IplImage		*_ocvImage;
	IplImage        *_srcOcvImage;
	int             _srcWidth;
	int             _srcHeight;

    cv::Mat         _mask;

	IplImage        *labelImage;
    QColor			brushcolor;
    int				thickness;
	QColor          _lineColor;
	int				_lineThickness;
    BrushInterface	*brushInterface;
    QString			brush;
    QPoint			lastPos;

    QPainterPath	pendingPath;

	bool			bPaintable;

	int             _labelType;

    ImageCompletionUI*  _mainWindow;
    bool _bDone;


	//////////////////////////////////////////////////////////////////////////
	// Region Competition Attributes
	/////////////////////////////////////////////////////////////////////////
	//CompetitionInterface* cf;

	int				curLabelingObject;

	int				objectCount;

	char*			labelMap;
    
	QString         filepath;

	bool			isEraser;

	int             m_method;

	// plot rect
	QPoint          _start_rect, _end_rect;
    bool            _bRectDrawing;

	// plot polygon
	QList<QPoint>   _polygonPointList;
	std::vector<cv::Point> _polygonPointList_cv; // Fix it
    bool            _bPolygonDrawing;
    bool			_bPolygonEndDrawing;

public:
    void getMainWindow(ImageCompletionUI* m_Window){this->_mainWindow = m_Window;}
	void setMethod(int a){m_method = a;}
	bool isUsePrior;
	void setLabelType(int i) { _labelType = i;}

	int Method() const { return m_method; }
	void Method(int val) { m_method = val; }

	double _seg_during;

    void redo();
    void undo();

    void setDefaultCursor();
  
public slots:
	void				changeObjectLabeling(QTreeWidgetItem*); 
	void				changeObjectLabeling(int);
	void				setEraser();
	void				updateLabelState();
private slots:
	void				updateObjectCount(int);

private:
        Ui::ButtomWindow              _bottomWindow;

};

#endif
