#include "imageproperties.h"
#include "ui_imageproperties.h"

ImageProperties::ImageProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageProperties)
{
    ui->setupUi(this);
}

ImageProperties::~ImageProperties()
{
    delete ui;
}
