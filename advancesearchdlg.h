#ifndef ADVANCESEARCHDLG_H
#define ADVANCESEARCHDLG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QStringList>
#include <QMap>
#include <QDebug>

#include "connection.h"

namespace Ui {
class AdvanceSearchDlg;
}

class AdvanceSearchDlg : public QDialog
{
    Q_OBJECT
    
public:
     AdvanceSearchDlg(QWidget *parent = 0);
    ~AdvanceSearchDlg();
    
private slots:
     void on_queryBtn_clicked();

private:
     QString generateSql(QMap<QString,QString> conditionMap,QStringList conditionField,QString tableName);
     void setModelHeaderData(QString tablename);

private:
    Ui::AdvanceSearchDlg *ui;
    
    QSqlDatabase db;
    
    QMap<QString,QString> tableNames;
    QSqlQueryModel *_eqmInfoModel;
    QSqlQueryModel *_abmInfoModel;
    QSqlQueryModel *_fegInfoModel;
    QSqlQueryModel *_fegpInfoModel;
    QSqlQueryModel *_mpInfoModel;
    QSqlQueryModel *_mprInfoModel;
    QSqlQueryModel *_oiaInfoModel;
    QSqlQueryModel *_oisInfoModel;
    

    QStringList _equipmentinfoCdtField;          // zhuang bei xin xi biao condition Field
    QStringList _abrasivemarkinfoCdtField;       // mo li biao ji xin xi biao condition Field
    QStringList _ferrographyinfoCdtField;        // tie pu zhi pu xin xi biao condition Field
    QStringList _ferrographypicinfoCdtField;     // tie pu tu pian cai ji xin xi biao condition Field
    QStringList _movepartinfoCdtField;           // dong bu jian xin xi biao condtion Field
    QStringList _movepartrepairinfoCdtField;     // dong bu jian wei xiu xin xi biao condition Field
    QStringList _oilanalyzeinfoCdtField;         // you yang fen xi xin xi biao condition Field
    QStringList _oilsampleinfoCdtField;          // you yang xin xi biao condition Field

    QMap<QString,QString> _equipmentinfoCdtMap;          // zhuang bei xin xi biao condition Map
    QMap<QString,QString> _abrasivemarkinfoCdtMap;       // mo li biao ji xin xi biao condition Map
    QMap<QString,QString> _ferrographyinfoCdtMap;        // tie pu zhi pu xin xi biao condition Map
    QMap<QString,QString> _ferrographypicinfoCdtMap;     // tie pu tu pian cai ji xin xi biao condition Map
    QMap<QString,QString> _movepartinfoCdtMap;           // dong bu jian xin xi biao condtion Map
    QMap<QString,QString> _movepartrepairinfoCdtMap;     // dong bu jian wei xiu xin xi biao condition Map
    QMap<QString,QString> _oilanalyzeinfoCdtMap;         // you yang fen xi xin xi biao condition Map
    QMap<QString,QString> _oilsampleinfoCdtMap;          // you yang xin xi biao condition Map

};

#endif // ADVANCESEARCHDLG_H
