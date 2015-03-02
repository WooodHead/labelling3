#ifndef BUTTOM_H
#define BUTTOM_H

#include <QWidget>

namespace Ui {
    class buttom;
}

class buttom : public QWidget
{
    Q_OBJECT

public:
    explicit buttom(QWidget *parent = 0);
    ~buttom();

private:
    Ui::buttom *ui;
};

#endif // BUTTOM_H
