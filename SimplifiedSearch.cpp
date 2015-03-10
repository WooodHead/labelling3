#include "SimplifiedSearch.h"
#include "ui_SimplifiedSearch.h"

#include "Connection.h"

SimplifiedSearch::SimplifiedSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimplifiedSearch)
{
    ui->setupUi(this);

    connect(ui->_buttonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->_buttonSearch, SIGNAL(clicked()), this, SLOT(search()));
    //connect(this, SIGNAL(flushLeftWindow()), parent, SLOT(flushLeftWindow()));
}

SimplifiedSearch::~SimplifiedSearch()
{
    delete ui;
}

void SimplifiedSearch::search()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("ferrographypicinfo");

    QString label = ui->_checkBoxUnLabelled->isChecked() ? "N" : "";
    QString unit = ui->_comboBoxUnit->currentText();
    QString planeID = ui->_comboBoxPlaneID->currentText();
    QString planeType = ui->_comboBoxPlaneType->currentText();

    QString sql;

    if(!label.isEmpty())
    {
        model->setFilter(label);
    }

    if(model->select())
    {
        QSqlDatabase db;

        QSqlQuery query;

        if(!createConnection(db))
        {
            QMessageBox::critical(0, qApp->tr("提示"), qApp->tr("数据库连接失败!"), QMessageBox::Cancel);
            return;
        }

        for(int i = 0; i < model->rowCount(); i++)
        {
            QString sql = "select ";
        }
    }
}
