#include "About.h"
#include "ui_About.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    this->setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint );
    this->setWindowTitle("关于");
}

About::~About()
{
    delete ui;
}

void About::on_pushButton_clicked()
{
    close();
}
