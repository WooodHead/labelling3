#include "propertynamedlg.h"
#include "ui_propertynamedlg.h"

ProPertyNameDlg::ProPertyNameDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProPertyNameDlg)
{
    ui->setupUi(this);
}

ProPertyNameDlg::~ProPertyNameDlg()
{
    delete ui;
}
