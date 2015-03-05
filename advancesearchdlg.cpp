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
    
    tableNames.insert("EqmInfo","equipmentinfo");       // 装备信息表
    tableNames.insert("AbmInfo","abrasivemarkinfo");    // 磨粒标注信息表
    tableNames.insert("FegInfo","ferrographyinfo");     // 铁谱质谱信息表
    tableNames.insert("FegPInfo","ferrographypicinfo"); // 铁谱图片信息表
    tableNames.insert("MpInfo","movepartinfo");         // 动部件信息表
    tableNames.insert("MprInfo","movepartrepairinfo");  //动部件维修信息表
    tableNames.insert("OiaInfo","oilanalyzeinfo");      // 油样分析信息表
    tableNames.insert("OisInfo","oilsampleinfo");       // 油样采样信息表
    
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
    
}

AdvanceSearchDlg::~AdvanceSearchDlg()
{
    delete ui;
}

void AdvanceSearchDlg::on_queryBtn_clicked()
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
}
