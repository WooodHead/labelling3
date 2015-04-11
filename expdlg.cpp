#include "expdlg.h"
#include "ui_expdlg.h"

ExpDlg::ExpDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExpDlg)
{
    ui->setupUi(this);
}

ExpDlg::~ExpDlg()
{
    delete ui;
}

ExpDlg::ExpDlg(QWidget *parent, QString sourcepicPath, QString resultPath)
    :QDialog(parent),
      ui(new Ui::ExpDlg)
{
    ui->setupUi(this);
    ui->picLineEdit->setText(sourcepicPath);
    ui->resultLineEdit->setText(resultPath);
    this->sourcepicPath = sourcepicPath;
    this->resultpicPath = resultPath;
    this->packgePath = "";

    connect(this,SIGNAL(initPathParams(QString,QString,QString)),parent,SLOT(setExpPath(QString,QString,QString)));
}

void ExpDlg::on_CancelPushButton_clicked()
{
    this->reject();
}

void ExpDlg::on_OKPushButton_clicked()
{
    if(this->sourcepicPath == "")
    {
        QMessageBox::warning(this,tr("提示"),tr("原始图片路径不能为空！"),QMessageBox::Close);
        ui->picLineEdit->setFocus();
    }
    else if(this->resultpicPath == "")
    {
        QMessageBox::warning(this,tr("提示"),tr("磨粒标注结果不能为空！"),QMessageBox::Close);
        ui->resultLineEdit->setFocus();
    }
    else if(this->packgePath == "")
    {
        QMessageBox::warning(this,tr("提示"),tr("打包路径不能为空！"),QMessageBox::Close);
        ui->packgeLineEdit->setFocus();
    }
    else
    {
        emit initPathParams(sourcepicPath,resultpicPath,packgePath);
        this->accept();
    }
}

void ExpDlg::on_picPushButton_clicked()
{
    QFileDialog *sourceDir = new QFileDialog(this,tr("选择铁谱图片目录"),"","");
    sourceDir->setFileMode(QFileDialog::DirectoryOnly);
    sourceDir->setViewMode(QFileDialog::Detail);
    QString sourcePath;
    if(sourceDir->exec())
    {
        QStringList sourcePaths = sourceDir->selectedFiles();
        sourcePath = sourcePaths.at(0);
        sourcepicPath = sourcePath;
        ui->picLineEdit->setText(sourcePath);
    }
}

void ExpDlg::on_resultPushButton_clicked()
{
    QFileDialog *resultDir = new QFileDialog(this,tr("选择磨粒标注结果目录"),"","");
    resultDir->setFileMode(QFileDialog::DirectoryOnly);
    resultDir->setViewMode(QFileDialog::Detail);
    QString resultPath;
    if(resultDir->exec())
    {
        QStringList resultPaths = resultDir->selectedFiles();
        resultPath = resultPaths.at(0);
        resultpicPath = resultPath;
        ui->resultLineEdit->setText(resultPath);
    }
}

void ExpDlg::on_packgePushButton_clicked()
{
    QFileDialog *targetDir = new QFileDialog(this,tr("选择打包存档目录"),"","");
    targetDir->setFileMode(QFileDialog::DirectoryOnly);
    targetDir->setViewMode(QFileDialog::Detail);
    QString targetPath;
    if(targetDir->exec())
    {
        QStringList targetPaths = targetDir->selectedFiles();
        targetPath = targetPaths.at(0);
        packgePath = targetPath;
        ui->packgeLineEdit->setText(targetPath);
    }
}


