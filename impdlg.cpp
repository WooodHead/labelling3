#include "impdlg.h"
#include "ui_impdlg.h"

ImpDlg::ImpDlg(QWidget *parent) :
    QDialog(parent),
    packgePath(""),
    ui(new Ui::ImpDlg)
{
    ui->setupUi(this);
    connect(this,SIGNAL(sendPackgePath(QString)),parent,SLOT(setImpPath(QString)));
}

ImpDlg::~ImpDlg()
{
    delete ui;
}

void ImpDlg::on_pushButton_clicked()
{
    QFileDialog *packgeDir = new QFileDialog(this,tr("选择打包文件路径"),"","");
    packgeDir->setFileMode(QFileDialog::DirectoryOnly);
    packgeDir->setViewMode(QFileDialog::Detail);
    
    if(packgeDir->exec())
    {
        QStringList packgePaths = packgeDir->selectedFiles();
        packgePath = packgePaths.at(0);
        ui->lineEdit->setText(packgePath);
    }
    else
        return;
}

void ImpDlg::on_okPushButton_clicked()
{
    if(packgePath.isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("打包文件路径不能为空"),QMessageBox::Close);
        ui->lineEdit->setFocus();
    }
    else
    {
        emit sendPackgePath(packgePath);
        this->accept();
    }
}

void ImpDlg::on_cancelPushButton_clicked()
{
    this->reject();
}
