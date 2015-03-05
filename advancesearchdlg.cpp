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
    
    createTableNames();
    
    createTableView();
    
    query();
   
}

AdvanceSearchDlg::~AdvanceSearchDlg()
{
    delete ui;
    db.close();
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
        int fieldIndex  = 0;
        QString fieldName = conditionField.at(fieldIndex);
        QString fieldValue;
        QMap<QString,QString>::iterator it;
        for(it = conditionMap.begin();it != conditionMap.end()-1;++it)
        {
            fieldName = it.key();
            fieldValue = it.value();
            sql.append(fieldName);
            sql.append("=");
            sql.append(fieldValue);
            sql.append(" and ");
        }
        fieldName = it.key();
        fieldValue = it.value();
        sql.append(fieldName);
        sql.append("=");
        sql.append(fieldValue);
        return sql;
    }
}

void AdvanceSearchDlg::query()
{
    // 装备信息表
    QString eqmTableName = tableNames.value("EqmInfo");
    QString eqmSql = generateSql(_equipmentinfoCdtMap,_equipmentinfoCdtField,eqmTableName);
    _eqmInfoModel->setQuery(eqmSql);
    setModelHeaderData("EqmInfo");
    qDebug()<<eqmSql;
    
    // 动部件信息表
    QString mpTableName = tableNames.value("MpInfo");
    QString mpSql = generateSql(_movepartinfoCdtMap,_movepartinfoCdtField,mpTableName);
    _mpInfoModel->setQuery(mpSql);
    setModelHeaderData("MpInfo");
    qDebug()<<mpSql;
    
    // 动部件维修信息表
    QString mprTableName = tableNames.value("MprInfo");
    QString mprSql = generateSql(_movepartrepairinfoCdtMap,_movepartrepairinfoCdtField,mprTableName);
    _mprInfoModel->setQuery(mprSql);
    setModelHeaderData("MprInfo");
    qDebug()<<mprSql;
    
    // 油样采集信息表
    QString oisTableName = tableNames.value("OisInfo");
    QString oisSql = generateSql(_oilsampleinfoCdtMap,_oilsampleinfoCdtField,oisTableName);
    _oisInfoModel->setQuery(oisSql);
    setModelHeaderData("OisInfo");
    qDebug()<<oisSql;
    
    // 油样检测分析信息表
    QString oiaTableName = tableNames.value("OiaInfo");
    QString oiaSql = generateSql(_oilanalyzeinfoCdtMap,_oilanalyzeinfoCdtField,oiaTableName);
    _oiaInfoModel->setQuery(oiaSql);
    setModelHeaderData("OiaInfo");
    qDebug()<<oiaSql;
    
    // 铁谱质谱信息表
    QString fegTableName = tableNames.value("FegInfo");
    QString fegSql = generateSql(_ferrographyinfoCdtMap,_ferrographyinfoCdtField,fegTableName);
    _fegInfoModel->setQuery(fegSql);
    setModelHeaderData("FegInfo");
    qDebug()<<fegSql;
    
    // 铁谱图片采集信息表
    QString fegpTableName = tableNames.value("FegPInfo");
    QString fegpSql = generateSql(_ferrographypicinfoCdtMap,_ferrographypicinfoCdtField,fegpTableName);
    _fegpInfoModel->setQuery(fegpSql);
    setModelHeaderData("FegPInfo");
    qDebug()<<fegpSql;
    
    // 磨粒标注信息表
    QString abmTableName = tableNames.value("AbmInfo");
    QString abmSql = generateSql(_abrasivemarkinfoCdtMap,_abrasivemarkinfoCdtField,abmTableName);
    _abmInfoModel->setQuery(abmSql);
    setModelHeaderData("AbmInfo");
    qDebug()<<abmSql;
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
    // 装备信息表
    _eqmInfoModel = new QSqlQueryModel;
    ui->eqmTableView->setModel(_eqmInfoModel);
    ui->eqmTableView->verticalHeader()->setVisible(false);
    ui->eqmTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->eqmTableView->setSelectionMode(QTableView::SingleSelection);
    ui->eqmTableView->setAlternatingRowColors(true);
    
    // 动部件信息表
    _mpInfoModel = new QSqlQueryModel;
    ui->mpTableView->setModel(_mpInfoModel);
    ui->mpTableView->verticalHeader()->setVisible(false);
    ui->mpTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->mpTableView->setSelectionMode(QTableView::SingleSelection);
    ui->mpTableView->setAlternatingRowColors(true);
    
    // 动部件维修信息表
    _mprInfoModel = new QSqlQueryModel;
    ui->mprTableView->setModel(_mprInfoModel);
    ui->mprTableView->verticalHeader()->setVisible(false);;
    ui->mprTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->mprTableView->setSelectionMode(QTableView::SingleSelection);
    ui->mprTableView->setAlternatingRowColors(true);
    
    // 油样采集信息表
    _oisInfoModel = new QSqlQueryModel;
    ui->oisTableView->setModel(_oisInfoModel);
    ui->oisTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->oisTableView->setSelectionMode(QTableView::SingleSelection);
    ui->oisTableView->setAlternatingRowColors(true);
    ui->oisTableView->verticalHeader()->setVisible(false);
    
    // 油样检测分析信息表
    _oiaInfoModel = new QSqlQueryModel;
    ui->oiaTableView->setModel(_oiaInfoModel);
    ui->oiaTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->oiaTableView->setSelectionMode(QTableView::SingleSelection);
    ui->oiaTableView->verticalHeader()->setVisible(false);
    ui->oiaTableView->setAlternatingRowColors(true);
    
    // 铁谱质谱信息表
    _fegInfoModel = new QSqlQueryModel;
    ui->fegTableView->setModel(_fegInfoModel);
    ui->fegTableView->setSelectionBehavior(QTableView::SelectRows);;
    ui->fegTableView->setSelectionMode(QTableView::SingleSelection);
    ui->fegTableView->verticalHeader()->setVisible(false);
    ui->fegTableView->setAlternatingRowColors(true);
    
    // 铁谱图片采集信息表
    _fegpInfoModel = new QSqlQueryModel;
    ui->fegpTableView->setModel(_fegpInfoModel);
    ui->fegpTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->fegpTableView->setSelectionMode(QTableView::SingleSelection);
    ui->fegpTableView->setAlternatingRowColors(true);
    ui->fegpTableView->verticalHeader()->setVisible(false);
    
    // 磨粒标注信息表
    _abmInfoModel = new QSqlQueryModel;
    ui->abmTableView->setModel(_abmInfoModel);
    ui->abmTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->abmTableView->setSelectionMode(QTableView::SingleSelection);
    ui->abmTableView->verticalHeader()->setVisible(false);
    ui->abmTableView->setAlternatingRowColors(true);
}

void AdvanceSearchDlg::setModelHeaderData(QString tablename)
{
    if(tablename == "EqmInfo")
    {
        _eqmInfoModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
        _eqmInfoModel->setHeaderData(1,Qt::Horizontal,tr("机号"));
        _eqmInfoModel->setHeaderData(2,Qt::Horizontal,tr("机型"));
        _eqmInfoModel->setHeaderData(3,Qt::Horizontal,tr("单位编号"));
        _eqmInfoModel->setHeaderData(4,Qt::Horizontal,tr("飞行小时数"));
        _eqmInfoModel->setHeaderData(5,Qt::Horizontal,tr("运行阶段"));
        _eqmInfoModel->setHeaderData(6,Qt::Horizontal,tr("维修次数"));
    }
    else if(tablename == "MpInfo")
    {
        _mpInfoModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
        _mpInfoModel->setHeaderData(1,Qt::Horizontal,tr("动部件编号"));
        _mpInfoModel->setHeaderData(2,Qt::Horizontal,tr("动部件类型"));
        _mpInfoModel->setHeaderData(3,Qt::Horizontal,tr("动部件名称"));
        _mpInfoModel->setHeaderData(4,Qt::Horizontal,tr("运行时数"));
        _mpInfoModel->setHeaderData(5,Qt::Horizontal,tr("运行阶段"));
        _mpInfoModel->setHeaderData(6,Qt::Horizontal,tr("机号"));
        _mpInfoModel->setHeaderData(7,Qt::Horizontal,tr("机型"));
        _mpInfoModel->setHeaderData(8,Qt::Horizontal,tr("动部件开始日期"));
        _mpInfoModel->setHeaderData(9,Qt::Horizontal,tr("动部件终止日期"));
    }
    else if(tablename == "MprInfo")
    {
        _mprInfoModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
        _mprInfoModel->setHeaderData(1,Qt::Horizontal,tr("动部件维修编号"));
        _mprInfoModel->setHeaderData(2,Qt::Horizontal,tr("动部件编号"));
        _mprInfoModel->setHeaderData(3,Qt::Horizontal,tr("动部件类型"));
        _mprInfoModel->setHeaderData(4,Qt::Horizontal,tr("维修原因"));
        _mprInfoModel->setHeaderData(5,Qt::Horizontal,tr("维修日期"));
        _mprInfoModel->setHeaderData(6,Qt::Horizontal,tr("维修次数"));
        _mprInfoModel->setHeaderData(7,Qt::Horizontal,tr("维修执行单位"));
        _mprInfoModel->setHeaderData(8,Qt::Horizontal,tr("维修内容"));
        _mprInfoModel->setHeaderData(9,Qt::Horizontal,tr("维修换件情况"));
    }
    else if(tablename == "AbmInfo")
    {
        _abmInfoModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
        _abmInfoModel->setHeaderData(1,Qt::Horizontal,tr("磨粒编号"));
        _abmInfoModel->setHeaderData(2,Qt::Horizontal,tr("铁谱图片编号"));
        _abmInfoModel->setHeaderData(3,Qt::Horizontal,tr("铁谱片编号"));
        _abmInfoModel->setHeaderData(4,Qt::Horizontal,tr("铁谱分析报告编号"));
        _abmInfoModel->setHeaderData(5,Qt::Horizontal,tr("磨粒标注人"));
        _abmInfoModel->setHeaderData(6,Qt::Horizontal,tr("磨粒图片路径"));
        _abmInfoModel->setHeaderData(7,Qt::Horizontal,tr("磨粒材质"));
        _abmInfoModel->setHeaderData(8,Qt::Horizontal,tr("磨粒位置"));
        _abmInfoModel->setHeaderData(9,Qt::Horizontal,tr("磨粒尺寸"));
        _abmInfoModel->setHeaderData(10,Qt::Horizontal,tr("磨粒周长"));
        _abmInfoModel->setHeaderData(11,Qt::Horizontal,tr("磨粒形状"));
        _abmInfoModel->setHeaderData(12,Qt::Horizontal,tr("磨粒颜色"));
        _abmInfoModel->setHeaderData(13,Qt::Horizontal,tr("磨粒表面纹理颜色"));
        _abmInfoModel->setHeaderData(14,Qt::Horizontal,tr("磨粒磨损类型"));
        _abmInfoModel->setHeaderData(15,Qt::Horizontal,tr("磨粒损伤类型"));
        _abmInfoModel->setHeaderData(16,Qt::Horizontal,tr("磨粒磨损机理"));
        _abmInfoModel->setHeaderData(17,Qt::Horizontal,tr("磨粒反映故障信息"));
        _abmInfoModel->setHeaderData(18,Qt::Horizontal,tr("磨粒典型性"));
    }
    else if(tablename == "FegInfo")
    {
        _fegInfoModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
        _fegInfoModel->setHeaderData(1,Qt::Horizontal,tr("铁谱片编号"));
        _fegInfoModel->setHeaderData(2,Qt::Horizontal,tr("铁谱分析报告编号"));
        _fegInfoModel->setHeaderData(3,Qt::Horizontal,tr("油样编号"));
        _fegInfoModel->setHeaderData(4,Qt::Horizontal,tr("分析铁谱仪型号"));
        _fegInfoModel->setHeaderData(5,Qt::Horizontal,tr("铁谱片制取油样消耗量"));
        _fegInfoModel->setHeaderData(6,Qt::Horizontal,tr("铁谱片制取方法"));
        _fegInfoModel->setHeaderData(7,Qt::Horizontal,tr("铁谱片制取人"));
    }
    else if(tablename == "FegPInfo")
    {
        _fegpInfoModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
        _fegpInfoModel->setHeaderData(1,Qt::Horizontal,tr("铁谱图片编号"));
        _fegpInfoModel->setHeaderData(2,Qt::Horizontal,tr("铁谱片编号"));
        _fegpInfoModel->setHeaderData(3,Qt::Horizontal,tr("铁谱分析报告编号"));
        _fegpInfoModel->setHeaderData(4,Qt::Horizontal,tr("显微镜型号"));
        _fegpInfoModel->setHeaderData(5,Qt::Horizontal,tr("图像采集器型号"));
        _fegpInfoModel->setHeaderData(6,Qt::Horizontal,tr("光源类型"));
        _fegpInfoModel->setHeaderData(7,Qt::Horizontal,tr("放大倍数"));
        _fegpInfoModel->setHeaderData(8,Qt::Horizontal,tr("铁谱图片采集区域"));
        _fegpInfoModel->setHeaderData(9,Qt::Horizontal,tr("铁谱图片采集人"));
        _fegpInfoModel->setHeaderData(10,Qt::Horizontal,tr("铁谱图片路径"));
        _fegpInfoModel->setHeaderData(11,Qt::Horizontal,tr("铁谱图片识别分析信息"));
        _fegpInfoModel->setHeaderData(11,Qt::Horizontal,tr("铁谱图片标识符号"));
        
    }
    else if(tablename == "OiaInfo")
    {
        _oiaInfoModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
        _oiaInfoModel->setHeaderData(1,Qt::Horizontal,tr("油样编号"));
        _oiaInfoModel->setHeaderData(2,Qt::Horizontal,tr("检测分析单位名称"));
        _oiaInfoModel->setHeaderData(3,Qt::Horizontal,tr("送检单位"));
        _oiaInfoModel->setHeaderData(4,Qt::Horizontal,tr("送检原因"));
        _oiaInfoModel->setHeaderData(5,Qt::Horizontal,tr("送检人"));
        _oiaInfoModel->setHeaderData(6,Qt::Horizontal,tr("收油样日期"));
        _oiaInfoModel->setHeaderData(7,Qt::Horizontal,tr("收油样人"));
        _oiaInfoModel->setHeaderData(8,Qt::Horizontal,tr("污染度分析方法"));
        _oiaInfoModel->setHeaderData(9,Qt::Horizontal,tr("污染度分析人"));
        _oiaInfoModel->setHeaderData(10,Qt::Horizontal,tr("污染度分析日期"));
        _oiaInfoModel->setHeaderData(11,Qt::Horizontal,tr("污染度分析设备"));
        _oiaInfoModel->setHeaderData(12,Qt::Horizontal,tr("污染度分析报告编号"));
        _oiaInfoModel->setHeaderData(13,Qt::Horizontal,tr("光谱分析方法"));
        _oiaInfoModel->setHeaderData(14,Qt::Horizontal,tr("光谱分析人"));
        _oiaInfoModel->setHeaderData(15,Qt::Horizontal,tr("光谱分析日期"));
        _oiaInfoModel->setHeaderData(16,Qt::Horizontal,tr("光谱分析设备"));
        _oiaInfoModel->setHeaderData(17,Qt::Horizontal,tr("光谱分析报告编号"));
        _oiaInfoModel->setHeaderData(18,Qt::Horizontal,tr("铁谱分析方法"));
        _oiaInfoModel->setHeaderData(19,Qt::Horizontal,tr("铁谱分析人"));
        _oiaInfoModel->setHeaderData(20,Qt::Horizontal,tr("铁谱分析日期"));
        _oiaInfoModel->setHeaderData(21,Qt::Horizontal,tr("铁谱分析设备"));
        _oiaInfoModel->setHeaderData(22,Qt::Horizontal,tr("铁谱分析报告编号"));
        _oiaInfoModel->setHeaderData(23,Qt::Horizontal,tr("理化分析方法"));
        _oiaInfoModel->setHeaderData(24,Qt::Horizontal,tr("理化分析人"));
        _oiaInfoModel->setHeaderData(25,Qt::Horizontal,tr("理化分析日期"));
        _oiaInfoModel->setHeaderData(26,Qt::Horizontal,tr("理化分析设备"));
        _oiaInfoModel->setHeaderData(27,Qt::Horizontal,tr("理化分析设备编号"));
    }
    else if(tablename == "OisInfo")
    {
        _oisInfoModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
        _oisInfoModel->setHeaderData(1,Qt::Horizontal,tr("油样编号"));
        _oisInfoModel->setHeaderData(2,Qt::Horizontal,tr("采样单位编号"));
        _oisInfoModel->setHeaderData(3,Qt::Horizontal,tr("机型"));
        _oisInfoModel->setHeaderData(4,Qt::Horizontal,tr("机号"));
        _oisInfoModel->setHeaderData(5,Qt::Horizontal,tr("监控部件名称"));
        _oisInfoModel->setHeaderData(6,Qt::Horizontal,tr("监控部件编号"));
        _oisInfoModel->setHeaderData(7,Qt::Horizontal,tr("采样点编号"));
        _oisInfoModel->setHeaderData(8,Qt::Horizontal,tr("滑油工作时数"));
        _oisInfoModel->setHeaderData(9,Qt::Horizontal,tr("滑油添加量"));
        _oisInfoModel->setHeaderData(10,Qt::Horizontal,tr("采样原因"));
        _oisInfoModel->setHeaderData(11,Qt::Horizontal,tr("采样单位"));
        _oisInfoModel->setHeaderData(12,Qt::Horizontal,tr("采样人"));
        _oisInfoModel->setHeaderData(13,Qt::Horizontal,tr("采样日期"));
        _oisInfoModel->setHeaderData(14,Qt::Horizontal,tr("采样时间"));
        _oisInfoModel->setHeaderData(15,Qt::Horizontal,tr("采样时机"));
        _oisInfoModel->setHeaderData(16,Qt::Horizontal,tr("采样方式"));
        _oisInfoModel->setHeaderData(17,Qt::Horizontal,tr("采样量"));
        _oisInfoModel->setHeaderData(18,Qt::Horizontal,tr("采样说明"));
        _oisInfoModel->setHeaderData(19,Qt::Horizontal,tr("送样人"));
        _oisInfoModel->setHeaderData(20,Qt::Horizontal,tr("送样日期"));
        _oisInfoModel->setHeaderData(21,Qt::Horizontal,tr("送样时间"));
    }
}
