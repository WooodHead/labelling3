#include <QtGui/QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <QDialog>
#include <QTextCodec>

#include "imagecompletionui.h"
#include "login.h"
#include "global.h"

#include "ui_SceneCompletionWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set Codec
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    Global::initialize();

    Login *loginDlg = new Login;
    loginDlg->show();

    return a.exec();
}
