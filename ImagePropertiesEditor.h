#ifndef IMAGEPROPERTIESEDITOR_H
#define IMAGEPROPERTIESEDITOR_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include <QSqlTableModel>
#include "Global.h"

namespace Ui {
class ImagePropertiesEditor;
}

class ImagePropertiesEditor : public QDialog
{
    Q_OBJECT
    
public:
    ImagePropertiesEditor(QWidget *parent, int index, QString primaryKeyValue);
    ~ImagePropertiesEditor();

    void showDlg();
    
private slots:
    void on__buttonCancel_3_clicked();

    void on__buttonSave_3_clicked();

private:
    Ui::ImagePropertiesEditor *ui;

    int _tableIndex;
    QString _primaryKeyValue;
    QSqlTableModel* _model;

private: // methods
    void initialize();
    void loadData();
signals:
    void flush();
};

#endif // IMAGEPROPERTIESEDITOR_H
