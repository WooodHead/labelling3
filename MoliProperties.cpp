#include "MoliProperties.h"
#include "ui_MoliProperties.h"

#include "Connection.h"

MoliProperties::MoliProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MoliProperties)
{
    ui->setupUi(this);

    if(!createConnection(_db))
    {
        QMessageBox::critical(0, qApp->tr("提示"),
                              qApp->tr("数据库连接失败!"),
                              QMessageBox::Cancel);
        return;
    }

    _model = new QSqlTableModel;
    _model->setTable("abrasivemarkinfo");
}

MoliProperties::~MoliProperties()
{
    delete ui;

    _db.close();
    delete _model;
}

void MoliProperties::on_pushButton_2_clicked()
{
    close();
}

void MoliProperties::on_pushButton_clicked()
{
    if(ui->_comboBoxMoliID->currentText().isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("磨粒编号不能为空!"), QMessageBox::Close);
        return;
    }
    else if(ui->_comboBoxMoliImageID->currentText().isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("铁谱图片编号不能为空!"), QMessageBox::Close);
        return;
    }
    else if(ui->_comboBoxMoliPianID->currentText().isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("铁谱片编号不能为空!"), QMessageBox::Close);
        return;
    }
    else if(ui->_editMoliPath->text().isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("铁谱图片路径不能为空!"), QMessageBox::Close);
        return;
    }
    else
    {
        _model->setFilter(QString("abrasiveid = '%1'").arg(ui->_comboBoxMoliID->currentText()));
        if(_model->select())
        {
            if(_model->rowCount() == 1)
            {
                QSqlRecord record = _model->record();
                record.setValue("abrasiveid", ui->_comboBoxMoliID->currentText());
                record.setValue("ferrographypicid", ui->_comboBoxMoliImageID->currentText());
                record.setValue("ferrographysheetid", ui->_comboBoxMoliPianID->currentText());
                record.setValue("ferrographyreportid", ui->_comboBoxMoliReportID->currentText());
                record.setValue("abrasivemarkstuff", ui->_comboBoxMoliGuy->currentText());
                record.setValue("abrasivepicpath", ui->_editMoliPath->text());
                record.setValue("abrasivematerial", ui->_comboBoxMoliProperty->currentText());
                record.setValue("abrasiveposition", ui->_comboBoxMoliPosition->currentText());
                record.setValue("abrasivesize", ui->_editMoliSize->text().toInt());

                record.setValue("abrasivesperimeter", ui->_editMoliLength->text().toDouble());
                record.setValue("abrasiveshape", ui->_comboBoxMoliShape->currentText());
                record.setValue("abrasivecolor", ui->_comboBoxMoliColor->currentText());
                record.setValue("abrasivesurfacetexturetype", ui->_comboBoxMoliSurface->currentText());
                record.setValue("abrasiveweartype", ui->_comboBoxMoliErodeType->currentText());
                record.setValue("abrasivedamagetype", ui->_comboBoxMoliErodePart->currentText());
                record.setValue("abrasivemechanismtype", ui->_comboBoxMoliErodeReason->currentText());
                record.setValue("abrasivefaultinformationreflection", ui->_comboBoxMoliGivenInfo->currentText());
                record.setValue("abrasivetypical", ui->_comboBoxMoliTypical->currentText());

                if(!_originalImagePath.isEmpty())
                {
                    QFile *file = new QFile(_originalImagePath);
                    file->open(QIODevice::ReadOnly);
                    QByteArray data = file->readAll();
                    record.setValue("abrasivePictureData", data);
                }

                _model->setRecord(0, record);
            }
            else if(_model->rowCount() == 0)
            {
                _model->insertRows(0, 1);
                _model->setData(_model->index(0, 0), ui->_comboBoxMoliID->currentText());
                _model->setData(_model->index(0, 1), ui->_comboBoxMoliImageID->currentText());
                _model->setData(_model->index(0, 2), ui->_comboBoxMoliPianID->currentText());
                _model->setData(_model->index(0, 3), ui->_comboBoxMoliReportID->currentText());
                _model->setData(_model->index(0, 4), ui->_comboBoxMoliGuy->currentText());

                _model->setData(_model->index(0, 5), ui->_editMoliPath->text());
                _model->setData(_model->index(0, 6), ui->_comboBoxMoliProperty->currentText());
                _model->setData(_model->index(0, 7), ui->_comboBoxMoliPosition->currentText());
                _model->setData(_model->index(0, 8), ui->_editMoliSize->text().toInt());
                _model->setData(_model->index(0, 9), ui->_editMoliLength->text().toDouble());
                _model->setData(_model->index(0, 10), ui->_comboBoxMoliShape->currentText());
                _model->setData(_model->index(0, 11), ui->_comboBoxMoliColor->currentText());
                _model->setData(_model->index(0, 12), ui->_comboBoxMoliSurface->currentText());
                _model->setData(_model->index(0, 13), ui->_comboBoxMoliErodeType->currentText());
                _model->setData(_model->index(0, 14), ui->_comboBoxMoliErodePart->currentText());
                _model->setData(_model->index(0, 15), ui->_comboBoxMoliErodeReason->currentText());
                _model->setData(_model->index(0, 16), ui->_comboBoxMoliGivenInfo->currentText());
                _model->setData(_model->index(0, 17), ui->_comboBoxMoliTypical->currentText());

                if(!_originalImagePath.isEmpty())
                {
                    QFile *file = new QFile(_originalImagePath);
                    file->open(QIODevice::ReadOnly);
                    QByteArray data = file->readAll();
                    _model->setData(_model->index(0, 18), data);
                }
            }

            if(!_model->submitAll())
            {
                _model->revertAll();
                QMessageBox::warning(this, tr("提示"), tr("保存失败!"), QMessageBox::Close);
            }
            else
            {
                QMessageBox::warning(this, tr("提示"), tr("保存成功!"), QMessageBox::Close);
            }
        }
    }
}

void MoliProperties::showDlg(QString imagePath, QString resultPath, QString maskPath)
{
    _originalImagePath = imagePath;
    _resultPath = resultPath;
    _maskPath = maskPath;

    ui->_editMoliPath->setText(imagePath);

    show();
}
