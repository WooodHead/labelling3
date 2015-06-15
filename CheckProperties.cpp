#include "CheckProperties.h"
#include "ui_CheckProperties.h"

CheckProperties::CheckProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckProperties)
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

    ui->btnClose->setIcon(Global::Awesome->icon(remove_));
}

CheckProperties::~CheckProperties()
{
    delete ui;

    _db.close();

    if(_model)
    {
        delete _model;
        _model = 0;
    }
}

QStringList CheckProperties::getItems(QSqlTableModel *model, QString fieldName)
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

void CheckProperties::computeMoliInfo(double imageScale, double &perimeter, double &maxHeight, double &maxWidth)
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

void CheckProperties::showDlg(QString imagePath, const QImage* result, const QImage* result2, const QImage* mask, const double imageScale)
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


void CheckProperties::on_btnClose_clicked()
{
    close();
}
