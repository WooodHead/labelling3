#ifndef IMAGEPROPERTIES_H
#define IMAGEPROPERTIES_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>

#define TABLE_N 8

namespace Ui {
class ImageProperties;
}

class ImageProperties : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImageProperties(QWidget *parent = 0);
    ~ImageProperties();
    
private:
    Ui::ImageProperties *ui;

    void load();

    QSqlTableModel* _models[TABLE_N];

    QStringList getItems(QSqlTableModel* model, int col);
};

#endif // IMAGEPROPERTIES_H
