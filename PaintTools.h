#ifndef _PAINTTOOLS_H_
#define _PAINTTOOLS_H_

#include <QRect>
#include <QObject>
#include <QStringList>
#include <QPainterPath>
#include <QImage>

#include "interfaces.h"

class PaintTools : public QObject, public BrushInterface
{
	Q_OBJECT
	Q_INTERFACES( BrushInterface );
	
public:
	QStringList	brushes() const;
	QRect		mousePress( const QString &brush, QPainter &painter, const QPoint &pos);
	QRect		mouseMove( const QString &brush, QPainter &painter, const QPoint &oldPos, const QPoint &newPos );
	QRect		mouseRelease( const QString &brush, QPainter &painter, const QPoint &pos );

};


#endif