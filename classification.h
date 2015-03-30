#ifndef CLASSIFICATION_H
#define CLASSIFICATION_H

#include <QWidget>
#include <QMessageBox>

#include "Global.h"

namespace Ui {
    class classification;
}

class classification : public QWidget
{
    Q_OBJECT

public:
    explicit classification(QWidget *parent = 0);
    ~classification();

private:
    Ui::classification *ui;
private slots:
    void on_updateButton_clicked();
    void on_saveButton_clicked();

};

#endif // CLASSIFICATION_H
