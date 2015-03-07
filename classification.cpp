#include "classification.h"
#include "ui_classification.h"
#include "Connection.h"
#include <QApplication>
#include <QTableView>
#include <QObject>
#include <QDate>
#include <QCheckBox>
#include <QSqlQuery>
#include <QTableWidget>

classification::classification(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::classification)
{
    ui->setupUi(this);
    int i=0;
    QSqlDatabase db;
    if(!createConnection(db))
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection."),
                              QMessageBox::Cancel);
    }

    QSqlQuery query;
    QString SQL1="select * from addtosql";
    query.prepare(SQL1);
    query.exec();

    while(query.next())
    {
        ui->twLend->setItem(i,0,new QTableWidgetItem(query.value(2).toString()));
        ui->twLend->setItem(i,1,new QTableWidgetItem(query.value(3).toString()));
        ui->twLend->setItem(i,2,new QTableWidgetItem(query.value(4).toString()));
        ui->twLend->setItem(i,3,new QTableWidgetItem(query.value(5).toString()));
        ui->twLend->setItem(i,4,new QTableWidgetItem(query.value(6).toString()));
        ui->twLend->setItem(i,5,new QTableWidgetItem(query.value(7).toString()));
        ui->twLend->setItem(i,6,new QTableWidgetItem(query.value(8).toString()));
        ui->twLend->setItem(i,7,new QTableWidgetItem(query.value(9).toString()));
        ui->twLend->setItem(i,8,new QTableWidgetItem(query.value(10).toString()));
        ui->twLend->setItem(i,9,new QTableWidgetItem(query.value(11).toString()));
        i++;
    }

}

classification::~classification()
{
    delete ui;
}


void classification::on_updateButton_clicked()
{
    int i=0;
    QSqlDatabase db;
    if(!createConnection(db))
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection."),
                              QMessageBox::Cancel);
    }

    QSqlQuery query;
    QString SQL1="select * from addtosql";
    query.prepare(SQL1);
    query.exec();

    while(query.next())
    {
        ui->twLend->setItem(i,0,new QTableWidgetItem(query.value(2).toString()));
        ui->twLend->setItem(i,1,new QTableWidgetItem(query.value(3).toString()));
        ui->twLend->setItem(i,2,new QTableWidgetItem(query.value(4).toString()));
        ui->twLend->setItem(i,3,new QTableWidgetItem(query.value(5).toString()));
        ui->twLend->setItem(i,4,new QTableWidgetItem(query.value(6).toString()));
        ui->twLend->setItem(i,5,new QTableWidgetItem(query.value(7).toString()));
        ui->twLend->setItem(i,6,new QTableWidgetItem(query.value(8).toString()));
        ui->twLend->setItem(i,7,new QTableWidgetItem(query.value(9).toString()));
        ui->twLend->setItem(i,8,new QTableWidgetItem(query.value(10).toString()));
        ui->twLend->setItem(i,9,new QTableWidgetItem(query.value(11).toString()));
        i++;
    }

}

void classification::on_saveButton_clicked()
{


}

