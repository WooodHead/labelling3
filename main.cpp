#include <QtGui/QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <QDialog>
#include <QTextCodec>
#include "imagecompletionui.h"

#include "ui_SceneCompletionWidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Set Codec
	QTextCodec *codec = QTextCodec::codecForName("System");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);


	//QApplication::setStyle(QStyleFactory::create("Cleanlooks"));/*Plastique*/
	QApplication::setPalette(QApplication::style()->standardPalette());


	ImageCompletionUI w;
	w.show();

/*
	QDialog *dialog = new QDialog;
	Ui::SceneCompletionWidget widget;
	widget.setupUi( dialog );
	dialog->show();*/


	return a.exec();
}
