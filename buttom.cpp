#include "buttom.h"
#include "ui_buttom.h"

buttom::buttom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::buttom)
{
    ui->setupUi(this);

}

buttom::~buttom()
{
    delete ui;
}
