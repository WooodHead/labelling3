#include "ImageProperties.h"
#include "ui_imageproperties.h"
#include "Global.h"

#include <QCloseEvent>

ImageProperties::ImageProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageProperties)
{
    ui->setupUi(this);
    setWindowTitle(tr("�༭ͼ������"));

    ui->_tabWidget->removeTab(8);
    ui->_tabWidget->removeTab(7);

    ui->_tabWidget->setCurrentIndex(0);
    ui->_buttonSave->setText(tr("����"));
    ui->_buttonSave->setEnabled(false);
    ui->_buttonSave->setIcon(Global::Awesome->icon(save));
    ui->_buttonCancel->setText(tr("ȡ��"));
    ui->_buttonCancel->setIcon(Global::Awesome->icon(remove_));
    ui->_buttonNext->setText(tr("��һ�ű�"));
    ui->_buttonNext->setIcon(Global::Awesome->icon(forward));

    connect(this, SIGNAL(flush()), parent, SLOT(flushBottom()));
    connect(this, SIGNAL(synchImageName(QString)), parent, SLOT(synchImageName(QString)));
    connect(this, SIGNAL(removeImage(QString)), parent, SLOT(removeImage(QString)));

    _bCommited = false;
    for(int i = 0; i < TABLE_N; i++ ) _models[i] = 0;
    for(int i = 0; i < TABLE_N; i++ ) _bSaved[i] = false;
    for(int i = 1; i < TABLE_N; i++ ) ui->_tabWidget->setTabEnabled(i, false);

    ui->_editEquipHours->setValidator(new QIntValidator(0, 1000000, this));
    ui->_editServiceNumber->setValidator(new QIntValidator(0, 1000000, this));
    ui->_editMovepartHours->setValidator(new QIntValidator(0, 1000000, this));
    ui->_editMovepartServiceNumber->setValidator(new QIntValidator(0, 1000000, this));
    ui->_editOilSampleHours->setValidator(new QIntValidator(0, 1000000, this));
    ui->_editOilSampleMount->setValidator(new QDoubleValidator(0.0, 1000000.0, 5, this));
    ui->_editOilSampleSampleMount->setValidator(new QDoubleValidator(0.0, 1000000.0, 5, this));
    ui->_editMentalMount->setValidator(new QDoubleValidator(0.0, 1000000.0, 5, this));
    ui->_editMentalSampleEnlarger->setValidator(new QIntValidator(1, 1000, this));
    ui->_editMoliSize->setValidator(new QDoubleValidator(0.0, 100.0, 5, this));
    ui->_editMoliLength->setValidator(new QDoubleValidator(0.0, 100.0, 5, this));

    ui->_timeEditOilSampleSampleTime->setDisplayFormat("yyyyMMdd");

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
    QString tableNames[TABLE_N] = { "equipmentinfo", "movepartinfo", "movepartrepairinfo", "oilsampleinfo", "oilanalyzeinfo", "ferrographyinfo", "ferrographypicinfo" };

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
    ui->_comboBoxMovepartType->addItems(getItems(_models[1],   "moveparttype"));

    QStringList list = QStringList() << "" << tr("��(A)") << tr("�ҷ�(B)") << tr("�з�(C)") << tr("����(D)") << tr("β��(E)") << tr("�м�(F)") << tr("�����(G)") << tr("��������װ��(H)") << tr("��֬(J)") << tr("��Һѹϵͳ(K)") << tr("��Һѹϵͳ(L)") << tr("ȼ��ϵͳ(M)") << tr("����(N)");
    ui->_comboBoxMovepartName->addItems(list);

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
    //ui->_comboBoxOilSampleID->addItems(getItems(_models[3], "oilsampleid"));
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
    //ui->_comboBoxOilAnalyzeOilSampleID->addItems(getItems(_models[4], "oilsampleid"));
    ui->_comboBoxOilAnalyzeUnitName->addItems(getItems(_models[4], "analyzedepartname"));
    ui->_comboBoxOilAnalyzeSendUnit->addItems(getItems(_models[4], "senddepart"));
    ui->_comboBoxOilAnalyzeSendGuy->addItems(getItems(_models[4], "sendstuff"));
    ui->_comboBoxOilAnalyzeReceiveGuy->addItems(getItems(_models[4], "receivestuff"));
    ui->_comboBoxOilAnalyzePollutionLevelMethod->addItems(getItems(_models[4], "contaminationanalyzemethod"));
    ui->_comboBoxOilAnalyzePollutionLevelGuy->addItems(getItems(_models[4], "contaminationanalyzestuff"));
    ui->_comboBoxOilAnalyzePolluteLevelEquip->addItems(getItems(_models[4], "contaminationanalyzeequipment"));
    //ui->_comboBoxOilAnalyzePolluteReportID->addItems(getItems(_models[4], "contaminationanalyzereportid"));
    ui->_comboBoxOilAnalyzeLightMethod->addItems(getItems(_models[4], "spectroscopymethod"));
    ui->_comboBoxOilAnalyzeLightGuy->addItems(getItems(_models[4], "spectroscopystuff"));
    ui->_comboBoxOilAnalyzeLightEquip->addItems(getItems(_models[4], "spectroscopyequipment"));
    //ui->_comboBoxOilAnalyzeLightEquipID->addItems(getItems(_models[4], "spectroscopyreportid"));
    ui->_comboBoxOilAnalyzeMentalMethod->addItems(getItems(_models[4], "ferrographymethod"));
    ui->_comboBoxOilAnalyzeMentalGuy->addItems(getItems(_models[4], "ferrographystuff"));
    ui->_comboBoxOilAnalyzeMentalEquip->addItems(getItems(_models[4], "ferrographyequipment"));
    //ui->_comboBoxOilAnalyzeMentalEquipID->addItems(getItems(_models[4], "ferrographyreportid"));

    ui->_comboBoxOilAnalyzeLihuaMethod->addItems(getItems(_models[4], "physicochemicalmethod"));
    ui->_comboBoxOilAnalyzeLihuaGuy->addItems(getItems(_models[4], "physicochemicalstuff"));
    ui->_comboBoxOilAnalyzeLihuaEquip->addItems(getItems(_models[4], "physicochemicalequipment"));
    //ui->_comboBoxOilAnalyzeLihuaEquipID->addItems(getItems(_models[4], "physicochemicalreportid"));

    // model 5
    //ui->_comboBoxMentalID->addItems(getItems(_models[5], "ferrographysheetid"));
    ui->_comboBoxMentalReportID->addItems(getItems(_models[5], "ferrographyreportid"));
    ui->_comboBoxMentalOilSampleID->addItems(getItems(_models[5], "oilsampleid"));

    QStringList list2 = QStringList() << "" << tr("��������������(A)") << tr("���׼�ʽ����������(B)") << tr("�����ȵ����������(C)") << tr("��תʽ����������(D)") << tr("��Ĥ������(E)");
    ui->_comboBoxMentalInstrumentType->addItems(list2);
    ui->_comboBoxMentalMethod->addItems(getItems(_models[5], "ferrographymakemethod"));
    ui->_comboBoxMentalGuy->addItems(getItems(_models[5], "ferrographymakestuff"));

    // model 6
    //ui->_comboBoxMentalSampleImageID->addItems(getItems(_models[6], "ferrographypicid"));
    ui->_comboBoxMentalSampleID->addItems(getItems(_models[6], "ferrographysheetid"));
    ui->_comboBoxMentalSampleReportID->addItems(getItems(_models[6], "ferrographyreportid"));
    ui->_comboBoxMentalSampleMicroType->addItems(getItems(_models[6], "microscopictype"));
    ui->_comboBoxMentalSampleSamplerType->addItems(getItems(_models[6], "imageacquisitiontype"));
    ui->_comboBoxMentalSampleGuy->addItems(getItems(_models[6], "imageacquisitionstuff"));

    QStringList list3 = QStringList() << "" << "LA" << "LB" << "LC" << "LD" << "LE";
    ui->_comboBoxMentalSampleLightType->addItems(list3);

    QStringList list4 = QStringList() << "" << tr("Ԥ����Ƭ���(QA)") << tr("��Ƭǰ���(QB)") << tr("��Ƭ�ж�(QC)") << tr("��Ƭ����(QD)") << tr("��Ƭ����(QE)");
    ui->_comboBoxMentalSampleArea->addItems(list4);

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
        QMessageBox::warning(this, tr("��ʾ"), tr("���Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxEquipPlaneType->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(0);
        QMessageBox::warning(this, tr("��ʾ"), tr("���Ͳ���Ϊ��!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxEquipUnitID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(0);
        QMessageBox::warning(this, tr("��ʾ"), tr("��λ��Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxMovepartID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(1);
        QMessageBox::warning(this, tr("��ʾ"), tr("��������Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMovepartName->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(1);
        QMessageBox::warning(this, tr("��ʾ"), tr("���������Ʋ���Ϊ��!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMovepartPlaneID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(1);
        QMessageBox::warning(this, tr("��ʾ"), tr("���Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMovepartPlaneType->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(1);
        QMessageBox::warning(this, tr("��ʾ"), tr("���Ͳ���Ϊ��!"), QMessageBox::Close);
        return false;
    }

    //    if(ui->_comboBoxMovepartServiceID->currentText().isEmpty())
    //    {
    //        ui->_tabWidget->setCurrentIndex(2);
    //        QMessageBox::warning(this, tr("��ʾ"), tr("������ά�ޱ�Ų���Ϊ��!"), QMessageBox::Close);
    //        return false;
    //    }
    //    if(ui->_comboBoxMovepartServiceMovepartID->currentText().isEmpty())
    //    {
    //        ui->_tabWidget->setCurrentIndex(2);
    //        QMessageBox::warning(this, tr("��ʾ"), tr("��������Ų���Ϊ��!"), QMessageBox::Close);
    //        return false;
    //    }

    if(ui->_dateEditOilSampleSampleDate->text().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(3);
        QMessageBox::warning(this, tr("��ʾ"), tr("�������ڲ���Ϊ��!"), QMessageBox::Close);
        return false;
    }

    if(ui->_timeEditOilSampleSampleTime->text().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(3);
        QMessageBox::warning(this, tr("��ʾ"), tr("����ʱ�䲻��Ϊ��!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxOilSampleID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(3);
        QMessageBox::warning(this, tr("��ʾ"), tr("������Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxOilSampleUnitID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(3);
        QMessageBox::warning(this, tr("��ʾ"), tr("��λ��Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxOilSamplePlaneID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(3);
        QMessageBox::warning(this, tr("��ʾ"), tr("���Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxOilSamplePlaneType->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(3);
        QMessageBox::warning(this, tr("��ʾ"), tr("���Ͳ���Ϊ��!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxOilSampleSamplePointID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(3);
        QMessageBox::warning(this, tr("��ʾ"), tr("�������Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxOilAnalyzeOilSampleID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(4);
        QMessageBox::warning(this, tr("��ʾ"), tr("������Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxMentalInstrumentType->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(5);
        QMessageBox::warning(this, tr("��ʾ"), tr("�����������ͺŲ���Ϊ��!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxMentalID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(5);
        QMessageBox::warning(this, tr("��ʾ"), tr("����Ƭ��Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMentalOilSampleID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(5);
        QMessageBox::warning(this, tr("��ʾ"), tr("������Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxMentalSampleArea->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(6);
        QMessageBox::warning(this, tr("��ʾ"), tr("����ͼƬ�ɼ�������Ϊ��!"), QMessageBox::Close);
        return false;
    }

    if(ui->_editMentalSampleEnlarger->text().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(6);
        QMessageBox::warning(this, tr("��ʾ"), tr("�Ŵ�������Ϊ��!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxMentalSampleLightType->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(6);
        QMessageBox::warning(this, tr("��ʾ"), tr("��Դ���Ͳ���Ϊ��!"), QMessageBox::Close);
        return false;
    }

    if(ui->_comboBoxMentalSampleImageID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(6);
        QMessageBox::warning(this, tr("��ʾ"), tr("����ͼƬ��Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }
    if(ui->_comboBoxMentalSampleID->currentText().isEmpty())
    {
        ui->_tabWidget->setCurrentIndex(6);
        QMessageBox::warning(this, tr("��ʾ"), tr("����Ƭ��Ų���Ϊ��!"), QMessageBox::Close);
        return false;
    }

    return true;
}

bool ImageProperties::isValid(int index)
{
    if(index == 0)
    {
        if(ui->_comboBoxEquipPlaneID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(0);
            QMessageBox::warning(this, tr("��ʾ"), tr("���Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxEquipPlaneType->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(0);
            QMessageBox::warning(this, tr("��ʾ"), tr("���Ͳ���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxEquipUnitID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(0);
            QMessageBox::warning(this, tr("��ʾ"), tr("��λ��Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
    }
    else if(index == 1)
    {
        if(ui->_comboBoxMovepartID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(1);
            QMessageBox::warning(this, tr("��ʾ"), tr("��������Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxMovepartName->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(1);
            QMessageBox::warning(this, tr("��ʾ"), tr("���������Ʋ���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxMovepartPlaneID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(1);
            QMessageBox::warning(this, tr("��ʾ"), tr("���Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxMovepartPlaneType->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(1);
            QMessageBox::warning(this, tr("��ʾ"), tr("���Ͳ���Ϊ��!"), QMessageBox::Close);
            return false;
        }
    }
    else if(index == 2)
    {
        //        if(ui->_comboBoxMovepartServiceID->currentText().isEmpty())
        //        {
        //            ui->_tabWidget->setCurrentIndex(2);
        //            QMessageBox::warning(this, tr("��ʾ"), tr("������ά�ޱ�Ų���Ϊ��!"), QMessageBox::Close);
        //            return false;
        //        }
        //        if(ui->_comboBoxMovepartServiceMovepartID->currentText().isEmpty())
        //        {
        //            ui->_tabWidget->setCurrentIndex(2);
        //            QMessageBox::warning(this, tr("��ʾ"), tr("��������Ų���Ϊ��!"), QMessageBox::Close);
        //            return false;
        //        }
    }
    else if(index == 3)
    {
        if(ui->_dateEditOilSampleSampleDate->text().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(3);
            QMessageBox::warning(this, tr("��ʾ"), tr("�������ڲ���Ϊ��!"), QMessageBox::Close);
            return false;
        }

        if(ui->_timeEditOilSampleSampleTime->text().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(3);
            QMessageBox::warning(this, tr("��ʾ"), tr("����ʱ�䲻��Ϊ��!"), QMessageBox::Close);
            return false;
        }

        if(ui->_comboBoxOilSampleID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(3);
            QMessageBox::warning(this, tr("��ʾ"), tr("������Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxOilSampleUnitID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(3);
            QMessageBox::warning(this, tr("��ʾ"), tr("��λ��Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxOilSamplePlaneID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(3);
            QMessageBox::warning(this, tr("��ʾ"), tr("���Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxOilSamplePlaneType->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(3);
            QMessageBox::warning(this, tr("��ʾ"), tr("���Ͳ���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxOilSampleSamplePointID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(3);
            QMessageBox::warning(this, tr("��ʾ"), tr("�������Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
    }
    else if(index == 4)
    {
        if(ui->_comboBoxOilAnalyzeOilSampleID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(4);
            QMessageBox::warning(this, tr("��ʾ"), tr("������Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
    }
    else if(index == 5)
    {
        if(ui->_comboBoxMentalInstrumentType->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(5);
            QMessageBox::warning(this, tr("��ʾ"), tr("�����������ͺŲ���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxMentalID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(5);
            QMessageBox::warning(this, tr("��ʾ"), tr("����Ƭ��Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxMentalOilSampleID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(5);
            QMessageBox::warning(this, tr("��ʾ"), tr("������Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
    }
    else if(index == 6)
    {
        if(ui->_comboBoxMentalSampleArea->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(6);
            QMessageBox::warning(this, tr("��ʾ"), tr("����ͼƬ�ɼ�������Ϊ��!"), QMessageBox::Close);
            return false;
        }

        if(ui->_editMentalSampleEnlarger->text().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(6);
            QMessageBox::warning(this, tr("��ʾ"), tr("�Ŵ�������Ϊ��!"), QMessageBox::Close);
            return false;
        }

        if(ui->_comboBoxMentalSampleLightType->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(6);
            QMessageBox::warning(this, tr("��ʾ"), tr("��Դ���Ͳ���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxMentalSampleImageID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(6);
            QMessageBox::warning(this, tr("��ʾ"), tr("����ͼƬ��Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
        if(ui->_comboBoxMentalSampleID->currentText().isEmpty())
        {
            ui->_tabWidget->setCurrentIndex(6);
            QMessageBox::warning(this, tr("��ʾ"), tr("����Ƭ��Ų���Ϊ��!"), QMessageBox::Close);
            return false;
        }
    }

    return true;
}

void ImageProperties::showDlg(QString filename)
{
    show();

    ui->_editMoliPath->setText(filename);
    ui->_editMentalSamplePath->setText(filename);

    _originalImagePath = filename;
    QPixmap image;
    if(image.load(_originalImagePath))
    {
        ui->_labelOriginalImage_2->setPixmap(image);
        ui->_labelOriginalImage_2->setScaledContents(true);
    }
}

void ImageProperties::on__buttonSave_clicked()
{
    int index = ui->_tabWidget->currentIndex();
    if( index != ui->_tabWidget->count()-1)
    {
        if(!isValid(index)) return;
        else _bSaved[index] = true;
    }
    else
    {
        _bSaved[TABLE_N-1] = true;
        for(int i = 0; i < TABLE_N; i++)
        {
            if(_bSaved[i] == false)
            {
                QMessageBox::warning(this, tr("��ʾ"), QString("��%1δ����").arg(i+1), QMessageBox::Close);
                return;
            }
        }
        if(!isValid()) return;

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
                record.setValue("moveparttype", ui->_comboBoxMovepartType->currentText());
                record.setValue("movepartname", ui->_comboBoxMovepartName->currentText());
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

                QString imagePath = ui->_editMentalSamplePath->text();
                if(!imagePath.isEmpty())
                {
                    QFile *file = new QFile(imagePath);
                    file->open(QIODevice::ReadOnly);
                    QByteArray data = file->readAll();
                    record.setValue("ferrographypic", data);
                }

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
                _models[6]->setData(_models[6]->index(0, 11), "N");

                QString imagePath = ui->_editMentalSamplePath->text();
                if(!imagePath.isEmpty())
                {
                    QFile *file = new QFile(imagePath);
                    file->open(QIODevice::ReadOnly);
                    QByteArray data = file->readAll();
                    _models[6]->setData(_models[6]->index(0, 12), data);
                }
                copyOrgImage(ui->_comboBoxMentalSampleImageID->currentText(), ui->_editMentalSamplePath->text());

                //                if(!copyTo.isEmpty())
                //                {
                //                    ui->_editMentalSamplePath->setText(copyTo);
                //                    emit synchImageName(copyTo);
                //                }
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
                QMessageBox::warning(this, tr("��ʾ"), tr("����ʧ��!"), QMessageBox::Close);
                return;
            }
        }

        // Save Summary Info;

        QSqlTableModel* model = new QSqlTableModel;
        model->setTable("sampleSummaryInfo");
        model->setFilter(QString("sampleID = '%1'").arg(ui->_comboBoxOilSampleSamplePointID->currentText()));
        if(model->select())
        {
            if(model->rowCount() == 1)
            {
                QSqlRecord record = model->record(0);
                record.setValue("sampleID", ui->_comboBoxOilSampleSamplePointID->currentText());
                record.setValue("equitName", ui->_comboBoxMovepartName->currentText());
                record.setValue("equipID", ui->_comboBoxEquipPlaneID->currentText());
                record.setValue("equipType", ui->_comboBoxEquipPlaneType->currentText());
                record.setValue("sampleMethod", ui->_comboBoxOilSampleSampleMethod->currentText());
                record.setValue("sampleAttention", ui->_editOilSampleInfo->text());
                model->setRecord(0, record);
            }
            else
            {
                model->insertRows(0, 1);
                model->setData(model->index(0, 0), ui->_comboBoxOilSampleSamplePointID->currentText());
                model->setData(model->index(0, 1), ui->_comboBoxMovepartName->currentText());
                model->setData(model->index(0, 2), ui->_comboBoxEquipPlaneID->currentText());
                model->setData(model->index(0, 3), ui->_comboBoxEquipPlaneType->currentText());
                model->setData(model->index(0, 4), ui->_comboBoxOilSampleSampleMethod->currentText());
                model->setData(model->index(0, 5), ui->_editOilSampleInfo->text());
            }
            model->submitAll();
        }

        _bCommited = true;
        QMessageBox::warning(this, tr("��ʾ"), tr("����ɹ�!"), QMessageBox::Close);

        emit flush();

        close();
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

void ImageProperties::on__buttonNext_clicked()
{
    int index = ui->_tabWidget->currentIndex();
    if(index < ui->_tabWidget->count())
    {
        if(!isValid(index)) return;
        else
        {
            _bSaved[index] = true;

            ui->_tabWidget->setTabEnabled(index + 1, true);
            ui->_tabWidget->setCurrentIndex(index + 1);
        }
    }
}

void ImageProperties::on__tabWidget_currentChanged(int index)
{
    if( index == ui->_tabWidget->count() - 1 )
    {
        ui->_buttonSave->setText(tr("�ύ"));
        ui->_buttonNext->setVisible(false);
    }
    else
    {
        ui->_buttonSave->setText(tr("����"));
        ui->_buttonNext->setVisible(true);
    }
    ui->_buttonSave->setEnabled(false);
}

void ImageProperties::on__buttonCancel_clicked()
{
    close();
}

void ImageProperties::closeEvent(QCloseEvent *event)
{
    if(!_bCommited)
    {
        QMessageBox::StandardButton reply = QMessageBox::warning(0, tr("��ʾ"), tr("�رս���������д�����ݶ�ʧ, �Ƿ�ȷ���˳�?"), QMessageBox::Ok | QMessageBox::Cancel);
        if(reply == QMessageBox::Ok)
        {
            emit removeImage(_originalImagePath);
        }
        else if(reply == QMessageBox::Cancel)
            event->ignore();
    }
}

void ImageProperties::on__comboBoxEquipPlaneID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxEquipPlaneType_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxEquipUnitID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editEquipHours_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxEquipRuntime_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editServiceNumber_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__dateEditOilSampleSampleDate_dateChanged(const QDate &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
    ui->_comboBoxOilSampleID->setEditText(generateOilSampleID());
}

void ImageProperties::on__timeEditOilSampleSampleTime_timeChanged(const QTime &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
    ui->_comboBoxOilSampleID->setEditText(generateOilSampleID());
}

void ImageProperties::on__comboBoxOilSamplePlaneID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
    ui->_comboBoxOilSampleID->setEditText(generateOilSampleID());
}

void ImageProperties::on__comboBoxMovepartServiceMovepartID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
    //    if(!ui->_dateEditOilSampleSampleDate->text().isEmpty() && !ui->_comboBoxOilSamplePlaneID->currentText().isEmpty()
    //            && !ui->_timeEditOilSampleSampleTime->text().isEmpty() && !ui->_comboBoxMovepartName->currentText().isEmpty())
    //    {
    //        QString id = ui->_comboBoxOilSamplePlaneID->currentText() + ui->_dateEditOilSampleSampleDate->date().toString("yyyyMMdd")
    //                + ui->_timeEditOilSampleSampleTime->time().toString("hh") + ui->_comboBoxMovepartName->currentText()+"0";
    //        ui->_comboBoxOilSampleID->setEditText(id);
    //    }
}

void ImageProperties::on__comboBoxMentalOilSampleID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
    ui->_comboBoxMentalID->setEditText(generateTiepupianID());
}

QString ImageProperties::generateTiepupianID()
{
    if(!ui->_comboBoxMentalOilSampleID->currentText().isEmpty() && !ui->_comboBoxMentalInstrumentType->currentText().isEmpty())
    {
        return ui->_comboBoxMentalOilSampleID->currentText() + "TPP" + QString(QChar::fromAscii(QChar('A').toAscii() + ui->_comboBoxMentalInstrumentType->currentIndex()-1)) + "00";
    }
    else
        return QString();
}

QString ImageProperties::generateOilSampleID()
{
    if(!ui->_comboBoxMovepartName->currentText().isEmpty() && !ui->_dateEditOilSampleSampleDate->text().isEmpty()
            && !ui->_timeEditOilSampleSampleTime->text().isEmpty() && !ui->_comboBoxOilSamplePlaneID->currentText().isEmpty())
    {
        return ui->_comboBoxOilSamplePlaneID->currentText() + ui->_dateEditOilSampleSampleDate->date().toString("yyyyMMdd")
                + ui->_timeEditOilSampleSampleTime->time().toString("hh") + QString(QChar::fromAscii(QChar('A').toAscii() + ui->_comboBoxMovepartName->currentIndex()-1))+"0";
    }
    else
    {
        return QString();
    }
}

QString ImageProperties::generateTieputupianID()
{
    if(!ui->_comboBoxMentalSampleID->currentText().isEmpty() &&
            !ui->_editMentalSampleEnlarger->text().isEmpty() &&
            !ui->_comboBoxMentalSampleLightType->currentText().isEmpty() &&
            !ui->_comboBoxMentalSampleArea->currentText().isEmpty())
    {
        return ui->_comboBoxMentalSampleID->currentText() + ui->_editMentalSampleEnlarger->text() + "x"
                + "L" + QString(QChar::fromAscii(QChar('A').toAscii()+ui->_comboBoxMentalSampleLightType->currentIndex()-1)) + "Q" + QString(QChar::fromAscii(QChar('A').toAscii()+ui->_comboBoxMentalSampleArea->currentIndex()-1))+"00";
    }
    else
    {
        return QString();
    }
}

void ImageProperties::on__comboBoxMovepartID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMovepartName_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);

    ui->_comboBoxOilSampleID->setEditText(generateOilSampleID());
}

void ImageProperties::on__comboBoxMovepartType_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMovepartMohe_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMovepartPlaneID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMovepartPlaneType_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__dateEditMovepartBegin_dateChanged(const QDate &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__dateEditMovepartEnd_dateChanged(const QDate &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editMovepartHours_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMovepartServiceID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMovepartServiceMovepartType_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__dateEditMovepartServiceDate_dateChanged(const QDate &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editMovepartServiceNumber_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMovepartServiceUnit_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editMovepartServiceReson_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editMovepartServiceContent_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editMovepartServiceInfo_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilSamplePlaneType_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilSampleMonitorPart_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilSampleMonitorPartID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilSampleSamplePointID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editOilSampleHours_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editOilSampleMount_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editOilSampleReason_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilSampleSampleUnit_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilSampleSampleGuy_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilSampleID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilSampleUnitID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilSampleSampleMethod_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editOilSampleSampleMount_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editOilSampleInfo_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilSampleSendGuy_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__dateEditOilSampleSendDate_dateChanged(const QDate &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__timeEditOilSampleSendTime_timeChanged(const QTime &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilSampleSituation_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeOilSampleID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);

    if(!ui->_comboBoxOilAnalyzeOilSampleID->currentText().isEmpty())
    {
        QString str = ui->_comboBoxOilAnalyzeOilSampleID->currentText();
        ui->_comboBoxOilAnalyzePolluteReportID->setEditText(str + "WRDReport");
        ui->_comboBoxOilAnalyzeLightEquipID->setEditText(str + "GPReport");
        ui->_comboBoxOilAnalyzeMentalEquipID->setEditText(str + "TPReport");
        ui->_comboBoxOilAnalyzeLihuaEquipID->setEditText(str + "LHReport");
    }
}

void ImageProperties::on__comboBoxOilAnalyzeUnitName_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeSendUnit_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeSendGuy_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editOilAnalyzeReason_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__dateEditOilAnalyzeReceiveDate_dateChanged(const QDate &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeReceiveGuy_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzePollutionLevelMethod_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzePollutionLevelGuy_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__dateEditOilAnalyzePollutionDate_dateChanged(const QDate &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzePolluteLevelEquip_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzePolluteReportID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeLightMethod_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeLightGuy_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__dateEditOilAnalyzeLightDate_dateChanged(const QDate &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeLightEquip_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeLightEquipID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeMentalMethod_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeMentalGuy_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__dateEditOilAnalyzeMentalDate_dateChanged(const QDate &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeMentalEquip_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeMentalEquipID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);

    ui->_comboBoxMentalReportID->setEditText(arg1);
}

void ImageProperties::on__comboBoxOilAnalyzeLihuaMethod_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeLihuaGuy_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__dateEditOilAnalyzeLihuaDate_dateChanged(const QDate &date)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeLihuaEquip_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxOilAnalyzeLihuaEquipID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMentalInstrumentType_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);

    ui->_comboBoxMentalID->setEditText(generateTiepupianID());
}

void ImageProperties::on__comboBoxMentalID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMentalReportID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editMentalMount_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMentalMethod_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMentalGuy_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMentalSampleImageID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMentalSampleID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);

    ui->_comboBoxMentalSampleImageID->setEditText(generateTieputupianID());
}

void ImageProperties::on__comboBoxMentalSampleReportID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMentalSampleMicroType_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMentalSampleSamplerType_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMentalSampleLightType_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
    ui->_comboBoxMentalSampleImageID->setEditText(generateTieputupianID());
}

void ImageProperties::on__editMentalSampleEnlarger_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
    ui->_comboBoxMentalSampleImageID->setEditText(generateTieputupianID());
}

void ImageProperties::on__comboBoxMentalSampleArea_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
    ui->_comboBoxMentalSampleImageID->setEditText(generateTieputupianID());
}

void ImageProperties::on__comboBoxMentalSampleGuy_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__editMentalSampleAnalysis_textChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMoliID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMoliImageID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMoliPianID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

void ImageProperties::on__comboBoxMoliReportID_editTextChanged(const QString &arg1)
{
    if(!ui->_buttonSave->isEnabled()) ui->_buttonSave->setEnabled(true);
}

QString ImageProperties::copyOrgImage(QString name, QString org)
{
    if(!QDir(Global::PathImage).exists())
    {
        QDir().mkdir(Global::PathImage);
    }

    QString copyTo = Global::PathImage + name + ".jpg";
    return QFile::copy(org, copyTo) == true ? copyTo : QString();
}

