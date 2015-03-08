#include "ImageProperties.h"
#include "ui_imageproperties.h"

ImageProperties::ImageProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageProperties)
{
    ui->setupUi(this);
    setWindowTitle(tr("编辑图像属性"));
    ui->_labelOriginalImage->setFixedSize(100, 50);
    ui->_labelResultImage->setFixedSize(100, 50);
    ui->_labelMaskImage->setFixedSize(100, 50);

    connect(this, SIGNAL(flush()), parent, SLOT(flushBottom()));

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
    ui->_comboBoxEquipPlaneID->addItems(getItems(_models[0],   "planeid"));
    ui->_comboBoxEquipPlaneType->addItems(getItems(_models[0], "planetype"));
    ui->_comboBoxEquipUnitID->addItems(getItems(_models[0],    "departid"));
    ui->_comboBoxEquipRuntime->addItems(getItems(_models[0],   "runstage"));

    // model 1
    ui->_comboBoxMovepartID->addItems(getItems(_models[1],     "movepartid"));
    ui->_comboBoxMovepartName->addItems(getItems(_models[1],   "moveparttype"));
    ui->_comboBoxMovepartType->addItems(getItems(_models[1],   "movepartname"));
    ui->_comboBoxMovepartMohe->addItems(getItems(_models[1],   "runstage"));
    ui->_comboBoxMovepartPlaneID->addItems(getItems(_models[1], "planeid"));
    ui->_comboBoxMovepartPlaneType->addItems(getItems(_models[1], "planetype"));
    ui->_dateEditMovepartBegin->setDate(QDate::currentDate());
    ui->_dateEditMovepartEnd->setDate(QDate::currentDate());

    // model 2
    ui->_comboBoxMovepartServiceID->addItems(getItems(_models[2], "movepartrepairid"));
    ui->_comboBoxMovepartServiceMovepartID->addItems(getItems(_models[2], "movepartid"));
    ui->_comboBoxMovepartServiceMovepartType->addItems(getItems(_models[2], "moveparttype"));
    ui->_comboBoxMovepartServiceUnit->addItems(getItems(_models[2], "repairdepart"));
    ui->_dateEditMovepartServiceDate->setDate(QDate::currentDate());

    // model 3
    ui->_comboBoxOilSampleID->addItems(getItems(_models[3], "oilsampleid"));
    ui->_comboBoxOilSampleUnitID->addItems(getItems(_models[3], "sampledepartid"));
    ui->_comboBoxOilSamplePlaneType->addItems(getItems(_models[3], "planetype"));
    ui->_comboBoxOilSamplePlaneID->addItems(getItems(_models[3], "planeid"));
    ui->_comboBoxOilSampleMonitorPart->addItems(getItems(_models[3], "monitorpartname"));
    ui->_comboBoxOilSampleMonitorPartID->addItems(getItems(_models[3], "monitorpartid"));
    ui->_comboBoxOilSampleSamplePointID->addItems(getItems(_models[3], "sampleid"));
    ui->_comboBoxOilSampleSampleUnit->addItems(getItems(_models[3], "sampledepartname"));
    ui->_comboBoxOilSampleSampleGuy->addItems(getItems(_models[3], "samplestuff"));
    ui->_comboBoxOilSampleSampleMethod->addItems(getItems(_models[3], "samplemethod"));
    ui->_comboBoxOilSampleSendGuy->addItems(getItems(_models[3], "sendstuff"));

    // model 4
    ui->_comboBoxOilAnalyzeOilSampleID->addItems(getItems(_models[4], "oilsampleid"));
    ui->_comboBoxOilAnalyzeUnitName->addItems(getItems(_models[4], "analyzedepartname"));
    ui->_comboBoxOilAnalyzeSendUnit->addItems(getItems(_models[4], "senddepart"));
    ui->_comboBoxOilAnalyzeSendGuy->addItems(getItems(_models[4], "sendstuff"));
    ui->_comboBoxOilAnalyzeReceiveGuy->addItems(getItems(_models[4], "receivestuff"));
    ui->_comboBoxOilAnalyzePollutionLevelMethod->addItems(getItems(_models[4], "contaminationanalyzemethod"));
    ui->_comboBoxOilAnalyzePollutionLevelGuy->addItems(getItems(_models[4], "contaminationanalyzestuff"));
    ui->_comboBoxOilAnalyzePolluteLevelEquip->addItems(getItems(_models[4], "contaminationanalyzeequipment"));
    ui->_comboBoxOilAnalyzePolluteReportID->addItems(getItems(_models[4], "contaminationanalyzereportid"));
    ui->_comboBoxOilAnalyzeLightMethod->addItems(getItems(_models[4], "spectroscopymethod"));
    ui->_comboBoxOilAnalyzeLightGuy->addItems(getItems(_models[4], "spectroscopystuff"));
    ui->_comboBoxOilAnalyzeLightEquip->addItems(getItems(_models[4], "spectroscopyequipment"));
    ui->_comboBoxOilAnalyzeLightEquipID->addItems(getItems(_models[4], "spectroscopyreportid"));
    ui->_comboBoxOilAnalyzeMentalMethod->addItems(getItems(_models[4], "ferrographymethod"));
    ui->_comboBoxOilAnalyzeMentalGuy->addItems(getItems(_models[4], "ferrographystuff"));
    ui->_comboBoxOilAnalyzeMentalEquip->addItems(getItems(_models[4], "ferrographyequipment"));
    ui->_comboBoxOilAnalyzeMentalEquipID->addItems(getItems(_models[4], "ferrographyreportid"));

    ui->_comboBoxOilAnalyzeLihuaMethod->addItems(getItems(_models[4], "physicochemicalmethod"));
    ui->_comboBoxOilAnalyzeLihuaGuy->addItems(getItems(_models[4], "physicochemicalstuff"));
    ui->_comboBoxOilAnalyzeLihuaEquip->addItems(getItems(_models[4], "physicochemicalequipment"));
    ui->_comboBoxOilAnalyzeLihuaEquipID->addItems(getItems(_models[4], "physicochemicalreportid"));

    // model 5
    ui->_comboBoxMentalID->addItems(getItems(_models[5], "ferrographysheetid"));
    ui->_comboBoxMentalReportID->addItems(getItems(_models[5], "ferrographyreportid"));
    ui->_comboBoxMentalOilSampleID->addItems(getItems(_models[5], "oilsampleid"));
    ui->_comboBoxMentalInstrumentType->addItems(getItems(_models[5], "ferrographyanalyzertype"));
    ui->_comboBoxMentalMethod->addItems(getItems(_models[5], "ferrographymakemethod"));
    ui->_comboBoxMentalGuy->addItems(getItems(_models[5], "ferrographymakestuff"));

    // model 6
    ui->_comboBoxMentalSampleImageID->addItems(getItems(_models[6], "ferrographypicid"));
    ui->_comboBoxMentalSampleID->addItems(getItems(_models[6], "ferrographysheetid"));
    ui->_comboBoxMentalSampleReportID->addItems(getItems(_models[6], "ferrographyreportid"));
    ui->_comboBoxMentalSampleMicroType->addItems(getItems(_models[6], "microscopictype"));
    ui->_comboBoxMentalSampleSamplerType->addItems(getItems(_models[6], "imageacquisitiontype"));
    ui->_comboBoxMentalSampleLightType->addItems(getItems(_models[6], "lightsourcetype"));
    ui->_comboBoxMentalSampleArea->addItems(getItems(_models[6], "imageacquisitionarea"));
    ui->_comboBoxMentalSampleGuy->addItems(getItems(_models[6], "imageacquisitionstuff"));
    QStringList list = QStringList() << "N" << "Y";
    ui->_comboBoxMentalSampleImageTag->addItems(list);
    ui->_comboBoxMentalSampleImageTag->setCurrentIndex(0);

    // model 7
    ui->_comboBoxMoliID->addItems(getItems(_models[7], "abrasiveid"));
    ui->_comboBoxMoliImageID->addItems(getItems(_models[7], "ferrographypicid"));
    ui->_comboBoxMoliPianID->addItems(getItems(_models[7], "ferrographysheetid"));
    ui->_comboBoxMoliReportID->addItems(getItems(_models[7], "ferrographyreportid"));
    ui->_comboBoxMoliGuy->addItems(getItems(_models[7], "abrasivemarkstuff"));
    ui->_comboBoxMoliProperty->addItems(getItems(_models[7], "abrasivematerial"));
    ui->_comboBoxMoliPosition->addItems(getItems(_models[7], "abrasiveposition"));
    ui->_comboBoxMoliShape->addItems(getItems(_models[7], "abrasiveshape"));
    ui->_comboBoxMoliColor->addItems(getItems(_models[7], "abrasivecolor"));
    ui->_comboBoxMoliSurface->addItems(getItems(_models[7], "abrasivesurfacetexturetype"));
    ui->_comboBoxMoliErodeType->addItems(getItems(_models[7], "abrasiveweartype"));
    ui->_comboBoxMoliErodePart->addItems(getItems(_models[7], "abrasivedamagetype"));
    ui->_comboBoxMoliErodeReason->addItems(getItems(_models[7], "abrasivemechanismtype"));
    ui->_comboBoxMoliGivenInfo->addItems(getItems(_models[7], "abrasivedamagetype"));
    ui->_comboBoxMoliTypical->addItems(getItems(_models[7], "abrasivetypical"));

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

QStringList ImageProperties::getItems(QSqlTableModel *model, QString fieldName)
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
    return true;
}

void ImageProperties::showDlg(QString filename)
{
    show();

    ui->_editMoliPath->setText(filename);
    ui->_editMentalSamplePath->setText(filename);

    //
    _originalImagePath = filename;
    QPixmap image;
    if(image.load(_originalImagePath))
    {
        ui->_labelOriginalImage->setPixmap(image);
        ui->_labelOriginalImage->setScaledContents(true);
    }
}

void ImageProperties::on__buttonClose_clicked()
{
    close();
}

void ImageProperties::on__buttonSave_clicked()
{
    if(!isValid()) return;

    // 0
    _models[0]->setFilter(QString("planeid = '%1'").arg(ui->_comboBoxEquipPlaneID->currentText()));
    if(_models[0]->select())
    {
        if(_models[0]->rowCount() == 1)
        {
            QSqlRecord record = _models[0]->record(0);
            record.setValue("planeid", ui->_comboBoxEquipPlaneID->currentText());
            record.setValue("planetype", ui->_comboBoxEquipPlaneType->currentText());
            record.setValue("departid", ui->_comboBoxEquipUnitID->currentText());
            record.setValue("runhour", ui->_editEquipHours->text().toInt());
            record.setValue("runstage", ui->_comboBoxEquipRuntime->currentText());
            record.setValue("repairtime", ui->_editServiceNumber->text().toInt());
            _models[0]->setRecord(0, record);
        }
        else if(_models[0]->rowCount() == 0)
        {
            _models[0]->insertRows(0, 1);
            _models[0]->setData(_models[0]->index(0, 0), ui->_comboBoxEquipPlaneID->currentText());
            _models[0]->setData(_models[0]->index(0, 1), ui->_comboBoxEquipPlaneType->currentText());
            _models[0]->setData(_models[0]->index(0, 2), ui->_comboBoxEquipUnitID->currentText());
            _models[0]->setData(_models[0]->index(0, 3), ui->_editEquipHours->text().toInt());
            _models[0]->setData(_models[0]->index(0, 4), ui->_comboBoxEquipRuntime->currentText());
            _models[0]->setData(_models[0]->index(0, 5), ui->_editServiceNumber->text().toInt());
        }
    }

    // 1
    _models[1]->setFilter(QString("movepartid = '%1'").arg(ui->_comboBoxMovepartID->currentText()));
    if(_models[1]->select())
    {
        if(_models[1]->rowCount() == 1)
        {
            QSqlRecord record = _models[1]->record();
            record.setValue("movepartid", ui->_comboBoxMovepartID->currentText());
            record.setValue("moveparttype", ui->_comboBoxMovepartName->currentText());
            record.setValue("movepartname", ui->_comboBoxMovepartType->currentText());
            record.setValue("runhour", ui->_editMovepartHours->text().toInt());
            record.setValue("runstage", ui->_comboBoxMovepartMohe->currentText());
            record.setValue("planeid", ui->_comboBoxMovepartPlaneID->currentText());
            record.setValue("planetype", ui->_comboBoxMovepartPlaneType->currentText());
            record.setValue("startdate", ui->_dateEditMovepartBegin->text());
            record.setValue("enddate", ui->_dateEditMovepartEnd->text());
            _models[1]->setRecord(0, record);
        }
        else if(_models[1]->rowCount() == 0)
        {
            _models[1]->insertRows(0, 1);
            _models[1]->setData(_models[1]->index(0, 0), ui->_comboBoxMovepartID->currentText());
            _models[1]->setData(_models[1]->index(0, 1), ui->_comboBoxMovepartName->currentText());
            _models[1]->setData(_models[1]->index(0, 2), ui->_comboBoxMovepartType->currentText());
            _models[1]->setData(_models[1]->index(0, 3), ui->_editMovepartHours->text().toInt());
            _models[1]->setData(_models[1]->index(0, 4), ui->_comboBoxMovepartMohe->currentText());
            _models[1]->setData(_models[1]->index(0, 5), ui->_comboBoxMovepartPlaneID->currentText());
            _models[1]->setData(_models[1]->index(0, 6), ui->_comboBoxMovepartPlaneType->currentText());
            _models[1]->setData(_models[1]->index(0, 7), ui->_dateEditMovepartBegin->text());
            _models[1]->setData(_models[1]->index(0, 8), ui->_dateEditMovepartEnd->text());
        }
    }

    // 2
    _models[2]->setFilter(QString("movepartrepairid = '%1'").arg(ui->_comboBoxMovepartServiceID->currentText()));
    if(_models[2]->select())
    {
        if(_models[2]->rowCount() == 1)
        {
            QSqlRecord record = _models[2]->record();
            record.setValue("movepartrepairid", ui->_comboBoxMovepartServiceID->currentText());
            record.setValue("movepartid", ui->_comboBoxMovepartServiceMovepartID->currentText());
            record.setValue("moveparttype", ui->_comboBoxMovepartServiceMovepartType->currentText());
            record.setValue("repairreason", ui->_editMovepartServiceReson->text());
            record.setValue("repairdate", ui->_dateEditMovepartServiceDate->text());
            record.setValue("repairtime", ui->_editMovepartServiceNumber->text().toInt());
            record.setValue("repairdepart", ui->_comboBoxMovepartServiceUnit->currentText());
            record.setValue("repaircontent", ui->_editMovepartServiceContent->text());
            record.setValue("repaircondition", ui->_editMovepartServiceInfo->text());
            _models[2]->setRecord(0, record);
        }
        else if(_models[2]->rowCount() == 0)
        {
            _models[2]->insertRows(0, 1);
            _models[2]->setData(_models[2]->index(0, 0), ui->_comboBoxMovepartServiceID->currentText());
            _models[2]->setData(_models[2]->index(0, 1), ui->_comboBoxMovepartServiceMovepartID->currentText());
            _models[2]->setData(_models[2]->index(0, 2), ui->_comboBoxMovepartServiceMovepartType->currentText());
            _models[2]->setData(_models[2]->index(0, 3), ui->_editMovepartServiceReson->text());
            _models[2]->setData(_models[2]->index(0, 4), ui->_dateEditMovepartServiceDate->text());
            _models[2]->setData(_models[2]->index(0, 5), ui->_editMovepartServiceNumber->text().toInt());
            _models[2]->setData(_models[2]->index(0, 6), ui->_comboBoxMovepartServiceUnit->currentText());
            _models[2]->setData(_models[2]->index(0, 7), ui->_editMovepartServiceContent->text());
            _models[2]->setData(_models[2]->index(0, 8), ui->_editMovepartServiceInfo->text());
        }
    }


    // 3
    _models[3]->setFilter(QString("oilsampleid = '%1'").arg(ui->_comboBoxOilSampleID->currentText()));
    if(_models[3]->select())
    {
        if(_models[3]->rowCount() == 1)
        {
            QSqlRecord record = _models[3]->record();
            record.setValue("oilsampleid", ui->_comboBoxOilSampleID->currentText());
            record.setValue("sampledepartid", ui->_comboBoxOilSampleUnitID->currentText());
            record.setValue("planetype", ui->_comboBoxOilSamplePlaneType->currentText());
            record.setValue("planeid", ui->_comboBoxOilSamplePlaneID->currentText());
            record.setValue("monitorpartname", ui->_comboBoxOilSampleMonitorPart->currentText());
            record.setValue("monitorpartid", ui->_comboBoxOilSampleMonitorPartID->currentText());
            record.setValue("sampleid", ui->_comboBoxOilSampleSamplePointID->currentText());
            record.setValue("oilworktime", ui->_editOilSampleHours->text());
            record.setValue("oiladdition", ui->_editOilSampleMount->text().toInt());
            record.setValue("samplereason", ui->_editOilSampleReason->text());
            record.setValue("sampledepartname", ui->_comboBoxOilSampleSampleUnit->currentText());
            record.setValue("samplestuff", ui->_comboBoxOilSampleSampleGuy->currentText());
            record.setValue("sampledate", ui->_dateEditOilSampleSampleDate->text());
            record.setValue("sampletime", ui->_timeEditOilSampleSampleTime->text());
            record.setValue("samplesituation", ui->_comboBoxOilSampleSituation->currentText());
            record.setValue("samplemethod", ui->_comboBoxOilSampleSampleMethod->currentText());
            record.setValue("samplevolume", ui->_editOilSampleSampleMount->text().toInt());
            record.setValue("sampleinstruction", ui->_editOilSampleInfo->text());
            record.setValue("sendstuff", ui->_comboBoxOilSampleSendGuy->currentText());
            record.setValue("senddate", ui->_dateEditOilSampleSendDate->text());
            record.setValue("sendtime", ui->_timeEditOilSampleSendTime->text());
            _models[3]->setRecord(0, record);
        }
        else if(_models[3]->rowCount() == 0)
        {
            _models[3]->insertRows(0, 1);
            _models[3]->setData(_models[3]->index(0, 0), ui->_comboBoxOilSampleID->currentText());
            _models[3]->setData(_models[3]->index(0, 1), ui->_comboBoxOilSampleUnitID->currentText());
            _models[3]->setData(_models[3]->index(0, 2), ui->_comboBoxOilSamplePlaneType->currentText());
            _models[3]->setData(_models[3]->index(0, 3), ui->_comboBoxOilSamplePlaneID->currentText());
            _models[3]->setData(_models[3]->index(0, 4), ui->_comboBoxOilSampleMonitorPart->currentText());
            _models[3]->setData(_models[3]->index(0, 5), ui->_comboBoxOilSampleMonitorPartID->currentText());
            _models[3]->setData(_models[3]->index(0, 6), ui->_comboBoxOilSampleSamplePointID->currentText());
            _models[3]->setData(_models[3]->index(0, 7), ui->_editOilSampleHours->text());
            _models[3]->setData(_models[3]->index(0, 8), ui->_editOilSampleMount->text().toInt());
            _models[3]->setData(_models[3]->index(0, 9), ui->_editOilSampleReason->text());
            _models[3]->setData(_models[3]->index(0, 10), ui->_comboBoxOilSampleSampleUnit->currentText());
            _models[3]->setData(_models[3]->index(0, 11), ui->_comboBoxOilSampleSampleGuy->currentText());
            _models[3]->setData(_models[3]->index(0, 12), ui->_dateEditOilSampleSampleDate->text());
            _models[3]->setData(_models[3]->index(0, 13), ui->_timeEditOilSampleSampleTime->text());
            _models[3]->setData(_models[3]->index(0, 14), ui->_comboBoxOilSampleSituation->currentText());
            _models[3]->setData(_models[3]->index(0, 15), ui->_comboBoxOilSampleSampleMethod->currentText());
            _models[3]->setData(_models[3]->index(0, 16), ui->_editOilSampleSampleMount->text().toInt());
            _models[3]->setData(_models[3]->index(0, 17), ui->_editOilSampleInfo->text());
            _models[3]->setData(_models[3]->index(0, 18), ui->_comboBoxOilSampleSendGuy->currentText());
            _models[3]->setData(_models[3]->index(0, 19), ui->_dateEditOilSampleSendDate->text());
            _models[3]->setData(_models[3]->index(0, 20), ui->_timeEditOilSampleSendTime->text());
        }
    }

    // 4
    _models[4]->setFilter(QString("oilsampleid = '%1'").arg(ui->_comboBoxOilAnalyzeOilSampleID->currentText()));
    if(_models[4]->select())
    {
        if(_models[4]->rowCount() == 1)
        {
            QSqlRecord record = _models[4]->record();
            record.setValue("oilsampleid", ui->_comboBoxOilAnalyzeOilSampleID->currentText());
            record.setValue("analyzedepartname", ui->_comboBoxOilAnalyzeUnitName->currentText());
            record.setValue("senddepart",  ui->_comboBoxOilAnalyzeSendUnit->currentText());
            record.setValue("sendreason", ui->_editOilAnalyzeReason->text());
            record.setValue("sendstuff", ui->_comboBoxOilAnalyzeSendGuy->currentText());
            record.setValue("receivedate", ui->_dateEditOilAnalyzeReceiveDate->text());
            record.setValue("receivestuff", ui->_comboBoxOilAnalyzeReceiveGuy->currentText());
            record.setValue("contaminationanalyzemethod", ui->_comboBoxOilAnalyzePollutionLevelMethod->currentText());
            record.setValue("contaminationanalyzestuff", ui->_comboBoxOilAnalyzePollutionLevelGuy->currentText());
            record.setValue("contaminationanalyzedate", ui->_dateEditOilAnalyzePollutionDate->text());
            record.setValue("contaminationanalyzeequipment", ui->_comboBoxOilAnalyzePolluteLevelEquip->currentText());
            record.setValue("contaminationanalyzereportid", ui->_comboBoxOilAnalyzePolluteReportID->currentText());

            record.setValue("spectroscopymethod", ui->_comboBoxOilAnalyzeLightMethod->currentText());
            record.setValue("spectroscopystuff", ui->_comboBoxOilAnalyzeLightGuy->currentText());
            record.setValue("spectroscopydate", ui->_dateEditOilAnalyzeLightDate->text());
            record.setValue("spectroscopyequipment", ui->_comboBoxOilAnalyzeLightEquip->currentText());
            record.setValue("spectroscopyreportid", ui->_comboBoxOilAnalyzeLightEquipID->currentText());

            record.setValue("ferrographymethod", ui->_comboBoxOilAnalyzeMentalMethod->currentText());
            record.setValue("ferrographystuff", ui->_comboBoxOilAnalyzeMentalGuy->currentText());
            record.setValue("ferrographydate", ui->_dateEditOilAnalyzeMentalDate->text());
            record.setValue("ferrographyequipment", ui->_comboBoxOilAnalyzeMentalEquip->currentText());
            record.setValue("ferrographyreportid", ui->_comboBoxOilAnalyzeMentalEquipID->currentText());

            record.setValue("physicochemicalmethod", ui->_comboBoxOilAnalyzeLihuaMethod->currentText());
            record.setValue("physicochemicalstuff", ui->_comboBoxOilAnalyzeLihuaGuy->currentText());
            record.setValue("physicochemicaldate", ui->_dateEditOilAnalyzeLihuaDate->text());
            record.setValue("physicochemicalequipment", ui->_comboBoxOilAnalyzeLihuaEquip->currentText());
            record.setValue("physicochemicalreportid", ui->_comboBoxOilAnalyzeLihuaEquipID->currentText());

            _models[4]->setRecord(0, record);
        }
        else if(_models[4]->rowCount() == 0)
        {
            _models[4]->insertRows(0, 1);
            _models[4]->setData(_models[4]->index(0, 0), ui->_comboBoxOilAnalyzeOilSampleID->currentText());
            _models[4]->setData(_models[4]->index(0, 1), ui->_comboBoxOilAnalyzeUnitName->currentText());
            _models[4]->setData(_models[4]->index(0, 2), ui->_comboBoxOilAnalyzeSendUnit->currentText());
            _models[4]->setData(_models[4]->index(0, 3), ui->_editOilAnalyzeReason->text());
            _models[4]->setData(_models[4]->index(0, 4), ui->_comboBoxOilAnalyzeSendGuy->currentText());
            _models[4]->setData(_models[4]->index(0, 5), ui->_dateEditOilAnalyzeReceiveDate->text());
            _models[4]->setData(_models[4]->index(0, 6), ui->_comboBoxOilAnalyzeReceiveGuy->currentText());
            _models[4]->setData(_models[4]->index(0, 7), ui->_comboBoxOilAnalyzePollutionLevelMethod->currentText());
            _models[4]->setData(_models[4]->index(0, 8), ui->_comboBoxOilAnalyzePollutionLevelGuy->currentText());
            _models[4]->setData(_models[4]->index(0, 9), ui->_dateEditOilAnalyzePollutionDate->text());
            _models[4]->setData(_models[4]->index(0, 10), ui->_comboBoxOilAnalyzePolluteLevelEquip->currentText());
            _models[4]->setData(_models[4]->index(0, 11), ui->_comboBoxOilAnalyzePolluteReportID->currentText());

            _models[4]->setData(_models[4]->index(0, 12), ui->_comboBoxOilAnalyzeLightMethod->currentText());
            _models[4]->setData(_models[4]->index(0, 13), ui->_comboBoxOilAnalyzeLightGuy->currentText());
            _models[4]->setData(_models[4]->index(0, 14), ui->_dateEditOilAnalyzeLightDate->text());
            _models[4]->setData(_models[4]->index(0, 15), ui->_comboBoxOilAnalyzeLightEquip->currentText());
            _models[4]->setData(_models[4]->index(0, 16), ui->_comboBoxOilAnalyzeLightEquipID->currentText());

            _models[4]->setData(_models[4]->index(0, 17), ui->_comboBoxOilAnalyzeMentalMethod->currentText());
            _models[4]->setData(_models[4]->index(0, 18), ui->_comboBoxOilAnalyzeMentalGuy->currentText());
            _models[4]->setData(_models[4]->index(0, 19), ui->_dateEditOilAnalyzeMentalDate->text());
            _models[4]->setData(_models[4]->index(0, 20), ui->_comboBoxOilAnalyzeMentalEquip->currentText());
            _models[4]->setData(_models[4]->index(0, 21), ui->_comboBoxOilAnalyzeMentalEquipID->currentText());

            _models[4]->setData(_models[4]->index(0, 22), ui->_comboBoxOilAnalyzeLihuaMethod->currentText());
            _models[4]->setData(_models[4]->index(0, 23), ui->_comboBoxOilAnalyzeLihuaGuy->currentText());
            _models[4]->setData(_models[4]->index(0, 24), ui->_dateEditOilAnalyzeLihuaDate->text());
            _models[4]->setData(_models[4]->index(0, 25), ui->_comboBoxOilAnalyzeLihuaEquip->currentText());
            _models[4]->setData(_models[4]->index(0, 26), ui->_comboBoxOilAnalyzeLihuaEquipID->currentText());
        }
    }

    // 5
    _models[5]->setFilter(QString("ferrographysheetid = '%1'").arg(ui->_comboBoxMentalID->currentText()));
    if(_models[5]->select())
    {
        if(_models[5]->rowCount() == 1)
        {
            QSqlRecord record = _models[5]->record();
            record.setValue("ferrographysheetid", ui->_comboBoxMentalID->currentText());
            record.setValue("ferrographyreportid", ui->_comboBoxMentalReportID->currentText());
            record.setValue("oilsampleid", ui->_comboBoxMentalOilSampleID->currentText());
            record.setValue("ferrographyanalyzertype", ui->_comboBoxMentalInstrumentType->currentText());
            record.setValue("ferrographymakeoilconsumption", ui->_editMentalMount->text());
            record.setValue("ferrographymakemethod", ui->_comboBoxMentalMethod->currentText());
            record.setValue("ferrographymakestuff", ui->_comboBoxMentalGuy->currentText());
            _models[5]->setRecord(0, record);
        }
        else if(_models[5]->rowCount() == 0)
        {
            _models[5]->insertRows(0, 1);
            _models[5]->setData(_models[5]->index(0, 0), ui->_comboBoxMentalID->currentText());
            _models[5]->setData(_models[5]->index(0, 1), ui->_comboBoxMentalReportID->currentText());
            _models[5]->setData(_models[5]->index(0, 2), ui->_comboBoxMentalOilSampleID->currentText());
            _models[5]->setData(_models[5]->index(0, 3), ui->_comboBoxMentalInstrumentType->currentText());
            _models[5]->setData(_models[5]->index(0, 4), ui->_editMentalMount->text());
            _models[5]->setData(_models[5]->index(0, 5), ui->_comboBoxMentalMethod->currentText());
            _models[5]->setData(_models[5]->index(0, 6), ui->_comboBoxMentalGuy->currentText());
        }
    }

    // 6
    _models[6]->setFilter(QString("ferrographypicid = '%1'").arg(ui->_comboBoxMentalSampleImageID->currentText()));
    if(_models[6]->select())
    {
        if(_models[6]->rowCount() == 1)
        {
            QSqlRecord record = _models[6]->record();
            record.setValue("ferrographypicid", ui->_comboBoxMentalSampleImageID->currentText());
            record.setValue("ferrographysheetid", ui->_comboBoxMentalSampleID->currentText());
            record.setValue("ferrographyreportid", ui->_comboBoxMentalSampleReportID->currentText());
            record.setValue("microscopictype", ui->_comboBoxMentalSampleMicroType->currentText());
            record.setValue("imageacquisitiontype", ui->_comboBoxMentalSampleSamplerType->currentText());
            record.setValue("lightsourcetype", ui->_comboBoxMentalSampleLightType->currentText());
            record.setValue("magnification", ui->_editMentalSampleEnlarger->text().toInt());

            record.setValue("imageacquisitionarea", ui->_comboBoxMentalSampleArea->currentText());
            record.setValue("imageacquisitionstuff", ui->_comboBoxMentalSampleGuy->currentText());
            record.setValue("ferrographypicpath", ui->_editMentalSamplePath->text());
            record.setValue("imagerecognitioninfoanalysis", ui->_editMentalSampleAnalysis->text());
            record.setValue("imagesymbol", ui->_comboBoxMentalSampleImageTag->currentText());
            _models[6]->setRecord(0, record);
        }
        else if(_models[6]->rowCount() == 0)
        {
            _models[6]->insertRows(0, 1);
            _models[6]->setData(_models[6]->index(0, 0), ui->_comboBoxMentalSampleImageID->currentText());
            _models[6]->setData(_models[6]->index(0, 1), ui->_comboBoxMentalSampleID->currentText());
            _models[6]->setData(_models[6]->index(0, 2), ui->_comboBoxMentalSampleReportID->currentText());
            _models[6]->setData(_models[6]->index(0, 3), ui->_comboBoxMentalSampleMicroType->currentText());
            _models[6]->setData(_models[6]->index(0, 4), ui->_comboBoxMentalSampleSamplerType->currentText());
            _models[6]->setData(_models[6]->index(0, 5), ui->_comboBoxMentalSampleLightType->currentText());
            _models[6]->setData(_models[6]->index(0, 6), ui->_editMentalSampleEnlarger->text().toInt());
            _models[6]->setData(_models[6]->index(0, 7), ui->_comboBoxMentalSampleArea->currentText());
            _models[6]->setData(_models[6]->index(0, 8), ui->_comboBoxMentalSampleGuy->currentText());
            _models[6]->setData(_models[6]->index(0, 9), ui->_editMentalSamplePath->text());
            _models[6]->setData(_models[6]->index(0, 10), ui->_editMentalSampleAnalysis->text());
            _models[6]->setData(_models[6]->index(0, 11), ui->_comboBoxMentalSampleImageTag->currentText());
        }
    }

    // 7
    _models[7]->setFilter(QString("abrasiveid = '%1'").arg(ui->_comboBoxMoliID->currentText()));
    if(_models[7]->select())
    {
        if(_models[7]->rowCount() == 1)
        {
            QSqlRecord record = _models[7]->record();
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

            _models[7]->setRecord(0, record);
        }
        else if(_models[7]->rowCount() == 0)
        {
            _models[7]->insertRows(0, 1);
            _models[7]->setData(_models[7]->index(0, 0), ui->_comboBoxMoliID->currentText());
            _models[7]->setData(_models[7]->index(0, 1), ui->_comboBoxMoliImageID->currentText());
            _models[7]->setData(_models[7]->index(0, 2), ui->_comboBoxMoliPianID->currentText());
            _models[7]->setData(_models[7]->index(0, 3), ui->_comboBoxMoliReportID->currentText());
            _models[7]->setData(_models[7]->index(0, 4), ui->_comboBoxMoliGuy->currentText());

            _models[7]->setData(_models[7]->index(0, 5), ui->_editMoliPath->text());
            _models[7]->setData(_models[7]->index(0, 6), ui->_comboBoxMoliProperty->currentText());
            _models[7]->setData(_models[7]->index(0, 7), ui->_comboBoxMoliPosition->currentText());
            _models[7]->setData(_models[7]->index(0, 8), ui->_editMoliSize->text().toInt());
            _models[7]->setData(_models[7]->index(0, 9), ui->_editMoliLength->text().toDouble());
            _models[7]->setData(_models[7]->index(0, 10), ui->_comboBoxMoliShape->currentText());
            _models[7]->setData(_models[7]->index(0, 11), ui->_comboBoxMoliColor->currentText());
            _models[7]->setData(_models[7]->index(0, 12), ui->_comboBoxMoliSurface->currentText());
            _models[7]->setData(_models[7]->index(0, 13), ui->_comboBoxMoliErodeType->currentText());
            _models[7]->setData(_models[7]->index(0, 14), ui->_comboBoxMoliErodePart->currentText());
            _models[7]->setData(_models[7]->index(0, 15), ui->_comboBoxMoliErodeReason->currentText());
            _models[7]->setData(_models[7]->index(0, 16), ui->_comboBoxMoliGivenInfo->currentText());
            _models[7]->setData(_models[7]->index(0, 17), ui->_comboBoxMoliTypical->currentText());

            if(!_originalImagePath.isEmpty())
            {
                QFile *file = new QFile(_originalImagePath);
                file->open(QIODevice::ReadOnly);
                QByteArray data = file->readAll();
                _models[7]->setData(_models[7]->index(0, 18), data);
            }
        }
    }

    for(int i = 0; i < TABLE_N; i++)
    {
        if(!_models[i]->submitAll())
        {
            qDebug() << i;
            for(int k = 0; k < i; k++)
            {
                _models[k]->revertAll();
            }
            QMessageBox::warning(this, tr("提示"), tr("保存失败!"), QMessageBox::Close);
            return;
        }
    }
    QMessageBox::warning(this, tr("提示"), tr("保存成功!"), QMessageBox::Close);

    emit flush();

    close();
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
