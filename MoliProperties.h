#ifndef MOLIPROPERTIES_H
#define MOLIPROPERTIES_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QFile>
#include <QSqlRecord>

namespace Ui {
class MoliProperties;
}

class MoliProperties : public QDialog
{
    Q_OBJECT
    
public:
    explicit MoliProperties(QWidget *parent = 0);
    ~MoliProperties();
    
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::MoliProperties *ui;

    QSqlTableModel* _model;
    QSqlDatabase _db;

    QString _originalImagePath;
    QString _resultPath;
    QString _maskPath;

    void showDlg(QString imagePath, QString resultPath, QString maskPath);
};

#endif // MOLIPROPERTIES_H
