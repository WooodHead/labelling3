#include "propertynamedlg.h"
#include "ui_propertynamedlg.h"

ProPertyNameDlg::ProPertyNameDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProPertyNameDlg)
{
    ui->setupUi(this);
    connect(this,SIGNAL(setpropertyName(QString)),parent,SLOT(setpropertyName(QString)));
}

ProPertyNameDlg::ProPertyNameDlg(QWidget *parent, QString propertyname):
    QDialog(parent),
    ui(new Ui::ProPertyNameDlg)
{
    ui->setupUi(this);
    
    ui->propertyNaeLineEdit->setText(propertyname);
    ui->propertyNaeLineEdit->setFocus();
    connect(this,SIGNAL(setpropertyName(QString)),parent,SLOT(setpropertyName(QString)));
    
//    connect(this,SIGNAL(accepted()),this,SLOT(accept()));
//    connect(this,SIGNAL(rejected()),this,SLOT(reject()));
    
//    ui->buttonBox->button(QDialogButtonBox)->setEnabled(false);
}

ProPertyNameDlg::~ProPertyNameDlg()
{
    delete ui;
}

//void ProPertyNameDlg::on_buttonBox_accepted()
//{
//    QString text = ui->propertyNaeLineEdit->text().trimmed();
//    if(text.isEmpty())
//    {
//        QMessageBox::warning(this,tr("提示"),tr("属性名不能为空"),QMessageBox::Close);
//        return;
//    }
//    else
//        emit setpropertyName(text);
//}

//void ProPertyNameDlg::on_buttonBox_clicked(QAbstractButton *button)
//{
//    QString text = ui->propertyNaeLineEdit->text().trimmed();
    
//    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
//    {
//        trriger(text);
////        QMessageBox::warning(this,tr("提示"),tr("属性名不能为空"),QMessageBox::Close);
//    }
//    else
//        emit this->rejected();
//}

void ProPertyNameDlg::trriger(QString text)
{
    if(text.isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("属性名不能为空"),QMessageBox::Close);
        ui->propertyNaeLineEdit->setFocus();
//        return;
    }
    else
    {
        QSqlQuery query;
        QString sql = "select * from propertyinfo where propertyname = '";
        sql.append(text);
        sql.append("'");
        query.exec(sql);
        if(query.next())
        {
            QMessageBox::warning(this,tr("提示"),tr("属性名以存在，请重新命名"),QMessageBox::Close);
            ui->propertyNaeLineEdit->setFocus();
        }
        else
        {
            emit setpropertyName(text);
    //        emit this->accepted();
            this->accept();
        }

    }
}

void ProPertyNameDlg::on_OkButton_clicked()
{
    QString text = ui->propertyNaeLineEdit->text().trimmed();
    trriger(text);
}

void ProPertyNameDlg::on_cancelButton_clicked()
{
//    emit this->rejected();
    this->reject();
}
