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

ExpDlg::ExpDlg(QWidget *parent, QString sourcepicPath, QString resultPath, QString maskPicPath)
    :QDialog(parent),
      ui(new Ui::ExpDlg)
{
    ui->setupUi(this);
    ui->picLineEdit->setText(sourcepicPath);
    ui->resultLineEdit->setText(resultPath);
    ui->maskLineEdit->setText(maskPicPath);
    this->_sourcePicPath = sourcepicPath;
    this->_resultPicPath = resultPath;
    this->_maskPicPath = maskPicPath;
    this->_packgePath = "";
    this->ui->picPushButton->setVisible(false);
    this->ui->resultPushButton->setVisible(false);
    this->ui->maskPushButton->setVisible(false);
    this->ui->picLineEdit->setEnabled(false);
    this->ui->resultLineEdit->setEnabled(false);
    this->ui->maskLineEdit->setEnabled(false);
    connect(this,SIGNAL(initPathParams(QString,QString,QString,QString)),parent,SLOT(setExpPath(QString,QString,QString,QString)));
}

void ExpDlg::on_CancelPushButton_clicked()
{
    this->reject();
}

void ExpDlg::on_OKPushButton_clicked()
{
    if(this->_sourcePicPath == "")
    {
        QMessageBox::warning(this,tr("提示"),tr("原始图片路径不能为空！"),QMessageBox::Close);
        ui->picLineEdit->setFocus();
    }
    else if(this->_resultPicPath == "")
    {
        QMessageBox::warning(this,tr("提示"),tr("磨粒标注结果不能为空！"),QMessageBox::Close);
        ui->resultLineEdit->setFocus();
    }
    else if(this->_maskPicPath == "")
    {
        QMessageBox::warning(this,tr("提示"),tr("掩码图像路径不能为空！"), QMessageBox::Close);
        ui->maskLineEdit->setFocus();
    }
    else if(this->_packgePath == "")
    {
        QMessageBox::warning(this,tr("提示"),tr("打包路径不能为空！"),QMessageBox::Close);
        ui->packgeLineEdit->setFocus();
    }
    else
    {
        emit initPathParams(_sourcePicPath,_resultPicPath, _maskPicPath, _packgePath);
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
        _sourcePicPath = sourcePath;
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
        _resultPicPath = resultPath;
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
        _packgePath = targetPath;
        ui->packgeLineEdit->setText(targetPath);
    }
}

void ExpDlg::on_maskPushButton_clicked()
{
    QFileDialog *maskDir = new QFileDialog(this, tr("选择磨粒掩码图像目录"), "", "");
    maskDir->setFileMode(QFileDialog::DirectoryOnly);
    maskDir->setViewMode(QFileDialog::Detail);
    QString maskPath;
    if(maskDir->exec())
    {
        QStringList maskPaths = maskDir->selectedFiles();
        maskPath = maskPaths.at(0);
        _resultPicPath = maskPath;
        ui->maskLineEdit->setText(maskPath);
    }
}
