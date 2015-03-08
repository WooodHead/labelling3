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
    createListWidget();
    
    connect(ui->tableListWidget,SIGNAL(currentRowChanged(int)),ui->conditionStackedWidget,SLOT(setCurrentIndex(int)));
    
    createTableNames();
    
    createTableView();
    
    initCbBox();
    
    query();
   
}

AdvanceSearchDlg::~AdvanceSearchDlg()
{
    delete ui;
    db.close();
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
            qDebug()<<fileInfo.fileName();

        // 当为目录时，递归的进行copy
        if(fileInfo.isDir())
        {
            if(!copyFiles(fileInfo.filePath(),
                          targetDir.filePath(fileInfo.fileName()),
                          convertIfExits))
                return false;
        }
        else
        {   //当允许覆盖操作时，将旧文件进行删除操作
            if(convertIfExits && targetDir.exists(fileInfo.fileName()))
            {
                targetDir.remove(fileInfo.fileName());
            }
            // 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                            targetDir.filePath(fileInfo.fileName()))){
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
//    ui->topWidget->setMaximumHeight(300);
//    ui->tableListWidget->setMaximumHeight(250);
//    ui->tableListWidget->setMaximumWidth(150);
//    ui->conditionStackedWidget->setMaximumHeight(250);
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
}



void AdvanceSearchDlg::on_PlaneIdChkBox_clicked()
{
    _eqmCdtMap.remove("planeid");
    if(ui->PlaneIdChkBox->isChecked())
    {   
        QString text = ui->planeidCbBox->currentText();
        _eqmCdtMap.insert("planeid",text);
    }
}


void AdvanceSearchDlg::on_planeidCbBox_currentIndexChanged(int index)
{
    _eqmCdtMap.remove("planeid");
    if(ui->PlaneIdChkBox->isChecked())
    {   
        QString text = ui->planeidCbBox->currentText();
        _eqmCdtMap.insert("planeid",text);
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
        qDebug()<<"Accept";
        qDebug()<<this->propertyName;
        
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
            QMessageBox::warning(this,tr("提示"),tr("保存查询属性成功"),QMessageBox::Close);
        else
            QMessageBox::warning(this,tr("提示"),tr("保存查询属性失败"),QMessageBox::Close);
    }
    else
        qDebug()<<"Reject";
}


void AdvanceSearchDlg::on_movepartIdChkBox_clicked()
{
    _mpCdtMap.remove("movepartid");
    if(ui->movepartIdChkBox->isChecked())
    {
        QString text = ui->movepartIdCbBox->currentText();
        _mpCdtMap.insert("movepartid",text);
    }
}


void AdvanceSearchDlg::on_movepartIdCbBox_currentIndexChanged(int index)
{
    _mpCdtMap.remove("movepartid");
    if(ui->movepartIdChkBox->isChecked())
    {
        QString text = ui->movepartIdCbBox->currentText();
        _mpCdtMap.insert("movepartid",text);
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
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("导入数据"),
                                                    tr(""),
                                                    tr("SqlFile(*.sql)"));
    if(filename.isEmpty())
        return;
    if(this->importDB(filename))
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
