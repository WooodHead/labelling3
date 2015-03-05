#include "imageproperties.h"
#include "ui_imageproperties.h"

ImageProperties::ImageProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageProperties)
{
    ui->setupUi(this);
    setWindowTitle(tr("编辑图像属性"));

    for(int i = 0; i < TABLE_N; i++ )
    {
        _models[i] = 0;
    }

    load();
}

ImageProperties::~ImageProperties()
{
    for(int i = 0; i < TABLE_N; i++)
    {
        if(_models[i])
        {
            delete _models[i];
            _models[i] = 0;
        }
    }

    delete ui;
}

void ImageProperties::load()
{
    QString tableNames[TABLE_N] = { "equipmentinfo", "movepartinfo", "movepartrepairinfo", "oilsampleinfo", "oilanalyzeinfo", "ferrographyinfo", "ferrographypicinfo", "abrasivemarkinfo" };

    // Set Table
    for(int i = 0; i < TABLE_N; i++)
    {
        if(!_models[i])
        {
            _models[i] = new QSqlTableModel;
            _models[i]->setTable(tableNames[i]);
            _models[i]->select();
        }
    }

    // model 0
    ui->_comboBoxEquipPlaneID->addItems(getItems(_models[0], 1));
    ui->_comboBoxEquipPlaneType->addItems(getItems(_models[0], 2));
    ui->_comboBoxEquipUnitID->addItems(getItems(_models[0], 3));
    ui->_comboBoxEquipRuntime->addItems(getItems(_models[0], 5));

    // model 1
    ui->_comboBoxMovepartID->addItems(getItems(_models[1], 1));
    ui->_comboBoxMovepartName->addItems(getItems(_models[1], 2));
    ui->_comboBoxMovepartType->addItems(getItems(_models[1], 3));
    ui->_comboBoxMovepartMohe->addItems(getItems(_models[1], 4));
    ui->_comboBoxMovepartPlaneID->addItems(getItems(_models[1], 6));
    ui->_comboBoxMovepartPlaneType->addItems(getItems(_models[1], 7));
    ui->_dateEditMovepartBegin->setDate(QDate::currentDate());
    ui->_dateEditMovepartEnd->setDate(QDate::currentDate());

    // model 2
    ui->_comboBoxMovepartServiceID->addItems(getItems(_models[2], 1));
    ui->_comboBoxMovepartServiceMovepartID->addItems(getItems(_models[2], 2));
    ui->_comboBoxMovepartServiceMovepartType->addItems(getItems(_models[2], 3));
    ui->_comboBoxMovepartServiceUnit->addItems(getItems(_models[2], 7));
    ui->_dateEditMovepartServiceDate->setDate(QDate::currentDate());

    // model 3
    ui->_comboBoxOilSampleID->addItems(getItems(_models[3], 1));
    ui->_comboBoxOilSampleUnitID->addItems(getItems(_models[3], 2));
    ui->_comboBoxOilSamplePlaneType->addItems(getItems(_models[3], 3));
    ui->_comboBoxOilSamplePlaneID->addItems(getItems(_models[3], 4));
    ui->_comboBoxOilSampleMonitorPart->addItems(getItems(_models[3], 5));
    ui->_comboBoxOilSampleMonitorPartID->addItems(getItems(_models[3], 6));
    ui->_comboBoxOilSampleSamplePointID->addItems(getItems(_models[3], 7));
    ui->_comboBoxOilSampleSampleUnit->addItems(getItems(_models[3], 11));
    ui->_comboBoxOilSampleSampleGuy->addItems(getItems(_models[3], 12));
    ui->_comboBoxOilSampleSampleMethod->addItems(getItems(_models[3], 15));
    ui->_comboBoxOilSampleSendGuy->addItems(getItems(_models[3], 18));

    // model 4
    ui->_comboBoxOilAnalyzeOilSampleID->addItems(getItems(_models[4], 1));
    ui->_comboBoxOilAnalyzeUnitName->addItems(getItems(_models[4], 2));
    ui->_comboBoxOilAnalyzeSendUnit->addItems(getItems(_models[4], 3));
    ui->_comboBoxOilAnalyzeSendGuy->addItems(getItems(_models[4], 4));
    ui->_comboBoxOilAnalyzeReceiveGuy->addItems(getItems(_models[4], 6));
    ui->_comboBoxOilAnalyzePollutionLevelMethod->addItems(getItems(_models[4], 7));
    ui->_comboBoxOilAnalyzePollutionLevelGuy->addItems(getItems(_models[4], 8));
    ui->_comboBoxOilAnalyzePolluteLevelEquip->addItems(getItems(_models[4], 10));
    ui->_comboBoxOilAnalyzePolluteReportID->addItems(getItems(_models[4], 11));
    ui->_comboBoxOilAnalyzeLightMethod->addItems(getItems(_models[4], 12));
    ui->_comboBoxOilAnalyzeLightGuy->addItems(getItems(_models[4], 13));
    ui->_comboBoxOilAnalyzeLightEquip->addItems(getItems(_models[4], 15));
    ui->_comboBoxOilAnalyzeLightEquipID->addItems(getItems(_models[4], 16));
    ui->_comboBoxOilAnalyzeMentalMethod->addItems(getItems(_models[4], 17));
    ui->_comboBoxOilAnalyzeMentalGuy->addItems(getItems(_models[4], 18));
    ui->_comboBoxOilAnalyzeMentalEquip->addItems(getItems(_models[4], 20));
    ui->_comboBoxOilAnalyzeMentalEquipID->addItems(getItems(_models[4], 21));

    ui->_comboBoxOilAnalyzeLihuaMethod->addItems(getItems(_models[4], 22));
    ui->_comboBoxOilAnalyzeLihuaGuy->addItems(getItems(_models[4], 23));
    ui->_comboBoxOilAnalyzeLihuaEquip->addItems(getItems(_models[4], 25));
    ui->_comboBoxOilAnalyzeLihuaEquipID->addItems(getItems(_models[4], 26));

    // model 5
    ui->_comboBoxMentalID->addItems(getItems(_models[5], 1));
    ui->_comboBoxMentalReportID->addItems(getItems(_models[5], 2));
    ui->_comboBoxMentalOilSampleID->addItems(getItems(_models[5], 3));
    ui->_comboBoxMentalInstrumentType->addItems(getItems(_models[5], 4));
    ui->_comboBoxMentalMethod->addItems(getItems(_models[5], 6));
    ui->_comboBoxMentalGuy->addItems(getItems(_models[5], 7));

    // model 6
    ui->_comboBoxMentalSampleImageID->addItems(getItems(_models[6], 1));
    ui->_comboBoxMentalSampleID->addItems(getItems(_models[6], 2));
    ui->_comboBoxMentalSampleReportID->addItems(getItems(_models[6], 3));
    ui->_comboBoxMentalSampleMicroType->addItems(getItems(_models[6], 4));
    ui->_comboBoxMentalSampleSamplerType->addItems(getItems(_models[6], 5));
    ui->_comboBoxMentalSampleLightType->addItems(getItems(_models[6], 6));
    ui->_comboBoxMentalSampleArea->addItems(getItems(_models[6], 8));
    ui->_comboBoxMentalSampleGuy->addItems(getItems(_models[6], 9));
    ui->_comboBoxMentalSampleImageTag->addItems(getItems(_models[6], 12));

    // model 7
    ui->_comboBoxMoliID->addItems(getItems(_models[7], 1));
    ui->_comboBoxMoliImageID->addItems(getItems(_models[7], 2));
    ui->_comboBoxMoliPianID->addItems(getItems(_models[7], 3));
    ui->_comboBoxMoliReportID->addItems(getItems(_models[7], 4));
    ui->_comboBoxMoliGuy->addItems(getItems(_models[7], 5));
    ui->_comboBoxMoliProperty->addItems(getItems(_models[7], 7));
    ui->_comboBoxMoliPosition->addItems(getItems(_models[7], 8));
    ui->_comboBoxMoliShape->addItems(getItems(_models[7], 11));
    ui->_comboBoxMoliColor->addItems(getItems(_models[7], 12));
    ui->_comboBoxMoliSurface->addItems(getItems(_models[7], 13));
    ui->_comboBoxMoliErodeType->addItems(getItems(_models[7], 14));
    ui->_comboBoxMoliErodePart->addItems(getItems(_models[7], 15));
    ui->_comboBoxMoliErodeReason->addItems(getItems(_models[7], 16));
    ui->_comboBoxMoliGivenInfo->addItems(getItems(_models[7], 17));
    ui->_comboBoxMoliTypical->addItems(getItems(_models[7], 18));

    // model 8 TODO
}

QStringList ImageProperties::getItems(QSqlTableModel* model, int col)
{
    QStringList list;
    list << "";
    for(int j = 0; j < model->rowCount(); j++)
    {
        QSqlRecord record = model->record(j);
        QString value = record.value(col).toString();
        if(!list.contains(value)) list << value;
    }

    return list;
}

bool ImageProperties::isValid()
{
    if(ui->_comboBoxEquipPlaneID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(0);
        QMessageBox::warning(this, tr("提示"), tr("机号不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxEquipPlaneType->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(0);
        QMessageBox::warning(this, tr("提示"), tr("机型不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxEquipUnitID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(0);
        QMessageBox::warning(this, tr("提示"), tr("单位编号不能为空!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxMovepartID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(1);
        QMessageBox::warning(this, tr("提示"), tr("动部件编号不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMovepartName->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(1);
        QMessageBox::warning(this, tr("提示"), tr("动部件名称不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMovepartPlaneID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(1);
        QMessageBox::warning(this, tr("提示"), tr("机号不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMovepartPlaneType->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(1);
        QMessageBox::warning(this, tr("提示"), tr("机型不能为空!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxMovepartServiceID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(2);
        QMessageBox::warning(this, tr("提示"), tr("动部件维修编号不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMovepartServiceMovepartID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(2);
        QMessageBox::warning(this, tr("提示"), tr("动部件编号不能为空!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxOilSampleID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(3);
        QMessageBox::warning(this, tr("提示"), tr("油样编号不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxOilSampleUnitID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(3);
        QMessageBox::warning(this, tr("提示"), tr("单位编号不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxOilSamplePlaneID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(3);
        QMessageBox::warning(this, tr("提示"), tr("机号不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxOilSamplePlaneType->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(3);
        QMessageBox::warning(this, tr("提示"), tr("机型不能为空!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxOilAnalyzeOilSampleID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(4);
        QMessageBox::warning(this, tr("提示"), tr("油样编号不能为空!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxMentalID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(5);
        QMessageBox::warning(this, tr("提示"), tr("铁谱片编号不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMentalOilSampleID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(5);
        QMessageBox::warning(this, tr("提示"), tr("油样编号不能为空!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxMentalSampleImageID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(6);
        QMessageBox::warning(this, tr("提示"), tr("铁谱图片编号不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMentalSampleID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(6);
        QMessageBox::warning(this, tr("提示"), tr("铁谱片编号不能为空!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxMoliID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(7);
        QMessageBox::warning(this, tr("提示"), tr("磨粒编号不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMoliImageID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(7);
        QMessageBox::warning(this, tr("提示"), tr("铁谱图片编号不能为空!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMoliPianID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(7);
        QMessageBox::warning(this, tr("提示"), tr("铁谱片编号不能为空!"), QMessageBox::Close);
        return false;
    }
}

void ImageProperties::on__buttonEquipCancel_clicked()
{
    close();
}

void ImageProperties::on__buttonMovepartCancel_clicked()
{
    close();
}

void ImageProperties::on__buttonMovepartServiceCancel_clicked()
{
    close();
}

void ImageProperties::on__buttonOilSampleCancel_clicked()
{
    close();
}

void ImageProperties::on__buttonOilAnalyzeCancel_clicked()
{
    close();
}

void ImageProperties::on__buttonMentalCancel_clicked()
{
    close();
}

void ImageProperties::on__buttonMentalSampleCancel_clicked()
{
    close();
}

void ImageProperties::on__buttonMoliCancel_clicked()
{
    close();
}

void ImageProperties::on__buttonEquipSave_clicked()
{

}

void ImageProperties::on__buttonClose_clicked()
{
    close();
}

void ImageProperties::on__buttonSave_clicked()
{
    if(!isValid()) return;


    // 0
    _models[0]->insertRows(0, 1);
    _models[0]->setData(_models[0]->index(0, 1), ui->_comboBoxEquipPlaneID->currentText());
    _models[0]->setData(_models[0]->index(0, 2), ui->_comboBoxEquipPlaneType->currentText());
    _models[0]->setData(_models[0]->index(0, 3), ui->_comboBoxEquipUnitID->currentText());
    _models[0]->setData(_models[0]->index(0, 4), ui->_editEquipHours->text().toInt());
    _models[0]->setData(_models[0]->index(0, 5), ui->_comboBoxEquipRuntime->currentText());
    _models[0]->setData(_models[0]->index(0, 6), ui->_editServiceNumber->text().toInt());

    // 1
    _models[1]->insertRows(0, 1);
    _models[1]->setData(_models[1]->index(0, 1), ui->_comboBoxMovepartID->currentText());
    _models[1]->setData(_models[1]->index(0, 2), ui->_comboBoxMovepartName->currentText());
    _models[1]->setData(_models[1]->index(0, 3), ui->_comboBoxMovepartType->currentText());
    _models[1]->setData(_models[1]->index(0, 4), ui->_editMovepartHours->text().toInt());
    _models[1]->setData(_models[1]->index(0, 5), ui->_comboBoxMovepartMohe->currentText());
    _models[1]->setData(_models[1]->index(0, 6), ui->_comboBoxMovepartPlaneID->currentText());
    _models[1]->setData(_models[1]->index(0, 7), ui->_comboBoxMovepartPlaneType->currentText());
    _models[1]->setData(_models[1]->index(0, 8), ui->_dateEditMovepartBegin->text());
    _models[1]->setData(_models[1]->index(0, 9), ui->_dateEditMovepartEnd->text());

    // 2
    _models[2]->insertRows(0, 1);
    _models[2]->setData(_models[2]->index(0, 1), ui->_comboBoxMovepartServiceID->currentText());
    _models[2]->setData(_models[2]->index(0, 2), ui->_comboBoxMovepartServiceMovepartID->currentText());
    _models[2]->setData(_models[2]->index(0, 3), ui->_comboBoxMovepartServiceMovepartType->currentText());
    _models[2]->setData(_models[2]->index(0, 4), ui->_editMovepartServiceReson->text());
    _models[2]->setData(_models[2]->index(0, 5), ui->_dateEditMovepartServiceDate->text());
    _models[2]->setData(_models[2]->index(0, 6), ui->_editMovepartServiceNumber->text().toInt());
    _models[2]->setData(_models[2]->index(0, 6), ui->_comboBoxMovepartServiceUnit->currentText());
    _models[2]->setData(_models[2]->index(0, 6), ui->_editMovepartServiceContent->text());
    _models[2]->setData(_models[2]->index(0, 6), ui->_editMovepartServiceInfo->text());

    // 3
    _models[3]->insertRows(0, 1);
    _models[3]->setData(_models[3]->index(0, 1), ui->_comboBoxOilSampleID->currentText());
    _models[3]->setData(_models[3]->index(0, 2), ui->_comboBoxOilSampleUnitID->currentText());
    _models[3]->setData(_models[3]->index(0, 3), ui->_comboBoxOilSamplePlaneType->currentText());
    _models[3]->setData(_models[3]->index(0, 4), ui->_comboBoxOilSamplePlaneID->currentText());
    _models[3]->setData(_models[3]->index(0, 5), ui->_comboBoxOilSampleMonitorPart->currentText());
    _models[3]->setData(_models[3]->index(0, 6), ui->_comboBoxOilSampleMonitorPartID->currentText());
    _models[3]->setData(_models[3]->index(0, 7), ui->_comboBoxOilSampleSamplePointID->currentText());
    _models[3]->setData(_models[3]->index(0, 8), ui->_editOilSampleHours->text().toInt());
    _models[3]->setData(_models[3]->index(0, 9), ui->_editOilSampleMount->text());
    _models[3]->setData(_models[3]->index(0, 10), ui->_editOilSampleReason->text());
    _models[3]->setData(_models[3]->index(0, 11), ui->_comboBoxOilSampleSampleUnit->currentText());
    _models[3]->setData(_models[3]->index(0, 12), ui->_comboBoxOilSampleSampleGuy->currentText());
    _models[3]->setData(_models[3]->index(0, 13), ui->_dateEditOilSampleSampleDate->text());
    _models[3]->setData(_models[3]->index(0, 14), ui->_timeEditOilSampleSampleTime->text());
    _models[3]->setData(_models[3]->index(0, 15), ui->_comboBoxOilSampleSampleMethod->currentText());
    _models[3]->setData(_models[3]->index(0, 16), ui->_editOilSampleSampleMount->text());
    _models[3]->setData(_models[3]->index(0, 17), ui->_editOilSampleInfo->text());
    _models[3]->setData(_models[3]->index(0, 18), ui->_comboBoxOilSampleSendGuy->currentText());
    _models[3]->setData(_models[3]->index(0, 19), ui->_dateEditOilSampleSendDate->text());
    _models[3]->setData(_models[3]->index(0, 20), ui->_timeEditOilSampleSendTime->text());

    // 4
    _models[4]->insertRows(0, 1);
    _models[4]->setData(_models[4]->index(0, 1), ui->_comboBoxOilAnalyzeOilSampleID->currentText());
    _models[4]->setData(_models[4]->index(0, 2), ui->_comboBoxOilAnalyzeUnitName->currentText());
    _models[4]->setData(_models[4]->index(0, 3), ui->_comboBoxOilAnalyzeSendUnit->currentText());
    _models[4]->setData(_models[4]->index(0, 4), ui->_comboBoxOilAnalyzeSendGuy->currentText());
    _models[4]->setData(_models[4]->index(0, 5), ui->_editOilAnalyzeReason->text());
    _models[4]->setData(_models[4]->index(0, 6), ui->_comboBoxOilAnalyzeReceiveGuy->currentText());
    _models[4]->setData(_models[4]->index(0, 7), ui->_dateEditOilAnalyzeReceiveDate->text());
    _models[4]->setData(_models[4]->index(0, 8), ui->_comboBoxOilAnalyzePollutionLevelMethod->currentText());
    _models[4]->setData(_models[4]->index(0, 9), ui->_comboBoxOilAnalyzePollutionLevelGuy->currentText());
    _models[4]->setData(_models[4]->index(0, 10), ui->_dateEditOilAnalyzePollutionDate->text());
    _models[4]->setData(_models[4]->index(0, 11), ui->_comboBoxOilAnalyzePolluteLevelEquip->currentText());
    _models[4]->setData(_models[4]->index(0, 12), ui->_comboBoxOilAnalyzePolluteReportID->currentText());

    _models[4]->setData(_models[4]->index(0, 13), ui->_comboBoxOilAnalyzeLightMethod->currentText());
    _models[4]->setData(_models[4]->index(0, 14), ui->_comboBoxOilAnalyzeLightGuy->currentText());
    _models[4]->setData(_models[4]->index(0, 15), ui->_dateEditOilAnalyzeLightDate->text());
    _models[4]->setData(_models[4]->index(0, 16), ui->_comboBoxOilAnalyzeLightEquip->currentText());
    _models[4]->setData(_models[4]->index(0, 17), ui->_comboBoxOilAnalyzeLightEquipID->currentText());

    _models[4]->setData(_models[4]->index(0, 18), ui->_comboBoxOilAnalyzeMentalMethod->currentText());
    _models[4]->setData(_models[4]->index(0, 19), ui->_comboBoxOilAnalyzeMentalGuy->currentText());
    _models[4]->setData(_models[4]->index(0, 20), ui->_dateEditOilAnalyzeMentalDate->text());
    _models[4]->setData(_models[4]->index(0, 21), ui->_comboBoxOilAnalyzeMentalEquip->currentText());
    _models[4]->setData(_models[4]->index(0, 22), ui->_comboBoxOilAnalyzeMentalEquipID->currentText());

    _models[4]->setData(_models[4]->index(0, 23), ui->_comboBoxOilAnalyzeLihuaMethod->currentText());
    _models[4]->setData(_models[4]->index(0, 24), ui->_comboBoxOilAnalyzeLihuaGuy->currentText());
    _models[4]->setData(_models[4]->index(0, 25), ui->_dateEditOilAnalyzeLihuaDate->text());
    _models[4]->setData(_models[4]->index(0, 26), ui->_comboBoxOilAnalyzeLihuaEquip->currentText());
    _models[4]->setData(_models[4]->index(0, 27), ui->_comboBoxOilAnalyzeLihuaEquipID->currentText());

    // 5
    _models[5]->insertRows(0, 1);
    _models[5]->setData(_models[5]->index(0, 1), ui->_comboBoxMentalID->currentText());
    _models[5]->setData(_models[5]->index(0, 2), ui->_comboBoxMentalReportID->currentText());
    _models[5]->setData(_models[5]->index(0, 3), ui->_comboBoxMentalOilSampleID->currentText());
    _models[5]->setData(_models[5]->index(0, 4), ui->_comboBoxMentalInstrumentType->currentText());
    _models[5]->setData(_models[5]->index(0, 5), ui->_editMentalMount->text());
    _models[5]->setData(_models[5]->index(0, 6), ui->_comboBoxMentalMethod->currentText());
    _models[5]->setData(_models[5]->index(0, 7), ui->_comboBoxMentalGuy->currentText());

    // 6
    _models[6]->insertRows(0, 1);
    _models[6]->setData(_models[6]->index(0, 1), ui->_comboBoxMentalSampleImageID->currentText());
    _models[6]->setData(_models[6]->index(0, 2), ui->_comboBoxMentalSampleID->currentText());
    _models[6]->setData(_models[6]->index(0, 3), ui->_comboBoxMentalSampleReportID->currentText());
    _models[6]->setData(_models[6]->index(0, 4), ui->_comboBoxMentalSampleMicroType->currentText());
    _models[6]->setData(_models[6]->index(0, 5), ui->_comboBoxMentalSampleSamplerType->currentText());
    _models[6]->setData(_models[6]->index(0, 6), ui->_comboBoxMentalSampleLightType->currentText());
    _models[6]->setData(_models[6]->index(0, 7), ui->_editMentalSampleEnlarger->text().toInt());
    _models[6]->setData(_models[6]->index(0, 8), ui->_comboBoxMentalSampleArea->currentText());
    _models[6]->setData(_models[6]->index(0, 9), ui->_comboBoxMentalSampleGuy->currentText());
    _models[6]->setData(_models[6]->index(0, 10), ui->_editMentalSamplePath->text());
    _models[6]->setData(_models[6]->index(0, 11), ui->_editMentalSampleAnalysis->text());
    _models[6]->setData(_models[6]->index(0, 12), ui->_comboBoxMentalSampleImageTag->currentText());

    // 7
    _models[7]->insertRows(0, 1);
    _models[7]->setData(_models[7]->index(0, 1), ui->_comboBoxMoliID->currentText());
    _models[7]->setData(_models[7]->index(0, 2), ui->_comboBoxMoliImageID->currentText());
    _models[7]->setData(_models[7]->index(0, 3), ui->_comboBoxMoliPianID->currentText());
    _models[7]->setData(_models[7]->index(0, 4), ui->_comboBoxMoliReportID->currentText());
    _models[7]->setData(_models[7]->index(0, 5), ui->_comboBoxMoliGuy->currentText());
    _models[7]->setData(_models[7]->index(0, 6), ui->_editMoliPath->text());
    _models[7]->setData(_models[7]->index(0, 7), ui->_comboBoxMoliProperty->currentText());
    _models[7]->setData(_models[7]->index(0, 8), ui->_comboBoxMoliPosition->currentText());
    _models[7]->setData(_models[7]->index(0, 9), ui->_editMoliSize->text());
    _models[7]->setData(_models[7]->index(0, 10), ui->_editMoliLength->text());
    _models[7]->setData(_models[7]->index(0, 11), ui->_comboBoxMoliShape->currentText());
    _models[7]->setData(_models[7]->index(0, 12), ui->_comboBoxMoliColor->currentText());
    _models[7]->setData(_models[7]->index(0, 13), ui->_comboBoxMoliSurface->currentText());
    _models[7]->setData(_models[7]->index(0, 14), ui->_comboBoxMoliErodeType->currentText());
    _models[7]->setData(_models[7]->index(0, 15), ui->_comboBoxMoliErodePart->currentText());
    _models[7]->setData(_models[7]->index(0, 16), ui->_comboBoxMoliErodeReason->currentText());
    _models[7]->setData(_models[7]->index(0, 17), ui->_comboBoxMoliGivenInfo->currentText());
    _models[7]->setData(_models[7]->index(0, 18), ui->_comboBoxMoliTypical->currentText());




    if(_models[0]->submitAll())
    {
        QMessageBox::warning(this, tr("提示"), tr("保存成功!"), QMessageBox::Close);
    }
    else
    {
        QMessageBox::warning(this, tr("提示"), tr("保存失败!"), QMessageBox::Close);
    }
}

void ImageProperties::on__comboBoxEquipPlaneID_textChanged(const QString &arg1)
{
    ui->_comboBoxMovepartPlaneID->setEditText(arg1);
    ui->_comboBoxOilSamplePlaneID->setEditText(arg1);
}

void ImageProperties::on__comboBoxEquipPlaneType_textChanged(const QString &arg1)
{
    ui->_comboBoxMovepartPlaneType->setEditText(arg1);
    ui->_comboBoxOilSamplePlaneType->setEditText(arg1);
}

void ImageProperties::on__comboBoxEquipUnitID_textChanged(const QString &arg1)
{
    ui->_comboBoxOilSampleUnitID->setEditText(arg1);
}

void ImageProperties::on__comboBoxMovepartID_textChanged(const QString &arg1)
{
    ui->_comboBoxMovepartServiceMovepartID->setEditText(arg1);
}

void ImageProperties::on__comboBoxOilSampleID_textChanged(const QString &arg1)
{
    ui->_comboBoxOilAnalyzeOilSampleID->setEditText(arg1);
    ui->_comboBoxMentalOilSampleID->setEditText(arg1);
}

void ImageProperties::on__comboBoxMentalID_textChanged(const QString &arg1)
{
    ui->_comboBoxMentalSampleID->setEditText(arg1);
    ui->_comboBoxMoliPianID->setEditText(arg1);
}

void ImageProperties::on__comboBoxMentalSampleImageID_textChanged(const QString &arg1)
{
    ui->_comboBoxMoliImageID->setEditText(arg1);
}
