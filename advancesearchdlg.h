#ifndef ADVANCESEARCHDLG_H
#define ADVANCESEARCHDLG_H

#include <QDialog>
#include <QStringList>
#include <QMap>

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
     QString geneSql(QStringList conditionList,QString tableName);

private:
    Ui::AdvanceSearchDlg *ui;

    QStringList _equipmentinfoCdtList;          // zhuang bei xin xi biao condition list
    QStringList _abrasivemarkinfoCdtList;       // mo li biao ji xin xi biao condition list
    QStringList _ferrographyinfoCdtList;        // tie pu zhi pu xin xi biao condition list
    QStringList _ferrographypicinfoCdtList;     // tie pu tu pian cai ji xin xi biao condition list
    QStringList _movepartinfoCdtList;           // dong bu jian xin xi biao condtion list
    QStringList _movepartrepairinfoCdtList;     // dong bu jian wei xiu xin xi biao condition list
    QStringList _oilanalyzeinfoCdtList;         // you yang fen xi xin xi biao condition list
    QStringList _oilsampleinfoCdtList;          // you yang xin xi biao condition list

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
