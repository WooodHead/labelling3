#include "ImageComparison.h"
#include "ui_ImageComparison.h"

ImageComparison::ImageComparison(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageComparison)
{
    ui->setupUi(this);
    ui->_labelOld->setScaledContents(true);
    ui->_labelNew->setScaledContents(true);

    connect(this, SIGNAL(toChangeId()), parent, SLOT(toChangeId()));
    connect(ui->_pushButtonOverwrite, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->_pushButtonModifyID, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->_pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

ImageComparison::~ImageComparison()
{
    delete ui;
}

void ImageComparison::setData(QByteArray old_, QByteArray new_)
{
    _old = old_;
    _new = new_;

    QPixmap oldPixmap, newPixmap;

    oldPixmap.loadFromData(_old);
    newPixmap.loadFromData(_new);

    ui->_labelOld->setPixmap(oldPixmap);
    ui->_labelNew->setPixmap(newPixmap);
}
