#include "advancesearchdlg.h"
#include "ui_advancesearchdlg.h"

AdvanceSearchDlg::AdvanceSearchDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvanceSearchDlg)
{
    ui->setupUi(this);
}

AdvanceSearchDlg::~AdvanceSearchDlg()
{
    delete ui;
}

void AdvanceSearchDlg::on_queryBtn_clicked()
{

}

QString AdvanceSearchDlg::geneSql(QStringList conditionList, QString tableName)
{
    QString sql = "select * from ";
    sql.append(tableName);
    sql.append(" ");
//    if(conditionList.isEmpty())

}
