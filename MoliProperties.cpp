#include "MoliProperties.h"
#include "ui_MoliProperties.h"

#include <opencv2/opencv.hpp>

MoliProperties::MoliProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MoliProperties)
{
    ui->setupUi(this);
    ui->_labelOriginalImage->setFixedSize(100, 50);
    ui->_labelOriginalImage->setScaledContents(true);
    ui->_labelResultImage->setFixedSize(100, 50);
    ui->_labelResultImage->setScaledContents(true);
    ui->_labelMaskImage->setFixedSize(100, 50);
    ui->_labelMaskImage->setScaledContents(true);

    setWindowFlags(Qt::Tool | Qt::WindowCloseButtonHint);
    this->move(250, 250);

    _bDirty = false;

    connect(this, SIGNAL(flushLeft(QString, QString)), parent, SLOT(flushLeft(QString, QString)));
    connect(this, SIGNAL(flushBottom()), parent, SLOT(flush()));
    connect(this, SIGNAL(saveImages()), parent, SLOT(save()));
    connect(this, SIGNAL(next()), parent, SLOT(next()));

    if(!Global::createConnection(_db))
    {
        QMessageBox::critical(0, qApp->tr("提示"),
                              qApp->tr("数据库连接失败!"),
                              QMessageBox::Cancel);
        return;
    }

    _model = new QSqlTableModel(this, _db);
    _model->setTable("abrasivemarkinfo");
    _model->select();

    ui->_comboBoxMoliID->addItems(getItems(_model, "abrasiveid"));
    ui->_comboBoxMoliImageID->addItems(getItems(_model, "ferrographypicid"));
    ui->_comboBoxMoliPianID->addItems(getItems(_model, "ferrographysheetid"));
    ui->_comboBoxMoliReportID->addItems(getItems(_model, "ferrographyreportid"));
    ui->_comboBoxMoliGuy->addItems(getItems(_model, "abrasivemarkstuff"));
    ui->_comboBoxMoliProperty->addItems(getItems(_model, "abrasivematerial"));
    ui->_comboBoxMoliPosition->addItems(getItems(_model, "abrasiveposition"));
    ui->_comboBoxMoliShape->addItems(getItems(_model, "abrasiveshape"));
    ui->_comboBoxMoliColor->addItems(getItems(_model, "abrasivecolor"));
    ui->_comboBoxMoliSurface->addItems(getItems(_model, "abrasivesurfacetexturetype"));
    ui->_comboBoxMoliErodeType->addItems(getItems(_model, "abrasiveweartype"));
    ui->_comboBoxMoliErodePart->addItems(getItems(_model, "abrasivedamagetype"));
    ui->_comboBoxMoliErodeReason->addItems(getItems(_model, "abrasivemechanismtype"));
    ui->_comboBoxMoliGivenInfo->addItems(getItems(_model, "abrasivedamagetype"));
    ui->_comboBoxMoliTypical->addItems(getItems(_model, "abrasivetypical"));

    ui->pushButton->setIcon(Global::Awesome->icon(save));
    ui->pushButton_2->setIcon(Global::Awesome->icon(remove_));
}

QStringList MoliProperties::getItems(QSqlTableModel *model, QString fieldName)
{
    QStringList list;
    list << "";

    for(int j = 0; j < model->rowCount(); j++)
    {
        QSqlRecord record = model->record(j);
        QString value = record.value(record.indexOf(fieldName)).toString();
        if(!list.contains(value)) list << value;
    }

    return list;
}

void MoliProperties::computeMoliInfo(double imageScale, double &perimeter, double &maxHeight, double &maxWidth)
{
    if(_result.isNull()) return;

    int perimeterPixels = 0;
    int minX = _result.width(), maxX = 0, minY = _result.height(), maxY = 0;

    for(int y = 0; y < _result.height(); y++)
    {
        for(int x = 0; x < _result.width(); x++)
        {
            QRgb color = _result.pixel(x, y);

            if(qRed(color) == 255 && qGreen(color) == 255 && qBlue(color) == 0)
            {
                perimeterPixels++;
                minX = cv::min(minX, x);
                maxX = cv::max(maxX, x);
                minY = cv::min(minY, y);
                maxY = cv::max(maxY, y);
            }
        }
    }

    perimeter = perimeterPixels * imageScale;
    maxWidth = (maxX - minX) * imageScale;
    maxHeight = (maxY - minY) * imageScale;
}

MoliProperties::~MoliProperties()
{
    delete ui;

    _db.close();
    delete _model;
    if(Global::Awesome)
    {
        delete Global::Awesome;
        Global::Awesome = 0;
    }
}

void MoliProperties::on_pushButton_2_clicked()
{
    if(_bDirty)
    {
        QMessageBox::StandardButton reply = QMessageBox::warning(0, tr("提示"), tr("关闭将导致所填写的数据丢失, 是否确认退出?"), QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok)  close();
        else return;
    }
    else
    {
        close();
    }
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
        bool bExist = false;
        _model->setFilter(QString("abrasiveid = '%1'").arg(ui->_comboBoxMoliID->currentText()));
        if(_model->select())
        {
            if(_model->rowCount() == 1)
            {
//                QMessageBox::StandardButton ret = QMessageBox::warning(this,
//                                                           tr("提示"),
//                                                           tr("磨粒编号已存在, 确认要保存?"),
//                                                           QMessageBox::Ok | QMessageBox::Cancel);
//                if(QMessageBox::Cancel == ret)
//                {
//                    return;
//                }
                bExist = true;
                QSqlRecord record = _model->record(0);
                record.setValue("abrasiveid", ui->_comboBoxMoliID->currentText());
                record.setValue("ferrographypicid", ui->_comboBoxMoliImageID->currentText());
                record.setValue("ferrographysheetid", ui->_comboBoxMoliPianID->currentText());
                record.setValue("ferrographyreportid", ui->_comboBoxMoliReportID->currentText());
                record.setValue("abrasivemarkstuff", ui->_comboBoxMoliGuy->currentText());
                record.setValue("abrasivepicpath", ui->_editMoliPath->text());
                record.setValue("abrasivematerial", ui->_comboBoxMoliProperty->currentText());
                record.setValue("abrasiveposition", ui->_comboBoxMoliPosition->currentText());
                record.setValue("abrasivesize", ui->_editMoliSize->text());

                record.setValue("abrasivesperimeter", ui->_editMoliLength->text().toDouble());
                record.setValue("abrasiveshape", ui->_comboBoxMoliShape->currentText());
                record.setValue("abrasivecolor", ui->_comboBoxMoliColor->currentText());
                record.setValue("abrasivesurfacetexturetype", ui->_comboBoxMoliSurface->currentText());
                record.setValue("abrasiveweartype", ui->_comboBoxMoliErodeType->currentText());
                record.setValue("abrasivedamagetype", ui->_comboBoxMoliErodePart->currentText());
                record.setValue("abrasivemechanismtype", ui->_comboBoxMoliErodeReason->currentText());
                record.setValue("abrasivefaultinformationreflection", ui->_comboBoxMoliGivenInfo->currentText());
                record.setValue("abrasivetypical", ui->_comboBoxMoliTypical->currentText());

//                if(!_originalImagePath.isEmpty())
//                {
//                    QFile *file = new QFile(_originalImagePath);
//                    file->open(QIODevice::ReadOnly);
//                    QByteArray data = file->readAll();
//                    file->close();
//                    record.setValue("abrasivePictureData", data);
//                }
//                if(!_result.isNull())
//                {
//                    QByteArray arr;
//                    QBuffer buffer(&arr);
//                    buffer.open(QIODevice::WriteOnly);
//                    _result.save(&buffer, Global::ExtResult.toUtf8().constData());
//                    record.setValue("abrasiveResultData", arr);
//                }
//                else
//                {
//                    record.setValue("abrasiveResultData", record.value("abrasiveResultData").toByteArray());
//                }

//                if(!_result2.isNull())
//                {
//                    QByteArray arr;
//                    QBuffer buffer(&arr);
//                    buffer.open(QIODevice::WriteOnly);
//                    _result2.save(&buffer, Global::ExtResult.toUtf8().constData());
//                    record.setValue("abrasiveResultData2", arr);
//                }
//                else
//                {
//                    record.setValue("abrasiveResultData2", record.value("abrasiveResultData2").toByteArray());
//                }

//                if(!_mask.isNull())
//                {
//                    QByteArray arr;
//                    QBuffer buffer(&arr);
//                    buffer.open(QIODevice::WriteOnly);
//                    _mask.save(&buffer, Global::ExtMask.toUtf8().constData());
//                    record.setValue("abrasiveMaskData", arr);
//                }
//                else
//                {
//                    record.setValue("abrasiveMaskData", record.value("abrasiveMaskData").toByteArray());
//                }

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
                _model->setData(_model->index(0, 8), ui->_editMoliSize->text());
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
                    file->close();
                    _model->setData(_model->index(0, 18), data);
                }
                if(!_result.isNull())
                {
                    QByteArray arr;
                    QBuffer buffer(&arr);
                    buffer.open(QIODevice::WriteOnly);
                    _result.save(&buffer, Global::ExtResult.toUtf8().constData());
                    _model->setData(_model->index(0, 19), arr);
                }
                if(!_mask.isNull())
                {
                    QByteArray arr;
                    QBuffer buffer(&arr);
                    buffer.open(QIODevice::WriteOnly);
                    _mask.save(&buffer, Global::ExtMask.toUtf8().constData());
                    _model->setData(_model->index(0, 20), arr);
                }
                if(!_result2.isNull())
                {
                    QByteArray arr;
                    QBuffer buffer(&arr);
                    buffer.open(QIODevice::WriteOnly);
                    _result2.save(&buffer, Global::ExtResult.toUtf8().constData());
                    _model->setData(_model->index(0, 23), arr);
                }
            }

            if(!_model->submitAll())
            {
                _model->revertAll();
                QMessageBox::warning(this, tr("提示"), tr("保存失败!"), QMessageBox::Close);
                close();
            }
            else
            {
                _bDirty = false;

                QSqlTableModel* model = new QSqlTableModel(this, _db);
                model->setTable("ferrographypicinfo");
                model->setFilter(QString("ferrographypicpath = '%1'").arg(_originalImagePath));

                if(model->select() && model->rowCount() == 1)
                {
                    QSqlRecord record = model->record(0);
                    if(record.value("imagesymbol").toString() == "N")
                    {
                        record.setValue("imagesymbol", "Y");
                    }
                    model->setRecord(0, record);

                    if(!model->submitAll())
                    {
                        _model->revertAll();
                        model->revertAll();
                        QMessageBox::warning(this, tr("提示"), tr("保存失败!"), QMessageBox::Close);
                        return;
                    }
                }

                Global::NewName = ui->_comboBoxMoliImageID->currentText();
                Global::MoliId = ui->_comboBoxMoliID->currentText();

                if(!bExist)
                {
                    emit saveImages();
                }
                emit flushBottom();
                emit flushLeft(_originalImagePath, "Y");

                QMessageBox::information(this, tr("提示"), tr("保存成功!"), QMessageBox::Close);
                close();

//                emit next();
            }
        }
    }
}

void MoliProperties::showDlg(QString imagePath, const QImage* result, const QImage* result2, const QImage* mask, const double imageScale)
{
    if(result) _result = *result;
    if(mask) _mask = *mask;
    if(result2) _result2 = *result2;

    _originalImagePath = imagePath;
    ui->_editMoliPath->setText(imagePath);

    // Compute moli info
    double perimeter = 0.0;
    double maxHeight = 0.0;
    double maxWidth = 0.0;

    // Check if existing
    _model->setFilter(QString("abrasivepicpath = '%1'").arg(_originalImagePath));
    if(_model->select() && _model->rowCount() == 1)
    {
        QSqlRecord record = _model->record(0);

        ui->_comboBoxMoliID->setEditText(record.value("abrasiveid").toString());
        ui->_comboBoxMoliImageID->setEditText(record.value("ferrographypicid").toString());
        ui->_comboBoxMoliPianID->setEditText(record.value("ferrographysheetid").toString());
        ui->_comboBoxMoliReportID->setEditText(record.value("ferrographyreportid").toString());
        ui->_comboBoxMoliGuy->setEditText(record.value("abrasivemarkstuff").toString());
        ui->_editMoliPath->setText(record.value("abrasivepicpath").toString());
        ui->_comboBoxMoliProperty->setEditText(record.value("abrasivematerial").toString());
        ui->_comboBoxMoliPosition->setEditText(record.value("abrasiveposition").toString());
        ui->_editMoliSize->setText(record.value("abrasivesize").toString());
        ui->_editMoliLength->setText(record.value("abrasivesperimeter").toString());
        ui->_comboBoxMoliShape->setEditText(record.value("abrasiveshape").toString());
        ui->_comboBoxMoliColor->setEditText(record.value("abrasivecolor").toString());
        ui->_comboBoxMoliSurface->setEditText(record.value("abrasivesurfacetexturetype").toString());
        ui->_comboBoxMoliErodeType->setEditText(record.value("abrasiveweartype").toString());
        ui->_comboBoxMoliErodePart->setEditText(record.value("abrasivedamagetype").toString());
        ui->_comboBoxMoliErodeReason->setEditText(record.value("abrasivemechanismtype").toString());
        ui->_comboBoxMoliGivenInfo->setEditText(record.value("abrasivefaultinformationreflection").toString());
        ui->_comboBoxMoliTypical->setEditText(record.value("abrasivetypical").toString());

        QPixmap image1, image2, image3;
        image1.loadFromData(record.value("abrasivePictureData").toByteArray());
        ui->_labelOriginalImage->setPixmap(image1);

        if(!_result.isNull())
        {
            ui->_labelResultImage->setPixmap(QPixmap::fromImage(_result));
            computeMoliInfo( imageScale, perimeter, maxHeight, maxWidth);
            ui->_editMoliLength->setText(QString::number(perimeter));
            ui->_editMoliSize->setText(QString("%1 x %2").arg(maxWidth).arg(maxHeight));
        }
        else
        {
            image2.loadFromData(record.value("abrasiveResultData").toByteArray());
            ui->_labelResultImage->setPixmap(image2);
        }

        if(!_mask.isNull())
        {
            ui->_labelMaskImage->setPixmap(QPixmap::fromImage(_mask));
        }
        else
        {
            image3.loadFromData(record.value("abrasiveMaskData").toByteArray());
            ui->_labelMaskImage->setPixmap(image3);
        }

        show();
    }
    else
    {
        QSqlTableModel* model = new QSqlTableModel(this, _db);
        model->setTable("ferrographypicinfo");
        model->setFilter(QString("ferrographypicpath = '%1'").arg(_originalImagePath));

        if(model->select() && model->rowCount() == 1)
        {
            QSqlRecord record = model->record(0);

            ui->_comboBoxMoliID->setEditText(record.value("ferrographypicid").toString()+"-00");
            ui->_comboBoxMoliImageID->setEditText(record.value("ferrographypicid").toString());
            ui->_comboBoxMoliPianID->setEditText(record.value("ferrographysheetid").toString());

            ui->_labelOriginalImage->setPixmap(QPixmap(imagePath));

            if(result) ui->_labelResultImage->setPixmap(QPixmap::fromImage(_result));
            if(mask) ui->_labelMaskImage->setPixmap(QPixmap::fromImage(_mask));

            computeMoliInfo( imageScale, perimeter, maxHeight, maxWidth);

            ui->_editMoliLength->setText(QString::number(perimeter));
            ui->_editMoliSize->setText(QString("%1 x %2").arg(maxWidth).arg(maxHeight));

            show();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先填写图像基本信息!"), QMessageBox::Close);
        }

        delete model;
    }
}

void MoliProperties::on__comboBoxMoliGivenInfo_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliID_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliImageID_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliPianID_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliReportID_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliGuy_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__editMoliPath_textChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliProperty_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliPosition_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__editMoliSize_textChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__editMoliLength_textChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliShape_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliColor_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliSurface_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliErodeType_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliErodePart_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliErodeReason_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}

void MoliProperties::on__comboBoxMoliTypical_editTextChanged(const QString &arg1)
{
    _bDirty = true;
}



