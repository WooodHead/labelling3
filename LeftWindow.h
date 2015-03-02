#ifndef _LEFTWINDOW_H_
#define _LEFTWINDOW_H_

#include <QStringList>
#include <QDir>

#include "ui_LeftWindow.h"

class CLeftWindow: public QWidget,
					public Ui_LeftWindow
{
	Q_OBJECT

public:
	CLeftWindow(QWidget* = 0);

};

#endif