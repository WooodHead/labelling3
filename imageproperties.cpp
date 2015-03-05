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
