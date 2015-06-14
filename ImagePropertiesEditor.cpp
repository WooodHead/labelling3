#include "ImagePropertiesEditor.h"
#include "ui_ImagePropertiesEditor.h"

ImagePropertiesEditor::ImagePropertiesEditor(QWidget *parent, int index, QString primaryKeyValue) :
    QDialog(parent),
    ui(new Ui::ImagePropertiesEditor)
{
    ui->setupUi(this);

    connect(this, SIGNAL(flush()), parent, SLOT(flushBottom()));

    _model = 0;
    _tableIndex = index;
    _primaryKeyValue = primaryKeyValue;

    initialize();
}

ImagePropertiesEditor::~ImagePropertiesEditor()
{
    if(_model)
    {
        delete _model;
        _model = 0;
    }

    delete ui;
}

void ImagePropertiesEditor::showDlg()
{
    QSqlDatabase db;
    if(Global::createConnection(db))
    {
        loadData();
    }

    if(_primaryKeyValue == "NEW")
        ui->comboBox_88->setEnabled(true);

    show();
}

void ImagePropertiesEditor::initialize()
{
    ui->_tabWidget->setCurrentIndex(_tableIndex);

    for(int i = 8; i >= 0; i--)
    {
        if(i != _tableIndex)
        {
            ui->_tabWidget->removeTab(i);
        }
    }
}

void ImagePropertiesEditor::loadData()
{
    QSqlRecord record;
    QString tables[] = {"equipmentinfo", "movepartinfo", "movepartrepairinfo", "oilsampleinfo", "oilanalyzeinfo", "ferrographyinfo", "ferrographypicinfo", "abrasivemarkinfo", "sampleSummaryInfo"};
    QString keyFieldNames[] = {"planeid", "movepartid", "movepartrepairid", "oilsampleid", "oilsampleid", "ferrographysheetid", "ferrographypicid", "abrasiveid", "sampleID"};

    QSqlDatabase db;
    if(Global::createConnection(db))
    {
        QString strSql = QString(" select * from %1 where %2 = '%3' limit 1").arg(tables[_tableIndex]).arg(keyFieldNames[_tableIndex]).arg(_primaryKeyValue);
        QSqlQuery query;
        query.prepare(strSql);

        if(query.exec() && query.next())
        {
            record = query.record();
        }
    }

    if(_tableIndex == 0)
    {
        ui->_comboBoxEquipPlaneID_3->setEditText(record.value("planeid").toString());
        ui->_comboBoxEquipPlaneType_3->setEditText(record.value("planetype").toString());
        ui->_comboBoxEquipUnitID_3->setEditText(record.value("departid").toString());
        ui->_editEquipHours_3->setText(record.value("runhour").toString());
        ui->_comboBoxEquipRuntime_3->setEditText(record.value("runstage").toString());
        ui->_editServiceNumber_3->setText(record.value("repairtime").toString());
    }
    else if(_tableIndex == 1)
    {
        ui->_comboBoxMovepartID_3->setEditText(record.value("movepartid").toString());
        ui->_comboBoxMovepartType_3->setEditText(record.value("moveparttype").toString());
        ui->_comboBoxMovepartName_3->setEditText(record.value("movepartname").toString());
        ui->_editMovepartHours_3->setText(record.value("runhour").toString());
        ui->_comboBoxMovepartMohe_3->setEditText(record.value("runstage").toString());
        ui->_comboBoxMovepartPlaneID_3->setEditText(record.value("planeid").toString());
        ui->_comboBoxMovepartPlaneType_3->setEditText(record.value("planetype").toString());
        ui->_dateEditMovepartBegin_3->setDate(QDate::fromString(record.value("startdate").toString()));
        ui->_dateEditMovepartEnd_3->setDate(QDate::fromString(record.value("enddate").toString()));
    }
    else if(_tableIndex == 2)
    {
        ui->_comboBoxMovepartServiceID_3->setEditText(record.value("movepartrepairid").toString());
        ui->_comboBoxMovepartServiceMovepartID_3->setEditText(record.value("movepartid").toString());
        ui->_comboBoxMovepartServiceMovepartType_3->setEditText(record.value("moveparttype").toString());
        ui->_editMovepartServiceReson_3->setText(record.value("repairreason").toString());
        ui->_dateEditMovepartServiceDate_3->setDate(QDate::fromString(record.value("repairdate").toString()));
        ui->_editMovepartServiceNumber_3->setText(record.value("repairtime").toString());
        ui->_comboBoxMovepartServiceUnit_3->setEditText(record.value("repairdepart").toString());
        ui->_editMovepartServiceContent_3->setText(record.value("repaircontent").toString());
        ui->_editMovepartServiceInfo_3->setText(record.value("repaircondition").toString());
    }
    else if(_tableIndex == 3)
    {
        ui->_comboBoxOilSampleID_3->setEditText(record.value("oilsampleid").toString());
        ui->_comboBoxOilSampleUnitID_3->setEditText(record.value("sampledepartid").toString());
        ui->_comboBoxOilSamplePlaneType_3->setEditText(record.value("planetype").toString());
        ui->_comboBoxOilSamplePlaneID_3->setEditText(record.value("planeid").toString());
        ui->_comboBoxOilSampleMonitorPart_3->setEditText(record.value("monitorpartname").toString());
        ui->_comboBoxOilSampleMonitorPartID_3->setEditText(record.value("monitorpartid").toString());

        ui->_comboBoxOilSampleSamplePointID_3->setEditText(record.value("sampleid").toString());
        ui->_editOilSampleHours_3->setText(record.value("oilworktime").toString());
        ui->_editOilSampleMount_3->setText(record.value("oiladdition").toString());
        ui->_editOilSampleReason_3->setText(record.value("samplereason").toString());
        ui->_comboBoxOilSampleSampleUnit_3->setEditText(record.value("sampledepartname").toString());
        ui->_comboBoxOilSampleSampleGuy_3->setEditText(record.value("samplestuff").toString());
        ui->_dateEditOilSampleSampleDate_3->setDate(QDate::fromString(record.value("sampledate").toString()));
        ui->_timeEditOilSampleSampleTime_3->setTime(QTime::fromString(record.value("sampletime").toString()));
        ui->_comboBoxOilSampleSituation_3->setEditText(record.value("samplesituation").toString());
        ui->_comboBoxOilSampleSampleMethod_3->setEditText(record.value("samplemethod").toString());
        ui->_editOilSampleSampleMount_3->setText(record.value("samplevolume").toString());
        ui->_editOilSampleInfo_3->setText(record.value("sampleinstruction").toString());
        ui->_comboBoxOilSampleSendGuy_3->setEditText(record.value("sendstuff").toString());
        ui->_dateEditOilSampleSendDate_3->setDate(QDate::fromString(record.value("senddate").toString()));
        ui->_timeEditOilSampleSendTime_3->setTime(QTime::fromString(record.value("sendtime").toString()));
    }
    else if(_tableIndex == 4)
    {
        ui->_comboBoxOilAnalyzeOilSampleID_3->setEditText(record.value("oilsampleid").toString());
        ui->_comboBoxOilAnalyzeUnitName_3->setEditText(record.value("analyzedepartname").toString());
        ui->_comboBoxOilAnalyzeSendUnit_3->setEditText(record.value("senddepart").toString());
        ui->_editOilAnalyzeReason_3->setText(record.value("sendreason").toString());
        ui->_comboBoxOilAnalyzeSendGuy_3->setEditText(record.value("sendstuff").toString());
        ui->_dateEditOilAnalyzeReceiveDate_3->setDate(QDate::fromString(record.value("receivedate").toString()));
        ui->_comboBoxOilAnalyzeReceiveGuy_3->setEditText(record.value("receivestuff").toString());
        ui->_comboBoxOilAnalyzePollutionLevelMethod_3->setEditText(record.value("contaminationanalyzemethod").toString());
        ui->_comboBoxOilAnalyzePollutionLevelGuy_3->setEditText(record.value("contaminationanalyzestuff").toString());
        ui->_dateEditOilAnalyzePollutionDate_3->setDate(QDate::fromString(record.value("contaminationanalyzedate").toString()));
        ui->_comboBoxOilAnalyzePolluteLevelEquip_3->setEditText(record.value("contaminationanalyzeequipment").toString());
        ui->_comboBoxOilAnalyzePolluteReportID_3->setEditText(record.value("contaminationanalyzereportid").toString());

        ui->_comboBoxOilAnalyzeLightMethod_3->setEditText(record.value("spectroscopymethod").toString());
        ui->_comboBoxOilAnalyzeLightGuy_3->setEditText(record.value("spectroscopystuff").toString());
        ui->_dateEditOilAnalyzeLightDate_3->setDate(QDate::fromString(record.value("spectroscopydate").toString()));
        ui->_comboBoxOilAnalyzeLightEquip_3->setEditText(record.value("spectroscopyequipment").toString());
        ui->_comboBoxOilAnalyzeLightEquipID_3->setEditText(record.value("spectroscopyreportid").toString());

        ui->_comboBoxOilAnalyzeMentalMethod_3->setEditText(record.value("ferrographymethod").toString());
        ui->_comboBoxOilAnalyzeMentalGuy_3->setEditText(record.value("ferrographystuff").toString());
        ui->_dateEditOilAnalyzeMentalDate_3->setDate(QDate::fromString(record.value("ferrographydate").toString()));
        ui->_comboBoxOilAnalyzeMentalEquip_3->setEditText(record.value("ferrographyequipment").toString());
        ui->_comboBoxOilAnalyzeMentalEquipID_3->setEditText(record.value("ferrographyreportid").toString());

        ui->_comboBoxOilAnalyzeLihuaMethod_3->setEditText(record.value("physicochemicalmethod").toString());
        ui->_comboBoxOilAnalyzeLihuaGuy_3->setEditText(record.value("physicochemicalstuff").toString());
        ui->_dateEditOilAnalyzeLihuaDate_3->setDate(QDate::fromString(record.value("physicochemicaldate").toString()));
        ui->_comboBoxOilAnalyzeLihuaEquip_3->setEditText(record.value("physicochemicalequipment").toString());
        ui->_comboBoxOilAnalyzeLihuaEquipID_3->setEditText(record.value("physicochemicalreportid").toString());
    }
    else if(_tableIndex == 5)
    {
        ui->_comboBoxMentalID_3->setEditText(record.value("ferrographysheetid").toString());
        ui->_comboBoxMentalReportID_3->setEditText(record.value("ferrographyreportid").toString());
        ui->_comboBoxMentalOilSampleID_3->setEditText(record.value("oilsampleid").toString());
        ui->_comboBoxMentalInstrumentType_3->setEditText(record.value("ferrographyanalyzertype").toString());
        ui->_editMentalMount_3->setText(record.value("ferrographymakeoilconsumption").toString());
        ui->_comboBoxMentalMethod_3->setEditText(record.value("ferrographymakemethod").toString());
        ui->_comboBoxMentalGuy_3->setEditText(record.value("ferrographymakestuff").toString());
    }
    else if(_tableIndex == 6)
    {
        ui->_comboBoxMentalSampleImageID_3->setEditText(record.value("ferrographypicid").toString());
        ui->_comboBoxMentalSampleID_3->setEditText(record.value("ferrographysheetid").toString());
        ui->_comboBoxMentalSampleReportID_3->setEditText(record.value("ferrographyreportid").toString());
        ui->_comboBoxMentalSampleMicroType_3->setEditText(record.value("microscopictype").toString());
        ui->_comboBoxMentalSampleSamplerType_3->setEditText(record.value("imageacquisitiontype").toString());

        ui->_comboBoxMentalSampleLightType_3->setEditText(record.value("lightsourcetype").toString());
        ui->_editMentalSampleEnlarger_3->setText(record.value("magnification").toString());

        ui->_comboBoxMentalSampleArea_3->setEditText(record.value("imageacquisitionarea").toString());
        ui->_comboBoxMentalSampleGuy_3->setEditText(record.value("imageacquisitionstuff").toString());
        ui->_editMentalSamplePath_3->setText(record.value("ferrographypicpath").toString());
        ui->_editMentalSampleAnalysis_3->setText(record.value("imagerecognitioninfoanalysis").toString());
    }
    else if(_tableIndex == 7)
    {
        ui->_comboBoxMoliID_3->setEditText(record.value("abrasiveid").toString());
        ui->_comboBoxMoliImageID_3->setEditText(record.value("ferrographypicid").toString());
        ui->_comboBoxMoliPianID_3->setEditText(record.value("ferrographysheetid").toString());
        ui->_comboBoxMoliReportID_3->setEditText(record.value("ferrographyreportid").toString());
        ui->_comboBoxMoliGuy_3->setEditText(record.value("abrasivemarkstuff").toString());

        ui->_editMoliPath_3->setText(record.value("abrasivepicpath").toString());
        ui->_comboBoxMoliProperty_3->setEditText(record.value("abrasivematerial").toString());
        ui->_comboBoxMoliPosition_3->setEditText(record.value("abrasiveposition").toString());
        ui->_editMoliSize_3->setText(record.value("abrasivesize").toString());

        ui->_editMoliLength_3->setText(record.value("abrasivesperimeter").toString());
        ui->_comboBoxMoliShape_3->setEditText(record.value("abrasiveshape").toString());
        ui->_comboBoxMoliColor_3->setEditText(record.value("abrasivecolor").toString());
        ui->_comboBoxMoliSurface_3->setEditText(record.value("abrasivesurfacetexturetype").toString());
        ui->_comboBoxMoliErodeType_3->setEditText(record.value("abrasiveweartype").toString());
        ui->_comboBoxMoliErodePart_3->setEditText(record.value("abrasivedamagetype").toString());
        ui->_comboBoxMoliErodeReason_3->setEditText(record.value("abrasivemechanismtype").toString());
        ui->_comboBoxMoliGivenInfo_3->setEditText(record.value("abrasivefaultinformationreflection").toString());
        ui->_comboBoxMoliTypical_3->setEditText(record.value("abrasivetypical").toString());
    }
    else if(_tableIndex == 8 && _primaryKeyValue != "NEW")
    {
        ui->comboBox_88->setEditText(record.value("sampleID").toString());
        ui->comboBox_89->setEditText(record.value("equitName").toString());
        ui->comboBox_90->setEditText(record.value("equipID").toString());
        ui->comboBox_91->setEditText(record.value("equipType").toString());
        ui->comboBox_92->setEditText(record.value("sampleMethod").toString());
        ui->lineEdit_22->setText(record.value("sampleAttention").toString());
    }
}


void ImagePropertiesEditor::on__buttonCancel_3_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(0, tr("提示"), tr("关闭将导致所填写的数据丢失, 是否确认退出?"), QMessageBox::Ok | QMessageBox::Cancel);
    if(reply == QMessageBox::Ok)
    {
        close();
    }
}

void ImagePropertiesEditor::on__buttonSave_3_clicked()
{
    QSqlRecord record;
    QString tables[] = {"equipmentinfo", "movepartinfo", "movepartrepairinfo", "oilsampleinfo", "oilanalyzeinfo", "ferrographyinfo", "ferrographypicinfo", "abrasivemarkinfo", "sampleSummaryInfo"};

    QSqlDatabase db;
    if(Global::createConnection(db))
    {
        _model = new QSqlTableModel(this, db);
        _model->setTable(tables[_tableIndex]);
        _model->select();
        if(_tableIndex == 0)
        {
            _model->setFilter(QString("planeid = '%1'").arg(ui->_comboBoxEquipPlaneID_3->currentText()));
            if(_model->select() && _model->rowCount() == 1)
            {
                QSqlRecord record = _model->record(0);
                record.setValue("planeid", ui->_comboBoxEquipPlaneID_3->currentText());
                record.setValue("planetype", ui->_comboBoxEquipPlaneType_3->currentText());
                record.setValue("departid", ui->_comboBoxEquipUnitID_3->currentText());
                record.setValue("runhour", ui->_editEquipHours_3->text().toDouble());
                record.setValue("runstage", ui->_comboBoxEquipRuntime_3->currentText());
                record.setValue("repairtime", ui->_editServiceNumber_3->text().toInt());
                _model->setRecord(0, record);
            }
        }
        else if(_tableIndex == 1)
        {
            _model->setFilter(QString("movepartid = '%1'").arg(ui->_comboBoxMovepartID_3->currentText()));
            if(_model->select() && _model->rowCount() == 1)
            {
                QSqlRecord record = _model->record();
                record.setValue("movepartid", ui->_comboBoxMovepartID_3->currentText());
                record.setValue("moveparttype", ui->_comboBoxMovepartType_3->currentText());
                record.setValue("movepartname", ui->_comboBoxMovepartName_3->currentText());
                record.setValue("runhour", ui->_editMovepartHours_3->text().toDouble());
                record.setValue("runstage", ui->_comboBoxMovepartMohe_3->currentText());
                record.setValue("planeid", ui->_comboBoxMovepartPlaneID_3->currentText());
                record.setValue("planetype", ui->_comboBoxMovepartPlaneType_3->currentText());
                record.setValue("startdate", ui->_dateEditMovepartBegin_3->text());
                record.setValue("enddate", ui->_dateEditMovepartEnd_3->text());
                _model->setRecord(0, record);
            }
        }
        else if(_tableIndex == 2)
        {
            _model->setFilter(QString("movepartrepairid = '%1'").arg(ui->_comboBoxMovepartServiceID_3->currentText()));
            if(_model->select() && _model->rowCount() == 1)
            {
                QSqlRecord record = _model->record();
                record.setValue("movepartrepairid", ui->_comboBoxMovepartServiceID_3->currentText());
                record.setValue("movepartid", ui->_comboBoxMovepartServiceMovepartID_3->currentText());
                record.setValue("moveparttype", ui->_comboBoxMovepartServiceMovepartType_3->currentText());
                record.setValue("repairreason", ui->_editMovepartServiceReson_3->text());
                record.setValue("repairdate", ui->_dateEditMovepartServiceDate_3->text());
                record.setValue("repairtime", ui->_editMovepartServiceNumber_3->text().toInt());
                record.setValue("repairdepart", ui->_comboBoxMovepartServiceUnit_3->currentText());
                record.setValue("repaircontent", ui->_editMovepartServiceContent_3->text());
                record.setValue("repaircondition", ui->_editMovepartServiceInfo_3->text());
                _model->setRecord(0, record);
            }
        }
        else if(_tableIndex == 3)
        {
            _model->setFilter(QString("oilsampleid = '%1'").arg(ui->_comboBoxOilSampleID_3->currentText()));
            if(_model->select() && _model->rowCount() == 1)
            {
                QSqlRecord record = _model->record();
                record.setValue("oilsampleid", ui->_comboBoxOilSampleID_3->currentText());
                record.setValue("sampledepartid", ui->_comboBoxOilSampleUnitID_3->currentText());
                record.setValue("planetype", ui->_comboBoxOilSamplePlaneType_3->currentText());
                record.setValue("planeid", ui->_comboBoxOilSamplePlaneID_3->currentText());
                record.setValue("monitorpartname", ui->_comboBoxOilSampleMonitorPart_3->currentText());
                record.setValue("monitorpartid", ui->_comboBoxOilSampleMonitorPartID_3->currentText());
                record.setValue("sampleid", ui->_comboBoxOilSampleSamplePointID_3->currentText());
                record.setValue("oilworktime", ui->_editOilSampleHours_3->text());
                record.setValue("oiladdition", ui->_editOilSampleMount_3->text().toInt());
                record.setValue("samplereason", ui->_editOilSampleReason_3->text());
                record.setValue("sampledepartname", ui->_comboBoxOilSampleSampleUnit_3->currentText());
                record.setValue("samplestuff", ui->_comboBoxOilSampleSampleGuy_3->currentText());
                record.setValue("sampledate", ui->_dateEditOilSampleSampleDate_3->text());
                record.setValue("sampletime", ui->_timeEditOilSampleSampleTime_3->text());
                record.setValue("samplesituation", ui->_comboBoxOilSampleSituation_3->currentText());
                record.setValue("samplemethod", ui->_comboBoxOilSampleSampleMethod_3->currentText());
                record.setValue("samplevolume", ui->_editOilSampleSampleMount_3->text().toInt());
                record.setValue("sampleinstruction", ui->_editOilSampleInfo_3->text());
                record.setValue("sendstuff", ui->_comboBoxOilSampleSendGuy_3->currentText());
                record.setValue("senddate", ui->_dateEditOilSampleSendDate_3->text());
                record.setValue("sendtime", ui->_timeEditOilSampleSendTime_3->text());
                _model->setRecord(0, record);
            }
        }
        else if(_tableIndex == 4)
        {
            _model->setFilter(QString("oilsampleid = '%1'").arg(ui->_comboBoxOilAnalyzeOilSampleID_3->currentText()));
            if(_model->select() && _model->rowCount() == 1)
            {
                QSqlRecord record = _model->record();
                record.setValue("oilsampleid", ui->_comboBoxOilAnalyzeOilSampleID_3->currentText());
                record.setValue("analyzedepartname", ui->_comboBoxOilAnalyzeUnitName_3->currentText());
                record.setValue("senddepart",  ui->_comboBoxOilAnalyzeSendUnit_3->currentText());
                record.setValue("sendreason", ui->_editOilAnalyzeReason_3->text());
                record.setValue("sendstuff", ui->_comboBoxOilAnalyzeSendGuy_3->currentText());
                record.setValue("receivedate", ui->_dateEditOilAnalyzeReceiveDate_3->text());
                record.setValue("receivestuff", ui->_comboBoxOilAnalyzeReceiveGuy_3->currentText());
                record.setValue("contaminationanalyzemethod", ui->_comboBoxOilAnalyzePollutionLevelMethod_3->currentText());
                record.setValue("contaminationanalyzestuff", ui->_comboBoxOilAnalyzePollutionLevelGuy_3->currentText());
                record.setValue("contaminationanalyzedate", ui->_dateEditOilAnalyzePollutionDate_3->text());
                record.setValue("contaminationanalyzeequipment", ui->_comboBoxOilAnalyzePolluteLevelEquip_3->currentText());
                record.setValue("contaminationanalyzereportid", ui->_comboBoxOilAnalyzePolluteReportID_3->currentText());

                record.setValue("spectroscopymethod", ui->_comboBoxOilAnalyzeLightMethod_3->currentText());
                record.setValue("spectroscopystuff", ui->_comboBoxOilAnalyzeLightGuy_3->currentText());
                record.setValue("spectroscopydate", ui->_dateEditOilAnalyzeLightDate_3->text());
                record.setValue("spectroscopyequipment", ui->_comboBoxOilAnalyzeLightEquip_3->currentText());
                record.setValue("spectroscopyreportid", ui->_comboBoxOilAnalyzeLightEquipID_3->currentText());

                record.setValue("ferrographymethod", ui->_comboBoxOilAnalyzeMentalMethod_3->currentText());
                record.setValue("ferrographystuff", ui->_comboBoxOilAnalyzeMentalGuy_3->currentText());
                record.setValue("ferrographydate", ui->_dateEditOilAnalyzeMentalDate_3->text());
                record.setValue("ferrographyequipment", ui->_comboBoxOilAnalyzeMentalEquip_3->currentText());
                record.setValue("ferrographyreportid", ui->_comboBoxOilAnalyzeMentalEquipID_3->currentText());

                record.setValue("physicochemicalmethod", ui->_comboBoxOilAnalyzeLihuaMethod_3->currentText());
                record.setValue("physicochemicalstuff", ui->_comboBoxOilAnalyzeLihuaGuy_3->currentText());
                record.setValue("physicochemicaldate", ui->_dateEditOilAnalyzeLihuaDate_3->text());
                record.setValue("physicochemicalequipment", ui->_comboBoxOilAnalyzeLihuaEquip_3->currentText());
                record.setValue("physicochemicalreportid", ui->_comboBoxOilAnalyzeLihuaEquipID_3->currentText());

                _model->setRecord(0, record);
            }
        }
        else if(_tableIndex == 5)
        {
            _model->setFilter(QString("ferrographysheetid = '%1'").arg(ui->_comboBoxMentalID_3->currentText()));
            if(_model->select() && _model->rowCount() == 1)
            {
                QSqlRecord record = _model->record();
                record.setValue("ferrographysheetid", ui->_comboBoxMentalID_3->currentText());
                record.setValue("ferrographyreportid", ui->_comboBoxMentalReportID_3->currentText());
                record.setValue("oilsampleid", ui->_comboBoxMentalOilSampleID_3->currentText());
                record.setValue("ferrographyanalyzertype", ui->_comboBoxMentalInstrumentType_3->currentText());
                record.setValue("ferrographymakeoilconsumption", ui->_editMentalMount_3->text());
                record.setValue("ferrographymakemethod", ui->_comboBoxMentalMethod_3->currentText());
                record.setValue("ferrographymakestuff", ui->_comboBoxMentalGuy_3->currentText());
                _model->setRecord(0, record);
            }
        }
        else if(_tableIndex == 6)
        {
            _model->setFilter(QString("ferrographypicid = '%1'").arg(ui->_comboBoxMentalSampleImageID_3->currentText()));
            if(_model->select() && _model->rowCount() == 1)
            {
                QSqlRecord record = _model->record();
                record.setValue("ferrographypicid", ui->_comboBoxMentalSampleImageID_3->currentText());
                record.setValue("ferrographysheetid", ui->_comboBoxMentalSampleID_3->currentText());
                record.setValue("ferrographyreportid", ui->_comboBoxMentalSampleReportID_3->currentText());
                record.setValue("microscopictype", ui->_comboBoxMentalSampleMicroType_3->currentText());
                record.setValue("imageacquisitiontype", ui->_comboBoxMentalSampleSamplerType_3->currentText());
                record.setValue("lightsourcetype", ui->_comboBoxMentalSampleLightType_3->currentText());
                record.setValue("magnification", ui->_editMentalSampleEnlarger_3->text().toInt());

                record.setValue("imageacquisitionarea", ui->_comboBoxMentalSampleArea_3->currentText());
                record.setValue("imageacquisitionstuff", ui->_comboBoxMentalSampleGuy_3->currentText());
                record.setValue("ferrographypicpath", ui->_editMentalSamplePath_3->text());
                record.setValue("imagerecognitioninfoanalysis", ui->_editMentalSampleAnalysis_3->text());
                _model->setRecord(0, record);
            }
        }
        else if(_tableIndex == 7)
        {
            _model->setFilter(QString("abrasiveid = '%1'").arg(ui->_comboBoxMoliID_3->currentText()));
            if(_model->select() && _model->rowCount() == 1)
            {
                QSqlRecord record = _model->record();
                record.setValue("abrasiveid", ui->_comboBoxMoliID_3->currentText());
                record.setValue("ferrographypicid", ui->_comboBoxMoliImageID_3->currentText());
                record.setValue("ferrographysheetid", ui->_comboBoxMoliPianID_3->currentText());
                record.setValue("ferrographyreportid", ui->_comboBoxMoliReportID_3->currentText());
                record.setValue("abrasivemarkstuff", ui->_comboBoxMoliGuy_3->currentText());
                record.setValue("abrasivepicpath", ui->_editMoliPath_3->text());
                record.setValue("abrasivematerial", ui->_comboBoxMoliProperty_3->currentText());
                record.setValue("abrasiveposition", ui->_comboBoxMoliPosition_3->currentText());
                record.setValue("abrasivesize", ui->_editMoliSize_3->text());

                record.setValue("abrasivesperimeter", ui->_editMoliLength_3->text().toDouble());
                record.setValue("abrasiveshape", ui->_comboBoxMoliShape_3->currentText());
                record.setValue("abrasivecolor", ui->_comboBoxMoliColor_3->currentText());
                record.setValue("abrasivesurfacetexturetype", ui->_comboBoxMoliSurface_3->currentText());
                record.setValue("abrasiveweartype", ui->_comboBoxMoliErodeType_3->currentText());
                record.setValue("abrasivedamagetype", ui->_comboBoxMoliErodePart_3->currentText());
                record.setValue("abrasivemechanismtype", ui->_comboBoxMoliErodeReason_3->currentText());
                record.setValue("abrasivefaultinformationreflection", ui->_comboBoxMoliGivenInfo_3->currentText());
                record.setValue("abrasivetypical", ui->_comboBoxMoliTypical_3->currentText());
                _model->setRecord(0, record);
            }
        }
        else if(_tableIndex == 8)
        {
            _model->setFilter(QString("sampleID = '%1'").arg(ui->comboBox_88->currentText()));
            if(_model->select() )
            {
                if(_model->rowCount() == 1 && _primaryKeyValue != "NEW")
                {
                    QSqlRecord record = _model->record(0);
                    record.setValue("sampleID", ui->comboBox_88->currentText());
                    record.setValue("equitName", ui->comboBox_89->currentText());
                    record.setValue("equipID", ui->comboBox_90->currentText());
                    record.setValue("equipType", ui->comboBox_91->currentText());
                    record.setValue("sampleMethod", ui->comboBox_92->currentText());
                    record.setValue("sampleAttention", ui->lineEdit_22->text());
                    _model->setRecord(0, record);
                }
                else if(_model->rowCount() == 0 && _primaryKeyValue == "NEW")
                {
                   _model->insertRows(0, 1);
                   _model->setData(_model->index(0, 0), ui->comboBox_88->currentText());
                   _model->setData(_model->index(0, 1), ui->comboBox_89->currentText());
                   _model->setData(_model->index(0, 2), ui->comboBox_90->currentText());
                   _model->setData(_model->index(0, 3), ui->comboBox_91->currentText());
                   _model->setData(_model->index(0, 4), ui->comboBox_92->currentText());
                   _model->setData(_model->index(0, 5), ui->lineEdit_22->text());
                }
                else if(_model->rowCount() > 0 && _primaryKeyValue == "NEW")
                {
                    QMessageBox::warning(this, tr("提示"), tr("采样点已存在!"), QMessageBox::Close);
                    return;
                }
            }
        }

        if(!_model->submitAll())
        {
            _model->revertAll();
            QMessageBox::warning(this, tr("提示"), tr("保存失败!"), QMessageBox::Close);
        }
        else
        {
            emit flush();
            QMessageBox::warning(this, tr("提示"), tr("保存成功!"), QMessageBox::Close);
            close();
        }
    }
}
