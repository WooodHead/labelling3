#ifndef SIMPLIFIEDSEARCH_H
#define SIMPLIFIEDSEARCH_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class SimplifiedSearch;
}

class SimplifiedSearch : public QDialog
{
    Q_OBJECT
    
public:
    explicit SimplifiedSearch(QWidget *parent = 0);
    ~SimplifiedSearch();
    
private:
    Ui::SimplifiedSearch *ui;

private slots:
    void search();

signals:
    void flushLeftWindow();
};

#endif // SIMPLIFIEDSEARCH_H
