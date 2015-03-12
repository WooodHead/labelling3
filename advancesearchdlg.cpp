#include "advancesearchdlg.h"
#include "ui_advancesearchdlg.h"

AdvanceSearchDlg::AdvanceSearchDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvanceSearchDlg)
{
    ui->setupUi(this);
    
    if(!createConnection(db))
    {
        QMessageBox::warning(this,tr("数据库提示"),tr("不能链接数据库"),QMessageBox::Close);
        return;
    }
    
    _awesome = new QtAwesome(this);
    _awesome->initFontAwesome();

    ui->queryBtn->setIcon(_awesome->icon(search));
    ui->exportBtn->setIcon(QIcon(":/new/prefix1/icons/export.png"));
    ui->importBtn->setIcon(QIcon(":/new/prefix1/icons/import.png"));
    ui->addtoBtn->setIcon(_awesome->icon(plus));
    ui->modifyButton->setIcon(_awesome->icon(pluscircle));

    propertymodel = 0;
    
    createListWidget();
    
    connect(ui->tableListWidget,SIGNAL(currentRowChanged(int)),ui->conditionStackedWidget,SLOT(setCurrentIndex(int)));
    
    usepropertyAction = new QAction(tr("使用属性"),ui->propertylistTableView);
    ui->propertylistTableView->addAction(usepropertyAction);
    connect(usepropertyAction,SIGNAL(triggered()),this,SLOT(useproperty()));
    
    managepropertyAction = new QAction(tr("更新属性"),ui->propertylistTableView);
    ui->propertylistTableView->addAction(managepropertyAction);;
    connect(managepropertyAction,SIGNAL(triggered()),this,SLOT(manageproperty()));
    
    ui->propertylistTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    
    createTableNames();
    
    createTableView();
    
    initCbBox();
    
    initpropertylistName();
    
    query();
    
    ui->modifyButton->setEnabled(false);
    ui->conditionStackedWidget->setCurrentIndex(0);
    
}

AdvanceSearchDlg::~AdvanceSearchDlg()
{
    delete ui;
    db.close();
}


void AdvanceSearchDlg::resetConditions()
{
    // clear all conditions
    _eqmCdtMap.clear();;
    _mpCdtMap.clear();
    _mprCdtMap.clear();
    _fegCdtMap.clear();
    _fegpCdtMap.clear();
    _oisCdtMap.clear();
    _oiaCdtMap.clear();
    _abmCdtMap.clear();
    
    // eqm
    ui->PlaneIdChkBox->setChecked(false);
    ui->planeTypeChkBox->setChecked(false);
    ui->runHourChkBox->setChecked(false);
    ui->runStageChkBox->setChecked(false);
    ui->repairTimeChkBox->setChecked(false);
    ui->departIdChkBox->setChecked(false);
    
    // mp
    ui->movepartIdChkBox->setChecked(false);
    ui->movepartNameChkBox->setChecked(false);
    ui->movepartTypeChkBox->setChecked(false);
    ui->partrunHourChkBox->setChecked(false);
    ui->movepartStartDataChkBox->setChecked(false);
    ui->movepartEndDataChkBox->setChecked(false);
    
    // mpr
    ui->movepartRepairIdChkBox->setChecked(false);
    ui->repairTimeChkBox->setChecked(false);
    ui->repairDateChkBox->setChecked(false);
    ui->repairDepartChkBox->setChecked(false);
    
    // feg
    ui->feg_ferrographyanalyzertypeChkBox->setChecked(false);
    ui->feg_ferrographymakemethodChkBox->setChecked(false);
    ui->feg_ferrographymakeoilconsumptionChkBox->setChecked(false);
    ui->feg_ferrographymakestuffChkBox->setChecked(false);
    ui->feg_ferrographysheetidChkBox->setChecked(false);
    
    
    // fegp
    ui->fegp_ferrographypicidChkBox->setChecked(false);
    ui->fegp_ferrographyreportidChkBox->setChecked(false);
    ui->fegp_imageacquisitionareaChkBox->setChecked(false);
    ui->fegp_imageacquisitionstuffChkBox->setChecked(false);
    ui->fegp_imageacquisitiontypeChkBox->setChecked(false);
    ui->fegp_imagerecognitioninfoanalysisChkBox->setChecked(false);
    ui->fegp_imagesymbolChkBox->setChecked(false);
    ui->fegp_lightsourcetypeChkBox->setChecked(false);
    ui->fegp_magnificationChkBox->setChecked(false);
    ui->fegp_microscopictypeChkBox->setChecked(false);
    
    // ois
    ui->oilsampleidChkBox->setChecked(false);
    ui->samplestuffChkBox->setChecked(false);
    ui->samplevolumeChkBox->setChecked(false);
    ui->sendstuffChkBox->setChecked(false);
    ui->sampledateChkBox->setChecked(false);
    ui->sampletimeChkBox->setChecked(false);
    ui->senddateChkBox->setChecked(false);
    ui->sendtimeChkBox->setChecked(false);
    ui->samplesituationChkBox->setChecked(false);
    ui->samplemethodChkBox->setChecked(false);
    ui->sampleidChkBox->setChecked(false);
    ui->sampledepartidChkBox->setChecked(false);
    ui->sampledepartnameChkBox->setChecked(false);
    ui->monitorpartidChkBox->setChecked(false);
    ui->monitorpartnameChkBox->setChecked(false);
    ui->oilworktimeChkBox->setChecked(false);
    ui->oiladditionChkBox->setChecked(false);
    
    // oia
    ui->oia_sendstuffChkBox->setChecked(false);
    ui->oia_senddepartChkBox->setChecked(false);
    ui->oia_analyzedepartnameChkBox->setChecked(false);
    ui->oia_receivedateChkBox->setChecked(false);
    ui->oia_receivestuffChkBox->setChecked(false);
    
    ui->oia_contaminationanalyzedateChkBox->setChecked(false);
    ui->oia_contaminationanalyzeequipmentChkBox->setChecked(false);
    ui->oia_contaminationanalyzemethodChkBox->setChecked(false);
    ui->oia_contaminationanalyzereportidChkBox->setChecked(false);
    ui->oia_contaminationanalyzereportidChkBox->setChecked(false);
    ui->oia_contaminationanalyzestuffChkBox->setChecked(false);
    
    ui->oia_spectroscopydateChkBox->setChecked(false);
    ui->oia_spectroscopyequipmentChkBox->setChecked(false);
    ui->oia_spectroscopymethodChkBox->setChecked(false);
    ui->oia_spectroscopyreportidChkBox->setChecked(false);
    ui->oia_spectroscopystuffChkBox->setChecked(false);
    
    ui->oia_ferrographydateChkBox->setChecked(false);
    ui->oia_ferrographyequipmentChkBox->setChecked(false);
    ui->oia_ferrographymethodChkBox->setChecked(false);
    ui->oia_ferrographyreportidChkBox->setChecked(false);
    ui->oia_ferrographystuffChkBox->setChecked(false);
    
    ui->oia_physicochemicaldateChkBox->setChecked(false);
    ui->oia_physicochemicalequipmentChkBox->setChecked(false);
    ui->oia_physicochemicalmethodChkBox->setChecked(false);
    ui->oia_physicochemicalreportidChkBox->setChecked(false);
    ui->oia_physicochemicalstuffChkBox->setChecked(false);
    
    // abm
    ui->abm_abrasiveidChkBox->setChecked(false);
    ui->abm_abrasivecolorChkBox->setChecked(false);
    ui->abm_abrasivepositionChkBox->setChecked(false);
    ui->abm_abrasiveshapeChkBox->setChecked(false);
    ui->abm_abrasivesperimeterChkBox->setChecked(false);
    ui->abm_abrasivesizeChkBox->setChecked(false);
    ui->abm_abrasivematerialChkBox->setChecked(false);
    ui->abm_abrasivemarkstuffChkBox->setChecked(false);
    ui->abm_abrasivetypicalChkBox->setChecked(false);
    ui->abm_abrasivemechanismtypeChkBox_->setChecked(false);
    ui->abm_abrasivedamagetypeChkBox->setChecked(false);
    ui->abm_abrasiveweartypeChkBox->setChecked(false);
    ui->abm_abrasivesurfacetexturetypeChkBox->setChecked(false);
    ui->abm_ferrographypicidChkBox->setChecked(false);
    ui->abm_ferrographyreportidChkBox->setChecked(false);
    ui->abm_ferrographysheetidChkBox->setChecked(false);
}


void AdvanceSearchDlg::useproperty()
{
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("QMessageBox::question()"), tr("确认使用当前属性查询?"),
                                  QMessageBox::Yes | QMessageBox::Cancel);
    
    if(reply == QMessageBox::Yes)
    {
        QModelIndex index = ui->propertylistTableView->currentIndex();
        QSqlRecord record = propertymodel->record(index.row());
        QString propertyname = record.value(0).toString();
        
        QString sql = "select * from propertyinfo where propertyname = '";
        sql.append(propertyname);
        sql.append("'");
        QSqlQuery query;
        query.exec(sql);
        query.next();
        
        this->resetConditions();
        
        int idx;
        
        QStringList eqmFields = query.value(2).toString().split("#");
        QStringList eqmValues = query.value(3).toString().split("#");
        if(!eqmFields.isEmpty())
        {
            idx = 0;
            foreach (QString field, eqmFields) {
                if(field == "planeid")
                {
                    ui->planeidCbBox->setCurrentIndex(ui->planeidCbBox->findText(eqmValues.at(idx)));
                    _eqmCdtMap.insert("planeid",eqmValues.at(idx));
                    ui->PlaneIdChkBox->setChecked(true);
                }
                else if(field == "planetype")
                {
                    ui->planeTypeCbBox->setCurrentIndex(ui->planeTypeCbBox->findText(eqmValues.at(idx)));
                    _eqmCdtMap.insert("planetype",eqmValues.at(idx));
                    ui->planeTypeChkBox->setChecked(true);
                }
                else if(field == "departid")
                {
                    ui->departIdCbBox->setCurrentIndex(ui->departIdCbBox->findText(eqmValues.at(idx)));
                    _eqmCdtMap.insert("departid",eqmValues.at(idx));
                    ui->departIdChkBox->setChecked(true);
                }
                else if(field == "runhour")
                {
                    ui->runHourCbBox->setCurrentIndex(ui->runHourCbBox->findText(eqmValues.at(idx)));
                    _eqmCdtMap.insert("runhour",eqmValues.at(idx));
                    ui->runHourChkBox->setChecked(true);
                }
                else if(field == "runstage")
                {
                    ui->runStageCbBox->setCurrentIndex(ui->runStageCbBox->findText(eqmValues.at(idx)));
                    _eqmCdtMap.insert("runstage",eqmValues.at(idx));
                    ui->runStageChkBox->setChecked(true);
                }
                else if(field == "repairtime")
                {
                    ui->repairTimeCbBox->setCurrentIndex(ui->repairTimeCbBox->findText(eqmValues.at(idx)));
                    _eqmCdtMap.insert("repairtime",eqmValues.at(idx));
                    ui->repairTimeChkBox->setChecked(true);
                }
                idx ++;
            }
        }
        
        QStringList mpFields = query.value(4).toString().split("#");
        QStringList mpValues = query.value(5).toString().split("#");
        if(!mpFields.isEmpty())
        {
            idx = 0;
            foreach (QString field, mpFields)
            {
                if(field == "movepartname")
                {
                    ui->movepartNameCbBox->setCurrentIndex(ui->movepartNameCbBox->findText(mpValues.at(idx)));
                    _mpCdtMap.insert("movepartname",mpValues.at(idx));
                    ui->movepartNameChkBox->setChecked(true);
                    
                }
                else if(field == "movepartid")
                {
                    ui->movepartIdCbBox->setCurrentIndex(ui->movepartIdCbBox->findText(mpValues.at(idx)));
                    _mpCdtMap.insert("movepartid",mpValues.at(idx));
                    ui->movepartIdChkBox->setChecked(true);
                }
                else if(field == "moveparttype")
                {
                    ui->movepartTypeCbBox->setCurrentIndex(ui->movepartTypeCbBox->findText(mpValues.at(idx)));
                    _mpCdtMap.insert("moveparttype",mpValues.at(idx));
                    ui->movepartTypeChkBox->setChecked(true);
                }
                else if(field == "runhour")
                {
                    ui->partrunHourCbBox->setCurrentIndex(ui->partrunHourCbBox->findText(mpValues.at(idx)));
                    _mpCdtMap.insert("runhour",mpValues.at(idx));
                    ui->partrunHourChkBox->setChecked(true);
                }
                else if(field == "startdate")
                {
                    QString datestr = mpValues.at(idx);
                    QStringList datelist = datestr.split("-");
                    int y = datelist.at(0).toInt();
                    int m = datelist.at(1).toInt();
                    int d = datelist.at(2).toInt();
                    ui->movepartStartDateEdit->setDate(QDate(y,m,d));
                    _mpCdtMap.insert("startdate",mpValues.at(idx));
                    ui->movepartStartDataChkBox->setChecked(true);
                }
                else if(field == "enddate")
                {
                    QString datestr = mpValues.at(idx);
                    QStringList datelist = datestr.split("-");
                    int y = datelist.at(0).toInt();
                    int m = datelist.at(1).toInt();
                    int d = datelist.at(2).toInt();
                    ui->movepartEndDateEdit->setDate(QDate(y,m,d));
                    _mpCdtMap.insert("enddate",mpValues.at(idx));
                    ui->movepartEndDataChkBox->setChecked(true);
                }
                idx ++;
            }
            
        }
        
        QStringList mprFields = query.value(6).toString().split("#");
        QStringList mprValues = query.value(7).toString().split("#");
        if(!mprFields.isEmpty())
        {
            idx = 0;
            foreach (QString field, mprFields) {
                if(field == "movepartrepairid")
                {
                    ui->movepartRepairIdCbBox->setCurrentIndex(ui->movepartRepairIdCbBox->findText(mprValues.at(idx)));
                    _mprCdtMap.insert("movepartrepairid",mprValues.at(idx));
                    ui->movepartRepairIdChkBox->setChecked(true);
                }
                else if(field == "repairtime")
                {
                    ui->repairrepairTimeCbBox->setCurrentIndex(ui->repairrepairTimeCbBox->findText(mprValues.at(idx)));
                    _mprCdtMap.insert("repairtime",mprValues.at(idx));
                    ui->repairrepairTimeChkBox->setChecked(true);
                }
                else if(field == "repairdate")
                {
                    QString datestr = mprValues.at(idx);
                    QStringList datelist = datestr.split("-");
                    int y = datelist.at(0).toInt();
                    int m = datelist.at(1).toInt();
                    int d = datelist.at(2).toInt();
                    ui->repairDateDateEdit->setDate(QDate(y,m,d));
                    _mprCdtMap.insert("repairdate",mprValues.at(idx));
                    ui->repairDateChkBox->setChecked(true);
                }
                else if(field == "repairdepart")
                {
                    ui->repairDepartCbBox->setCurrentIndex(ui->repairDepartCbBox->findText(mprValues.at(idx)));
                    _mprCdtMap.insert("repairdepart",mprValues.at(idx));
                    ui->repairDepartChkBox->setChecked(true);
                }
                idx ++;
            }
        }
        
        QStringList fegFields = query.value(8).toString().split("#");
        QStringList fegValues = query.value(9).toString().split("#");
        if(!fegFields.isEmpty())
        {
            idx = 0;
            foreach (QString field, fegFields)
            {
                if(field == "ferrographysheetid")
                {
                    ui->feg_ferrographysheetidCbBox->setCurrentIndex(ui->feg_ferrographysheetidCbBox->findText(fegValues.at(idx)));
                    _fegCdtMap.insert("ferrographysheetid",fegValues.at(idx));
                    ui->feg_ferrographysheetidChkBox->setChecked(true);
                }
                else if(field == "ferrographymakestuff")
                {
                    ui->feg_ferrographymakestuffLineEdit->setText(fegValues.at(idx));
                    _fegCdtMap.insert("ferrographymakestuff",fegValues.at(idx));
                    ui->feg_ferrographymakestuffChkBox->setChecked(true);
                }
                else if(field == "ferrographymakemethod")
                {
                    ui->feg_ferrographymakemethodCbBox->setCurrentIndex(ui->feg_ferrographymakemethodCbBox->findText(fegValues.at(idx)));
                    _fegCdtMap.insert("ferrographymakemethod",fegValues.at(idx));
                    ui->feg_ferrographymakemethodChkBox->setChecked(true);
                }
                else if(field == "ferrographyanalyzertype")
                {
                    ui->feg_ferrographyanalyzertypeCbBox->setCurrentIndex(ui->feg_ferrographyanalyzertypeCbBox->findText(fegValues.at(idx)));
                    _fegCdtMap.insert("ferrographyanalyzertype",fegValues.at(idx));
                    ui->feg_ferrographyanalyzertypeChkBox->setChecked(true);
                }
                else if(field == "ferrographymakeoilconsumption")
                {
                    ui->feg_ferrographymakeoilconsumptionLineEdit->setText(fegValues.at(idx));
                    _fegCdtMap.insert("ferrographymakeoilconsumption",fegValues.at(idx));
                    ui->feg_ferrographymakeoilconsumptionChkBox->setChecked(true);
                }
                idx ++;
            }
        }
        
        QStringList fegpFields = query.value(10).toString().split("#");
        QStringList fegpValues = query.value(11).toString().split("#");
        if(!fegpFields.isEmpty())
        {
            idx = 0;
            foreach (QString field, fegpFields)
            {
                if(field == "ferrographypicid")
                {
                    ui->fegp_ferrographypicidCbBox->setCurrentIndex(ui->fegp_ferrographypicidCbBox->findText(fegpValues.at(idx)));
                    _fegpCdtMap.insert("ferrographypicid",fegpValues.at(idx));
                    ui->fegp_ferrographypicidChkBox->setChecked(true);
                }
                else if(field == "ferrographyreportid")
                {
                    ui->fegp_ferrographyreportidCbBox->setCurrentIndex(ui->fegp_ferrographyreportidCbBox->findText(fegpValues.at(idx)));
                    _fegpCdtMap.insert("ferrographyreportid",fegpValues.at(idx));
                    ui->fegp_ferrographyreportidChkBox->setChecked(true);
                }
                else if(field == "imagesymbol")
                {
                    ui->fegp_imagesymbolCbBox->setCurrentIndex(ui->fegp_imagesymbolCbBox->findText(fegpValues.at(idx)));
                    _fegpCdtMap.insert("imagesymbol",fegpValues.at(idx));
                    ui->fegp_imagesymbolChkBox->setChecked(true);
                }
                else if(field == "imageacquisitionarea")
                {
                    ui->fegp_imageacquisitionareaCbBox->setCurrentIndex(ui->fegp_imageacquisitionareaCbBox->findText(fegpValues.at(idx)));
                    _fegpCdtMap.insert("imageacquisitionarea",fegpValues.at(idx));
                    ui->fegp_imageacquisitionareaChkBox->setChecked(true);
                }
                else if(field == "microscopictype")
                {
                    ui->fegp_microscopictypeCbBox->setCurrentIndex(ui->fegp_microscopictypeCbBox->findText(fegpValues.at(idx)));
                    _fegpCdtMap.insert("microscopictype",fegpValues.at(idx));
                    ui->fegp_microscopictypeChkBox->setChecked(true);
                }
                else if(field == "lightsourcetype")
                {
                    ui->fegp_lightsourcetypeCbBox->setCurrentIndex(ui->fegp_lightsourcetypeCbBox->findText(fegpValues.at(idx)));
                    _fegpCdtMap.insert("lightsourcetype",fegpValues.at(idx));
                    ui->fegp_lightsourcetypeChkBox->setChecked(true);
                }
                else if(field == "magnification")
                {
                    ui->fegp_magnificationCbBox->setCurrentIndex(ui->fegp_magnificationCbBox->findText(fegpValues.at(idx)));
                    _fegpCdtMap.insert("magnification",fegpValues.at(idx));
                    ui->fegp_magnificationChkBox->setChecked(true);
                }
                else if(field == "imageacquisitiontype")
                {
                    ui->fegp_imageacquisitiontypeCbBox->setCurrentIndex(ui->fegp_imageacquisitiontypeCbBox->findText(fegpValues.at(idx)));
                    _fegpCdtMap.insert("imageacquisitiontype",fegpValues.at(idx));
                    ui->fegp_imageacquisitiontypeChkBox->setChecked(true);
                }
                else if(field == "imageacquisitionstuff")
                {
                    ui->fegp_imageacquisitionstuffLineEdit->setText(fegpValues.at(idx));
                    _fegpCdtMap.insert("imageacquisitionstuff",fegpValues.at(idx));
                    ui->fegp_imageacquisitionstuffChkBox->setChecked(true);
                }
                else if(field == "imagerecognitioninfoanalysis")
                {
                    ui->fegp_imagerecognitioninfoanalysisLineEdit->setText(fegpValues.at(idx));
                    _fegpCdtMap.insert("imagerecognitioninfoanalysis",fegpValues.at(idx));
                    ui->fegp_imagerecognitioninfoanalysisChkBox->setChecked(true);
                }
                idx++;
            }
        }
        
        QStringList oisFields = query.value(12).toString().split("#");
        QStringList oisValues = query.value(13).toString().split("#");
        if(!oisFields.isEmpty())
        {
            idx = 0;
            foreach (QString field, oisFields)
            {
                if(field == "oilsampleid")
                {
                    ui->oilsampleidCbBox->setCurrentIndex(ui->oilsampleidCbBox->findText(oisValues.at(idx)));
                    _oisCdtMap.insert("oilsampleid",oisValues.at(idx));
                    ui->oilsampleidChkBox->setChecked(true);
                }
                else if(field == "samplestuff")
                {
                    ui->samplestuffLineEdit->setText(oisValues.at(idx));
                    _oisCdtMap.insert("samplestuff",oisValues.at(idx));
                    ui->samplestuffChkBox->setChecked(true);
                }
                else if(field == "sendstuff")
                {
                    ui->sendstuffLineEdit->setText(oisValues.at(idx));
                    _oisCdtMap.insert("sendstuff",oisValues.at(idx));
                    ui->sendstuffChkBox->setChecked(true);
                }
                else if(field == "sampledate")
                {
                    QString datestr = oisValues.at(idx);
                    QStringList datelist = datestr.split("-");
                    int y = datelist.at(0).toInt();
                    int m = datelist.at(1).toInt();
                    int d = datelist.at(2).toInt();
                    ui->sampledateDateEdit->setDate(QDate(y,m,d));
                    _oisCdtMap.insert("sampledate",oisValues.at(idx));
                    ui->sampledateChkBox->setChecked(true);
                }
                else if(field == "senddate")
                {
                    QString datestr = oisValues.at(idx);
                    QStringList datelist = datestr.split("-");
                    int y = datelist.at(0).toInt();
                    int m = datelist.at(1).toInt();
                    int d = datelist.at(2).toInt();
                    ui->senddateDateEdit->setDate(QDate(y,m,d));
                    _oisCdtMap.insert("senddate",oisValues.at(idx));
                    ui->senddateChkBox->setChecked(true);
                }
                else if(field == "sampletime")
                {
                    QString timestr = oisValues.at(idx);
                    QStringList timelist = timestr.split(":");
                    int h = timelist.at(0).toInt();
                    int m = timelist.at(1).toInt();
                    QStringList seclist = timelist.at(2).split(" ");
                    int s = seclist.at(0).toInt();
                    ui->sampletimeTimeEdit->setTime(QTime(h,m));
                    _oisCdtMap.insert("sampletime",oisValues.at(idx));
                    ui->sampletimeChkBox->setChecked(true);
                }
                else if(field == "sendtime")
                {
                    QString timestr = oisValues.at(idx);
                    QStringList timelist = timestr.split(":");
                    int h = timelist.at(0).toInt();
                    int m = timelist.at(1).toInt();
                    QStringList seclist = timelist.at(2).split(" ");
                    int s = seclist.at(0).toInt();
                    ui->sendtimeTimeEdit->setTime(QTime(h,m));;
                    _oisCdtMap.insert("sendtime",oisValues.at(idx));
                    ui->sendtimeChkBox->setChecked(true);
                }
                else if(field == "samplesituation")
                {
                    ui->samplesituationCbBox->setCurrentIndex(ui->samplesituationCbBox->findText(oisValues.at(idx)));
                    _oisCdtMap.insert("samplesituation",oisValues.at(idx));
                    ui->samplesituationChkBox->setChecked(true);
                }
                else if(field == "samplemethod")
                {
                    ui->samplemethodCbBox->setCurrentIndex(ui->samplemethodCbBox->findText(oisValues.at(idx)));
                    _oisCdtMap.insert("samplemethod",oisValues.at(idx));
                    ui->samplemethodChkBox->setChecked(true);
                }
                else if(field == "sampleid")
                {
                    ui->sampleidCbBox->setCurrentIndex(ui->sampleidCbBox->findText(oisValues.at(idx)));
                    _oisCdtMap.insert("sampleid",oisValues.at(idx));
                    ui->sampleidChkBox->setChecked(true);
                }
                else if(field == "sampledepartid")
                {
                    ui->sampledepartidCbBox->setCurrentIndex(ui->sampledepartidCbBox->findText(oisValues.at(idx)));
                    _oisCdtMap.insert("sampledepartid",oisValues.at(idx));
                    ui->sampledepartidChkBox->setChecked(true);
                }
                else if(field == "sampledepartname")
                {
                    ui->sampledepartnameCbBox->setCurrentIndex(ui->sampledepartnameCbBox->findText(oisValues.at(idx)));
                    _oisCdtMap.insert("sampledepartname",oisValues.at(idx));
                    ui->sampledepartnameChkBox->setChecked(true);
                }
                else if(field == "monitorpartid")
                {
                    ui->monitorpartidCbBox->setCurrentIndex(ui->monitorpartidCbBox->findText(oisValues.at(idx)));
                    _oisCdtMap.insert("monitorpartid",oisValues.at(idx));
                    ui->monitorpartidChkBox->setChecked(true);
                }
                else if(field == "monitorpartname")
                {
                    ui->monitorpartnameCbBox->setCurrentIndex(ui->monitorpartnameCbBox->findText(oisValues.at(idx)));
                    _oisCdtMap.insert("monitorpartname",oisValues.at(idx));
                    ui->monitorpartnameChkBox->setChecked(true);
                }
                else if(field == "oilworktime")
                {
                    ui->oilworktimeLineEdit->setText(oisValues.at(idx));
                    _oisCdtMap.insert("oilworktime",oisValues.at(idx));
                    ui->oilworktimeChkBox->setChecked(true);
                }
                else if(field == "oiladdition")
                {
                    ui->oiladditionLineEdit->setText(oisValues.at(idx));
                    _oisCdtMap.insert("oiladdition",oisValues.at(idx));
                    ui->oiladditionChkBox->setChecked(true);
                }
                idx ++;
            }
        }
        
        QStringList oiaFields = query.value(14).toString().split("#");
        QStringList oiaValues = query.value(15).toString().split("#");
        if(!oiaFields.isEmpty())
        {
            idx = 0;
            foreach (QString field, oiaFields)
            {
                if(field == "sendstuff")
                {
                    ui->oia_sendstuffLineEdit->setText(oiaValues.at(idx));
                    _oiaCdtMap.insert("sendstuff",oiaValues.at(idx));
                    ui->oia_sendstuffChkBox->setChecked(true);
                }
                else if(field == "senddepart")
                {
                    ui->oia_senddepartCbBox->setCurrentIndex(ui->oia_senddepartCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("senddepart",oiaValues.at(idx));
                    ui->oia_senddepartChkBox->setChecked(true);
                }
                else if(field == "receivestuff")
                {
                    ui->oia_receivestuffLineEdit->setText(oiaValues.at(idx));
                    _oiaCdtMap.insert("receivestuff",oiaValues.at(idx));
                    ui->oia_receivestuffChkBox->setChecked(true);
                }
                else if(field == "receivedate")
                {
                    QString datestr = oiaValues.at(idx);
                    QStringList datelist = datestr.split("-");
                    int y = datelist.at(0).toInt();
                    int m = datelist.at(1).toInt();
                    int d = datelist.at(2).toInt();
                    ui->oia_receivedateDateEdit->setDate(QDate(y,m,d));
                    _oiaCdtMap.insert("receivedate",oiaValues.at(idx));
                    ui->oia_receivedateChkBox->setChecked(true);
                }
                else if(field == "analyzedepartname")
                {
                    ui->oia_analyzedepartnameCbBox->setCurrentIndex(ui->oia_analyzedepartnameCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("analyzedepartname",oiaValues.at(idx));
                    ui->oia_analyzedepartnameChkBox->setChecked(true);
                }
                else if(field == "contaminationanalyzestuff")
                {
                    ui->oia_contaminationanalyzestuffChkLineEdit->setText(oiaValues.at(idx));
                    _oiaCdtMap.insert("contaminationanalyzestuff",oiaValues.at(idx));
                    ui->oia_contaminationanalyzestuffChkBox->setChecked(true);
                }
                else if(field == "contaminationanalyzemethod")
                {
                    ui->oia_contaminationanalyzemethodCbBox->setCurrentIndex(ui->oia_contaminationanalyzemethodCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("contaminationanalyzemethod",oiaValues.at(idx));
                    ui->oia_contaminationanalyzestuffChkBox->setChecked(true);
                }
                else if(field == "contaminationanalyzeequipment")
                {
                    ui->oia_contaminationanalyzeequipmentCbBox->setCurrentIndex(ui->oia_contaminationanalyzeequipmentCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("contaminationanalyzeequipment",oiaValues.at(idx));
                    ui->oia_contaminationanalyzeequipmentChkBox->setChecked(true);
                }
                else if(field == "contaminationanalyzedate")
                {
                    QString datestr = oiaValues.at(idx);
                    QStringList datelist = datestr.split("-");
                    int y = datelist.at(0).toInt();
                    int m = datelist.at(1).toInt();
                    int d = datelist.at(2).toInt();
                    ui->oia_contaminationanalyzedateDateEdit->setDate(QDate(y,m,d));
                    _oiaCdtMap.insert("contaminationanalyzedate",oiaValues.at(idx));
                    ui->oia_contaminationanalyzedateChkBox->setChecked(true);
                }
                else if(field == "contaminationanalyzereportid")
                {
                    ui->oia_contaminationanalyzereportidCbBox->setCurrentIndex(ui->oia_contaminationanalyzereportidCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("contaminationanalyzereportid",oiaValues.at(idx));
                    ui->oia_contaminationanalyzereportidChkBox->setChecked(true);
                }
                else if(field == "spectroscopystuff")
                {
                    ui->oia_spectroscopystuffLineEdit->setText(oiaValues.at(idx));
                    _oiaCdtMap.insert("spectroscopystuff",oiaValues.at(idx));
                    ui->oia_spectroscopystuffChkBox->setChecked(true);
                }
                else if(field == "spectroscopymethod")
                {
                    ui->oia_spectroscopymethodCbBo->setCurrentIndex(ui->oia_spectroscopymethodCbBo->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("spectroscopymethod",oiaValues.at(idx));
                    ui->oia_spectroscopymethodChkBox->setChecked(true);
                }
                else if(field == "spectroscopyequipment")
                {
                    ui->oia_spectroscopyequipmentCbBox->setCurrentIndex(ui->oia_spectroscopyequipmentCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("spectroscopyequipment",oiaValues.at(idx));
                    ui->oia_spectroscopyequipmentChkBox->setChecked(true);
                }
                else if(field == "spectroscopydate")
                {
                    QString datestr = oiaValues.at(idx);
                    QStringList datelist = datestr.split("-");
                    int y = datelist.at(0).toInt();
                    int m = datelist.at(1).toInt();
                    int d = datelist.at(2).toInt();
                    ui->oia_spectroscopydateDateEdit->setDate(QDate(y,m,d));
                    _oiaCdtMap.insert("spectroscopydate",oiaValues.at(idx));
                    ui->oia_spectroscopydateChkBox->setChecked(true);
                }
                else if(field == "spectroscopyreportid")
                {
                    ui->oia_spectroscopyreportidCbBox->setCurrentIndex(ui->oia_spectroscopyreportidCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("spectroscopyreportid",oiaValues.at(idx));
                    ui->oia_spectroscopyreportidChkBox->setChecked(true);
                }
                else if(field == "ferrographystuff")
                {
                    ui->oia_ferrographystuffLineEdit->setText(oiaValues.at(idx));
                    _oiaCdtMap.insert("ferrographystuff",oiaValues.at(idx));
                    ui->oia_ferrographystuffChkBox->setChecked(true);
                }
                else if(field == "ferrographymethod")
                {
                    ui->oia_ferrographymethodCbBox->setCurrentIndex(ui->oia_ferrographymethodCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("ferrographymethod",oiaValues.at(idx));
                    ui->oia_ferrographymethodChkBox->setChecked(true);
                }
                else if(field == "ferrographyequipment")
                {
                    ui->oia_ferrographyequipmentCbBox->setCurrentIndex(ui->oia_ferrographyequipmentCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("ferrographyequipment",oiaValues.at(idx));
                    ui->oia_ferrographyequipmentChkBox->setChecked(true);
                }
                else if(field == "ferrographydate")
                {
                    QString datestr = oiaValues.at(idx);
                    QStringList datelist = datestr.split("-");
                    int y = datelist.at(0).toInt();
                    int m = datelist.at(1).toInt();
                    int d = datelist.at(2).toInt();
                    ui->oia_ferrographydateDateEdit->setDate(QDate(y,m,d));
                    _oiaCdtMap.insert("ferrographydate",oiaValues.at(idx));
                    ui->oia_ferrographydateChkBox->setChecked(true);
                }
                else if(field == "ferrographyreportid")
                {
                    ui->oia_ferrographyreportidCbBox->setCurrentIndex(ui->oia_ferrographyreportidCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("ferrographyreportid",oiaValues.at(idx));
                    ui->oia_ferrographyreportidChkBox->setChecked(true);
                }
                else if(field == "physicochemicalstuff")
                {
                    ui->oia_physicochemicalstuffLineEdit->setText(oiaValues.at(idx));
                    _oiaCdtMap.insert("physicochemicalstuff",oiaValues.at(idx));
                    ui->oia_physicochemicalstuffChkBox->setChecked(true);
                }
                else if(field == "physicochemicalmethod")
                {
                    ui->oia_physicochemicalmethodCbBox->setCurrentIndex(ui->oia_physicochemicalmethodCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("physicochemicalmethod",oiaValues.at(idx));
                    ui->oia_physicochemicalmethodChkBox->setChecked(true);
                }
                else if(field == "physicochemicalequipment")
                {
                    ui->oia_physicochemicalequipmentCbBox->setCurrentIndex(ui->oia_physicochemicalequipmentCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("physicochemicalequipment",oiaValues.at(idx));
                    ui->oia_physicochemicalequipmentChkBox->setChecked(true);
                }
                else if(field == "physicochemicaldate")
                {
                    QString datestr = oiaValues.at(idx);
                    QStringList datelist = datestr.split("-");
                    int y = datelist.at(0).toInt();
                    int m = datelist.at(1).toInt();
                    int d = datelist.at(2).toInt();
                    ui->oia_physicochemicaldateDateEdit->setDate(QDate(y,m,d));
                    _oiaCdtMap.insert("physicochemicaldate",oiaValues.at(idx));
                    ui->oia_physicochemicaldateChkBox->setChecked(true);
                }
                else if(field == "physicochemicalreportid")
                {
                    ui->oia_physicochemicalreportidCbBox->setCurrentIndex(ui->oia_physicochemicalreportidCbBox->findText(oiaValues.at(idx)));
                    _oiaCdtMap.insert("physicochemicalreportid",oiaValues.at(idx));
                    ui->oia_physicochemicalreportidChkBox->setChecked(true);
                }
                idx ++;
            }
        }
        
        QStringList abmFields = query.value(16).toString().split("#");
        QStringList abmValues = query.value(17).toString().split("#");
        if(!abmFields.isEmpty())
        {
            idx =0;
            foreach (QString field, abmFields)
            {
                if(field == "abrasiveid")
                {
                    ui->abm_abrasiveidCbBox->setCurrentIndex(ui->abm_abrasiveidCbBox->findText(abmValues.at(idx)));
                    _abmCdtMap.insert("abrasiveid",abmValues.at(idx));
                    ui->abm_abrasiveidChkBox->setChecked(true);
                }
                else if(field == "abrasiveshape")
                {
                    ui->abm_abrasiveshapeCbBox->setCurrentIndex(ui->abm_abrasiveshapeCbBox->findText(abmValues.at(idx)));
                    _abmCdtMap.insert("abrasiveshape",abmValues.at(idx));
                    ui->abm_abrasiveshapeChkBox->setChecked(true);
                }
                else if(field == "abrasiveposition")
                {
                    ui->abm_abrasivepositionCbBox->setCurrentIndex(ui->abm_abrasivepositionCbBox->findText(abmValues.at(idx)));
                    _abmCdtMap.insert("abrasiveposition",abmValues.at(idx));
                    ui->abm_abrasivepositionChkBox->setChecked(true);
                }
                else if(field == "abrasivematerial")
                {
                    ui->abm_abrasivematerialCbBox->setCurrentIndex(ui->abm_abrasivematerialCbBox->findText(abmValues.at(idx)));
                    _abmCdtMap.insert("abrasivematerial",abmValues.at(idx));
                    ui->abm_abrasivematerialChkBox->setChecked(true);
                }
                else if(field == "abrasivecolor")
                {
                    ui->abm_abrasivecolorCbBox->setCurrentIndex(ui->abm_abrasivecolorCbBox->findText(abmValues.at(idx)));
                    _abmCdtMap.insert("abrasivecolor",abmValues.at(idx));
                    ui->abm_abrasivecolorChkBox->setChecked(true);
                }
                else if(field == "abrasivesperimeter")
                {
                    ui->abm_abrasivesperimeterLineEdit->setText(abmValues.at(idx));
                    _abmCdtMap.insert("abrasivesperimeter",abmValues.at(idx));
                    ui->abm_abrasivesperimeterChkBox->setChecked(true);
                }
                else if(field == "abrasivesize")
                {
                    ui->abm_abrasivesizeLineEdit->setText(abmValues.at(idx));
                    _abmCdtMap.insert("abrasivesize",abmValues.at(idx));
                    ui->abm_abrasivesizeChkBox->setChecked(true);
                }
                else if(field == "abrasivemarkstuff")
                {
                    ui->abm_abrasivemarkstuffLineEdit->setText(abmValues.at(idx));
                    _abmCdtMap.insert("abrasivemarkstuff",abmValues.at(idx));
                    ui->abm_abrasivemarkstuffChkBox->setChecked(true);
                }
                else if(field == "abrasivetypical")
                {
                    ui->abm_abrasivetypicalLineEdit->setText(abmValues.at(idx));
                    _abmCdtMap.insert("abrasivetypical",abmValues.at(idx));
                    ui->abm_abrasivetypicalChkBox->setChecked(true);
                }
                else if(field == "abrasivemechanismtype")
                {
                    ui->abm_abrasivemechanismtypeLineEdit->setText(abmValues.at(idx));
                    _abmCdtMap.insert("abrasivemechanismtype",abmValues.at(idx));
                    ui->abm_abrasivemechanismtypeChkBox_->setChecked(true);
                }
                else if(field == "abrasivedamagetype")
                {
                    ui->abm_abrasivedamagetypeCbBox->setCurrentIndex(ui->abm_abrasivedamagetypeCbBox->findText(abmValues.at(idx)));
                    _abmCdtMap.insert("abrasivedamagetype",abmValues.at(idx));
                    ui->abm_abrasivedamagetypeChkBox->setChecked(true);
                }
                else if(field == "abrasiveweartype")
                {
                    ui->abm_abrasiveweartypeCbBox->setCurrentIndex(ui->abm_abrasiveweartypeCbBox->findText(abmValues.at(idx)));
                    _abmCdtMap.insert("abrasiveweartype",abmValues.at(idx));
                    ui->abm_abrasiveweartypeChkBox->setChecked(true);
                }
                else if(field == "abrasivesurfacetexturetype")
                {
                    ui->abm_abrasivesurfacetexturetypeCbBox->setCurrentIndex(ui->abm_abrasivesurfacetexturetypeCbBox->findText(abmValues.at(idx)));
                    _abmCdtMap.insert("abrasivesurfacetexturetype",abmValues.at(idx));
                    ui->abm_abrasivesurfacetexturetypeChkBox->setChecked(true);
                }
                else if(field == "ferrographysheetid")
                {
                    ui->abm_ferrographysheetidCbBox->setCurrentIndex(ui->abm_ferrographysheetidCbBox->findText(abmValues.at(idx)));
                    _abmCdtMap.insert("ferrographysheetid",abmValues.at(idx));
                    ui->abm_ferrographysheetidChkBox->setChecked(true);
                }
                else if(field == "ferrographypicid")
                {
                    ui->abm_ferrographypicidCbBox->setCurrentIndex(ui->abm_ferrographypicidCbBox->findText(abmValues.at(idx)));
                    _abmCdtMap.insert("ferrographypicid",abmValues.at(idx));
                    ui->abm_ferrographypicidChkBox->setChecked(true);
                }
                else if(field == "ferrographyreportid")
                {
                    ui->abm_ferrographyreportidCbBox->setCurrentIndex(ui->abm_ferrographypicidCbBox->findText(abmValues.at(idx)));
                    _abmCdtMap.insert("ferrographyreportid",abmValues.at(idx));
                    ui->abm_ferrographypicidChkBox->setChecked(true);
                }
                idx++;
            }
        }
        
        this->query();
    }
}


void AdvanceSearchDlg::manageproperty()
{
//    QMessageBox::warning(this,tr("提示"),tr("管理属性"),QMessageBox::Close);
    this->resetConditions();
    QModelIndex index = ui->propertylistTableView->currentIndex();
    QSqlRecord record = propertymodel->record(index.row());
    QString propertyname = record.value(0).toString();
    this->reloadConditions(propertyname);
    ui->queryBtn->setEnabled(false);
    ui->modifyButton->setEnabled(true);
}


bool AdvanceSearchDlg::importDB(const QString &path)
{
    QSqlQuery query(db);
    QFile file(path);
    file.open(QFile::ReadOnly);
    int count = 0;
    
    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString sql=in.readLine();
        // 通过分析values(E'),判断是否有二进制数据,如没有直接运行sql语句,如有则需要将16进制文本转换为blob数据
        QRegExp reg("E'([0-9a-f]{1,})'");
        
        if(!sql.contains(reg))
        {
            if(query.exec(sql))
                count ++;
        }else
        {
            int pos=0;
            QStringList bList;
            
            // 探索所有的blob字段
            while((pos=reg.indexIn(sql,pos))!=-1)
            {
                bList.append(reg.cap(0));
                
                QString blob=reg.cap(1);
                pos+=reg.matchedLength();
            }
            
            // blob字段填充占位符
            foreach(QString key,bList)
            {
                sql.replace(key,"?");
            }
            
            query.prepare(sql);
            
            // 绑定占位符数据
            for(int i=0;i<bList.size();i++)
            {
                // 去除E''
                QString hexBlob=bList[i].mid(2,bList[i].size()-1);
                // 还原16进制数据
                QByteArray ba=QByteArray::fromHex(hexBlob.toLocal8Bit());
                
                query.bindValue(i,ba);
            }
            query.exec();
        }
    }
    return true;
}


bool AdvanceSearchDlg::exportDB(const QSqlQueryModel *model, const QString &tablename, const QString &path)
{
    QStringList vList;
    int count = model->rowCount();
    for(int i =0;i<count;++i)
    {
        QSqlRecord record = model->record(i);
        QString prefix=QString("insert into %1(").arg(tablename); // 记录属性字段名
        QString suffix="values(";                                 // 记录属性值
        
        // 遍历属性字段
        for(int j=0;j<record.count();j++)
        {
            QSqlField field=record.field(j);
            QString fieldName=field.name();
            
            switch(field.type())
            {
            case QVariant::String:
                prefix+=fieldName;
                suffix+=QString("'%1'").arg(record.value(j).toString());
                break;
            case QVariant::ByteArray:
            {
                prefix+=fieldName;
                QByteArray data=record.value(j).toByteArray();
                if(data.isNull())
                {
                    suffix+="null";
                }else
                {
                    suffix+=QString("E'%1'").arg(data.toHex().data()); // blob数据按16进制格式导出
                }
            }
                break;
            default:
                prefix+=fieldName;
                suffix+=record.value(j).toString();
            }
            
            if(record.count()==1)
            {
                prefix+=")";
                suffix+=")";
            }else if(j!=record.count()-1)
            {
                prefix+=",";
                suffix+=",";
            }else if(j==record.count()-1)
            {
                prefix+=")";
                suffix+=")";
            }
        }
        // 组装sql语句 insert into auth_test values(0,'hello',E'003f')
        QString iSql=QString("%1 %2;").arg(prefix).arg(suffix);
        vList.append(iSql);
        //        qDebug()<<iSql;
    }
    QFile file(path);
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    
    // 将sql语句写入文件
    QTextStream out(&file);
    foreach(QString line,vList)
    {
        out<<line+"\n";
    }
    return true;
}


bool AdvanceSearchDlg::copyFiles(QString fromDir, QString toDir, QStringList filenames, bool convertIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    
    if(!targetDir.exists())
    {
        //< 如果目标目录不存在，则进行创建
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }
    if(fromDir == toDir)
        return true;
    foreach (QString filename, filenames) {
        QString fromFullName = fromDir + filename;
        QString toFullName   = toDir   + filename;
        bool fromexist = sourceDir.exists(fromFullName);
        if(!fromexist)
        {
            QString msg = "文件 " + filename + " 不存在，请检查文件与数据库一致"; 
            QMessageBox::warning(this,tr("提示"),msg,QMessageBox::Close);
            continue;
        }
        bool exist = targetDir.exists(toFullName);
        if(exist)
            if(convertIfExist)
            {
                targetDir.remove(toFullName);
            }
        if(!QFile::copy(fromFullName,toFullName))
            return false;
    }
    return true;
}


bool AdvanceSearchDlg::copyFiles(QString fromDir, QString toDir, bool convertIfExits)
{
    /**
     *@brief 将fromDir文件夹内的图片文件，拷贝到toDir文件夹下
     *@param fromDir 图片文件的源目录
     *@param toDir   拷贝图片文件的目标目录
     *@param convertIfExits 是否覆盖已存在文件标识，默认值是false
     */
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    
    if(!targetDir.exists())
    {
        //< 如果目标目录不存在，则进行创建
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }
    
    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    // 遍历所有文件信息
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        // 去除当前目录和父目录
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        // 数据库文件处理
        if(fileInfo.fileName().split(".")[1] == "sql")
            continue;
        
        // 当为目录时，递归的进行copy
        if(fileInfo.isDir())
        {
            //            if(!copyFiles(fileInfo.filePath(),
            //                          targetDir.filePath(fileInfo.fileName()),
            //                          convertIfExits))
            //                return false;
            continue;
        }
        else
        {   //当允许覆盖操作时，将旧文件进行删除操作
            if(convertIfExits && targetDir.exists(fileInfo.fileName()))
            {
                targetDir.remove(fileInfo.fileName());
            }
            // 进行文件copy
            if(targetDir.exists(fileInfo.fileName()))
                continue;
            if(!QFile::copy(fileInfo.filePath(),targetDir.filePath(fileInfo.fileName())))
            {
                qDebug()<<"false";
                return false;
            }
        }
    }
    return true;
}


void AdvanceSearchDlg::on_queryBtn_clicked()
{
    query();
}

QString AdvanceSearchDlg::generateSql(QMap<QString, QString> conditionMap,QStringList conditionField, QString tableName)
{
    QString sql = "select * from ";
    sql.append(tableName);
    sql.append(" ");
    if(conditionMap.isEmpty())
    {
        return sql;
    }
    else
    {
        sql.append("where ");
        QString fieldName;
        QString fieldValue;
        QMap<QString,QString>::iterator it;
        it = conditionMap.begin();
        fieldName = it.key();
        fieldValue = it.value();
        sql.append(fieldName);
        sql.append("='");
        sql.append(fieldValue);
        sql.append("'");
        ++it;
        for(;it != conditionMap.end(); ++it)
        {
            fieldName = it.key();
            fieldValue = it.value();
            sql.append(" and ");
            sql.append(fieldName);
            sql.append("='");
            sql.append(fieldValue);
            sql.append("'");
        }
        
        return sql;
    }
}


void AdvanceSearchDlg::query()
{
    // 装备信息表
    QString eqmTableName = tableNames.value("EqmInfo");
    QString eqmSql = generateSql(_eqmCdtMap,_eqmCdtField,eqmTableName);
    _eqmInfoModel->setQuery(eqmSql);
    setModelHeaderData("EqmInfo");
    qDebug()<<eqmSql;
    
    // 动部件信息表
    QString mpTableName = tableNames.value("MpInfo");
    QString mpSql = generateSql(_mpCdtMap,_mpCdtField,mpTableName);
    _mpInfoModel->setQuery(mpSql);
    setModelHeaderData("MpInfo");
    qDebug()<<mpSql;
    
    // 动部件维修信息表
    QString mprTableName = tableNames.value("MprInfo");
    QString mprSql = generateSql(_mprCdtMap,_mprCdtField,mprTableName);
    _mprInfoModel->setQuery(mprSql);
    setModelHeaderData("MprInfo");
    qDebug()<<mprSql;
    
    // 油样采集信息表
    QString oisTableName = tableNames.value("OisInfo");
    QString oisSql = generateSql(_oisCdtMap,_oisCdtField,oisTableName);
    _oisInfoModel->setQuery(oisSql);
    setModelHeaderData("OisInfo");
    qDebug()<<oisSql;
    
    // 油样检测分析信息表
    QString oiaTableName = tableNames.value("OiaInfo");
    QString oiaSql = generateSql(_oiaCdtMap,_oiaCdtField,oiaTableName);
    _oiaInfoModel->setQuery(oiaSql);
    setModelHeaderData("OiaInfo");
    qDebug()<<oiaSql;
    
    // 铁谱质谱信息表
    QString fegTableName = tableNames.value("FegInfo");
    QString fegSql = generateSql(_fegCdtMap,_fegCdtField,fegTableName);
    _fegInfoModel->setQuery(fegSql);
    setModelHeaderData("FegInfo");
    qDebug()<<fegSql;
    
    // 铁谱图片采集信息表
    QString fegpTableName = tableNames.value("FegPInfo");
    QString fegpSql = generateSql(_fegpCdtMap,_fegpCdtField,fegpTableName);
    _fegpInfoModel->setQuery(fegpSql);
    setModelHeaderData("FegPInfo");
    qDebug()<<fegpSql;
    
    // 磨粒标注信息表
    QString abmTableName = tableNames.value("AbmInfo");
    QString abmSql = generateSql(_abmCdtMap,_abmCdtField,abmTableName);
    _abmInfoModel->setQuery(abmSql);
    setModelHeaderData("AbmInfo");
    qDebug()<<abmSql;
}


void AdvanceSearchDlg::createListWidget()
{
    ui->tableListWidget->insertItem(0,tr("装备信息表"));
    ui->tableListWidget->insertItem(1,tr("动部件信息表"));
    ui->tableListWidget->insertItem(2,tr("动部件维修信息表"));
    ui->tableListWidget->insertItem(3,tr("油样采集信息表"));
    ui->tableListWidget->insertItem(4,tr("油样检测分析表"));
    ui->tableListWidget->insertItem(5,tr("铁谱图片采集信息表"));
    ui->tableListWidget->insertItem(6,tr("铁谱制谱信息表"));
    ui->tableListWidget->insertItem(7,tr("磨粒标注信息表"));
    
    ui->tableListWidget->setCurrentRow(0);
    
    int wheight = this->height();
    int wwidth  = this->width();
    ui->conditionGroupBox->setMaximumHeight(wheight/3);
    ui->groupBox->setMaximumWidth(wwidth/5);
    ui->propertylistGroupBox->setMaximumWidth(wwidth/5);
}


void AdvanceSearchDlg::createTableNames()
{
    tableNames.insert("EqmInfo","equipmentinfo");       // 装备信息表
    tableNames.insert("AbmInfo","abrasivemarkinfo");    // 磨粒标注信息表
    tableNames.insert("FegInfo","ferrographyinfo");     // 铁谱质谱信息表
    tableNames.insert("FegPInfo","ferrographypicinfo"); // 铁谱图片信息表
    tableNames.insert("MpInfo","movepartinfo");         // 动部件信息表
    tableNames.insert("MprInfo","movepartrepairinfo");  //动部件维修信息表
    tableNames.insert("OiaInfo","oilanalyzeinfo");      // 油样分析信息表
    tableNames.insert("OisInfo","oilsampleinfo");       // 油样采样信息表
}


void AdvanceSearchDlg::createTableView()
{
    ui->queryResultTabWidget->setCurrentIndex(0);
    
    // 装备信息表
    _eqmInfoModel = new QSqlQueryModel;
    ui->eqmTableView->setModel(_eqmInfoModel);
    ui->eqmTableView->verticalHeader()->setVisible(false);
    ui->eqmTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->eqmTableView->setSelectionMode(QTableView::SingleSelection);
    ui->eqmTableView->setAlternatingRowColors(true);
    ui->eqmTableView->resizeColumnsToContents();
    
    // 动部件信息表
    _mpInfoModel = new QSqlQueryModel;
    ui->mpTableView->setModel(_mpInfoModel);
    ui->mpTableView->verticalHeader()->setVisible(false);
    ui->mpTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->mpTableView->setSelectionMode(QTableView::SingleSelection);
    ui->mpTableView->setAlternatingRowColors(true);
    ui->mpTableView->resizeColumnsToContents();
    
    // 动部件维修信息表
    _mprInfoModel = new QSqlQueryModel;
    ui->mprTableView->setModel(_mprInfoModel);
    ui->mprTableView->verticalHeader()->setVisible(false);;
    ui->mprTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->mprTableView->setSelectionMode(QTableView::SingleSelection);
    ui->mprTableView->setAlternatingRowColors(true);
    ui->mprTableView->resizeColumnsToContents();
    
    // 油样采集信息表
    _oisInfoModel = new QSqlQueryModel;
    ui->oisTableView->setModel(_oisInfoModel);
    ui->oisTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->oisTableView->setSelectionMode(QTableView::SingleSelection);
    ui->oisTableView->setAlternatingRowColors(true);
    ui->oisTableView->verticalHeader()->setVisible(false);
    ui->oisTableView->resizeColumnsToContents();
    
    // 油样检测分析信息表
    _oiaInfoModel = new QSqlQueryModel;
    ui->oiaTableView->setModel(_oiaInfoModel);
    ui->oiaTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->oiaTableView->setSelectionMode(QTableView::SingleSelection);
    ui->oiaTableView->verticalHeader()->setVisible(false);
    ui->oiaTableView->setAlternatingRowColors(true);
    ui->oiaTableView->resizeColumnsToContents();
    
    // 铁谱质谱信息表
    _fegInfoModel = new QSqlQueryModel;
    ui->fegTableView->setModel(_fegInfoModel);
    ui->fegTableView->setSelectionBehavior(QTableView::SelectRows);;
    ui->fegTableView->setSelectionMode(QTableView::SingleSelection);
    ui->fegTableView->verticalHeader()->setVisible(false);
    ui->fegTableView->setAlternatingRowColors(true);
    ui->fegTableView->resizeColumnsToContents();
    
    // 铁谱图片采集信息表
    _fegpInfoModel = new QSqlQueryModel;
    ui->fegpTableView->setModel(_fegpInfoModel);
    ui->fegpTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->fegpTableView->setSelectionMode(QTableView::SingleSelection);
    ui->fegpTableView->setAlternatingRowColors(true);
    ui->fegpTableView->verticalHeader()->setVisible(false);
    ui->fegpTableView->resizeColumnsToContents();
    
    // 磨粒标注信息表
    _abmInfoModel = new QSqlQueryModel;
    ui->abmTableView->setModel(_abmInfoModel);
    ui->abmTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->abmTableView->setSelectionMode(QTableView::SingleSelection);
    ui->abmTableView->verticalHeader()->setVisible(false);
    ui->abmTableView->setAlternatingRowColors(true);
    ui->abmTableView->resizeColumnsToContents();
}


void AdvanceSearchDlg::setModelHeaderData(QString tablename)
{
    if(tablename == "EqmInfo")
    {
        _eqmInfoModel->setHeaderData(eqm_planeid,   Qt::Horizontal,tr("机号"));
        _eqmInfoModel->setHeaderData(eqm_planetype, Qt::Horizontal,tr("机型"));
        _eqmInfoModel->setHeaderData(eqm_deoartid,  Qt::Horizontal,tr("单位编号"));
        _eqmInfoModel->setHeaderData(eqm_runhour,   Qt::Horizontal,tr("飞行小时数"));
        _eqmInfoModel->setHeaderData(eqm_runstage,  Qt::Horizontal,tr("运行阶段"));
        _eqmInfoModel->setHeaderData(eqm_repairtime,Qt::Horizontal,tr("维修次数"));
    }
    else if(tablename == "MpInfo")
    {
        _mpInfoModel->setHeaderData(mp_movepartid,  Qt::Horizontal,tr("动部件编号"));
        _mpInfoModel->setHeaderData(mp_moveparttype,Qt::Horizontal,tr("动部件类型"));
        _mpInfoModel->setHeaderData(mp_movepartname,Qt::Horizontal,tr("动部件名称"));
        _mpInfoModel->setHeaderData(mp_runhour,     Qt::Horizontal,tr("运行时数"));
        _mpInfoModel->setHeaderData(mp_runstage,    Qt::Horizontal,tr("运行阶段"));
        _mpInfoModel->setHeaderData(mp_planeid,     Qt::Horizontal,tr("机号"));
        _mpInfoModel->setHeaderData(mp_planetype,   Qt::Horizontal,tr("机型"));
        _mpInfoModel->setHeaderData(mp_startdat,    Qt::Horizontal,tr("动部件开始日期"));
        _mpInfoModel->setHeaderData(mp_enddate,     Qt::Horizontal,tr("动部件终止日期"));
    }
    else if(tablename == "MprInfo")
    {
        _mprInfoModel->setHeaderData(mpr_movepartrepairid,Qt::Horizontal,tr("动部件维修编号"));
        _mprInfoModel->setHeaderData(mpr_movepartid,      Qt::Horizontal,tr("动部件编号"));
        _mprInfoModel->setHeaderData(mpr_moveparttype,    Qt::Horizontal,tr("动部件类型"));
        _mprInfoModel->setHeaderData(mpr_repairreason,    Qt::Horizontal,tr("维修原因"));
        _mprInfoModel->setHeaderData(mpr_repairdate,      Qt::Horizontal,tr("维修日期"));
        _mprInfoModel->setHeaderData(mpr_repairtime,      Qt::Horizontal,tr("维修次数"));
        _mprInfoModel->setHeaderData(mpr_repairdepart,    Qt::Horizontal,tr("维修执行单位"));
        _mprInfoModel->setHeaderData(mpr_repaircontent,   Qt::Horizontal,tr("维修内容"));
        _mprInfoModel->setHeaderData(mpr_repaircondition, Qt::Horizontal,tr("维修换件情况"));
    }
    else if(tablename == "AbmInfo")
    {
        _abmInfoModel->setHeaderData(abm_abrasiveid,                        Qt::Horizontal,tr("磨粒编号"));
        _abmInfoModel->setHeaderData(abm_ferrographypicid,                  Qt::Horizontal,tr("铁谱图片编号"));
        _abmInfoModel->setHeaderData(abm_ferrographysheetid,                Qt::Horizontal,tr("铁谱片编号"));
        _abmInfoModel->setHeaderData(abm_ferrographyreportid,               Qt::Horizontal,tr("铁谱分析报告编号"));
        _abmInfoModel->setHeaderData(abm_abrasivemarkstuff,                 Qt::Horizontal,tr("磨粒标注人"));
        _abmInfoModel->setHeaderData(abm_abrasivepicpath,                   Qt::Horizontal,tr("磨粒图片路径"));
        _abmInfoModel->setHeaderData(abm_abrasivematerial,                  Qt::Horizontal,tr("磨粒材质"));
        _abmInfoModel->setHeaderData(abm_abrasiveposition,                  Qt::Horizontal,tr("磨粒位置"));
        _abmInfoModel->setHeaderData(abm_abrasivesize,                      Qt::Horizontal,tr("磨粒尺寸"));
        _abmInfoModel->setHeaderData(abm_abrasivesperimeter,                Qt::Horizontal,tr("磨粒周长"));
        _abmInfoModel->setHeaderData(abm_abrasiveshape,                     Qt::Horizontal,tr("磨粒形状"));
        _abmInfoModel->setHeaderData(abm_abrasivecolor,                     Qt::Horizontal,tr("磨粒颜色"));
        _abmInfoModel->setHeaderData(abm_abrasivesurfacetexturetype,        Qt::Horizontal,tr("磨粒表面纹理颜色"));
        _abmInfoModel->setHeaderData(abm_abrasiveweartype,                  Qt::Horizontal,tr("磨粒磨损类型"));
        _abmInfoModel->setHeaderData(abm_abrasivedamagetype,                Qt::Horizontal,tr("磨粒损伤类型"));
        _abmInfoModel->setHeaderData(abm_abrasivemechanismtype,             Qt::Horizontal,tr("磨粒磨损机理"));
        _abmInfoModel->setHeaderData(abm_abrasivefaultinformationreflection,Qt::Horizontal,tr("磨粒反映故障信息"));
        _abmInfoModel->setHeaderData(abm_abrasivetypical,                   Qt::Horizontal,tr("磨粒典型性"));
    }
    else if(tablename == "FegInfo")
    {
        _fegInfoModel->setHeaderData(feg_ferrographysheetid,           Qt::Horizontal,tr("铁谱片编号"));
        _fegInfoModel->setHeaderData(feg_ferrographyreportid,          Qt::Horizontal,tr("铁谱分析报告编号"));
        _fegInfoModel->setHeaderData(feg_oilsampleid,                  Qt::Horizontal,tr("油样编号"));
        _fegInfoModel->setHeaderData(feg_ferrographyanalyertype,       Qt::Horizontal,tr("分析铁谱仪型号"));
        _fegInfoModel->setHeaderData(feg_ferrographymakeoilconsumption,Qt::Horizontal,tr("铁谱片制取油样消耗量"));
        _fegInfoModel->setHeaderData(feg_ferrographymakemethod,        Qt::Horizontal,tr("铁谱片制取方法"));
        _fegInfoModel->setHeaderData(feg_ferrographymakestuff,         Qt::Horizontal,tr("铁谱片制取人"));
    }
    else if(tablename == "FegPInfo")
    {
        _fegpInfoModel->setHeaderData(fegp_ferrographypicid,            Qt::Horizontal,tr("铁谱图片编号"));
        _fegpInfoModel->setHeaderData(fegp_ferrographysheetid,          Qt::Horizontal,tr("铁谱片编号"));
        _fegpInfoModel->setHeaderData(fegp_ferrographyreportid,         Qt::Horizontal,tr("铁谱分析报告编号"));
        _fegpInfoModel->setHeaderData(fegp_microscopictype,             Qt::Horizontal,tr("显微镜型号"));
        _fegpInfoModel->setHeaderData(fegp_imageacquisitiontype,        Qt::Horizontal,tr("图像采集器型号"));
        _fegpInfoModel->setHeaderData(fegp_lightsourcetype,             Qt::Horizontal,tr("光源类型"));
        _fegpInfoModel->setHeaderData(fegp_magnification,               Qt::Horizontal,tr("放大倍数"));
        _fegpInfoModel->setHeaderData(fegp_imageacquisitionarea,        Qt::Horizontal,tr("铁谱图片采集区域"));
        _fegpInfoModel->setHeaderData(fegp_imageacquisitionstuff,       Qt::Horizontal,tr("铁谱图片采集人"));
        _fegpInfoModel->setHeaderData(fegp_ferrographypicpath,          Qt::Horizontal,tr("铁谱图片路径"));
        _fegpInfoModel->setHeaderData(fegp_imagerecognitioninfoanalysis,Qt::Horizontal,tr("铁谱图片识别分析信息"));
        _fegpInfoModel->setHeaderData(fegp_imagesymbol,                 Qt::Horizontal,tr("铁谱图片标识符号"));
        
    }
    else if(tablename == "OiaInfo")
    {
        _oiaInfoModel->setHeaderData(oia_oilsampleid,                  Qt::Horizontal,tr("油样编号"));
        _oiaInfoModel->setHeaderData(oia_analyzedepartname,            Qt::Horizontal,tr("检测分析单位名称"));
        _oiaInfoModel->setHeaderData(oia_senddepart,                   Qt::Horizontal,tr("送检单位"));
        _oiaInfoModel->setHeaderData(oia_sendreason,                   Qt::Horizontal,tr("送检原因"));
        _oiaInfoModel->setHeaderData(oia_sendstuff,                    Qt::Horizontal,tr("送检人"));
        _oiaInfoModel->setHeaderData(oia_receivedate,                  Qt::Horizontal,tr("收油样日期"));
        _oiaInfoModel->setHeaderData(oia_receivestuff,                 Qt::Horizontal,tr("收油样人"));
        _oiaInfoModel->setHeaderData(oia_contaminationanalyzemethod,   Qt::Horizontal,tr("污染度分析方法"));
        _oiaInfoModel->setHeaderData(oia_contaminationanalyzestuff,    Qt::Horizontal,tr("污染度分析人"));
        _oiaInfoModel->setHeaderData(oia_contaminationanalyzedate,     Qt::Horizontal,tr("污染度分析日期"));
        _oiaInfoModel->setHeaderData(oia_contaminationanalyzeequipment,Qt::Horizontal,tr("污染度分析设备"));
        _oiaInfoModel->setHeaderData(oia_contaminationanalyzereportid, Qt::Horizontal,tr("污染度分析报告编号"));
        _oiaInfoModel->setHeaderData(oia_spectroscopymethod,           Qt::Horizontal,tr("光谱分析方法"));
        _oiaInfoModel->setHeaderData(oia_spectroscopystuff,            Qt::Horizontal,tr("光谱分析人"));
        _oiaInfoModel->setHeaderData(oia_spectroscopydate,             Qt::Horizontal,tr("光谱分析日期"));
        _oiaInfoModel->setHeaderData(oia_spectroscopyequipment,        Qt::Horizontal,tr("光谱分析设备"));
        _oiaInfoModel->setHeaderData(oia_spectroscopyreportid,         Qt::Horizontal,tr("光谱分析报告编号"));
        _oiaInfoModel->setHeaderData(oia_ferrographymethod,            Qt::Horizontal,tr("铁谱分析方法"));
        _oiaInfoModel->setHeaderData(oia_ferrographystuff,             Qt::Horizontal,tr("铁谱分析人"));
        _oiaInfoModel->setHeaderData(oia_ferrographydate,              Qt::Horizontal,tr("铁谱分析日期"));
        _oiaInfoModel->setHeaderData(oia_ferrographyequipment,         Qt::Horizontal,tr("铁谱分析设备"));
        _oiaInfoModel->setHeaderData(oia_ferrographyreportid,          Qt::Horizontal,tr("铁谱分析报告编号"));
        _oiaInfoModel->setHeaderData(oia_physicochemicalmethod,        Qt::Horizontal,tr("理化分析方法"));
        _oiaInfoModel->setHeaderData(oia_physicochemicalstuff,         Qt::Horizontal,tr("理化分析人"));
        _oiaInfoModel->setHeaderData(oia_physicochemicaldate,          Qt::Horizontal,tr("理化分析日期"));
        _oiaInfoModel->setHeaderData(oia_physicochemicalequipment,     Qt::Horizontal,tr("理化分析设备"));
        _oiaInfoModel->setHeaderData(oia_physicochemicalreportid,      Qt::Horizontal,tr("理化分析设备编号"));
    }
    else if(tablename == "OisInfo")
    {
        _oisInfoModel->setHeaderData(ois_oilsampleid,      Qt::Horizontal,tr("油样编号"));
        _oisInfoModel->setHeaderData(ois_sampledepartid,   Qt::Horizontal,tr("采样单位编号"));
        _oisInfoModel->setHeaderData(ois_planetype,        Qt::Horizontal,tr("机型"));
        _oisInfoModel->setHeaderData(ois_planeid,          Qt::Horizontal,tr("机号"));
        _oisInfoModel->setHeaderData(ois_monitorpartname,  Qt::Horizontal,tr("监控部件名称"));
        _oisInfoModel->setHeaderData(ois_monitorpartid,    Qt::Horizontal,tr("监控部件编号"));
        _oisInfoModel->setHeaderData(ois_sampleid,         Qt::Horizontal,tr("采样点编号"));
        _oisInfoModel->setHeaderData(ois_oilworktime,      Qt::Horizontal,tr("滑油工作时数"));
        _oisInfoModel->setHeaderData(ois_oiladdition,      Qt::Horizontal,tr("滑油添加量"));
        _oisInfoModel->setHeaderData(ois_samplereason,     Qt::Horizontal,tr("采样原因"));
        _oisInfoModel->setHeaderData(ois_sampledepartname, Qt::Horizontal,tr("采样单位"));
        _oisInfoModel->setHeaderData(ois_samplestuff,      Qt::Horizontal,tr("采样人"));
        _oisInfoModel->setHeaderData(ois_sampledate,       Qt::Horizontal,tr("采样日期"));
        _oisInfoModel->setHeaderData(ois_sampletime,       Qt::Horizontal,tr("采样时间"));
        _oisInfoModel->setHeaderData(ois_samplesituation,  Qt::Horizontal,tr("采样时机"));
        _oisInfoModel->setHeaderData(ois_samplemethod,     Qt::Horizontal,tr("采样方式"));
        _oisInfoModel->setHeaderData(ois_samplevolume,     Qt::Horizontal,tr("采样量"));
        _oisInfoModel->setHeaderData(ois_sampleinstruction,Qt::Horizontal,tr("采样说明"));
        _oisInfoModel->setHeaderData(ois_sendstuff,        Qt::Horizontal,tr("送样人"));
        _oisInfoModel->setHeaderData(ois_senddate,         Qt::Horizontal,tr("送样日期"));
        _oisInfoModel->setHeaderData(ois_sendtime,         Qt::Horizontal,tr("送样时间"));
    }
}


void AdvanceSearchDlg::initpropertylistName()
{
    if(propertymodel)   
    {
        delete propertymodel;
        propertymodel = 0;
    }
    propertymodel = new QSqlQueryModel;
    propertymodel->setQuery("select propertyname from propertyinfo");
    propertymodel->setHeaderData(0,Qt::Horizontal,tr("查询属性名"));
    ui->propertylistTableView->setModel(propertymodel);
    ui->propertylistTableView->setAlternatingRowColors(true);
    ui->propertylistTableView->setSelectionMode(QTableView::SingleSelection);
    ui->propertylistTableView->setEditTriggers(QTableView::NoEditTriggers);
    ui->propertylistTableView->verticalHeader()->setVisible(false);
}

void AdvanceSearchDlg::initCbBox()
{
    QSqlQuery query;
    
    query.exec("select * from equipmentinfo");
    
    while(query.next())
    { 
        if(ui->planeidCbBox->findText(query.value(eqm_planeid).toString()) == -1)
            ui->planeidCbBox->insertItem(-1,query.value(eqm_planeid).toString());
        if(ui->planeTypeCbBox->findText(query.value(eqm_planetype).toString()) == -1)
            ui->planeTypeCbBox->insertItem(-1,query.value(eqm_planetype).toString());
        if(ui->departIdCbBox->findText(query.value(eqm_deoartid).toString()) == -1)
            ui->departIdCbBox->insertItem(-1,query.value(eqm_deoartid).toString());
        if(ui->runHourCbBox->findText(query.value(eqm_runhour).toString()) == -1)
            ui->runHourCbBox->insertItem(-1,query.value(eqm_runhour).toString());
        if(ui->runStageCbBox->findText(query.value(eqm_runstage).toString()) == -1)
            ui->runStageCbBox->insertItem(-1,query.value(eqm_runstage).toString());
        if(ui->repairTimeCbBox->findText(query.value(eqm_repairtime).toString()) == -1)
            ui->repairTimeCbBox->insertItem(-1,query.value(eqm_repairtime).toString());
    }
    ui->planeidCbBox->setCurrentIndex(0);
    ui->planeTypeCbBox->setCurrentIndex(0);
    ui->departIdCbBox->setCurrentIndex(0);
    ui->runHourCbBox->setCurrentIndex(0);
    ui->runStageCbBox->setCurrentIndex(0);
    
    query.exec("select * from movepartinfo");
    while(query.next())
    {
        if(ui->movepartIdCbBox->findText(query.value(mp_movepartid).toString()) == -1)
            ui->movepartIdCbBox->insertItem(-1,query.value(mp_movepartid).toString());
        if(ui->movepartNameCbBox->findText(query.value(mp_movepartname).toString()) == -1)
            ui->movepartNameCbBox->insertItem(-1,query.value(mp_movepartname).toString());
        if(ui->movepartTypeCbBox->findText(query.value(mp_moveparttype).toString()) == -1)
            ui->movepartTypeCbBox->insertItem(-1,query.value(mp_moveparttype).toString());
        if(ui->partrunHourCbBox->findText(query.value(mp_runhour).toString()) == -1)
            ui->partrunHourCbBox->insertItem(-1,query.value(mp_runhour).toString());
    }
    ui->movepartIdCbBox->setCurrentIndex(0);
    ui->movepartNameCbBox->setCurrentIndex(0);
    ui->movepartTypeCbBox->setCurrentIndex(0);
    ui->partrunHourCbBox->setCurrentIndex(0);
    
    query.exec("select * from movepartrepairinfo");
    while(query.next())
    {
        if(ui->movepartRepairIdCbBox->findText(query.value(mpr_movepartrepairid).toString()) == -1)
            ui->movepartRepairIdCbBox->insertItem(-1,query.value(mpr_movepartrepairid).toString());
        if(ui->repairDepartCbBox->findText(query.value(mpr_repairdepart).toString()) == -1)
            ui->repairDepartCbBox->insertItem(-1,query.value(mpr_repairdepart).toString());
        if(ui->repairrepairTimeCbBox->findText(query.value(mpr_repairtime).toString()) == -1)
            ui->repairrepairTimeCbBox->insertItem(-1,query.value(mpr_repairtime).toString());
    }
    ui->movepartRepairIdCbBox->setCurrentIndex(0);
    ui->repairDepartCbBox->setCurrentIndex(0);
    ui->repairTimeCbBox->setCurrentIndex(0);
    
    query.exec("select * from oilsampleinfo");
    while(query.next())
    {
        if(ui->oilsampleidCbBox->findText(query.value(ois_oilsampleid).toString()) == -1)
            ui->oilsampleidCbBox->insertItem(-1,query.value(ois_oilsampleid).toString());
        if(ui->samplesituationCbBox->findText(query.value(ois_samplesituation).toString()) == -1)
            ui->samplesituationCbBox->insertItem(-1,query.value(ois_samplesituation).toString());;
        if(ui->samplemethodCbBox->findText(query.value(ois_samplemethod).toString()) == -1)
            ui->samplemethodCbBox->insertItem(-1,query.value(ois_samplemethod).toString());
        if(ui->sampleidCbBox->findText(query.value(ois_sampleid).toString()) == -1)
            ui->sampleidCbBox->insertItem(-1,query.value(ois_sampleid).toString());
        if(ui->sampledepartidCbBox->findText(query.value(ois_sampledepartid).toString()) == -1)
            ui->sampledepartidCbBox->insertItem(-1,query.value(ois_sampledepartid).toString());
        if(ui->sampledepartnameCbBox->findText(query.value(ois_sampledepartname).toString()) == -1)
            ui->sampledepartnameCbBox->insertItem(-1,query.value(ois_sampledepartname).toString());
        if(ui->monitorpartidCbBox->findText(query.value(ois_monitorpartid).toString()) == -1)
            ui->monitorpartidCbBox->insertItem(-1,query.value(ois_monitorpartid).toString());
        if(ui->monitorpartnameCbBox->findText(query.value(ois_monitorpartname).toString()) == -1)
            ui->monitorpartnameCbBox->insertItem(-1,query.value(ois_monitorpartname).toString());
    }
    ui->oilsampleidCbBox->setCurrentIndex(0);
    ui->samplesituationCbBox->setCurrentIndex(0);
    ui->samplemethodCbBox->setCurrentIndex(0);
    ui->sampleidCbBox->setCurrentIndex(0);
    ui->sampledepartidCbBox->setCurrentIndex(0);
    ui->sampledepartnameCbBox->setCurrentIndex(0);
    ui->monitorpartidCbBox->setCurrentIndex(0);
    ui->monitorpartnameCbBox->setCurrentIndex(0);
    
    query.exec("select * from oilanalyzeinfo");
    while(query.next())
    {
        if(ui->oia_senddepartCbBox->findText(query.value(oia_senddepart).toString()) == -1)
            ui->oia_senddepartCbBox->insertItem(-1,query.value(oia_senddepart).toString());
        if(ui->oia_analyzedepartnameCbBox->findText(query.value(oia_analyzedepartname).toString()) == -1)
            ui->oia_analyzedepartnameCbBox->insertItem(-1,query.value(oia_analyzedepartname).toString());
        if(ui->oia_contaminationanalyzemethodCbBox->findText(query.value(oia_contaminationanalyzemethod).toString()) == -1)
            ui->oia_contaminationanalyzemethodCbBox->insertItem(-1,query.value(oia_analyzedepartname).toString());
        if(ui->oia_contaminationanalyzeequipmentCbBox->findText(query.value(oia_contaminationanalyzeequipment).toString()) == -1)
            ui->oia_contaminationanalyzeequipmentCbBox->insertItem(-1,query.value(oia_contaminationanalyzeequipment).toString());
        if(ui->oia_contaminationanalyzereportidCbBox->findText(query.value(oia_contaminationanalyzereportid).toString()) == -1)
            ui->oia_contaminationanalyzereportidCbBox->insertItem(-1,query.value(oia_contaminationanalyzereportid).toString());
        if(ui->oia_spectroscopymethodCbBo->findText(query.value(oia_spectroscopymethod).toString()) == -1)
            ui->oia_spectroscopymethodCbBo->insertItem(-1,query.value(oia_spectroscopymethod).toString());
        if(ui->oia_spectroscopyequipmentCbBox->findText(query.value(oia_spectroscopyequipment).toString()) == -1)
            ui->oia_spectroscopyequipmentCbBox->insertItem(-1,query.value(oia_spectroscopyequipment).toString());
        if(ui->oia_spectroscopyreportidCbBox->findText(query.value(oia_spectroscopyreportid).toString()) == -1)
            ui->oia_spectroscopyreportidCbBox->insertItem(-1,query.value(oia_spectroscopyreportid).toString());
        if(ui->oia_physicochemicalmethodCbBox->findText(query.value(oia_physicochemicalmethod).toString()) == -1)
            ui->oia_physicochemicalmethodCbBox->insertItem(-1,query.value(oia_physicochemicalmethod).toString());
        if(ui->oia_physicochemicalequipmentCbBox->findText(query.value(oia_physicochemicalequipment).toString()) == -1)
            ui->oia_physicochemicalequipmentCbBox->insertItem(-1,query.value(oia_physicochemicalequipment).toString());
        if(ui->oia_physicochemicalreportidCbBox->findText(query.value(oia_physicochemicalreportid).toString()) == -1)
            ui->oia_physicochemicalreportidCbBox->insertItem(-1,query.value(oia_physicochemicalreportid).toString());
        if(ui->oia_ferrographymethodCbBox->findText(query.value(oia_ferrographymethod).toString()) == -1)
            ui->oia_ferrographymethodCbBox->insertItem(-1,query.value(oia_ferrographymethod).toString());
        if(ui->oia_ferrographyequipmentCbBox->findText(query.value(oia_ferrographyequipment).toString()) == -1)
            ui->oia_ferrographyequipmentCbBox->insertItem(-1,query.value(oia_ferrographyequipment).toString());
        if(ui->oia_ferrographyreportidCbBox->findText(query.value(oia_ferrographyreportid).toString()) == -1)
            ui->oia_ferrographyreportidCbBox->insertItem(-1,query.value(oia_ferrographyreportid).toString());
    }
    
    query.exec("select * from abrasivemarkinfo");
    while(query.next())
    {
        if(ui->abm_abrasiveidCbBox->findText(query.value(abm_abrasiveid).toString()) == -1)
            ui->abm_abrasiveidCbBox->insertItem(-1,query.value(abm_abrasiveid).toString());
        if(ui->abm_abrasivecolorCbBox->findText(query.value(abm_abrasivecolor).toString()) == -1)
            ui->abm_abrasivecolorCbBox->insertItem(-1,query.value(abm_abrasivecolor).toString());
        if(ui->abm_abrasiveshapeCbBox->findText(query.value(abm_abrasiveshape).toString()) == -1)
            ui->abm_abrasiveshapeCbBox->insertItem(-1,query.value(abm_abrasiveshape).toString());
        if(ui->abm_abrasivematerialCbBox->findText(query.value(abm_abrasivematerial).toString()) == -1)
            ui->abm_abrasivematerialCbBox->insertItem(-1,query.value(abm_abrasivematerial).toString());
        if(ui->abm_abrasivepositionCbBox->findText(query.value(abm_abrasiveposition).toString()) == -1)
            ui->abm_abrasivepositionCbBox->insertItem(-1,query.value(abm_abrasiveposition).toString());
        if(ui->abm_abrasivedamagetypeCbBox->findText(query.value(abm_abrasivedamagetype).toString()) == -1)
            ui->abm_abrasivedamagetypeCbBox->insertItem(-1,query.value(abm_abrasivedamagetype).toString());
        if(ui->abm_abrasiveweartypeCbBox->findText(query.value(abm_abrasiveweartype).toString()) == -1)
            ui->abm_abrasiveweartypeCbBox->insertItem(-1,query.value(abm_abrasiveweartype).toString());
        if(ui->abm_abrasivesurfacetexturetypeCbBox->findText(query.value(abm_abrasivesurfacetexturetype).toString()) == -1)
            ui->abm_abrasivesurfacetexturetypeCbBox->insertItem(-1,query.value(abm_abrasivesurfacetexturetype).toString());
        if(ui->abm_ferrographysheetidCbBox->findText(query.value(abm_ferrographysheetid).toString())== -1)
            ui->abm_ferrographysheetidCbBox->insertItem(-1,query.value(abm_ferrographysheetid).toString());
        if(ui->abm_ferrographypicidCbBox->findText(query.value(abm_ferrographypicid).toString()) == -1)
            ui->abm_ferrographypicidCbBox->insertItem(-1,query.value(abm_ferrographypicid).toString());
        if(ui->abm_ferrographyreportidCbBox->findText(query.value(abm_ferrographyreportid).toString()) == -1)
            ui->abm_ferrographyreportidCbBox->insertItem(-1,query.value(abm_ferrographyreportid).toString());
    }
    
    query.exec("select * from ferrographypicinfo");
    while(query.next())
    {
        if(ui->fegp_ferrographypicidCbBox->findText(query.value(fegp_ferrographypicid).toString()) == -1)
            ui->fegp_ferrographypicidCbBox->insertItem(-1,query.value(fegp_ferrographypicid).toString());
        if(ui->fegp_imagesymbolCbBox->findText(query.value(fegp_imagesymbol).toString()) == -1)
            ui->fegp_imagesymbolCbBox->insertItem(-1,query.value(fegp_imagesymbol).toString());
        if(ui->fegp_ferrographyreportidCbBox->findText(query.value(fegp_ferrographyreportid).toString()) == -1)
            ui->fegp_ferrographyreportidCbBox->insertItem(-1,query.value(fegp_ferrographyreportid).toString());
        if(ui->fegp_imageacquisitionareaCbBox->findText(query.value(fegp_imageacquisitionarea).toString()) == -1)
            ui->fegp_imageacquisitionareaCbBox->insertItem(-1,query.value(fegp_imageacquisitionarea).toString());
        if(ui->fegp_imageacquisitiontypeCbBox->findText(query.value(fegp_imageacquisitiontype).toString()) == -1)
            ui->fegp_imageacquisitiontypeCbBox->insertItem(-1,query.value(fegp_imageacquisitiontype).toString());
        if(ui->fegp_lightsourcetypeCbBox->findText(query.value(fegp_lightsourcetype).toString()) == -1)
            ui->fegp_lightsourcetypeCbBox->insertItem(-1,query.value(fegp_lightsourcetype).toString());
        if(ui->fegp_magnificationCbBox->findText(query.value(fegp_magnification).toString()) == -1)
            ui->fegp_magnificationCbBox->insertItem(-1,query.value(fegp_magnification).toString());
        if(ui->fegp_microscopictypeCbBox->findText(query.value(fegp_microscopictype).toString()) == -1)
            ui->fegp_microscopictypeCbBox->insertItem(-1,query.value(fegp_microscopictype).toString());
    }
    query.exec("select * from ferrographyinfo");
    while(query.next())
    {
        if(ui->feg_ferrographysheetidCbBox->findText(query.value(feg_ferrographysheetid).toString()) == -1)
            ui->feg_ferrographysheetidCbBox->insertItem(-1,query.value(feg_ferrographysheetid).toString());
        if(ui->feg_ferrographymakemethodCbBox->findText(query.value(feg_ferrographymakemethod).toString()) == -1)
            ui->feg_ferrographymakemethodCbBox->insertItem(-1,query.value(feg_ferrographymakemethod).toString());
        if(ui->feg_ferrographyanalyzertypeCbBox->findText(query.value(feg_ferrographyanalyertype).toString()) == -1)
            ui->feg_ferrographyanalyzertypeCbBox->insertItem(-1,query.value(feg_ferrographyanalyertype).toString());
    }
}



void AdvanceSearchDlg::on_PlaneIdChkBox_clicked()
{
    _eqmCdtMap.remove("planeid");
    // forein key
    _mpCdtMap.remove("planeid");
    _oisCdtMap.remove("planeid");
    if(ui->PlaneIdChkBox->isChecked())
    {   
        QString text = ui->planeidCbBox->currentText();
        _eqmCdtMap.insert("planeid",text);
        _mpCdtMap.insert("planeid",text);
        _oisCdtMap.insert("planeid",text);
    }
}


void AdvanceSearchDlg::on_planeidCbBox_currentIndexChanged(int index)
{
    _eqmCdtMap.remove("planeid");
    // forein key
    _mpCdtMap.remove("planeid");
    _oisCdtMap.remove("planeid");
    if(ui->PlaneIdChkBox->isChecked())
    {   
        QString text = ui->planeidCbBox->currentText();
        _eqmCdtMap.insert("planeid",text);
        _mpCdtMap.insert("planeid",text);
        _oisCdtMap.insert("planeid",text);
    }
}


void AdvanceSearchDlg::on_planeTypeChkBox_clicked()
{
    _eqmCdtMap.remove("planetype");
    if(ui->planeTypeChkBox->isChecked())
    {
        QString text = ui->planeTypeCbBox->currentText();
        _eqmCdtMap.insert("planetype",text);
    }
}


void AdvanceSearchDlg::on_planeTypeCbBox_currentIndexChanged(int index)
{
    _eqmCdtMap.remove("planetype");
    if(ui->planeTypeChkBox->isChecked())
    {
        QString text = ui->planeTypeCbBox->currentText();
        _eqmCdtMap.insert("planetype",text);
    }
}


void AdvanceSearchDlg::on_departIdChkBox_clicked()
{
    _eqmCdtMap.remove("departid");
    if(ui->departIdChkBox->isChecked())
    {
        QString text = ui->departIdCbBox->currentText();
        _eqmCdtMap.insert("departid",text);
    }
}


void AdvanceSearchDlg::on_departIdCbBox_currentIndexChanged(int index)
{
    _eqmCdtMap.remove("departid");
    if(ui->departIdChkBox->isChecked())
    {
        QString text = ui->departIdCbBox->currentText();
        _eqmCdtMap.insert("departid",text);
    }
}


void AdvanceSearchDlg::on_runHourChkBox_clicked()
{
    _eqmCdtMap.remove("runhour");
    if(ui->runHourChkBox->isChecked())
    {
        QString text = ui->runHourCbBox->currentText();
        _eqmCdtMap.insert("runhour",text);
    }
}


void AdvanceSearchDlg::on_runHourCbBox_currentIndexChanged(int index)
{
    _eqmCdtMap.remove("runhour");
    if(ui->runHourChkBox->isChecked())
    {
        QString text = ui->runHourCbBox->currentText();
        _eqmCdtMap.insert("runhour",text);
    }
}


void AdvanceSearchDlg::on_runStageChkBox_clicked()
{
    _eqmCdtMap.remove("runstage");
    if(ui->runStageChkBox->isChecked())
    {
        QString text = ui->runStageCbBox->currentText();
        _eqmCdtMap.insert("runstage",text);
    }
}


void AdvanceSearchDlg::on_runStageCbBox_currentIndexChanged(int index)
{
    _eqmCdtMap.remove("runstage");
    if(ui->runStageChkBox->isChecked())
    {
        QString text = ui->runStageCbBox->currentText();
        _eqmCdtMap.insert("runstage",text);
    }
}


void AdvanceSearchDlg::on_repairTimeChkBox_clicked()
{
    _eqmCdtMap.remove("repairtime");
    if(ui->repairTimeChkBox->isChecked())
    {
        QString text = ui->repairTimeCbBox->currentText();
        _eqmCdtMap.insert("repairtime",text);
    }
}


void AdvanceSearchDlg::on_repairTimeCbBox_currentIndexChanged(int index)
{
    _eqmCdtMap.remove("repairtime");
    if(ui->repairTimeChkBox->isChecked())
    {
        QString text = ui->repairTimeCbBox->currentText();
        _eqmCdtMap.insert("repairtime",text);
    }
}


void AdvanceSearchDlg::on_addtoBtn_clicked()
{   
    ppnDlg = new ProPertyNameDlg(this);
    if(ppnDlg->exec()== QDialog::Accepted)
    {
        //        qDebug()<<"Accept";
        //        qDebug()<<this->propertyName;
        
        QString eqmFields = "";
        QString eqmValues = "";
        if(!_eqmCdtMap.isEmpty())
        {
            QMap<QString,QString>::iterator it;
            it = _eqmCdtMap.begin();
            eqmFields.append(it.key());
            eqmValues.append(it.value());
            ++it;
            for(;it != _eqmCdtMap.end();++it)
            {
                eqmFields.append("#");
                eqmFields.append(it.key());
                eqmValues.append("#");
                eqmValues.append(it.value());
            }
        }
        
        QString mpFields = "";
        QString mpValues = "";
        if(!_mpCdtMap.isEmpty())
        {
            QMap<QString,QString>::iterator it;
            it = _mpCdtMap.begin();
            mpFields.append(it.key());
            mpValues.append(it.value());
            ++it;
            for(;it !=_mpCdtMap.end();++it)
            {
                mpFields.append("#");
                mpValues.append("#");
                mpFields.append(it.key());
                mpValues.append(it.value());
            }
        }
        
        QString mprFields = "";
        QString mprValues = "";
        if(!_mprCdtMap.isEmpty())
        {
            QMap<QString,QString>::iterator it;
            it = _mprCdtMap.begin();
            mprFields.append(it.key());
            mprValues.append(it.value());
            ++it;
            for(;it != _mprCdtMap.end();++it)
            {
                mprFields.append("#");
                mprValues.append("#");
                mprFields.append(it.key());
                mprValues.append(it.value());
            }
        }
        
        QString fegFields = "";
        QString fegValues = "";
        if(!_fegCdtMap.isEmpty())
        {
            QMap<QString,QString>::iterator it;
            it = _fegCdtMap.begin();
            fegFields.append(it.key());
            fegValues.append(it.value());
            ++it;
            for(;it!=_fegCdtMap.end();++it)
            {
                fegFields.append("#");
                fegValues.append("#");
                fegFields.append(it.key());
                fegValues.append(it.value());
            }
        }
        
        QString fegpFields = "";
        QString fegpValues = "";
        if(!_fegpCdtMap.isEmpty())
        {
            QMap<QString,QString>::iterator it;
            it = _fegpCdtMap.begin();
            fegpFields.append(it.key());
            fegpValues.append(it.value());
            ++it;
            for(;it != _fegpCdtMap.end();++it)
            {
                fegpFields.append("#");
                fegpValues.append("#");
                fegpFields.append(it.key());
                fegpValues.append(it.value());
            }
        }
        
        QString oisFields = "";
        QString oisValues = "";
        if(!_oisCdtMap.isEmpty())
        {
            QMap<QString,QString>::iterator it;
            it = _oisCdtMap.begin();
            oisFields.append(it.key());
            oisValues.append(it.value());
            ++it;
            for(;it!=_oisCdtMap.end();++it)
            {
                oisFields.append("#");
                oisValues.append("#");
                fegpFields.append(it.key());
                fegpValues.append(it.value());
            }
        }
        
        QString oiaFields = "";
        QString oiaValues = "";
        if(!_oiaCdtMap.isEmpty())
        {
            QMap<QString,QString>::iterator it;
            it = _oiaCdtMap.begin();
            oiaFields.append(it.key());
            oiaValues.append(it.value());
            ++it;
            for(;it!=_oiaCdtMap.end();++it)
            {
                oiaFields.append("#");
                oisValues.append("#");
                fegpFields.append(it.key());
                fegpValues.append(it.value());
            }
        }
        
        QString abmFields = "";
        QString abmValues = "";
        if(!_abmCdtMap.isEmpty())
        {
            QMap<QString,QString>::iterator it;
            it = _abmCdtMap.begin();
            abmFields.append(it.key());
            abmValues.append(it.value());
            ++it;
            for(;it != _abmCdtMap.end();++it)
            {
                abmFields.append("#");
                abmValues.append("#");
                abmFields.append(it.key());
                abmValues.append(it.value());
            }
        }
        
        QSqlQuery query;
        QString count;
        query.exec("select count(*) from propertyinfo");
        if(query.next())
            count = query.value(0).toString();
        qDebug()<<count;
        QString propertySql = "insert into propertyinfo values(";
        propertySql.append(count);
        propertySql.append(",'");
        propertySql.append(propertyName);
        propertySql.append("','");
        propertySql.append(eqmFields);
        propertySql.append("','");
        propertySql.append(eqmValues);
        propertySql.append("','");
        propertySql.append(mpFields);
        propertySql.append("','");
        propertySql.append(mpValues);
        propertySql.append("','");
        propertySql.append(mprFields);
        propertySql.append("','");
        propertySql.append(mprValues);
        propertySql.append("','");
        propertySql.append(fegFields);
        propertySql.append("','");
        propertySql.append(fegValues);
        propertySql.append("','");
        propertySql.append(fegpFields);
        propertySql.append("','");
        propertySql.append(fegpValues);
        propertySql.append("','");
        propertySql.append(oisFields);
        propertySql.append("','");
        propertySql.append(oisValues);
        propertySql.append("','");
        propertySql.append(oiaFields);
        propertySql.append("','");
        propertySql.append(oiaValues);
        propertySql.append("','");
        propertySql.append(abmFields);
        propertySql.append("','");
        propertySql.append(abmValues);
        propertySql.append("')");
        
        if(query.exec(propertySql))
        {
            initpropertylistName();
            QMessageBox::warning(this,tr("提示"),tr("保存查询属性成功"),QMessageBox::Close);
        }
        else
            QMessageBox::warning(this,tr("提示"),tr("保存查询属性失败"),QMessageBox::Close);
    }
    else
        ;
    //        qDebug()<<"Reject";
}


void AdvanceSearchDlg::on_movepartIdChkBox_clicked()
{
    _mpCdtMap.remove("movepartid");
    //foreign key
    _mprCdtMap.remove("movepartid");
    if(ui->movepartIdChkBox->isChecked())
    {
        QString text = ui->movepartIdCbBox->currentText();
        _mpCdtMap.insert("movepartid",text);
        //foreign key
        _mprCdtMap.insert("movepartid",text);
    }
}


void AdvanceSearchDlg::on_movepartIdCbBox_currentIndexChanged(int index)
{
    _mpCdtMap.remove("movepartid");
    //foreign key
    _mprCdtMap.remove("movepartid");
    if(ui->movepartIdChkBox->isChecked())
    {
        QString text = ui->movepartIdCbBox->currentText();
        _mpCdtMap.insert("movepartid",text);
        //foreign key
        _mprCdtMap.insert("movepartid",text);
    }
}

void AdvanceSearchDlg::on_movepartNameChkBox_clicked()
{
    _mpCdtMap.remove("movepartname");
    if(ui->movepartNameChkBox->isChecked())
    {
        QString text = ui->movepartNameCbBox->currentText();
        _mpCdtMap.insert("movepartname",text);
    }
}

void AdvanceSearchDlg::on_movepartNameCbBox_currentIndexChanged(int index)
{
    _mpCdtMap.remove("movepartname");
    if(ui->movepartNameChkBox->isChecked())
    {
        QString text = ui->movepartNameCbBox->currentText();
        _mpCdtMap.insert("movepartname",text);
    }
}

void AdvanceSearchDlg::on_movepartTypeChkBox_clicked()
{
    _mpCdtMap.remove("moveparttype");
    if(ui->movepartTypeChkBox->isChecked())
    {
        QString text = ui->movepartTypeCbBox->currentText();
        _mpCdtMap.insert("moveparttype",text);
    }
}

void AdvanceSearchDlg::on_movepartTypeCbBox_currentIndexChanged(int index)
{
    _mpCdtMap.remove("moveparttype");
    if(ui->movepartTypeChkBox->isChecked())
    {
        QString text = ui->movepartTypeCbBox->currentText();
        _mpCdtMap.insert("moveparttype",text);
    }
}

void AdvanceSearchDlg::on_partrunHourChkBox_clicked()
{
    _mpCdtMap.remove("runhour");
    if(ui->partrunHourChkBox->isChecked())
    {
        QString text = ui->partrunHourCbBox->currentText();
        _mpCdtMap.insert("runhour",text);
    }
}

void AdvanceSearchDlg::on_partrunHourCbBox_currentIndexChanged(int index)
{
    _mpCdtMap.remove("runhour");
    if(ui->partrunHourChkBox->isChecked())
    {
        QString text = ui->partrunHourCbBox->currentText();
        _mpCdtMap.insert("runhour",text);
    }
}

void AdvanceSearchDlg::on_movepartStartDateEdit_dateChanged(const QDate &date)
{
    _mpCdtMap.remove("startdate");
    if(ui->movepartStartDataChkBox->isChecked())
    {
        QString text = date.toString("yyyy-MM-dd");
        _mpCdtMap.insert("startdate",text);
    }
}

void AdvanceSearchDlg::on_movepartStartDataChkBox_clicked()
{
    _mpCdtMap.remove("startdate");
    if(ui->movepartStartDataChkBox->isChecked())
    {
        QString text = ui->movepartStartDateEdit->date().toString("yyyy-MM-dd");
        _mpCdtMap.insert("startdate",text);
    }
}

void AdvanceSearchDlg::on_movepartEndDateEdit_dateChanged(const QDate &date)
{
    _mpCdtMap.remove("enddate");
    if(ui->movepartEndDataChkBox->isChecked())
    {
        QString text = date.toString("yyyy-MM-dd");
        _mpCdtMap.insert("enddate",text);
    }
}


void AdvanceSearchDlg::on_movepartEndDataChkBox_clicked()
{
    _mpCdtMap.remove("enddate");
    if(ui->movepartEndDataChkBox->isChecked())
    {
        QString text = ui->movepartEndDateEdit->date().toString("yyyy-MM-dd");
        _mpCdtMap.insert("enddate",text);
    }
}


void AdvanceSearchDlg::setpropertyName(QString propertyname)
{
    this->propertyName = propertyname;
}


void AdvanceSearchDlg::on_exportBtn_clicked()
{
    QFileDialog *packgeFileDlg = new QFileDialog(this,
                                                 tr("选择保存路径"),
                                                 tr(""),
                                                 tr(""));
    packgeFileDlg->setFileMode(QFileDialog::DirectoryOnly);
    QString packgepath;
    if(packgeFileDlg->exec())
    {
        QStringList packgepaths = packgeFileDlg->selectedFiles();
        packgepath = packgepaths.at(0);
    }
    else
        return;
    QStringList imgFileNames;
    int count = _fegpInfoModel->rowCount();
    for(int i=0;i<count;++i)
    {
        QSqlRecord record = _fegpInfoModel->record(i);
        QStringList templist = record.value(fegp_ferrographypicpath).toString().split("/");
        imgFileNames.append(templist.at(templist.count()-1));
    }
    
    QString sqlfilepath = packgepath + "/backup.sql";
    QString sourceimgtopath = packgepath + "/source/";
    QString resultimgtopath = packgepath + "/result/";
    QString sourceimgfrompath = Global::PathImage;
    QString resultimgfrompath = Global::PathResult;
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("保存导出数据"),
                                                    sqlfilepath,
                                                    tr("SqlFile(*.sql)"));
    if(filename.isEmpty())
        return;
    if(this->exportDB(_eqmInfoModel,"equipmentinfo",filename) &&
            this->copyFiles(sourceimgfrompath,sourceimgtopath,imgFileNames) &&
            this->copyFiles(resultimgfrompath,resultimgtopath,imgFileNames))
        QMessageBox::warning(this,
                             tr("提示"),
                             tr("数据导出成功"),
                             QMessageBox::Close);
    else
        QMessageBox::warning(this,
                             tr("提示"),
                             tr("数据导出失败"),
                             QMessageBox::Close);
}



void AdvanceSearchDlg::on_importBtn_clicked()
{
    
    QFileDialog *packgeFileDlg = new QFileDialog(this,
                                                 tr("选择数据导入路径"),
                                                 tr(""),
                                                 tr(""));
    packgeFileDlg->setFileMode(QFileDialog::DirectoryOnly);
    
    QString packgefilepath;
    if(packgeFileDlg->exec())
    {
        QStringList packgefilepaths = packgeFileDlg->selectedFiles();
        packgefilepath = packgefilepaths.at(0);
    }
    else
        return;
    
    QString sqlfilepath = packgefilepath + "/";
    QString sourceimgfrompath = packgefilepath + "/source/";
    QString sourceimgtopath   = Global::PathImage;
    QString resultimgfrompath = packgefilepath + "/result/";
    QString resultimgtopath   = Global::PathResult;
    
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("导入数据"),
                                                    sqlfilepath,
                                                    tr("SqlFile(*.sql)"));
    if(filename.isEmpty())
        return;
    
    
    if(this->importDB(filename) && 
            this->copyFiles(sourceimgfrompath,sourceimgtopath)&&
            this->copyFiles(resultimgfrompath,resultimgtopath))
        QMessageBox::warning(this,
                             tr("提示"),
                             tr("数据导入成功"),
                             QMessageBox::Close);
    else
        QMessageBox::warning(this,
                             tr("提示"),
                             tr("数据导入失败"),
                             QMessageBox::Close);
}

void AdvanceSearchDlg::on_movepartRepairIdChkBox_clicked()
{
    _mprCdtMap.remove("movepartrepairid");
    if(ui->movepartRepairIdChkBox->isChecked())
    {
        QString text = ui->movepartRepairIdCbBox->currentText();
        _mprCdtMap.insert("movepartrepairid",text);
    }
}

void AdvanceSearchDlg::on_movepartRepairIdCbBox_currentIndexChanged(int index)
{
    _mprCdtMap.remove("movepartrepairid");
    if(ui->movepartRepairIdChkBox->isChecked())
    {
        QString text = ui->movepartRepairIdCbBox->currentText();
        _mprCdtMap.insert("movepartrepairid",text);
    }
}

void AdvanceSearchDlg::on_repairDepartChkBox_clicked()
{
    _mprCdtMap.remove("repairdepart");
    if(ui->repairDepartChkBox->isChecked())
    {
        QString text = ui->repairDepartCbBox->currentText();
        _mprCdtMap.insert("repairdepart",text);
    }
}

void AdvanceSearchDlg::on_repairDepartCbBox_currentIndexChanged(int index)
{
    _mprCdtMap.remove("repairdepart");
    if(ui->repairDepartChkBox->isChecked())
    {
        QString text = ui->repairDepartCbBox->currentText();
        _mprCdtMap.insert("repairdepart",text);
    }
}

void AdvanceSearchDlg::on_repairrepairTimeChkBox_clicked()
{
    _mprCdtMap.remove("repairtime");
    if(ui->repairrepairTimeChkBox->isChecked())
    {
        QString text = ui->repairrepairTimeCbBox->currentText();
        _mprCdtMap.insert("repairtime",text);
    }
}

void AdvanceSearchDlg::on_repairrepairTimeCbBox_currentIndexChanged(int index)
{
    _mprCdtMap.remove("repairtime");
    if(ui->repairrepairTimeChkBox->isChecked())
    {
        QString text = ui->repairrepairTimeCbBox->currentText();
        _mprCdtMap.insert("repairtime",text);
    }
}

void AdvanceSearchDlg::on_repairDateDateEdit_dateChanged(const QDate &date)
{
    _mprCdtMap.remove("repairdate");
    if(ui->repairDateChkBox->isChecked())
    {
        QString text = date.toString("yyyy-MM-dd");
        _mprCdtMap.insert("repairdate",text);
    }
}

void AdvanceSearchDlg::on_repairDateChkBox_clicked()
{
    _mprCdtMap.remove("repairdate");
    if(ui->repairDateChkBox->isChecked())
    {
        QString text = ui->repairDateDateEdit->date().toString("yyyy-MM-dd");
        _mprCdtMap.insert("repairdate",text);
    }
}

void AdvanceSearchDlg::on_samplestuffChkBox_clicked()
{
    _oisCdtMap.remove("samplestuff");
    if(ui->samplestuffChkBox->isChecked())
    {
        QString text = ui->samplestuffLineEdit->text();
        _oisCdtMap.insert("samplestuff",text);
    }
}



void AdvanceSearchDlg::on_samplestuffLineEdit_textChanged(const QString &arg1)
{
    _oisCdtMap.remove("samplestuff");
    if(ui->samplestuffChkBox->isChecked())
    {
        QString text = ui->samplestuffLineEdit->text();
        _oisCdtMap.insert("samplestuff",text);
    }
}

void AdvanceSearchDlg::on_samplevolumeChkBox_clicked()
{
    _oisCdtMap.remove("samplevolume");
    if(ui->samplevolumeChkBox->isChecked())
    {
        QString text = ui->samplevolumeLineEdit->text();
        _oisCdtMap.insert("samplevolume",text);
    }
}

void AdvanceSearchDlg::on_samplevolumeLineEdit_textChanged(const QString &arg1)
{
    _oisCdtMap.remove("samplevolume");
    if(ui->samplevolumeChkBox->isChecked())
    {
        QString text = ui->samplevolumeLineEdit->text();
        _oisCdtMap.insert("samplevolume",text);
    }
}

void AdvanceSearchDlg::on_sampledateChkBox_clicked()
{
    _oisCdtMap.remove("sampledate");
    if(ui->sampledateChkBox->isChecked())
    {
        QString text = ui->sampledateDateEdit->date().toString("yyyy-MM-dd");
        _oisCdtMap.insert("sampledate",text);
    }
}

void AdvanceSearchDlg::on_sampledateDateEdit_dateChanged(const QDate &date)
{
    _oisCdtMap.remove("sampledate");
    if(ui->sampledateChkBox->isChecked())
    {
        QString text = ui->sampledateDateEdit->date().toString("yyyy-MM-dd");
        _oisCdtMap.insert("sampledate",text);
    }
}

void AdvanceSearchDlg::on_sampletimeChkBox_clicked()
{
    _oisCdtMap.remove("sampletime");
    if(ui->sampletimeChkBox->isChecked())
    {
        QString text = ui->sampletimeTimeEdit->time().toString("h:mm AP");
        _oisCdtMap.insert("sampletime",text);
    }
}

void AdvanceSearchDlg::on_sampletimeTimeEdit_timeChanged(const QTime &date)
{
    _oisCdtMap.remove("sampletime");
    if(ui->sampletimeChkBox->isChecked())
    {
        QString text = ui->sampletimeTimeEdit->time().toString("h:mm AP");
        _oisCdtMap.insert("sampletime",text);
    }
}

void AdvanceSearchDlg::on_senddateChkBox_clicked()
{
    _oisCdtMap.remove("senddate");
    if(ui->senddateChkBox->isChecked())
    {
        QString text = ui->senddateDateEdit->date().toString("yyyy-MM-dd");
        _oisCdtMap.insert("senddate",text);
    }
}

void AdvanceSearchDlg::on_senddateDateEdit_dateChanged(const QDate &date)
{
    _oisCdtMap.remove("senddate");
    if(ui->senddateChkBox->isChecked())
    {
        QString text = ui->senddateDateEdit->date().toString("yyyy-MM-dd");
        _oisCdtMap.insert("senddate",text);
    }
}


void AdvanceSearchDlg::on_oiladditionChkBox_clicked()
{
    _oisCdtMap.remove("oiladdition");
    if(ui->oiladditionChkBox->isChecked())
    {
        QString text = ui->oiladditionLineEdit->text();
        _oisCdtMap.insert("oiladdition",text);
    }
}

void AdvanceSearchDlg::on_oiladditionLineEdit_textChanged(const QString &arg1)
{
    _oisCdtMap.remove("oiladdition");
    if(ui->oiladditionChkBox->isChecked())
    {
        QString text = ui->oiladditionLineEdit->text();
        _oisCdtMap.insert("oiladdition",text);
    }
}

void AdvanceSearchDlg::on_oilworktimeChkBox_clicked()
{
    _oisCdtMap.remove("oilworktime");
    if(ui->oilworktimeChkBox->isChecked())
    {
        QString text = ui->oilworktimeLineEdit->text();
        _oisCdtMap.insert("oilworktime",text);
    }
}

void AdvanceSearchDlg::on_oilworktimeLineEdit_textChanged(const QString &arg1)
{
    _oisCdtMap.remove("oilworktime");
    if(ui->oilworktimeChkBox->isChecked())
    {
        QString text = ui->oilworktimeLineEdit->text();
        _oisCdtMap.insert("oilworktime",text);
    }
}

void AdvanceSearchDlg::on_monitorpartidChkBox_clicked()
{
    _oisCdtMap.remove("monitorpartid");
    if(ui->monitorpartidChkBox->isChecked())
    {
        QString text = ui->monitorpartidCbBox->currentText();
        _oisCdtMap.insert("monitorpartid",text);
    }
}



void AdvanceSearchDlg::on_monitorpartidCbBox_currentIndexChanged(int index)
{
    _oisCdtMap.remove("monitorpartid");
    if(ui->monitorpartidChkBox->isChecked())
    {
        QString text = ui->monitorpartidCbBox->currentText();
        _oisCdtMap.insert("monitorpartid",text);
    }
}

//void AdvanceSearchDlg::on_monitorpartidLineEdit_textChanged(const QString &arg1)
//{
//    _oisCdtMap.remove("monitorpartid");
//    if(ui->monitorpartidChkBox->isChecked())
//    {
//        QString text = ui->monitorpartidLineEdit->text();
//        _oisCdtMap.insert("monitorpartid",text);
//    }
//}

void AdvanceSearchDlg::on_monitorpartnameChkBox_clicked()
{
    _oisCdtMap.remove("monitorpartname");
    if(ui->monitorpartnameChkBox->isChecked())
    {
        QString text = ui->monitorpartnameCbBox->currentText();
        _oisCdtMap.insert("monitorpartname",text);
    }
}




void AdvanceSearchDlg::on_monitorpartnameCbBox_currentIndexChanged(int index)
{
    _oisCdtMap.remove("monitorpartname");
    if(ui->monitorpartnameChkBox->isChecked())
    {
        QString text = ui->monitorpartnameCbBox->currentText();
        _oisCdtMap.insert("monitorpartname",text);
    }
}

//void AdvanceSearchDlg::on_monitorpartnameLineEdit_textChanged(const QString &arg1)
//{
//    _oisCdtMap.remove("monitorpartname");
//    if(ui->monitorpartnameChkBox->isChecked())
//    {
//        QString text = ui->monitorpartnameLineEdit->text();
//        _oisCdtMap.insert("monitorpartname",text);
//    }
//}

void AdvanceSearchDlg::on_oilsampleidChkBox_clicked()
{
    _oisCdtMap.remove("oilsampleid");
    // foreign key
    _oiaCdtMap.remove("oiasampleid");
    _fegCdtMap.remove("oilsampleid");
    if(ui->oilsampleidChkBox->isChecked())
    {
        QString text = ui->oilsampleidCbBox->currentText();
        _oisCdtMap.insert("oilsampleid",text);
        
        _oiaCdtMap.insert("oilsampleid",text);
        _fegCdtMap.insert("oilsampleid",text);
    }
}

//void AdvanceSearchDlg::on_oilsampleidLineEdit_textChanged(const QString &arg1)
//{
//    _oisCdtMap.remove("oilsampleid");
//    if(ui->oilsampleidChkBox->isChecked())
//    {
//        QString text = ui->oilsampleidCbBox->currentText();
//        _oisCdtMap.insert("oilsampleid",text);
//    }
//}

void AdvanceSearchDlg::on_oilsampleidCbBox_currentIndexChanged(int index)
{
    _oisCdtMap.remove("oilsampleid");
    _oiaCdtMap.remove("oiasampleid");
    _fegCdtMap.remove("oilsampleid");
    if(ui->oilsampleidChkBox->isChecked())
    {
        QString text = ui->oilsampleidCbBox->currentText();
        _oisCdtMap.insert("oilsampleid",text);
        _oiaCdtMap.insert("oilsampleid",text);
        _fegCdtMap.insert("oilsampleid",text);
    }
}


void AdvanceSearchDlg::on_samplesituationChkBox_clicked()
{
    _oisCdtMap.remove("samplesituation");
    if(ui->samplesituationChkBox->isChecked())
    {
        QString text = ui->samplesituationCbBox->currentText();
        _oisCdtMap.insert("samplesituation",text);
    }
}


void AdvanceSearchDlg::on_samplesituationCbBox_currentIndexChanged(int index)
{
    _oisCdtMap.remove("samplesituation");
    if(ui->samplesituationChkBox->isChecked())
    {
        QString text = ui->samplesituationCbBox->currentText();
        _oisCdtMap.insert("samplesituation",text);
    }
}

//void AdvanceSearchDlg::on_samplesituationLineEdit_textChanged(const QString &arg1)
//{
//    _oisCdtMap.remove("samplesituation");
//    if(ui->samplesituationChkBox->isChecked())
//    {
//        QString text = ui->samplesituationLineEdit->text();
//        _oisCdtMap.insert("samplesituation",text);
//    }
//}

void AdvanceSearchDlg::on_sampleidChkBox_clicked()
{
    _oisCdtMap.remove("sampleid");
    if(ui->sampleidChkBox->isChecked())
    {
        QString text = ui->sampleidCbBox->currentText();
        _oisCdtMap.insert("sampleid",text);
    }
}


void AdvanceSearchDlg::on_sampleidCbBox_currentIndexChanged(int index)
{
    _oisCdtMap.remove("sampleid");
    if(ui->sampleidChkBox->isChecked())
    {
        QString text = ui->sampleidCbBox->currentText();
        _oisCdtMap.insert("sampleid",text);
    }
}

//void AdvanceSearchDlg::on_sampleidLineEdit_textChanged(const QString &arg1)
//{
//    _oisCdtMap.remove("sampleid");
//    if(ui->sampleidChkBox->isChecked())
//    {
//        QString text = ui->sampleidLineEdit->text();
//        _oisCdtMap.insert("sampleid",text);
//    }
//}

void AdvanceSearchDlg::on_samplemethodChkBox_clicked()
{
    _oisCdtMap.remove("samplemethod");
    if(ui->samplemethodChkBox->isChecked())
    {
        QString text = ui->samplemethodCbBox->currentText();
        _oisCdtMap.insert("samplemethod",text);
    }
}


void AdvanceSearchDlg::on_samplemethodCbBox_currentIndexChanged(int index)
{
    _oisCdtMap.remove("samplemethod");
    if(ui->samplemethodChkBox->isChecked())
    {
        QString text = ui->samplemethodCbBox->currentText();
        _oisCdtMap.insert("samplemethod",text);
    }
}

//void AdvanceSearchDlg::on_samplemethodLineEdit_textChanged(const QString &arg1)
//{
//    _oisCdtMap.remove("samplemethod");
//    if(ui->samplemethodChkBox->isChecked())
//    {
//        QString text = ui->samplemethodLineEdit->text();
//        _oisCdtMap.insert("samplemethod",text);
//    }
//}

void AdvanceSearchDlg::on_sampledepartidChkBox_clicked()
{
    _oisCdtMap.remove("sampledepartid");
    if(ui->sampledepartidChkBox->isChecked())
    {
        QString text = ui->sampledepartidCbBox->currentText();
        _oisCdtMap.insert("sampledepartid",text);
    }
}


void AdvanceSearchDlg::on_sampledepartidCbBox_currentIndexChanged(int index)
{
    _oisCdtMap.remove("sampledepartid");
    if(ui->sampledepartidChkBox->isChecked())
    {
        QString text = ui->sampledepartidCbBox->currentText();
        _oisCdtMap.insert("sampledepartid",text);
    }
}

//void AdvanceSearchDlg::on_sampledepartidlineEdit_textChanged(const QString &arg1)
//{
//    _oisCdtMap.remove("sampledepartid");
//    if(ui->sampledepartidChkBox->isChecked())
//    {
//        QString text = ui->sampledepartidlineEdit->text();
//        _oisCdtMap.insert("sampledepartid",text);
//    }
//}

void AdvanceSearchDlg::on_sampledepartnameChkBox_clicked()
{
    _oisCdtMap.remove("sampledepartname");
    if(ui->sampledepartnameChkBox->isChecked())
    {
        QString text = ui->sampledepartnameCbBox->currentText();
        _oisCdtMap.insert("sampledepartname",text);
    }
}


void AdvanceSearchDlg::on_sampledepartnameCbBox_currentIndexChanged(int index)
{
    _oisCdtMap.remove("sampledepartname")   ;
    if(ui->sampledepartnameChkBox->isChecked())
    {
        QString text = ui->sampledepartnameCbBox->currentText();
        _oisCdtMap.insert("sampledepartname",text);
    }
}
//void AdvanceSearchDlg::on_sampledepartnameLineEdit_textChanged(const QString &arg1)
//{
//    _oisCdtMap.remove("sampledepartname");
//    if(ui->sampledepartnameChkBox->isChecked())
//    {
//        QString text = ui->sampledepartnameLineEdit->text();
//        _oisCdtMap.insert("sampledepartname",text);
//    }
//}

void AdvanceSearchDlg::on_sendstuffChkBox_clicked()
{
    _oisCdtMap.remove("sendstuff");
    if(ui->sendstuffChkBox->isChecked())
    {
        QString text = ui->sendstuffLineEdit->text();
        _oisCdtMap.insert("sendstuff",text);
    }
}

void AdvanceSearchDlg::on_sendstuffLineEdit_textChanged(const QString &arg1)
{
    _oisCdtMap.remove("sendstuff");
    if(ui->sendstuffChkBox->isChecked())
    {
        QString text = ui->sendstuffLineEdit->text();
        _oisCdtMap.insert("sendstuff",text);
    }
}



void AdvanceSearchDlg::on_sendtimeChkBox_clicked()
{
    _oisCdtMap.remove("sendtime");
    if(ui->sendtimeChkBox->isChecked())
    {
        QString text = ui->sendtimeTimeEdit->time().toString("h:mm Ap");
        _oisCdtMap.insert("sendtime",text);
    }
}


void AdvanceSearchDlg::on_sendtimeTimeEdit_timeChanged(const QTime &date)
{
    _oisCdtMap.remove("sendtime");
    if(ui->sendtimeChkBox->isChecked())
    {
        QString text = ui->sendtimeTimeEdit->time().toString("h:mm Ap");
        _oisCdtMap.insert("sendtime",text);
    }
}


void AdvanceSearchDlg::on_oia_sendstuffChkBox_clicked()
{
    _oiaCdtMap.remove("sendstuff");
    if(ui->oia_sendstuffChkBox->isChecked())
    {
        QString text = ui->sendstuffLineEdit->text();
        _oiaCdtMap.insert("sendstuff",text);
    }
}

void AdvanceSearchDlg::on_oia_sendstuffLineEdit_textChanged(const QString &arg1)
{
    _oiaCdtMap.remove("sendstuff");
    if(ui->oia_sendstuffChkBox->isChecked())
    {
        QString text = ui->sendstuffLineEdit->text();
        _oiaCdtMap.insert("sendstuff",text);
    }
}

void AdvanceSearchDlg::on_oia_senddepartChkBox_clicked()
{
    _oiaCdtMap.remove("senddepart");
    if(ui->oia_senddepartChkBox->isChecked())
    {
        QString text = ui->oia_senddepartCbBox->currentText();
        _oiaCdtMap.insert("senddepart",text);
    }
}

void AdvanceSearchDlg::on_oia_senddepartCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("senddepart");
    if(ui->oia_senddepartChkBox->isChecked())
    {
        QString text = ui->oia_senddepartCbBox->currentText();
        _oiaCdtMap.insert("senddepart",text);
    }
}

void AdvanceSearchDlg::on_oia_receivestuffChkBox_clicked()
{
    _oiaCdtMap.remove("receivestuff");
    if(ui->oia_receivestuffChkBox->isChecked())
    {
        QString text = ui->oia_receivestuffLineEdit->text();
        _oiaCdtMap.insert("receivestuff",text);
    }
}

void AdvanceSearchDlg::on_oia_receivestuffLineEdit_textChanged(const QString &arg1)
{
    _oiaCdtMap.remove("receivestuff");
    if(ui->oia_receivestuffChkBox->isChecked())
    {
        QString text = ui->oia_receivestuffLineEdit->text();
        _oiaCdtMap.insert("receivestuff",text);
    }
}

void AdvanceSearchDlg::on_oia_receivedateChkBox_clicked()
{
    _oiaCdtMap.remove("receivedate");
    if(ui->oia_receivedateChkBox->isChecked())
    {
        QString text = ui->oia_receivedateDateEdit->date().toString("yyyy-MM-dd");
        _oiaCdtMap.insert("receivedate",text);
    }
}

void AdvanceSearchDlg::on_oia_receivedateDateEdit_dateChanged(const QDate &date)
{
    _oiaCdtMap.remove("receivedate");
    if(ui->oia_receivedateChkBox->isChecked())
    {
        QString text = ui->oia_receivedateDateEdit->date().toString("yyyy-MM-dd");
        _oiaCdtMap.insert("receivedate",text);
    }
}

//void AdvanceSearchDlg::on_oia_receivedateDateEdit_timeChanged(const QTime &time)
//{
//    _oiaCdtMap.remove("receivedate");
//    if(ui->oia_receivedateChkBox->isChecked())
//    {
//        QString text = ui->oia_receivedateDateEdit->date().toString("yyyy-MM-dd");
//        _oiaCdtMap.insert("receivedate",text);
//    }
//}


void AdvanceSearchDlg::on_oia_analyzedepartnameChkBox_clicked()
{
    _oiaCdtMap.remove("analyzedepartname");
    if(ui->oia_analyzedepartnameChkBox->isChecked())
    {
        QString text = ui->oia_analyzedepartnameCbBox->currentText();
        _oiaCdtMap.insert("analyzedepartname",text);
    }
}


void AdvanceSearchDlg::on_oia_analyzedepartnameCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("analyzedepartname");
    if(ui->oia_analyzedepartnameChkBox->isChecked())
    {
        QString text = ui->oia_analyzedepartnameCbBox->currentText();
        _oiaCdtMap.insert("analyzedepartname",text);
    }
}


void AdvanceSearchDlg::on_oia_contaminationanalyzestuffChkBox_clicked()
{
    _oiaCdtMap.remove("contaminationanalyzestuff");
    if(ui->oia_contaminationanalyzestuffChkBox->isChecked())
    {
        QString text = ui->oia_contaminationanalyzestuffChkLineEdit->text();
        _oiaCdtMap.insert("contaminationanalyzestuff",text);
    }
}


void AdvanceSearchDlg::on_oia_contaminationanalyzestuffChkLineEdit_textChanged(const QString &arg1)
{
    _oiaCdtMap.remove("contaminationanalyzestuff");
    if(ui->oia_contaminationanalyzestuffChkBox->isChecked())
    {
        QString text = ui->oia_contaminationanalyzestuffChkLineEdit->text();
        _oiaCdtMap.insert("contaminationanalyzestuff",text);
    }
}


void AdvanceSearchDlg::on_oia_contaminationanalyzemethodChkBox_clicked()
{
    _oiaCdtMap.remove("contaminationanalyzemethod");
    if(ui->oia_contaminationanalyzemethodChkBox->isChecked())
    {
        QString text = ui->oia_contaminationanalyzemethodCbBox->currentText();
        _oiaCdtMap.insert("contaminationanalyzemethod",text);
    }
}


void AdvanceSearchDlg::on_oia_contaminationanalyzemethodCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("contaminationanalyzemethod");
    if(ui->oia_contaminationanalyzemethodChkBox->isChecked())
    {
        QString text = ui->oia_contaminationanalyzemethodCbBox->currentText();
        _oiaCdtMap.insert("contaminationanalyzemethod",text);
    }
}


void AdvanceSearchDlg::on_oia_contaminationanalyzeequipmentChkBox_clicked()
{
    _oiaCdtMap.remove("contaminationanalyzeequipment");
    if(ui->oia_contaminationanalyzeequipmentChkBox->isChecked())
    {
        QString text = ui->oia_contaminationanalyzeequipmentCbBox->currentText();
        _oiaCdtMap.insert("contaminationanalyzeequipment",text);
    }
}


void AdvanceSearchDlg::on_oia_contaminationanalyzeequipmentCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("contaminationanalyzeequipment");
    if(ui->oia_contaminationanalyzeequipmentChkBox->isChecked())
    {
        QString text = ui->oia_contaminationanalyzeequipmentCbBox->currentText();
        _oiaCdtMap.insert("contaminationanalyzeequipment",text);
    }
}


void AdvanceSearchDlg::on_oia_contaminationanalyzedateChkBox_clicked()
{
    _oiaCdtMap.remove("contaminationanalyzedate");
    if(ui->oia_contaminationanalyzedateChkBox->isChecked())
    {
        QString text = ui->oia_contaminationanalyzedateDateEdit->date().toString("yyyy-MM-dd");
        _oiaCdtMap.insert("contaminationanalyzedate",text);
    }
}


void AdvanceSearchDlg::on_oia_contaminationanalyzedateDateEdit_dateChanged(const QDate &date)
{
    _oiaCdtMap.remove("contaminationanalyzedate");
    if(ui->oia_contaminationanalyzedateChkBox->isChecked())
    {
        QString text = ui->oia_contaminationanalyzedateDateEdit->date().toString("yyyy-MM-dd");
        _oiaCdtMap.insert("contaminationanalyzedate",text);
    }
}



void AdvanceSearchDlg::on_oia_contaminationanalyzereportidChkBox_clicked()
{
    _oiaCdtMap.remove("contaminationanalyzereportid");
    if(ui->oia_contaminationanalyzereportidChkBox->isChecked())
    {
        QString text = ui->oia_contaminationanalyzereportidCbBox->currentText();
        _oiaCdtMap.insert("contaminationanalyzereportid",text);
    }
}

void AdvanceSearchDlg::on_oia_contaminationanalyzereportidCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("contaminationanalyzereportid");
    if(ui->oia_contaminationanalyzereportidChkBox->isChecked())
    {
        QString text = ui->oia_contaminationanalyzereportidCbBox->currentText();
        _oiaCdtMap.insert("contaminationanalyzereportid",text);
    }
}

void AdvanceSearchDlg::on_oia_spectroscopystuffChkBox_clicked()
{
    _oiaCdtMap.remove("spectroscopystuff");
    if(ui->oia_spectroscopystuffChkBox->isChecked())
    {
        QString text = ui->oia_spectroscopystuffLineEdit->text();
        _oiaCdtMap.insert("spectroscopystuff",text);
    }
}

void AdvanceSearchDlg::on_oia_spectroscopystuffLineEdit_textChanged(const QString &arg1)
{
    _oiaCdtMap.remove("spectroscopystuff");
    if(ui->oia_spectroscopystuffChkBox->isChecked())
    {
        QString text = ui->oia_spectroscopystuffLineEdit->text();
        _oiaCdtMap.insert("spectroscopystuff",text);
    }
}

void AdvanceSearchDlg::on_oia_spectroscopymethodChkBox_clicked()
{
    _oiaCdtMap.remove("spectroscopymethod");
    if(ui->oia_spectroscopymethodChkBox->isChecked())
    {
        QString text = ui->oia_spectroscopymethodCbBo->currentText();
        _oiaCdtMap.insert("spectroscopymethod",text);
    }
}

void AdvanceSearchDlg::on_oia_spectroscopymethodCbBo_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("spectroscopymethod");
    if(ui->oia_spectroscopymethodChkBox->isChecked())
    {
        QString text = ui->oia_spectroscopymethodCbBo->currentText();
        _oiaCdtMap.insert("spectroscopymethod",text);
    }
}

void AdvanceSearchDlg::on_oia_spectroscopyequipmentChkBox_clicked()
{
    _oiaCdtMap.remove("spectroscopyequipment");
    if(ui->oia_spectroscopyequipmentChkBox->isChecked())
    {
        QString text = ui->oia_spectroscopyequipmentCbBox->currentText();
        _oiaCdtMap.insert("spectroscopyequipment",text);
    }
}

void AdvanceSearchDlg::on_oia_spectroscopyequipmentCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("spectroscopyequipment");
    if(ui->oia_spectroscopyequipmentChkBox->isChecked())
    {
        QString text = ui->oia_spectroscopyequipmentCbBox->currentText();
        _oiaCdtMap.insert("spectroscopyequipment",text);
    }
}

void AdvanceSearchDlg::on_oia_spectroscopydateChkBox_clicked()
{
    _oiaCdtMap.remove("spectroscopydate");
    if(ui->oia_spectroscopydateChkBox->isChecked())
    {
        QString text = ui->oia_spectroscopydateDateEdit->date().toString("yyyy-MM-dd");
        _oiaCdtMap.insert("spectroscopydate",text);
    }
}

void AdvanceSearchDlg::on_oia_spectroscopydateDateEdit_dateChanged(const QDate &date)
{
    _oiaCdtMap.remove("spectroscopydate");
    if(ui->oia_spectroscopydateChkBox->isChecked())
    {
        QString text = ui->oia_spectroscopydateDateEdit->date().toString("yyyy-MM-dd");
        _oiaCdtMap.insert("spectroscopydate",text);
    }
}

void AdvanceSearchDlg::on_oia_spectroscopyreportidChkBox_clicked()
{
    _oiaCdtMap.remove("spectroscopyreportid");
    if(ui->oia_spectroscopyreportidChkBox->isChecked())
    {
        QString text = ui->oia_spectroscopyreportidCbBox->currentText();
        _oiaCdtMap.insert("spectroscopyreportid",text);
    }
}

void AdvanceSearchDlg::on_oia_spectroscopyreportidCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("spectroscopyreportid");
    if(ui->oia_spectroscopyreportidChkBox->isChecked())
    {
        QString text = ui->oia_spectroscopyreportidCbBox->currentText();
        _oiaCdtMap.insert("spectroscopyreportid",text);
    }
}

void AdvanceSearchDlg::on_oia_physicochemicalstuffChkBox_clicked()
{
    _oiaCdtMap.remove("physicochemicalstuff");
    if(ui->oia_physicochemicalstuffChkBox->isChecked())
    {
        QString text = ui->oia_physicochemicalstuffLineEdit->text();
        _oiaCdtMap.insert("physicochemicalstuff",text);
    }
}

void AdvanceSearchDlg::on_oia_physicochemicalstuffLineEdit_textChanged(const QString &arg1)
{
    _oiaCdtMap.remove("physicochemicalstuff");
    if(ui->oia_physicochemicalstuffChkBox->isChecked())
    {
        QString text = ui->oia_physicochemicalstuffLineEdit->text();
        _oiaCdtMap.insert("physicochemicalstuff",text);
    }
}

void AdvanceSearchDlg::on_oia_physicochemicalmethodChkBox_clicked()
{
    _oiaCdtMap.remove("physicochemicalmethod");
    if(ui->oia_physicochemicalmethodChkBox->isChecked())
    {
        QString text = ui->oia_physicochemicalmethodCbBox->currentText();
        _oiaCdtMap.insert("physicochemicalmethod",text);
    }
}

void AdvanceSearchDlg::on_oia_physicochemicalmethodCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("physicochemicalmethod");
    if(ui->oia_physicochemicalmethodChkBox->isChecked())
    {
        QString text = ui->oia_physicochemicalmethodCbBox->currentText();
        _oiaCdtMap.insert("physicochemicalmethod",text);
    }
}

void AdvanceSearchDlg::on_oia_physicochemicalequipmentChkBox_clicked()
{
    _oiaCdtMap.remove("physicochemicalequipment");
    if(ui->oia_physicochemicalequipmentChkBox->isChecked())
    {
        QString text = ui->oia_physicochemicalequipmentCbBox->currentText();
        _oiaCdtMap.insert("physicochemicalequipment",text);
    }
}

void AdvanceSearchDlg::on_oia_physicochemicalequipmentCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("physicochemicalequipment");
    if(ui->oia_physicochemicalequipmentChkBox->isChecked())
    {
        QString text = ui->oia_physicochemicalequipmentCbBox->currentText();
        _oiaCdtMap.insert("physicochemicalequipment",text);
    }
}

void AdvanceSearchDlg::on_oia_physicochemicaldateChkBox_clicked()
{
    _oiaCdtMap.remove("physicochemicaldate");
    if(ui->oia_physicochemicaldateChkBox->isChecked())
    {
        QString text = ui->oia_physicochemicaldateDateEdit->date().toString("yyyy-MM-dd");
        _oiaCdtMap.insert("physicochemicaldate",text);
    }
}

void AdvanceSearchDlg::on_oia_physicochemicaldateDateEdit_dateChanged(const QDate &date)
{
    _oiaCdtMap.remove("physicochemicaldate");
    if(ui->oia_physicochemicaldateChkBox->isChecked())
    {
        QString text = ui->oia_physicochemicaldateDateEdit->date().toString("yyyy-MM-dd");
        _oiaCdtMap.insert("physicochemicaldate",text);
    }
}

void AdvanceSearchDlg::on_oia_physicochemicalreportidChkBox_clicked()
{
    _oiaCdtMap.remove("physicochemicalreportid");
    if(ui->oia_physicochemicalreportidChkBox->isChecked())
    {
        QString text = ui->oia_physicochemicalreportidCbBox->currentText();
        _oiaCdtMap.insert("physicochemicalreportid",text);
    }
}

void AdvanceSearchDlg::on_oia_physicochemicalreportidCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("physicochemicalreportid");
    if(ui->oia_physicochemicalreportidChkBox->isChecked())
    {
        QString text = ui->oia_physicochemicalreportidCbBox->currentText();
        _oiaCdtMap.insert("physicochemicalreportid",text);
    }
}

void AdvanceSearchDlg::on_oia_ferrographystuffChkBox_clicked()
{
    _oiaCdtMap.remove("ferrographystuff");
    if(ui->oia_ferrographystuffChkBox->isChecked())
    {
        QString text = ui->oia_ferrographystuffLineEdit->text();
        _oiaCdtMap.insert("ferrographystuff",text);
    }
}

void AdvanceSearchDlg::on_oia_ferrographystuffLineEdit_textChanged(const QString &arg1)
{
    _oiaCdtMap.remove("ferrographystuff");
    if(ui->oia_ferrographystuffChkBox->isChecked())
    {
        QString text = ui->oia_ferrographystuffLineEdit->text();
        _oiaCdtMap.insert("ferrographystuff",text);
    }
}

void AdvanceSearchDlg::on_oia_ferrographymethodChkBox_clicked()
{
    _oiaCdtMap.remove("ferrographymethod");
    if(ui->oia_ferrographymethodChkBox->isChecked())
    {
        QString text = ui->oia_ferrographyreportidCbBox->currentText();
        _oiaCdtMap.insert("ferrographymethod",text);
    }
}

void AdvanceSearchDlg::on_oia_ferrographymethodCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("ferrographymethod");
    if(ui->oia_ferrographymethodChkBox->isChecked())
    {
        QString text = ui->oia_ferrographyreportidCbBox->currentText();
        _oiaCdtMap.insert("ferrographymethod",text);
    }
}

void AdvanceSearchDlg::on_oia_ferrographyequipmentChkBox_clicked()
{
    _oiaCdtMap.remove("ferrographyequipment");
    if(ui->oia_ferrographyequipmentChkBox->isChecked())
    {
        QString text = ui->oia_ferrographyequipmentCbBox->currentText();
        _oiaCdtMap.insert("ferrographyequipment",text);
    }
}

void AdvanceSearchDlg::on_oia_ferrographyequipmentCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("ferrographyequipment");
    if(ui->oia_ferrographyequipmentChkBox->isChecked())
    {
        QString text = ui->oia_ferrographyequipmentCbBox->currentText();
        _oiaCdtMap.insert("ferrographyequipment",text);
    }
}

void AdvanceSearchDlg::on_oia_ferrographydateChkBox_clicked()
{
    _oiaCdtMap.remove("ferrographydate");
    if(ui->oia_ferrographydateChkBox->isChecked())
    {
        QString text = ui->oia_ferrographydateDateEdit->date().toString("yyyy-MM-dd");
        _oiaCdtMap.insert("ferrographydate",text);
    }
}

void AdvanceSearchDlg::on_oia_ferrographydateDateEdit_dateChanged(const QDate &date)
{
    _oiaCdtMap.remove("ferrographydate");
    if(ui->oia_ferrographydateChkBox->isChecked())
    {
        QString text = ui->oia_ferrographydateDateEdit->date().toString("yyyy-MM-dd");
        _oiaCdtMap.insert("ferrographydate",text);
    }
}

void AdvanceSearchDlg::on_oia_ferrographyreportidChkBox_clicked()
{
    _oiaCdtMap.remove("ferrographyreportid");
    if(ui->oia_ferrographyreportidChkBox->isChecked())
    {
        QString text = ui->oia_ferrographyreportidCbBox->currentText();
        _oiaCdtMap.insert("ferrographyreportid",text);
    }
}

void AdvanceSearchDlg::on_oia_ferrographyreportidCbBox_currentIndexChanged(int index)
{
    _oiaCdtMap.remove("ferrographyreportid");
    if(ui->oia_ferrographyreportidChkBox->isChecked())
    {
        QString text = ui->oia_ferrographyreportidCbBox->currentText();
        _oiaCdtMap.insert("ferrographyreportid",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasiveidChkBox_clicked()
{
    _abmCdtMap.remove("abrasiveid");
    if(ui->abm_abrasiveidChkBox->isChecked())
    {
        QString text = ui->abm_abrasiveidCbBox->currentText();
        _abmCdtMap.insert("abrasiveid",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasiveidCbBox_currentIndexChanged(int index)
{
    _abmCdtMap.remove("abrasiveid");
    if(ui->abm_abrasiveidChkBox->isChecked())
    {
        QString text = ui->abm_abrasiveidCbBox->currentText();
        _abmCdtMap.insert("abrasiveid",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasiveshapeChkBox_clicked()
{
    _abmCdtMap.remove("abrasiveshape");
    if(ui->abm_abrasiveshapeChkBox->isChecked())
    {
        QString text = ui->abm_abrasiveshapeCbBox->currentText();
        _abmCdtMap.insert("abrasiveshape",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasiveshapeCbBox_currentIndexChanged(int index)
{
    _abmCdtMap.remove("abrasiveshape");
    if(ui->abm_abrasiveshapeChkBox->isChecked())
    {
        QString text = ui->abm_abrasiveshapeCbBox->currentText();
        _abmCdtMap.insert("abrasiveshape",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivepositionChkBox_clicked()
{
    _abmCdtMap.remove("abrasiveposition");
    if(ui->abm_abrasivepositionChkBox->isChecked())
    {
        QString text = ui->abm_abrasivepositionCbBox->currentText();
        _abmCdtMap.insert("abrasiveposition",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivepositionCbBox_currentIndexChanged(int index)
{
    _abmCdtMap.remove("abrasiveposition");
    if(ui->abm_abrasivepositionChkBox->isChecked())
    {
        QString text = ui->abm_abrasivepositionCbBox->currentText();
        _abmCdtMap.insert("abrasiveposition",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivematerialChkBox_clicked()
{
    _abmCdtMap.remove("abrasivematerial");
    if(ui->abm_abrasivematerialChkBox->isChecked())
    {
        QString text = ui->abm_abrasivematerialCbBox->currentText();
        _abmCdtMap.insert("abrasivematerial",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivematerialCbBox_currentIndexChanged(int index)
{
    _abmCdtMap.remove("abrasivematerial");
    if(ui->abm_abrasivematerialChkBox->isChecked())
    {
        QString text = ui->abm_abrasivematerialCbBox->currentText();
        _abmCdtMap.insert("abrasivematerial",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivecolorChkBox_clicked()
{
    _abmCdtMap.remove("abrasivecolor");
    if(ui->abm_abrasivecolorChkBox->isChecked())
    {
        QString text = ui->abm_abrasivecolorCbBox->currentText();
        _abmCdtMap.insert("abrasivecolor",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivecolorCbBox_currentIndexChanged(int index)
{
    _abmCdtMap.remove("abrasivecolor");
    if(ui->abm_abrasivecolorChkBox->isChecked())
    {
        QString text = ui->abm_abrasivecolorCbBox->currentText();
        _abmCdtMap.insert("abrasivecolor",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivesperimeterChkBox_clicked()
{
    _abmCdtMap.remove("abrasivesperimeter");
    if(ui->abm_abrasivesperimeterChkBox->isChecked())
    {
        QString text = ui->abm_abrasivesperimeterLineEdit->text();
        _abmCdtMap.insert("abrasivesperimeter",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivesperimeterLineEdit_textChanged(const QString &arg1)
{
    _abmCdtMap.remove("abrasivesperimeter");
    if(ui->abm_abrasivesperimeterChkBox->isChecked())
    {
        QString text = ui->abm_abrasivesperimeterLineEdit->text();
        _abmCdtMap.insert("abrasivesperimeter",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivesizeChkBox_clicked()
{
    _abmCdtMap.remove("abrasivesize");
    if(ui->abm_abrasivesizeChkBox->isChecked())
    {
        QString text = ui->abm_abrasivesizeLineEdit->text();
        _abmCdtMap.insert("abrasivesize",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivesizeLineEdit_textChanged(const QString &arg1)
{
    _abmCdtMap.remove("abrasivesize");
    if(ui->abm_abrasivesizeChkBox->isChecked())
    {
        QString text = ui->abm_abrasivesizeLineEdit->text();
        _abmCdtMap.insert("abrasivesize",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivemarkstuffChkBox_clicked()
{
    _abmCdtMap.remove("abrasivemarkstuff");
    if(ui->abm_abrasivemarkstuffChkBox->isChecked())
    {
        QString text = ui->abm_abrasivemarkstuffLineEdit->text();
        _abmCdtMap.insert("abrasivemarkstuff",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivemarkstuffLineEdit_textChanged(const QString &arg1)
{
    _abmCdtMap.remove("abrasivemarkstuff");
    if(ui->abm_abrasivemarkstuffChkBox->isChecked())
    {
        QString text = ui->abm_abrasivemarkstuffLineEdit->text();
        _abmCdtMap.insert("abrasivemarkstuff",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivetypicalChkBox_clicked()
{
    _abmCdtMap.remove("abrasivetypical");
    if(ui->abm_abrasivetypicalChkBox->isChecked())
    {
        QString text = ui->abm_abrasivetypicalLineEdit->text();
        _abmCdtMap.insert("abrasivetypical",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivetypicalLineEdit_textChanged(const QString &arg1)
{
    _abmCdtMap.remove("abrasivetypical");
    if(ui->abm_abrasivetypicalChkBox->isChecked())
    {
        QString text = ui->abm_abrasivetypicalLineEdit->text();
        _abmCdtMap.insert("abrasivetypical",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivemechanismtypeChkBox__clicked()
{
    _abmCdtMap.remove("abrasivemechanismtype");
    if(ui->abm_abrasivemechanismtypeChkBox_->isChecked())
    {
        QString text = ui->abm_abrasivemechanismtypeLineEdit->text();
        _abmCdtMap.insert("abrasivemechanismtype",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivemechanismtypeLineEdit_textChanged(const QString &arg1)
{
    _abmCdtMap.remove("abrasivemechanismtype");
    if(ui->abm_abrasivemechanismtypeChkBox_->isChecked())
    {
        QString text = ui->abm_abrasivemechanismtypeLineEdit->text();
        _abmCdtMap.insert("abrasivemechanismtype",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivedamagetypeChkBox_clicked()
{
    _abmCdtMap.remove("abrasivedamagetype");
    if(ui->abm_abrasivedamagetypeChkBox->isChecked())
    {
        QString text = ui->abm_abrasivedamagetypeCbBox->currentText();
        _abmCdtMap.insert("abrasivedamagetype",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivedamagetypeCbBox_currentIndexChanged(int index)
{
    _abmCdtMap.remove("abrasivedamagetype");
    if(ui->abm_abrasivedamagetypeChkBox->isChecked())
    {
        QString text = ui->abm_abrasivedamagetypeCbBox->currentText();
        _abmCdtMap.insert("abrasivedamagetype",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasiveweartypeChkBox_clicked()
{
    _abmCdtMap.remove("abrasiveweartype");
    if(ui->abm_abrasiveweartypeChkBox->isChecked())
    {
        QString text = ui->abm_abrasiveweartypeCbBox->currentText();
        _abmCdtMap.insert("abrasiveweartype",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasiveweartypeCbBox_currentIndexChanged(int index)
{
    _abmCdtMap.remove("abrasiveweartype");
    if(ui->abm_abrasiveweartypeChkBox->isChecked())
    {
        QString text = ui->abm_abrasiveweartypeCbBox->currentText();
        _abmCdtMap.insert("abrasiveweartype",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivesurfacetexturetypeChkBox_clicked()
{
    _abmCdtMap.remove("abrasivesurfacetexturetype");
    if(ui->abm_abrasivesurfacetexturetypeChkBox->isChecked())
    {
        QString text = ui->abm_abrasivesurfacetexturetypeCbBox->currentText();
        _abmCdtMap.insert("abrasivesurfacetexturetype",text);
    }
}

void AdvanceSearchDlg::on_abm_abrasivesurfacetexturetypeCbBox_currentIndexChanged(int index)
{
    _abmCdtMap.remove("abrasivesurfacetexturetype");
    if(ui->abm_abrasivesurfacetexturetypeChkBox->isChecked())
    {
        QString text = ui->abm_abrasivesurfacetexturetypeCbBox->currentText();
        _abmCdtMap.insert("abrasivesurfacetexturetype",text);
    }
}

void AdvanceSearchDlg::on_abm_ferrographysheetidChkBox_clicked()
{
    _abmCdtMap.remove("ferrographysheetid");
    if(ui->abm_ferrographysheetidChkBox->isChecked())
    {
        QString text = ui->abm_ferrographysheetidCbBox->currentText();
        _abmCdtMap.insert("ferrographysheetid",text);
    }
}

void AdvanceSearchDlg::on_abm_ferrographysheetidCbBox_currentIndexChanged(int index)
{
    _abmCdtMap.remove("ferrographysheetid");
    if(ui->abm_ferrographysheetidChkBox->isChecked())
    {
        QString text = ui->abm_ferrographysheetidCbBox->currentText();
        _abmCdtMap.insert("ferrographysheetid",text);
    }
}

void AdvanceSearchDlg::on_abm_ferrographypicidChkBox_clicked()
{
    _abmCdtMap.remove("ferrographypicid");
    if(ui->abm_ferrographypicidChkBox->isChecked())
    {
        QString text = ui->abm_ferrographypicidCbBox->currentText();
        _abmCdtMap.insert("ferrographypicid",text);
    }
}

void AdvanceSearchDlg::on_abm_ferrographypicidCbBox_currentIndexChanged(int index)
{
    _abmCdtMap.remove("ferrographypicid");
    if(ui->abm_ferrographypicidChkBox->isChecked())
    {
        QString text = ui->abm_ferrographypicidCbBox->currentText();
        _abmCdtMap.insert("ferrographypicid",text);
    }
}

void AdvanceSearchDlg::on_abm_ferrographyreportidChkBox_clicked()
{
    _abmCdtMap.remove("ferrographyreportid");
    if(ui->abm_ferrographyreportidChkBox->isChecked())
    {
        QString text = ui->abm_ferrographyreportidCbBox->currentText();
        _abmCdtMap.insert("ferrographyreportid",text);
    }
}

void AdvanceSearchDlg::on_abm_ferrographyreportidCbBox_currentIndexChanged(int index)
{
    _abmCdtMap.remove("ferrographyreportid");
    if(ui->abm_ferrographyreportidChkBox->isChecked())
    {
        QString text = ui->abm_ferrographyreportidCbBox->currentText();
        _abmCdtMap.insert("ferrographyreportid",text);
    }
}

void AdvanceSearchDlg::on_fegp_ferrographypicidChkBox_clicked()
{
    _fegpCdtMap.remove("ferrographypicid");
    if(ui->fegp_ferrographypicidChkBox->isChecked())
    {
        QString text =ui->fegp_ferrographypicidCbBox->currentText();
        _fegpCdtMap.insert("ferrographypicid",text);
    }
}

void AdvanceSearchDlg::on_fegp_ferrographypicidCbBox_currentIndexChanged(int index)
{
    _fegpCdtMap.remove("ferrographypicid");
    if(ui->fegp_ferrographypicidChkBox->isChecked())
    {
        QString text =ui->fegp_ferrographypicidCbBox->currentText();
        _fegpCdtMap.insert("ferrographypicid",text);
    }
}

void AdvanceSearchDlg::on_fegp_imagesymbolChkBox_clicked()
{
    _fegpCdtMap.remove("imagesymbol");
    if(ui->fegp_imagesymbolChkBox->isChecked())
    {
        QString text = ui->fegp_imagesymbolCbBox->currentText();
        _fegpCdtMap.insert("imagesymbol",text);
    }
}

void AdvanceSearchDlg::on_fegp_imagesymbolCbBox_currentIndexChanged(int index)
{
    _fegpCdtMap.remove("imagesymbol");
    if(ui->fegp_imagesymbolChkBox->isChecked())
    {
        QString text = ui->fegp_imagesymbolCbBox->currentText();
        _fegpCdtMap.insert("imagesymbol",text);
    }
}

void AdvanceSearchDlg::on_fegp_ferrographyreportidChkBox_clicked()
{
    _fegpCdtMap.remove("ferrographyreportid");
    if(ui->fegp_ferrographyreportidChkBox->isChecked())
    {
        QString text = ui->fegp_ferrographyreportidCbBox->currentText();
        _fegpCdtMap.insert("ferrographyreportid",text);
    }
}

void AdvanceSearchDlg::on_fegp_ferrographyreportidCbBox_currentIndexChanged(int index)
{
    _fegpCdtMap.remove("ferrographyreportid");
    if(ui->fegp_ferrographyreportidChkBox->isChecked())
    {
        QString text = ui->fegp_ferrographyreportidCbBox->currentText();
        _fegpCdtMap.insert("ferrographyreportid",text);
    }
}

void AdvanceSearchDlg::on_fegp_imageacquisitionareaChkBox_clicked()
{
    _fegpCdtMap.remove("imageacquisitionarea");
    if(ui->fegp_imageacquisitionareaChkBox->isChecked())
    {
        QString text = ui->fegp_imageacquisitionareaCbBox->currentText();
        _fegpCdtMap.insert("imageacquisitionarea",text);
    }
}

void AdvanceSearchDlg::on_fegp_imageacquisitionareaCbBox_currentIndexChanged(int index)
{
    _fegpCdtMap.remove("imageacquisitionarea");
    if(ui->fegp_imageacquisitionareaChkBox->isChecked())
    {
        QString text = ui->fegp_imageacquisitionareaCbBox->currentText();
        _fegpCdtMap.insert("imageacquisitionarea",text);
    }
}

void AdvanceSearchDlg::on_fegp_microscopictypeChkBox_clicked()
{
    _fegpCdtMap.remove("microscopictype");
    if(ui->fegp_microscopictypeChkBox->isChecked())
    {
        QString text = ui->fegp_microscopictypeCbBox->currentText();
        _fegpCdtMap.insert("microscopictype",text);
    }
}

void AdvanceSearchDlg::on_fegp_microscopictypeCbBox_currentIndexChanged(int index)
{
    _fegpCdtMap.remove("microscopictype");
    if(ui->fegp_microscopictypeChkBox->isChecked())
    {
        QString text = ui->fegp_microscopictypeCbBox->currentText();
        _fegpCdtMap.insert("microscopictype",text);
    }
}

void AdvanceSearchDlg::on_fegp_lightsourcetypeChkBox_clicked()
{
    _fegpCdtMap.remove("lightsourcetype");
    if(ui->fegp_lightsourcetypeChkBox->isChecked())
    {
        QString text = ui->fegp_lightsourcetypeCbBox->currentText();
        _fegpCdtMap.insert("lightsourcetype",text);
    }
}

void AdvanceSearchDlg::on_fegp_lightsourcetypeCbBox_currentIndexChanged(int index)
{
    _fegpCdtMap.remove("lightsourcetype");
    if(ui->fegp_lightsourcetypeChkBox->isChecked())
    {
        QString text = ui->fegp_lightsourcetypeCbBox->currentText();
        _fegpCdtMap.insert("lightsourcetype",text);
    }
}

void AdvanceSearchDlg::on_fegp_magnificationChkBox_clicked()
{
    _fegpCdtMap.remove("magnification");
    if(ui->fegp_magnificationChkBox->isChecked())
    {
        QString text = ui->fegp_magnificationCbBox->currentText();
        _fegpCdtMap.insert("magnification",text);
    }
}

void AdvanceSearchDlg::on_fegp_magnificationCbBox_currentIndexChanged(int index)
{
    _fegpCdtMap.remove("magnification");
    if(ui->fegp_magnificationChkBox->isChecked())
    {
        QString text = ui->fegp_magnificationCbBox->currentText();
        _fegpCdtMap.insert("magnification",text);
    }
}

void AdvanceSearchDlg::on_fegp_imageacquisitiontypeChkBox_clicked()
{
    _fegpCdtMap.remove("imageacquisitiontype");
    if(ui->fegp_imageacquisitiontypeChkBox->isChecked())
    {
        QString text = ui->fegp_imageacquisitiontypeCbBox->currentText();
        _fegpCdtMap.insert("imageacquisitiontype",text);
    }
}

void AdvanceSearchDlg::on_fegp_imageacquisitiontypeCbBox_currentIndexChanged(int index)
{
    _fegpCdtMap.remove("imageacquisitiontype");
    if(ui->fegp_imageacquisitiontypeChkBox->isChecked())
    {
        QString text = ui->fegp_imageacquisitiontypeCbBox->currentText();
        _fegpCdtMap.insert("imageacquisitiontype",text);
    }
}

void AdvanceSearchDlg::on_fegp_imageacquisitionstuffChkBox_clicked()
{
    _fegpCdtMap.remove("imageacquisitionstuff");
    if(ui->fegp_imageacquisitionstuffChkBox->isChecked())
    {
        QString text = ui->fegp_imageacquisitionstuffLineEdit->text();
        _fegpCdtMap.insert("imageacquisitionstuff",text);
    }
}

void AdvanceSearchDlg::on_fegp_imageacquisitionstuffLineEdit_textChanged(const QString &arg1)
{
    _fegpCdtMap.remove("imageacquisitionstuff");
    if(ui->fegp_imageacquisitionstuffChkBox->isChecked())
    {
        QString text = ui->fegp_imageacquisitionstuffLineEdit->text();
        _fegpCdtMap.insert("imageacquisitionstuff",text);
    }
}

void AdvanceSearchDlg::on_fegp_imagerecognitioninfoanalysisChkBox_clicked()
{
    _fegpCdtMap.remove("imagerecognitioninfoanalysis");
    if(ui->fegp_imagerecognitioninfoanalysisChkBox->isChecked())
    {
        QString text = ui->fegp_imagerecognitioninfoanalysisLineEdit->text();
        _fegpCdtMap.insert("imagerecognitioninfoanalysis",text);
    }
}

void AdvanceSearchDlg::on_fegp_imagerecognitioninfoanalysisLineEdit_textChanged(const QString &arg1)
{
    _fegpCdtMap.remove("imagerecognitioninfoanalysis");
    if(ui->fegp_imagerecognitioninfoanalysisChkBox->isChecked())
    {
        QString text = ui->fegp_imagerecognitioninfoanalysisLineEdit->text();
        _fegpCdtMap.insert("imagerecognitioninfoanalysis",text);
    }
}

void AdvanceSearchDlg::on_feg_ferrographysheetidChkBox_clicked()
{
    _fegCdtMap.remove("ferrographysheetid");
    //foreign key
    _fegpCdtMap.remove("ferrographysheetid");
    if(ui->feg_ferrographysheetidChkBox->isChecked())
    {
        QString text = ui->feg_ferrographysheetidCbBox->currentText();
        _fegCdtMap.insert("ferrographysheetid",text);
        _fegpCdtMap.insert("ferrographysheetid",text);
    }
}

void AdvanceSearchDlg::on_feg_ferrographysheetidCbBox_currentIndexChanged(int index)
{
    _fegCdtMap.remove("ferrographysheetid");
    //foreign key
    _fegpCdtMap.remove("ferrographysheetid");
    if(ui->feg_ferrographysheetidChkBox->isChecked())
    {
        QString text = ui->feg_ferrographysheetidCbBox->currentText();
        _fegCdtMap.insert("ferrographysheetid",text);
        _fegpCdtMap.insert("ferrographysheetid",text);
    }
}

void AdvanceSearchDlg::on_feg_ferrographymakestuffChkBox_clicked()
{
    _fegCdtMap.remove("ferrographymakestuff");
    if(ui->feg_ferrographymakestuffChkBox->isChecked())
    {
        QString text = ui->feg_ferrographymakestuffLineEdit->text();
        _fegCdtMap.insert("ferrographymakestuff",text);
    }
}

void AdvanceSearchDlg::on_feg_ferrographymakestuffLineEdit_textChanged(const QString &arg1)
{
    _fegCdtMap.remove("ferrographymakestuff");
    if(ui->feg_ferrographymakestuffChkBox->isChecked())
    {
        QString text = ui->feg_ferrographymakestuffLineEdit->text();
        _fegCdtMap.insert("ferrographymakestuff",text);
    }
}

void AdvanceSearchDlg::on_feg_ferrographymakemethodChkBox_clicked()
{
    _fegCdtMap.remove("ferrographymakemethod");
    if(ui->feg_ferrographymakemethodChkBox->isChecked())
    {
        QString text = ui->feg_ferrographymakemethodCbBox->currentText();
        _fegCdtMap.insert("ferrographymakemethod",text);
    }
}

void AdvanceSearchDlg::on_feg_ferrographymakemethodCbBox_currentIndexChanged(int index)
{
    _fegCdtMap.remove("ferrographymakemethod");
    if(ui->feg_ferrographymakemethodChkBox->isChecked())
    {
        QString text = ui->feg_ferrographymakemethodCbBox->currentText();
        _fegCdtMap.insert("ferrographymakemethod",text);
    }
}

void AdvanceSearchDlg::on_feg_ferrographyanalyzertypeChkBox_clicked()
{
    _fegCdtMap.remove("ferrographyanalyzertype");
    if(ui->feg_ferrographyanalyzertypeChkBox->isChecked())
    {
        QString text = ui->feg_ferrographyanalyzertypeCbBox->currentText();
        _fegCdtMap.insert("ferrographyanalyzertype",text);
    }
}

void AdvanceSearchDlg::on_feg_ferrographyanalyzertypeCbBox_currentIndexChanged(int index)
{
    _fegCdtMap.remove("ferrographyanalyzertype");
    if(ui->feg_ferrographyanalyzertypeChkBox->isChecked())
    {
        QString text = ui->feg_ferrographyanalyzertypeCbBox->currentText();
        _fegCdtMap.insert("ferrographyanalyzertype",text);
    }
}

void AdvanceSearchDlg::on_feg_ferrographymakeoilconsumptionChkBox_clicked()
{
    _fegCdtMap.remove("ferrographymakeoilconsumption");
    if(ui->feg_ferrographymakeoilconsumptionChkBox->isChecked())
    {
        QString text = ui->feg_ferrographymakeoilconsumptionLineEdit->text();
        _fegCdtMap.insert("ferrographymakeoilconsumption",text);
    }
}

void AdvanceSearchDlg::on_feg_ferrographymakeoilconsumptionLineEdit_textChanged(const QString &arg1)
{
    _fegCdtMap.remove("ferrographymakeoilconsumption");
    if(ui->feg_ferrographymakeoilconsumptionChkBox->isChecked())
    {
        QString text = ui->feg_ferrographymakeoilconsumptionLineEdit->text();
        _fegCdtMap.insert("ferrographymakeoilconsumption",text);
    }
}

void AdvanceSearchDlg::reloadConditions(QString propertyname)
{
    //    QMessageBox::StandardButton reply;
    //    reply = QMessageBox::question(this, tr("QMessageBox::question()"), tr("确认使用当前属性查询?"),
    //                                  QMessageBox::Yes | QMessageBox::Cancel);
    
    //    if(reply == QMessageBox::Yes)
    //    {
    //        QModelIndex index = ui->propertylistTableView->currentIndex();
    //        QSqlRecord record = propertymodel->record(index.row());
    //        QString propertyname = record.value(0).toString();
    
    QString sql = "select * from propertyinfo where propertyname = '";
    sql.append(propertyname);
    sql.append("'");
    QSqlQuery query;
    query.exec(sql);
    query.next();
    
    this->resetConditions();
    
    int idx;
    
    QStringList eqmFields = query.value(2).toString().split("#");
    QStringList eqmValues = query.value(3).toString().split("#");
    if(!eqmFields.isEmpty())
    {
        idx = 0;
        foreach (QString field, eqmFields) {
            if(field == "planeid")
            {
                ui->planeidCbBox->setCurrentIndex(ui->planeidCbBox->findText(eqmValues.at(idx)));
                _eqmCdtMap.insert("planeid",eqmValues.at(idx));
                ui->PlaneIdChkBox->setChecked(true);
            }
            else if(field == "planetype")
            {
                ui->planeTypeCbBox->setCurrentIndex(ui->planeTypeCbBox->findText(eqmValues.at(idx)));
                _eqmCdtMap.insert("planetype",eqmValues.at(idx));
                ui->planeTypeChkBox->setChecked(true);
            }
            else if(field == "departid")
            {
                ui->departIdCbBox->setCurrentIndex(ui->departIdCbBox->findText(eqmValues.at(idx)));
                _eqmCdtMap.insert("departid",eqmValues.at(idx));
                ui->departIdChkBox->setChecked(true);
            }
            else if(field == "runhour")
            {
                ui->runHourCbBox->setCurrentIndex(ui->runHourCbBox->findText(eqmValues.at(idx)));
                _eqmCdtMap.insert("runhour",eqmValues.at(idx));
                ui->runHourChkBox->setChecked(true);
            }
            else if(field == "runstage")
            {
                ui->runStageCbBox->setCurrentIndex(ui->runStageCbBox->findText(eqmValues.at(idx)));
                _eqmCdtMap.insert("runstage",eqmValues.at(idx));
                ui->runStageChkBox->setChecked(true);
            }
            else if(field == "repairtime")
            {
                ui->repairTimeCbBox->setCurrentIndex(ui->repairTimeCbBox->findText(eqmValues.at(idx)));
                _eqmCdtMap.insert("repairtime",eqmValues.at(idx));
                ui->repairTimeChkBox->setChecked(true);
            }
            idx ++;
        }
    }
    
    QStringList mpFields = query.value(4).toString().split("#");
    QStringList mpValues = query.value(5).toString().split("#");
    if(!mpFields.isEmpty())
    {
        idx = 0;
        foreach (QString field, mpFields)
        {
            if(field == "movepartname")
            {
                ui->movepartNameCbBox->setCurrentIndex(ui->movepartNameCbBox->findText(mpValues.at(idx)));
                _mpCdtMap.insert("movepartname",mpValues.at(idx));
                ui->movepartNameChkBox->setChecked(true);
                
            }
            else if(field == "movepartid")
            {
                ui->movepartIdCbBox->setCurrentIndex(ui->movepartIdCbBox->findText(mpValues.at(idx)));
                _mpCdtMap.insert("movepartid",mpValues.at(idx));
                ui->movepartIdChkBox->setChecked(true);
            }
            else if(field == "moveparttype")
            {
                ui->movepartTypeCbBox->setCurrentIndex(ui->movepartTypeCbBox->findText(mpValues.at(idx)));
                _mpCdtMap.insert("moveparttype",mpValues.at(idx));
                ui->movepartTypeChkBox->setChecked(true);
            }
            else if(field == "runhour")
            {
                ui->partrunHourCbBox->setCurrentIndex(ui->partrunHourCbBox->findText(mpValues.at(idx)));
                _mpCdtMap.insert("runhour",mpValues.at(idx));
                ui->partrunHourChkBox->setChecked(true);
            }
            else if(field == "startdate")
            {
                QString datestr = mpValues.at(idx);
                QStringList datelist = datestr.split("-");
                int y = datelist.at(0).toInt();
                int m = datelist.at(1).toInt();
                int d = datelist.at(2).toInt();
                ui->movepartStartDateEdit->setDate(QDate(y,m,d));
                _mpCdtMap.insert("startdate",mpValues.at(idx));
                ui->movepartStartDataChkBox->setChecked(true);
            }
            else if(field == "enddate")
            {
                QString datestr = mpValues.at(idx);
                QStringList datelist = datestr.split("-");
                int y = datelist.at(0).toInt();
                int m = datelist.at(1).toInt();
                int d = datelist.at(2).toInt();
                ui->movepartEndDateEdit->setDate(QDate(y,m,d));
                _mpCdtMap.insert("enddate",mpValues.at(idx));
                ui->movepartEndDataChkBox->setChecked(true);
            }
            idx ++;
        }
        
    }
    
    QStringList mprFields = query.value(6).toString().split("#");
    QStringList mprValues = query.value(7).toString().split("#");
    if(!mprFields.isEmpty())
    {
        idx = 0;
        foreach (QString field, mprFields) {
            if(field == "movepartrepairid")
            {
                ui->movepartRepairIdCbBox->setCurrentIndex(ui->movepartRepairIdCbBox->findText(mprValues.at(idx)));
                _mprCdtMap.insert("movepartrepairid",mprValues.at(idx));
                ui->movepartRepairIdChkBox->setChecked(true);
            }
            else if(field == "repairtime")
            {
                ui->repairrepairTimeCbBox->setCurrentIndex(ui->repairrepairTimeCbBox->findText(mprValues.at(idx)));
                _mprCdtMap.insert("repairtime",mprValues.at(idx));
                ui->repairrepairTimeChkBox->setChecked(true);
            }
            else if(field == "repairdate")
            {
                QString datestr = mprValues.at(idx);
                QStringList datelist = datestr.split("-");
                int y = datelist.at(0).toInt();
                int m = datelist.at(1).toInt();
                int d = datelist.at(2).toInt();
                ui->repairDateDateEdit->setDate(QDate(y,m,d));
                _mprCdtMap.insert("repairdate",mprValues.at(idx));
                ui->repairDateChkBox->setChecked(true);
            }
            else if(field == "repairdepart")
            {
                ui->repairDepartCbBox->setCurrentIndex(ui->repairDepartCbBox->findText(mprValues.at(idx)));
                _mprCdtMap.insert("repairdepart",mprValues.at(idx));
                ui->repairDepartChkBox->setChecked(true);
            }
            idx ++;
        }
    }
    
    QStringList fegFields = query.value(8).toString().split("#");
    QStringList fegValues = query.value(9).toString().split("#");
    if(!fegFields.isEmpty())
    {
        idx = 0;
        foreach (QString field, fegFields)
        {
            if(field == "ferrographysheetid")
            {
                ui->feg_ferrographysheetidCbBox->setCurrentIndex(ui->feg_ferrographysheetidCbBox->findText(fegValues.at(idx)));
                _fegCdtMap.insert("ferrographysheetid",fegValues.at(idx));
                ui->feg_ferrographysheetidChkBox->setChecked(true);
            }
            else if(field == "ferrographymakestuff")
            {
                ui->feg_ferrographymakestuffLineEdit->setText(fegValues.at(idx));
                _fegCdtMap.insert("ferrographymakestuff",fegValues.at(idx));
                ui->feg_ferrographymakestuffChkBox->setChecked(true);
            }
            else if(field == "ferrographymakemethod")
            {
                ui->feg_ferrographymakemethodCbBox->setCurrentIndex(ui->feg_ferrographymakemethodCbBox->findText(fegValues.at(idx)));
                _fegCdtMap.insert("ferrographymakemethod",fegValues.at(idx));
                ui->feg_ferrographymakemethodChkBox->setChecked(true);
            }
            else if(field == "ferrographyanalyzertype")
            {
                ui->feg_ferrographyanalyzertypeCbBox->setCurrentIndex(ui->feg_ferrographyanalyzertypeCbBox->findText(fegValues.at(idx)));
                _fegCdtMap.insert("ferrographyanalyzertype",fegValues.at(idx));
                ui->feg_ferrographyanalyzertypeChkBox->setChecked(true);
            }
            else if(field == "ferrographymakeoilconsumption")
            {
                ui->feg_ferrographymakeoilconsumptionLineEdit->setText(fegValues.at(idx));
                _fegCdtMap.insert("ferrographymakeoilconsumption",fegValues.at(idx));
                ui->feg_ferrographymakeoilconsumptionChkBox->setChecked(true);
            }
            idx ++;
        }
    }
    
    QStringList fegpFields = query.value(10).toString().split("#");
    QStringList fegpValues = query.value(11).toString().split("#");
    if(!fegpFields.isEmpty())
    {
        idx = 0;
        foreach (QString field, fegpFields)
        {
            if(field == "ferrographypicid")
            {
                ui->fegp_ferrographypicidCbBox->setCurrentIndex(ui->fegp_ferrographypicidCbBox->findText(fegpValues.at(idx)));
                _fegpCdtMap.insert("ferrographypicid",fegpValues.at(idx));
                ui->fegp_ferrographypicidChkBox->setChecked(true);
            }
            else if(field == "ferrographyreportid")
            {
                ui->fegp_ferrographyreportidCbBox->setCurrentIndex(ui->fegp_ferrographyreportidCbBox->findText(fegpValues.at(idx)));
                _fegpCdtMap.insert("ferrographyreportid",fegpValues.at(idx));
                ui->fegp_ferrographyreportidChkBox->setChecked(true);
            }
            else if(field == "imagesymbol")
            {
                ui->fegp_imagesymbolCbBox->setCurrentIndex(ui->fegp_imagesymbolCbBox->findText(fegpValues.at(idx)));
                _fegpCdtMap.insert("imagesymbol",fegpValues.at(idx));
                ui->fegp_imagesymbolChkBox->setChecked(true);
            }
            else if(field == "imageacquisitionarea")
            {
                ui->fegp_imageacquisitionareaCbBox->setCurrentIndex(ui->fegp_imageacquisitionareaCbBox->findText(fegpValues.at(idx)));
                _fegpCdtMap.insert("imageacquisitionarea",fegpValues.at(idx));
                ui->fegp_imageacquisitionareaChkBox->setChecked(true);
            }
            else if(field == "microscopictype")
            {
                ui->fegp_microscopictypeCbBox->setCurrentIndex(ui->fegp_microscopictypeCbBox->findText(fegpValues.at(idx)));
                _fegpCdtMap.insert("microscopictype",fegpValues.at(idx));
                ui->fegp_microscopictypeChkBox->setChecked(true);
            }
            else if(field == "lightsourcetype")
            {
                ui->fegp_lightsourcetypeCbBox->setCurrentIndex(ui->fegp_lightsourcetypeCbBox->findText(fegpValues.at(idx)));
                _fegpCdtMap.insert("lightsourcetype",fegpValues.at(idx));
                ui->fegp_lightsourcetypeChkBox->setChecked(true);
            }
            else if(field == "magnification")
            {
                ui->fegp_magnificationCbBox->setCurrentIndex(ui->fegp_magnificationCbBox->findText(fegpValues.at(idx)));
                _fegpCdtMap.insert("magnification",fegpValues.at(idx));
                ui->fegp_magnificationChkBox->setChecked(true);
            }
            else if(field == "imageacquisitiontype")
            {
                ui->fegp_imageacquisitiontypeCbBox->setCurrentIndex(ui->fegp_imageacquisitiontypeCbBox->findText(fegpValues.at(idx)));
                _fegpCdtMap.insert("imageacquisitiontype",fegpValues.at(idx));
                ui->fegp_imageacquisitiontypeChkBox->setChecked(true);
            }
            else if(field == "imageacquisitionstuff")
            {
                ui->fegp_imageacquisitionstuffLineEdit->setText(fegpValues.at(idx));
                _fegpCdtMap.insert("imageacquisitionstuff",fegpValues.at(idx));
                ui->fegp_imageacquisitionstuffChkBox->setChecked(true);
            }
            else if(field == "imagerecognitioninfoanalysis")
            {
                ui->fegp_imagerecognitioninfoanalysisLineEdit->setText(fegpValues.at(idx));
                _fegpCdtMap.insert("imagerecognitioninfoanalysis",fegpValues.at(idx));
                ui->fegp_imagerecognitioninfoanalysisChkBox->setChecked(true);
            }
            idx++;
        }
    }
    
    QStringList oisFields = query.value(12).toString().split("#");
    QStringList oisValues = query.value(13).toString().split("#");
    if(!oisFields.isEmpty())
    {
        idx = 0;
        foreach (QString field, oisFields)
        {
            if(field == "oilsampleid")
            {
                ui->oilsampleidCbBox->setCurrentIndex(ui->oilsampleidCbBox->findText(oisValues.at(idx)));
                _oisCdtMap.insert("oilsampleid",oisValues.at(idx));
                ui->oilsampleidChkBox->setChecked(true);
            }
            else if(field == "samplestuff")
            {
                ui->samplestuffLineEdit->setText(oisValues.at(idx));
                _oisCdtMap.insert("samplestuff",oisValues.at(idx));
                ui->samplestuffChkBox->setChecked(true);
            }
            else if(field == "sendstuff")
            {
                ui->sendstuffLineEdit->setText(oisValues.at(idx));
                _oisCdtMap.insert("sendstuff",oisValues.at(idx));
                ui->sendstuffChkBox->setChecked(true);
            }
            else if(field == "sampledate")
            {
                QString datestr = oisValues.at(idx);
                QStringList datelist = datestr.split("-");
                int y = datelist.at(0).toInt();
                int m = datelist.at(1).toInt();
                int d = datelist.at(2).toInt();
                ui->sampledateDateEdit->setDate(QDate(y,m,d));
                _oisCdtMap.insert("sampledate",oisValues.at(idx));
                ui->sampledateChkBox->setChecked(true);
            }
            else if(field == "senddate")
            {
                QString datestr = oisValues.at(idx);
                QStringList datelist = datestr.split("-");
                int y = datelist.at(0).toInt();
                int m = datelist.at(1).toInt();
                int d = datelist.at(2).toInt();
                ui->senddateDateEdit->setDate(QDate(y,m,d));
                _oisCdtMap.insert("senddate",oisValues.at(idx));
                ui->senddateChkBox->setChecked(true);
            }
            else if(field == "sampletime")
            {
                QString timestr = oisValues.at(idx);
                QStringList timelist = timestr.split(":");
                int h = timelist.at(0).toInt();
                int m = timelist.at(1).toInt();
                QStringList seclist = timelist.at(2).split(" ");
                int s = seclist.at(0).toInt();
                ui->sampletimeTimeEdit->setTime(QTime(h,m));
                _oisCdtMap.insert("sampletime",oisValues.at(idx));
                ui->sampletimeChkBox->setChecked(true);
            }
            else if(field == "sendtime")
            {
                QString timestr = oisValues.at(idx);
                QStringList timelist = timestr.split(":");
                int h = timelist.at(0).toInt();
                int m = timelist.at(1).toInt();
                QStringList seclist = timelist.at(2).split(" ");
                int s = seclist.at(0).toInt();
                ui->sendtimeTimeEdit->setTime(QTime(h,m));;
                _oisCdtMap.insert("sendtime",oisValues.at(idx));
                ui->sendtimeChkBox->setChecked(true);
            }
            else if(field == "samplesituation")
            {
                ui->samplesituationCbBox->setCurrentIndex(ui->samplesituationCbBox->findText(oisValues.at(idx)));
                _oisCdtMap.insert("samplesituation",oisValues.at(idx));
                ui->samplesituationChkBox->setChecked(true);
            }
            else if(field == "samplemethod")
            {
                ui->samplemethodCbBox->setCurrentIndex(ui->samplemethodCbBox->findText(oisValues.at(idx)));
                _oisCdtMap.insert("samplemethod",oisValues.at(idx));
                ui->samplemethodChkBox->setChecked(true);
            }
            else if(field == "sampleid")
            {
                ui->sampleidCbBox->setCurrentIndex(ui->sampleidCbBox->findText(oisValues.at(idx)));
                _oisCdtMap.insert("sampleid",oisValues.at(idx));
                ui->sampleidChkBox->setChecked(true);
            }
            else if(field == "sampledepartid")
            {
                ui->sampledepartidCbBox->setCurrentIndex(ui->sampledepartidCbBox->findText(oisValues.at(idx)));
                _oisCdtMap.insert("sampledepartid",oisValues.at(idx));
                ui->sampledepartidChkBox->setChecked(true);
            }
            else if(field == "sampledepartname")
            {
                ui->sampledepartnameCbBox->setCurrentIndex(ui->sampledepartnameCbBox->findText(oisValues.at(idx)));
                _oisCdtMap.insert("sampledepartname",oisValues.at(idx));
                ui->sampledepartnameChkBox->setChecked(true);
            }
            else if(field == "monitorpartid")
            {
                ui->monitorpartidCbBox->setCurrentIndex(ui->monitorpartidCbBox->findText(oisValues.at(idx)));
                _oisCdtMap.insert("monitorpartid",oisValues.at(idx));
                ui->monitorpartidChkBox->setChecked(true);
            }
            else if(field == "monitorpartname")
            {
                ui->monitorpartnameCbBox->setCurrentIndex(ui->monitorpartnameCbBox->findText(oisValues.at(idx)));
                _oisCdtMap.insert("monitorpartname",oisValues.at(idx));
                ui->monitorpartnameChkBox->setChecked(true);
            }
            else if(field == "oilworktime")
            {
                ui->oilworktimeLineEdit->setText(oisValues.at(idx));
                _oisCdtMap.insert("oilworktime",oisValues.at(idx));
                ui->oilworktimeChkBox->setChecked(true);
            }
            else if(field == "oiladdition")
            {
                ui->oiladditionLineEdit->setText(oisValues.at(idx));
                _oisCdtMap.insert("oiladdition",oisValues.at(idx));
                ui->oiladditionChkBox->setChecked(true);
            }
            idx ++;
        }
    }
    
    QStringList oiaFields = query.value(14).toString().split("#");
    QStringList oiaValues = query.value(15).toString().split("#");
    if(!oiaFields.isEmpty())
    {
        idx = 0;
        foreach (QString field, oiaFields)
        {
            if(field == "sendstuff")
            {
                ui->oia_sendstuffLineEdit->setText(oiaValues.at(idx));
                _oiaCdtMap.insert("sendstuff",oiaValues.at(idx));
                ui->oia_sendstuffChkBox->setChecked(true);
            }
            else if(field == "senddepart")
            {
                ui->oia_senddepartCbBox->setCurrentIndex(ui->oia_senddepartCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("senddepart",oiaValues.at(idx));
                ui->oia_senddepartChkBox->setChecked(true);
            }
            else if(field == "receivestuff")
            {
                ui->oia_receivestuffLineEdit->setText(oiaValues.at(idx));
                _oiaCdtMap.insert("receivestuff",oiaValues.at(idx));
                ui->oia_receivestuffChkBox->setChecked(true);
            }
            else if(field == "receivedate")
            {
                QString datestr = oiaValues.at(idx);
                QStringList datelist = datestr.split("-");
                int y = datelist.at(0).toInt();
                int m = datelist.at(1).toInt();
                int d = datelist.at(2).toInt();
                ui->oia_receivedateDateEdit->setDate(QDate(y,m,d));
                _oiaCdtMap.insert("receivedate",oiaValues.at(idx));
                ui->oia_receivedateChkBox->setChecked(true);
            }
            else if(field == "analyzedepartname")
            {
                ui->oia_analyzedepartnameCbBox->setCurrentIndex(ui->oia_analyzedepartnameCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("analyzedepartname",oiaValues.at(idx));
                ui->oia_analyzedepartnameChkBox->setChecked(true);
            }
            else if(field == "contaminationanalyzestuff")
            {
                ui->oia_contaminationanalyzestuffChkLineEdit->setText(oiaValues.at(idx));
                _oiaCdtMap.insert("contaminationanalyzestuff",oiaValues.at(idx));
                ui->oia_contaminationanalyzestuffChkBox->setChecked(true);
            }
            else if(field == "contaminationanalyzemethod")
            {
                ui->oia_contaminationanalyzemethodCbBox->setCurrentIndex(ui->oia_contaminationanalyzemethodCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("contaminationanalyzemethod",oiaValues.at(idx));
                ui->oia_contaminationanalyzestuffChkBox->setChecked(true);
            }
            else if(field == "contaminationanalyzeequipment")
            {
                ui->oia_contaminationanalyzeequipmentCbBox->setCurrentIndex(ui->oia_contaminationanalyzeequipmentCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("contaminationanalyzeequipment",oiaValues.at(idx));
                ui->oia_contaminationanalyzeequipmentChkBox->setChecked(true);
            }
            else if(field == "contaminationanalyzedate")
            {
                QString datestr = oiaValues.at(idx);
                QStringList datelist = datestr.split("-");
                int y = datelist.at(0).toInt();
                int m = datelist.at(1).toInt();
                int d = datelist.at(2).toInt();
                ui->oia_contaminationanalyzedateDateEdit->setDate(QDate(y,m,d));
                _oiaCdtMap.insert("contaminationanalyzedate",oiaValues.at(idx));
                ui->oia_contaminationanalyzedateChkBox->setChecked(true);
            }
            else if(field == "contaminationanalyzereportid")
            {
                ui->oia_contaminationanalyzereportidCbBox->setCurrentIndex(ui->oia_contaminationanalyzereportidCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("contaminationanalyzereportid",oiaValues.at(idx));
                ui->oia_contaminationanalyzereportidChkBox->setChecked(true);
            }
            else if(field == "spectroscopystuff")
            {
                ui->oia_spectroscopystuffLineEdit->setText(oiaValues.at(idx));
                _oiaCdtMap.insert("spectroscopystuff",oiaValues.at(idx));
                ui->oia_spectroscopystuffChkBox->setChecked(true);
            }
            else if(field == "spectroscopymethod")
            {
                ui->oia_spectroscopymethodCbBo->setCurrentIndex(ui->oia_spectroscopymethodCbBo->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("spectroscopymethod",oiaValues.at(idx));
                ui->oia_spectroscopymethodChkBox->setChecked(true);
            }
            else if(field == "spectroscopyequipment")
            {
                ui->oia_spectroscopyequipmentCbBox->setCurrentIndex(ui->oia_spectroscopyequipmentCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("spectroscopyequipment",oiaValues.at(idx));
                ui->oia_spectroscopyequipmentChkBox->setChecked(true);
            }
            else if(field == "spectroscopydate")
            {
                QString datestr = oiaValues.at(idx);
                QStringList datelist = datestr.split("-");
                int y = datelist.at(0).toInt();
                int m = datelist.at(1).toInt();
                int d = datelist.at(2).toInt();
                ui->oia_spectroscopydateDateEdit->setDate(QDate(y,m,d));
                _oiaCdtMap.insert("spectroscopydate",oiaValues.at(idx));
                ui->oia_spectroscopydateChkBox->setChecked(true);
            }
            else if(field == "spectroscopyreportid")
            {
                ui->oia_spectroscopyreportidCbBox->setCurrentIndex(ui->oia_spectroscopyreportidCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("spectroscopyreportid",oiaValues.at(idx));
                ui->oia_spectroscopyreportidChkBox->setChecked(true);
            }
            else if(field == "ferrographystuff")
            {
                ui->oia_ferrographystuffLineEdit->setText(oiaValues.at(idx));
                _oiaCdtMap.insert("ferrographystuff",oiaValues.at(idx));
                ui->oia_ferrographystuffChkBox->setChecked(true);
            }
            else if(field == "ferrographymethod")
            {
                ui->oia_ferrographymethodCbBox->setCurrentIndex(ui->oia_ferrographymethodCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("ferrographymethod",oiaValues.at(idx));
                ui->oia_ferrographymethodChkBox->setChecked(true);
            }
            else if(field == "ferrographyequipment")
            {
                ui->oia_ferrographyequipmentCbBox->setCurrentIndex(ui->oia_ferrographyequipmentCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("ferrographyequipment",oiaValues.at(idx));
                ui->oia_ferrographyequipmentChkBox->setChecked(true);
            }
            else if(field == "ferrographydate")
            {
                QString datestr = oiaValues.at(idx);
                QStringList datelist = datestr.split("-");
                int y = datelist.at(0).toInt();
                int m = datelist.at(1).toInt();
                int d = datelist.at(2).toInt();
                ui->oia_ferrographydateDateEdit->setDate(QDate(y,m,d));
                _oiaCdtMap.insert("ferrographydate",oiaValues.at(idx));
                ui->oia_ferrographydateChkBox->setChecked(true);
            }
            else if(field == "ferrographyreportid")
            {
                ui->oia_ferrographyreportidCbBox->setCurrentIndex(ui->oia_ferrographyreportidCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("ferrographyreportid",oiaValues.at(idx));
                ui->oia_ferrographyreportidChkBox->setChecked(true);
            }
            else if(field == "physicochemicalstuff")
            {
                ui->oia_physicochemicalstuffLineEdit->setText(oiaValues.at(idx));
                _oiaCdtMap.insert("physicochemicalstuff",oiaValues.at(idx));
                ui->oia_physicochemicalstuffChkBox->setChecked(true);
            }
            else if(field == "physicochemicalmethod")
            {
                ui->oia_physicochemicalmethodCbBox->setCurrentIndex(ui->oia_physicochemicalmethodCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("physicochemicalmethod",oiaValues.at(idx));
                ui->oia_physicochemicalmethodChkBox->setChecked(true);
            }
            else if(field == "physicochemicalequipment")
            {
                ui->oia_physicochemicalequipmentCbBox->setCurrentIndex(ui->oia_physicochemicalequipmentCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("physicochemicalequipment",oiaValues.at(idx));
                ui->oia_physicochemicalequipmentChkBox->setChecked(true);
            }
            else if(field == "physicochemicaldate")
            {
                QString datestr = oiaValues.at(idx);
                QStringList datelist = datestr.split("-");
                int y = datelist.at(0).toInt();
                int m = datelist.at(1).toInt();
                int d = datelist.at(2).toInt();
                ui->oia_physicochemicaldateDateEdit->setDate(QDate(y,m,d));
                _oiaCdtMap.insert("physicochemicaldate",oiaValues.at(idx));
                ui->oia_physicochemicaldateChkBox->setChecked(true);
            }
            else if(field == "physicochemicalreportid")
            {
                ui->oia_physicochemicalreportidCbBox->setCurrentIndex(ui->oia_physicochemicalreportidCbBox->findText(oiaValues.at(idx)));
                _oiaCdtMap.insert("physicochemicalreportid",oiaValues.at(idx));
                ui->oia_physicochemicalreportidChkBox->setChecked(true);
            }
            idx ++;
        }
    }
    
    QStringList abmFields = query.value(16).toString().split("#");
    QStringList abmValues = query.value(17).toString().split("#");
    if(!abmFields.isEmpty())
    {
        idx =0;
        foreach (QString field, abmFields)
        {
            if(field == "abrasiveid")
            {
                ui->abm_abrasiveidCbBox->setCurrentIndex(ui->abm_abrasiveidCbBox->findText(abmValues.at(idx)));
                _abmCdtMap.insert("abrasiveid",abmValues.at(idx));
                ui->abm_abrasiveidChkBox->setChecked(true);
            }
            else if(field == "abrasiveshape")
            {
                ui->abm_abrasiveshapeCbBox->setCurrentIndex(ui->abm_abrasiveshapeCbBox->findText(abmValues.at(idx)));
                _abmCdtMap.insert("abrasiveshape",abmValues.at(idx));
                ui->abm_abrasiveshapeChkBox->setChecked(true);
            }
            else if(field == "abrasiveposition")
            {
                ui->abm_abrasivepositionCbBox->setCurrentIndex(ui->abm_abrasivepositionCbBox->findText(abmValues.at(idx)));
                _abmCdtMap.insert("abrasiveposition",abmValues.at(idx));
                ui->abm_abrasivepositionChkBox->setChecked(true);
            }
            else if(field == "abrasivematerial")
            {
                ui->abm_abrasivematerialCbBox->setCurrentIndex(ui->abm_abrasivematerialCbBox->findText(abmValues.at(idx)));
                _abmCdtMap.insert("abrasivematerial",abmValues.at(idx));
                ui->abm_abrasivematerialChkBox->setChecked(true);
            }
            else if(field == "abrasivecolor")
            {
                ui->abm_abrasivecolorCbBox->setCurrentIndex(ui->abm_abrasivecolorCbBox->findText(abmValues.at(idx)));
                _abmCdtMap.insert("abrasivecolor",abmValues.at(idx));
                ui->abm_abrasivecolorChkBox->setChecked(true);
            }
            else if(field == "abrasivesperimeter")
            {
                ui->abm_abrasivesperimeterLineEdit->setText(abmValues.at(idx));
                _abmCdtMap.insert("abrasivesperimeter",abmValues.at(idx));
                ui->abm_abrasivesperimeterChkBox->setChecked(true);
            }
            else if(field == "abrasivesize")
            {
                ui->abm_abrasivesizeLineEdit->setText(abmValues.at(idx));
                _abmCdtMap.insert("abrasivesize",abmValues.at(idx));
                ui->abm_abrasivesizeChkBox->setChecked(true);
            }
            else if(field == "abrasivemarkstuff")
            {
                ui->abm_abrasivemarkstuffLineEdit->setText(abmValues.at(idx));
                _abmCdtMap.insert("abrasivemarkstuff",abmValues.at(idx));
                ui->abm_abrasivemarkstuffChkBox->setChecked(true);
            }
            else if(field == "abrasivetypical")
            {
                ui->abm_abrasivetypicalLineEdit->setText(abmValues.at(idx));
                _abmCdtMap.insert("abrasivetypical",abmValues.at(idx));
                ui->abm_abrasivetypicalChkBox->setChecked(true);
            }
            else if(field == "abrasivemechanismtype")
            {
                ui->abm_abrasivemechanismtypeLineEdit->setText(abmValues.at(idx));
                _abmCdtMap.insert("abrasivemechanismtype",abmValues.at(idx));
                ui->abm_abrasivemechanismtypeChkBox_->setChecked(true);
            }
            else if(field == "abrasivedamagetype")
            {
                ui->abm_abrasivedamagetypeCbBox->setCurrentIndex(ui->abm_abrasivedamagetypeCbBox->findText(abmValues.at(idx)));
                _abmCdtMap.insert("abrasivedamagetype",abmValues.at(idx));
                ui->abm_abrasivedamagetypeChkBox->setChecked(true);
            }
            else if(field == "abrasiveweartype")
            {
                ui->abm_abrasiveweartypeCbBox->setCurrentIndex(ui->abm_abrasiveweartypeCbBox->findText(abmValues.at(idx)));
                _abmCdtMap.insert("abrasiveweartype",abmValues.at(idx));
                ui->abm_abrasiveweartypeChkBox->setChecked(true);
            }
            else if(field == "abrasivesurfacetexturetype")
            {
                ui->abm_abrasivesurfacetexturetypeCbBox->setCurrentIndex(ui->abm_abrasivesurfacetexturetypeCbBox->findText(abmValues.at(idx)));
                _abmCdtMap.insert("abrasivesurfacetexturetype",abmValues.at(idx));
                ui->abm_abrasivesurfacetexturetypeChkBox->setChecked(true);
            }
            else if(field == "ferrographysheetid")
            {
                ui->abm_ferrographysheetidCbBox->setCurrentIndex(ui->abm_ferrographysheetidCbBox->findText(abmValues.at(idx)));
                _abmCdtMap.insert("ferrographysheetid",abmValues.at(idx));
                ui->abm_ferrographysheetidChkBox->setChecked(true);
            }
            else if(field == "ferrographypicid")
            {
                ui->abm_ferrographypicidCbBox->setCurrentIndex(ui->abm_ferrographypicidCbBox->findText(abmValues.at(idx)));
                _abmCdtMap.insert("ferrographypicid",abmValues.at(idx));
                ui->abm_ferrographypicidChkBox->setChecked(true);
            }
            else if(field == "ferrographyreportid")
            {
                ui->abm_ferrographyreportidCbBox->setCurrentIndex(ui->abm_ferrographypicidCbBox->findText(abmValues.at(idx)));
                _abmCdtMap.insert("ferrographyreportid",abmValues.at(idx));
                ui->abm_ferrographypicidChkBox->setChecked(true);
            }
            idx++;
        }
    }
}

void AdvanceSearchDlg::on_modifyButton_clicked()
{

    QModelIndex index = ui->propertylistTableView->currentIndex();
    QSqlRecord record = propertymodel->record(index.row());
    QString propertyname = record.value(0).toString();
    
    QString uid;
    QSqlQuery *query = new QSqlQuery;
    QString uidsql = "select uid from propertyinfo where propertyname = '";
    uidsql.append(propertyname);
    uidsql.append("'");
    query->exec(uidsql);
    if(query->next())
        uid = query->value(0).toString();
    
    ppnDlg = new ProPertyNameDlg(this,propertyname);
    if(ppnDlg->exec()== QDialog::Accepted)
    {
        
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, tr("QMessageBox::question()"), tr("确认修改当前属性?"),
                                          QMessageBox::Yes | QMessageBox::Cancel);
        
            if(reply == QMessageBox::Yes)
            {
                QString eqmFields = "";
                QString eqmValues = "";
                if(!_eqmCdtMap.isEmpty())
                {
                    QMap<QString,QString>::iterator it;
                    it = _eqmCdtMap.begin();
                    eqmFields.append(it.key());
                    eqmValues.append(it.value());
                    ++it;
                    for(;it != _eqmCdtMap.end();++it)
                    {
                        eqmFields.append("#");
                        eqmFields.append(it.key());
                        eqmValues.append("#");
                        eqmValues.append(it.value());
                    }
                }
                
                QString mpFields = "";
                QString mpValues = "";
                if(!_mpCdtMap.isEmpty())
                {
                    QMap<QString,QString>::iterator it;
                    it = _mpCdtMap.begin();
                    mpFields.append(it.key());
                    mpValues.append(it.value());
                    ++it;
                    for(;it !=_mpCdtMap.end();++it)
                    {
                        mpFields.append("#");
                        mpValues.append("#");
                        mpFields.append(it.key());
                        mpValues.append(it.value());
                    }
                }
                
                QString mprFields = "";
                QString mprValues = "";
                if(!_mprCdtMap.isEmpty())
                {
                    QMap<QString,QString>::iterator it;
                    it = _mprCdtMap.begin();
                    mprFields.append(it.key());
                    mprValues.append(it.value());
                    ++it;
                    for(;it != _mprCdtMap.end();++it)
                    {
                        mprFields.append("#");
                        mprValues.append("#");
                        mprFields.append(it.key());
                        mprValues.append(it.value());
                    }
                }
                
                QString fegFields = "";
                QString fegValues = "";
                if(!_fegCdtMap.isEmpty())
                {
                    QMap<QString,QString>::iterator it;
                    it = _fegCdtMap.begin();
                    fegFields.append(it.key());
                    fegValues.append(it.value());
                    ++it;
                    for(;it!=_fegCdtMap.end();++it)
                    {
                        fegFields.append("#");
                        fegValues.append("#");
                        fegFields.append(it.key());
                        fegValues.append(it.value());
                    }
                }
                
                QString fegpFields = "";
                QString fegpValues = "";
                if(!_fegpCdtMap.isEmpty())
                {
                    QMap<QString,QString>::iterator it;
                    it = _fegpCdtMap.begin();
                    fegpFields.append(it.key());
                    fegpValues.append(it.value());
                    ++it;
                    for(;it != _fegpCdtMap.end();++it)
                    {
                        fegpFields.append("#");
                        fegpValues.append("#");
                        fegpFields.append(it.key());
                        fegpValues.append(it.value());
                    }
                }
                
                QString oisFields = "";
                QString oisValues = "";
                if(!_oisCdtMap.isEmpty())
                {
                    QMap<QString,QString>::iterator it;
                    it = _oisCdtMap.begin();
                    oisFields.append(it.key());
                    oisValues.append(it.value());
                    ++it;
                    for(;it!=_oisCdtMap.end();++it)
                    {
                        oisFields.append("#");
                        oisValues.append("#");
                        fegpFields.append(it.key());
                        fegpValues.append(it.value());
                    }
                }
                
                QString oiaFields = "";
                QString oiaValues = "";
                if(!_oiaCdtMap.isEmpty())
                {
                    QMap<QString,QString>::iterator it;
                    it = _oiaCdtMap.begin();
                    oiaFields.append(it.key());
                    oiaValues.append(it.value());
                    ++it;
                    for(;it!=_oiaCdtMap.end();++it)
                    {
                        oiaFields.append("#");
                        oisValues.append("#");
                        fegpFields.append(it.key());
                        fegpValues.append(it.value());
                    }
                }
                
                QString abmFields = "";
                QString abmValues = "";
                if(!_abmCdtMap.isEmpty())
                {
                    QMap<QString,QString>::iterator it;
                    it = _abmCdtMap.begin();
                    abmFields.append(it.key());
                    abmValues.append(it.value());
                    ++it;
                    for(;it != _abmCdtMap.end();++it)
                    {
                        abmFields.append("#");
                        abmValues.append("#");
                        abmFields.append(it.key());
                        abmValues.append(it.value());
                    }
                }
        
                QString propertySql = "update propertyinfo set propertyname = '";
                propertySql.append(propertyName);
                propertySql.append("', eqmfields = '");
                propertySql.append(eqmFields);
                propertySql.append("', eqmvalues = '");
                propertySql.append(eqmValues);
                propertySql.append("', mpfields = '");
                propertySql.append(mpFields);
                propertySql.append("', mpvalues = '");
                propertySql.append(mpValues);
                propertySql.append("', mprfields = '");
                propertySql.append(mprFields);
                propertySql.append("', mprvalues = '");
                propertySql.append(mprValues);
                propertySql.append("', fegfields = '");
                propertySql.append(fegFields);
                propertySql.append("', fegvalues = '");
                propertySql.append(fegValues);
                propertySql.append("', fegpfields = '");
                propertySql.append(fegpFields);
                propertySql.append("', fegpvalues = '");
                propertySql.append(fegpValues);
                propertySql.append("', oisfields = '");
                propertySql.append(oisFields);
                propertySql.append("', oisvalues = '");
                propertySql.append(oisValues);
                propertySql.append("', oiafields = '");
                propertySql.append(oiaFields);
                propertySql.append("', oiavalues = '");
                propertySql.append(oiaValues);
                propertySql.append("', abmfields = '");
                propertySql.append(abmFields);
                propertySql.append("', abmvalues = '");
                propertySql.append(abmValues);
                propertySql.append("' where uid = ");
                propertySql.append(uid);

                if(query->exec(propertySql))
                {
                    initpropertylistName();
                    QMessageBox::warning(this,tr("提示"),tr("更新查询属性成功"),QMessageBox::Close);
                }
                else
                    QMessageBox::warning(this,tr("提示"),tr("更新查询属性失败"),QMessageBox::Close);
            }
            else
                this->propertyName = "";
    }
    else
        ;
    
    ui->queryBtn->setEnabled(true);
    ui->modifyButton->setEnabled(false);
}
