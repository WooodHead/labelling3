#include "propertynamedlg.h"
#include "ui_propertynamedlg.h"

ProPertyNameDlg::ProPertyNameDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProPertyNameDlg)
{
    ui->setupUi(this);
    connect(this,SIGNAL(setpropertyName(QString)),parent,SLOT(setpropertyName(QString)));
}

ProPertyNameDlg::~ProPertyNameDlg()
{
    delete ui;
}

void ProPertyNameDlg::on_buttonBox_accepted()
{
    QString text = ui->propertyNaeLineEdit->text().trimmed();
    if(text.isEmpty())
        QMessageBox::warning(this,tr("提示"),tr("属性名不能为空"),QMessageBox::Close);
    else
        emit setpropertyName(text);
}
