#include <QtGui/QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <QDialog>
#include <QTextCodec>

#include "imagecompletionui.h"
#include "Login.h"
#include "Global.h"

#include "ui_SceneCompletionWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set Codec
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    Global::initialize();

    (new Login)->show();

    return a.exec();
}

