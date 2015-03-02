#ifndef SEARCHDATA_H
#define SEARCHDATA_H

#include <QWidget>

namespace Ui {
    class Searchdata;
}

class Searchdata : public QWidget
{
    Q_OBJECT

public:
    explicit Searchdata(QWidget *parent = 0);
    ~Searchdata();
    int uid;

private slots:
    void on_linkbutton_clicked();
    void on_addtobutton_clicked();


private:
    Ui::Searchdata *ui;
};



#endif // SEARCHDATA_H
