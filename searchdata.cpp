#include "searchdata.h"
#include "ui_searchdata.h"
#include "connection.h"
#include <QApplication>
#include <QTableView>
#include <QObject>
#include <QDate>
#include <QCheckBox>
#include <QSqlQuery>
#include <QTableWidget>

Searchdata::Searchdata(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Searchdata)
{
    ui->setupUi(this);
    ui->twLend->clear();
    ui->twLend->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend->setItem(0,1,new QTableWidgetItem("机号"));
    ui->twLend->setItem(0,2,new QTableWidgetItem("机型"));
    ui->twLend->setItem(0,3,new QTableWidgetItem("单位编号"));
    ui->twLend->setItem(0,4,new QTableWidgetItem("飞行小时数"));
    ui->twLend->setItem(0,5,new QTableWidgetItem("运行阶段"));
    ui->twLend->setItem(0,6,new QTableWidgetItem("修理次数"));
    ui->twLend_2->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_2->setItem(0,1,new QTableWidgetItem("动部件编号"));
    ui->twLend_2->setItem(0,2,new QTableWidgetItem("动部件型号"));
    ui->twLend_2->setItem(0,3,new QTableWidgetItem("动部件名称"));
    ui->twLend_2->setItem(0,4,new QTableWidgetItem("运行时数"));
    ui->twLend_2->setItem(0,5,new QTableWidgetItem("运行阶段"));
    ui->twLend_2->setItem(0,6,new QTableWidgetItem("机号"));
    ui->twLend_2->setItem(0,7,new QTableWidgetItem("机型"));
    ui->twLend_2->setItem(0,8,new QTableWidgetItem("动部件起始日期"));
    ui->twLend_2->setItem(0,9,new QTableWidgetItem("动部件终止日期"));
    ui->twLend_3->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_3->setItem(0,1,new QTableWidgetItem("动部件维修编号"));
    ui->twLend_3->setItem(0,2,new QTableWidgetItem("动部件编号"));
    ui->twLend_3->setItem(0,3,new QTableWidgetItem("动部件型号"));
    ui->twLend_3->setItem(0,4,new QTableWidgetItem("维修原因"));
    ui->twLend_3->setItem(0,5,new QTableWidgetItem("维修日期"));
    ui->twLend_3->setItem(0,6,new QTableWidgetItem("维修次数"));
    ui->twLend_3->setItem(0,7,new QTableWidgetItem("维修执行单位"));
    ui->twLend_3->setItem(0,8,new QTableWidgetItem("维修内容"));
    ui->twLend_3->setItem(0,9,new QTableWidgetItem("维修换件情况"));
    ui->twLend_4->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_4->setItem(0,1,new QTableWidgetItem("油样编号"));
    ui->twLend_4->setItem(0,2,new QTableWidgetItem("采样单位编号"));
    ui->twLend_4->setItem(0,3,new QTableWidgetItem("机型"));
    ui->twLend_4->setItem(0,4,new QTableWidgetItem("机号"));
    ui->twLend_4->setItem(0,5,new QTableWidgetItem("监控部件名称"));
    ui->twLend_4->setItem(0,6,new QTableWidgetItem("监控部件编号"));
    ui->twLend_4->setItem(0,7,new QTableWidgetItem("采样点编号"));
    ui->twLend_4->setItem(0,8,new QTableWidgetItem("滑油工作时数"));
    ui->twLend_4->setItem(0,9,new QTableWidgetItem("滑油添加量"));
    ui->twLend_4->setItem(0,10,new QTableWidgetItem("采样原因"));
    ui->twLend_4->setItem(0,11,new QTableWidgetItem("采样单位"));
    ui->twLend_4->setItem(0,12,new QTableWidgetItem("采样人"));
    ui->twLend_4->setItem(0,13,new QTableWidgetItem("采样日期"));
    ui->twLend_4->setItem(0,14,new QTableWidgetItem("采样时间"));
    ui->twLend_4->setItem(0,15,new QTableWidgetItem("采样时机"));
    ui->twLend_4->setItem(0,16,new QTableWidgetItem("采样方式"));
    ui->twLend_4->setItem(0,17,new QTableWidgetItem("采样量"));
    ui->twLend_4->setItem(0,18,new QTableWidgetItem("采样说明"));
    ui->twLend_4->setItem(0,19,new QTableWidgetItem("送样人"));
    ui->twLend_4->setItem(0,20,new QTableWidgetItem("送样日期"));
    ui->twLend_4->setItem(0,21,new QTableWidgetItem("送样时间"));
    ui->twLend_4->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_4->setItem(0,1,new QTableWidgetItem("油样编号"));
    ui->twLend_4->setItem(0,2,new QTableWidgetItem("采样单位编号"));
    ui->twLend_4->setItem(0,3,new QTableWidgetItem("机型"));
    ui->twLend_4->setItem(0,4,new QTableWidgetItem("机号"));
    ui->twLend_4->setItem(0,5,new QTableWidgetItem("监控部件名称"));
    ui->twLend_4->setItem(0,6,new QTableWidgetItem("监控部件编号"));
    ui->twLend_4->setItem(0,7,new QTableWidgetItem("采样点编号"));
    ui->twLend_4->setItem(0,8,new QTableWidgetItem("滑油工作时数"));
    ui->twLend_4->setItem(0,9,new QTableWidgetItem("滑油添加量"));
    ui->twLend_4->setItem(0,10,new QTableWidgetItem("采样原因"));
    ui->twLend_4->setItem(0,11,new QTableWidgetItem("采样单位"));
    ui->twLend_4->setItem(0,12,new QTableWidgetItem("采样人"));
    ui->twLend_4->setItem(0,13,new QTableWidgetItem("采样日期"));
    ui->twLend_4->setItem(0,14,new QTableWidgetItem("采样时间"));
    ui->twLend_4->setItem(0,15,new QTableWidgetItem("采样时机"));
    ui->twLend_4->setItem(0,16,new QTableWidgetItem("采样方式"));
    ui->twLend_4->setItem(0,17,new QTableWidgetItem("采样量"));
    ui->twLend_4->setItem(0,18,new QTableWidgetItem("采样说明"));
    ui->twLend_4->setItem(0,19,new QTableWidgetItem("送样人"));
    ui->twLend_4->setItem(0,20,new QTableWidgetItem("送样日期"));
    ui->twLend_4->setItem(0,21,new QTableWidgetItem("送样时间"));
    ui->twLend_4->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_4->setItem(0,1,new QTableWidgetItem("油样编号"));
    ui->twLend_4->setItem(0,2,new QTableWidgetItem("采样单位编号"));
    ui->twLend_4->setItem(0,3,new QTableWidgetItem("机型"));
    ui->twLend_4->setItem(0,4,new QTableWidgetItem("机号"));
    ui->twLend_4->setItem(0,5,new QTableWidgetItem("监控部件名称"));
    ui->twLend_4->setItem(0,6,new QTableWidgetItem("监控部件编号"));
    ui->twLend_4->setItem(0,7,new QTableWidgetItem("采样点编号"));
    ui->twLend_4->setItem(0,8,new QTableWidgetItem("滑油工作时数"));
    ui->twLend_4->setItem(0,9,new QTableWidgetItem("滑油添加量"));
    ui->twLend_4->setItem(0,10,new QTableWidgetItem("采样原因"));
    ui->twLend_4->setItem(0,11,new QTableWidgetItem("采样单位"));
    ui->twLend_4->setItem(0,12,new QTableWidgetItem("采样人"));
    ui->twLend_4->setItem(0,13,new QTableWidgetItem("采样日期"));
    ui->twLend_4->setItem(0,14,new QTableWidgetItem("采样时间"));
    ui->twLend_4->setItem(0,15,new QTableWidgetItem("采样时机"));
    ui->twLend_4->setItem(0,16,new QTableWidgetItem("采样方式"));
    ui->twLend_4->setItem(0,17,new QTableWidgetItem("采样量"));
    ui->twLend_4->setItem(0,18,new QTableWidgetItem("采样说明"));
    ui->twLend_4->setItem(0,19,new QTableWidgetItem("送样人"));
    ui->twLend_4->setItem(0,20,new QTableWidgetItem("送样日期"));
    ui->twLend_4->setItem(0,21,new QTableWidgetItem("送样时间"));
    ui->twLend_5->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_5->setItem(0,1,new QTableWidgetItem("油样编号"));
    ui->twLend_5->setItem(0,2,new QTableWidgetItem("检测分析单位名称"));
    ui->twLend_5->setItem(0,3,new QTableWidgetItem("送检单位"));
    ui->twLend_5->setItem(0,4,new QTableWidgetItem("送检原因"));
    ui->twLend_5->setItem(0,5,new QTableWidgetItem("送样人"));
    ui->twLend_5->setItem(0,6,new QTableWidgetItem("收油样日期"));
    ui->twLend_5->setItem(0,7,new QTableWidgetItem("收油样人"));
    ui->twLend_5->setItem(0,8,new QTableWidgetItem("污染度分析方法"));
    ui->twLend_5->setItem(0,9,new QTableWidgetItem("污染度分析人"));
    ui->twLend_5->setItem(0,10,new QTableWidgetItem("污染度分析日期"));
    ui->twLend_5->setItem(0,11,new QTableWidgetItem("污染度分析设备"));
    ui->twLend_5->setItem(0,12,new QTableWidgetItem("污染度分析报告编号"));
    ui->twLend_5->setItem(0,13,new QTableWidgetItem("光谱分析方法"));
    ui->twLend_5->setItem(0,14,new QTableWidgetItem("光谱分析人"));
    ui->twLend_5->setItem(0,15,new QTableWidgetItem("光谱分析日期"));
    ui->twLend_5->setItem(0,16,new QTableWidgetItem("光谱分析设备"));
    ui->twLend_5->setItem(0,17,new QTableWidgetItem("光谱分析报告编号"));
    ui->twLend_5->setItem(0,18,new QTableWidgetItem("铁谱分析方法"));
    ui->twLend_5->setItem(0,19,new QTableWidgetItem("铁谱分析人"));
    ui->twLend_5->setItem(0,20,new QTableWidgetItem("铁谱分析日期"));
    ui->twLend_5->setItem(0,21,new QTableWidgetItem("铁谱分析设备"));
    ui->twLend_5->setItem(0,22,new QTableWidgetItem("铁谱分析报告编号"));
    ui->twLend_5->setItem(0,23,new QTableWidgetItem("理化分析方法"));
    ui->twLend_5->setItem(0,24,new QTableWidgetItem("理化分析人"));
    ui->twLend_5->setItem(0,25,new QTableWidgetItem("理化分析日期"));
    ui->twLend_5->setItem(0,26,new QTableWidgetItem("理化分析设备"));
    ui->twLend_5->setItem(0,27,new QTableWidgetItem("理化分析报告编号"));
    ui->twLend_6->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_6->setItem(0,1,new QTableWidgetItem("铁谱片编号"));
    ui->twLend_6->setItem(0,2,new QTableWidgetItem("铁谱分析报告编号"));
    ui->twLend_6->setItem(0,3,new QTableWidgetItem("油样编号"));
    ui->twLend_6->setItem(0,4,new QTableWidgetItem("分析铁谱仪型号"));
    ui->twLend_6->setItem(0,5,new QTableWidgetItem("铁谱片制取油样消耗量"));
    ui->twLend_6->setItem(0,6,new QTableWidgetItem("铁谱片制取方法"));
    ui->twLend_6->setItem(0,7,new QTableWidgetItem("铁谱片制取人"));
    ui->twLend_7->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_7->setItem(0,1,new QTableWidgetItem("铁谱图片编号"));
    ui->twLend_7->setItem(0,2,new QTableWidgetItem("铁谱片编号"));
    ui->twLend_7->setItem(0,3,new QTableWidgetItem("铁谱分析报告编号"));
    ui->twLend_7->setItem(0,4,new QTableWidgetItem("显微镜型号"));
    ui->twLend_7->setItem(0,5,new QTableWidgetItem("图像采集器型号"));
    ui->twLend_7->setItem(0,6,new QTableWidgetItem("光源类型"));
    ui->twLend_7->setItem(0,7,new QTableWidgetItem("放大倍数"));
    ui->twLend_7->setItem(0,8,new QTableWidgetItem("铁谱图片采集区域"));
    ui->twLend_7->setItem(0,9,new QTableWidgetItem("铁谱图片采集人"));
    ui->twLend_7->setItem(0,10,new QTableWidgetItem("铁谱图片(路径)"));
    ui->twLend_7->setItem(0,11,new QTableWidgetItem("铁谱图片识别分析信息"));
    ui->twLend_7->setItem(0,12,new QTableWidgetItem("铁谱图片标识符号"));
    ui->twLend_8->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_8->setItem(0,1,new QTableWidgetItem("磨粒编号"));
    ui->twLend_8->setItem(0,2,new QTableWidgetItem("铁谱图片编号"));
    ui->twLend_8->setItem(0,3,new QTableWidgetItem("铁谱片编号"));
    ui->twLend_8->setItem(0,4,new QTableWidgetItem("铁谱分析报告编号"));
    ui->twLend_8->setItem(0,5,new QTableWidgetItem("磨粒标注人"));
    ui->twLend_8->setItem(0,6,new QTableWidgetItem("磨粒图片（路径）"));
    ui->twLend_8->setItem(0,7,new QTableWidgetItem("磨粒材质"));
    ui->twLend_8->setItem(0,8,new QTableWidgetItem("磨粒位置"));
    ui->twLend_8->setItem(0,9,new QTableWidgetItem("磨粒尺寸"));
    ui->twLend_8->setItem(0,10,new QTableWidgetItem("磨粒周长"));
    ui->twLend_8->setItem(0,11,new QTableWidgetItem("磨粒形状"));
    ui->twLend_8->setItem(0,12,new QTableWidgetItem("磨粒颜色"));
    ui->twLend_8->setItem(0,13,new QTableWidgetItem("磨粒表面纹理类型"));
    ui->twLend_8->setItem(0,14,new QTableWidgetItem("磨粒磨损类型"));
    ui->twLend_8->setItem(0,15,new QTableWidgetItem("磨粒损伤部位"));
    ui->twLend_8->setItem(0,16,new QTableWidgetItem("磨粒磨损机理"));
    ui->twLend_8->setItem(0,17,new QTableWidgetItem("磨粒反映故障信息"));
    ui->twLend_8->setItem(0,18,new QTableWidgetItem("磨粒典型性"));
    QSqlDatabase db2;//创建一个SQL数据库实例

    uid=0;
    if(!createConnection(db2))//调用connection.h头文件中定义的createConnection函数连接数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                                      qApp->tr("Unable to establish a database connection."),
                                      QMessageBox::Cancel);
    }
    QSqlQuery query3;
    QString SQL3="select * from addtosql where uid=?";
    do
    {
        uid++;
        query3.prepare(SQL3);//从Tuser表中获取全部用户数据
        query3.addBindValue(uid);//获取uid的值作为key值
        query3.exec();
    }while(query3.next());
    uid=uid-1;
}

Searchdata::~Searchdata()
{
    delete ui;
}

/*void Searchdata::on_linkbutton_clicked()
{
    QSqlDatabase db;
    QSqlQuery query;
    QString sql1;
    sql1="select * from Equipmentinfo where id=?";

    if(!createConnection(db))//和数据库建立连接
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                          qApp->tr("Unable to establish a database connection."),
                          QMessageBox::Cancel);
    }


    if(ui->checkBox->isChecked())
    {
        QMessageBox::critical(0, qApp->tr(" open database"),
                                qApp->tr("establish a database connection."),
                                QMessageBox::Cancel);



    }
    db.close();


}*/

void Searchdata::on_linkbutton_clicked()//点击登录按钮
{
    ui->twLend->clear();
    ui->twLend_2->clear();
    ui->twLend_3->clear();
    ui->twLend_4->clear();
    ui->twLend_5->clear();
    ui->twLend_6->clear();
    ui->twLend_7->clear();
    ui->twLend_8->clear();
    ui->twLend->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend->setItem(0,1,new QTableWidgetItem("机号"));
    ui->twLend->setItem(0,2,new QTableWidgetItem("机型"));
    ui->twLend->setItem(0,3,new QTableWidgetItem("单位编号"));
    ui->twLend->setItem(0,4,new QTableWidgetItem("飞行小时数"));
    ui->twLend->setItem(0,5,new QTableWidgetItem("运行阶段"));
    ui->twLend->setItem(0,6,new QTableWidgetItem("修理次数"));
    ui->twLend_2->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_2->setItem(0,1,new QTableWidgetItem("动部件编号"));
    ui->twLend_2->setItem(0,2,new QTableWidgetItem("动部件型号"));
    ui->twLend_2->setItem(0,3,new QTableWidgetItem("动部件名称"));
    ui->twLend_2->setItem(0,4,new QTableWidgetItem("运行时数"));
    ui->twLend_2->setItem(0,5,new QTableWidgetItem("运行阶段"));
    ui->twLend_2->setItem(0,6,new QTableWidgetItem("机号"));
    ui->twLend_2->setItem(0,7,new QTableWidgetItem("机型"));
    ui->twLend_2->setItem(0,8,new QTableWidgetItem("动部件起始日期"));
    ui->twLend_2->setItem(0,9,new QTableWidgetItem("动部件终止日期"));
    ui->twLend_3->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_3->setItem(0,1,new QTableWidgetItem("动部件维修编号"));
    ui->twLend_3->setItem(0,2,new QTableWidgetItem("动部件编号"));
    ui->twLend_3->setItem(0,3,new QTableWidgetItem("动部件型号"));
    ui->twLend_3->setItem(0,4,new QTableWidgetItem("维修原因"));
    ui->twLend_3->setItem(0,5,new QTableWidgetItem("维修日期"));
    ui->twLend_3->setItem(0,6,new QTableWidgetItem("维修次数"));
    ui->twLend_3->setItem(0,7,new QTableWidgetItem("维修执行单位"));
    ui->twLend_3->setItem(0,8,new QTableWidgetItem("维修内容"));
    ui->twLend_3->setItem(0,9,new QTableWidgetItem("维修换件情况"));
    ui->twLend_4->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_4->setItem(0,1,new QTableWidgetItem("油样编号"));
    ui->twLend_4->setItem(0,2,new QTableWidgetItem("采样单位编号"));
    ui->twLend_4->setItem(0,3,new QTableWidgetItem("机型"));
    ui->twLend_4->setItem(0,4,new QTableWidgetItem("机号"));
    ui->twLend_4->setItem(0,5,new QTableWidgetItem("监控部件名称"));
    ui->twLend_4->setItem(0,6,new QTableWidgetItem("监控部件编号"));
    ui->twLend_4->setItem(0,7,new QTableWidgetItem("采样点编号"));
    ui->twLend_4->setItem(0,8,new QTableWidgetItem("滑油工作时数"));
    ui->twLend_4->setItem(0,9,new QTableWidgetItem("滑油添加量"));
    ui->twLend_4->setItem(0,10,new QTableWidgetItem("采样原因"));
    ui->twLend_4->setItem(0,11,new QTableWidgetItem("采样单位"));
    ui->twLend_4->setItem(0,12,new QTableWidgetItem("采样人"));
    ui->twLend_4->setItem(0,13,new QTableWidgetItem("采样日期"));
    ui->twLend_4->setItem(0,14,new QTableWidgetItem("采样时间"));
    ui->twLend_4->setItem(0,15,new QTableWidgetItem("采样时机"));
    ui->twLend_4->setItem(0,16,new QTableWidgetItem("采样方式"));
    ui->twLend_4->setItem(0,17,new QTableWidgetItem("采样量"));
    ui->twLend_4->setItem(0,18,new QTableWidgetItem("采样说明"));
    ui->twLend_4->setItem(0,19,new QTableWidgetItem("送样人"));
    ui->twLend_4->setItem(0,20,new QTableWidgetItem("送样日期"));
    ui->twLend_4->setItem(0,21,new QTableWidgetItem("送样时间"));
    ui->twLend_5->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_5->setItem(0,1,new QTableWidgetItem("油样编号"));
    ui->twLend_5->setItem(0,2,new QTableWidgetItem("检测分析单位名称"));
    ui->twLend_5->setItem(0,3,new QTableWidgetItem("送检单位"));
    ui->twLend_5->setItem(0,4,new QTableWidgetItem("送检原因"));
    ui->twLend_5->setItem(0,5,new QTableWidgetItem("送样人"));
    ui->twLend_5->setItem(0,6,new QTableWidgetItem("收油样日期"));
    ui->twLend_5->setItem(0,7,new QTableWidgetItem("收油样人"));
    ui->twLend_5->setItem(0,8,new QTableWidgetItem("污染度分析方法"));
    ui->twLend_5->setItem(0,9,new QTableWidgetItem("污染度分析人"));
    ui->twLend_5->setItem(0,10,new QTableWidgetItem("污染度分析日期"));
    ui->twLend_5->setItem(0,11,new QTableWidgetItem("污染度分析设备"));
    ui->twLend_5->setItem(0,12,new QTableWidgetItem("污染度分析报告编号"));
    ui->twLend_5->setItem(0,13,new QTableWidgetItem("光谱分析方法"));
    ui->twLend_5->setItem(0,14,new QTableWidgetItem("光谱分析人"));
    ui->twLend_5->setItem(0,15,new QTableWidgetItem("光谱分析日期"));
    ui->twLend_5->setItem(0,16,new QTableWidgetItem("光谱分析设备"));
    ui->twLend_5->setItem(0,17,new QTableWidgetItem("光谱分析报告编号"));
    ui->twLend_5->setItem(0,18,new QTableWidgetItem("铁谱分析方法"));
    ui->twLend_5->setItem(0,19,new QTableWidgetItem("铁谱分析人"));
    ui->twLend_5->setItem(0,20,new QTableWidgetItem("铁谱分析日期"));
    ui->twLend_5->setItem(0,21,new QTableWidgetItem("铁谱分析设备"));
    ui->twLend_5->setItem(0,22,new QTableWidgetItem("铁谱分析报告编号"));
    ui->twLend_5->setItem(0,23,new QTableWidgetItem("理化分析方法"));
    ui->twLend_5->setItem(0,24,new QTableWidgetItem("理化分析人"));
    ui->twLend_5->setItem(0,25,new QTableWidgetItem("理化分析日期"));
    ui->twLend_5->setItem(0,26,new QTableWidgetItem("理化分析设备"));
    ui->twLend_5->setItem(0,27,new QTableWidgetItem("理化分析报告编号"));
    ui->twLend_6->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_6->setItem(0,1,new QTableWidgetItem("铁谱片编号"));
    ui->twLend_6->setItem(0,2,new QTableWidgetItem("铁谱分析报告编号"));
    ui->twLend_6->setItem(0,3,new QTableWidgetItem("油样编号"));
    ui->twLend_6->setItem(0,4,new QTableWidgetItem("分析铁谱仪型号"));
    ui->twLend_6->setItem(0,5,new QTableWidgetItem("铁谱片制取油样消耗量"));
    ui->twLend_6->setItem(0,6,new QTableWidgetItem("铁谱片制取方法"));
    ui->twLend_6->setItem(0,7,new QTableWidgetItem("铁谱片制取人"));
    ui->twLend_7->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_7->setItem(0,1,new QTableWidgetItem("铁谱图片编号"));
    ui->twLend_7->setItem(0,2,new QTableWidgetItem("铁谱片编号"));
    ui->twLend_7->setItem(0,3,new QTableWidgetItem("铁谱分析报告编号"));
    ui->twLend_7->setItem(0,4,new QTableWidgetItem("显微镜型号"));
    ui->twLend_7->setItem(0,5,new QTableWidgetItem("图像采集器型号"));
    ui->twLend_7->setItem(0,6,new QTableWidgetItem("光源类型"));
    ui->twLend_7->setItem(0,7,new QTableWidgetItem("放大倍数"));
    ui->twLend_7->setItem(0,8,new QTableWidgetItem("铁谱图片采集区域"));
    ui->twLend_7->setItem(0,9,new QTableWidgetItem("铁谱图片采集人"));
    ui->twLend_7->setItem(0,10,new QTableWidgetItem("铁谱图片(路径)"));
    ui->twLend_7->setItem(0,11,new QTableWidgetItem("铁谱图片识别分析信息"));
    ui->twLend_7->setItem(0,12,new QTableWidgetItem("铁谱图片标识符号"));
    ui->twLend_8->setItem(0,0,new QTableWidgetItem("序号"));
    ui->twLend_8->setItem(0,1,new QTableWidgetItem("磨粒编号"));
    ui->twLend_8->setItem(0,2,new QTableWidgetItem("铁谱图片编号"));
    ui->twLend_8->setItem(0,3,new QTableWidgetItem("铁谱片编号"));
    ui->twLend_8->setItem(0,4,new QTableWidgetItem("铁谱分析报告编号"));
    ui->twLend_8->setItem(0,5,new QTableWidgetItem("磨粒标注人"));
    ui->twLend_8->setItem(0,6,new QTableWidgetItem("磨粒图片（路径）"));
    ui->twLend_8->setItem(0,7,new QTableWidgetItem("磨粒材质"));
    ui->twLend_8->setItem(0,8,new QTableWidgetItem("磨粒位置"));
    ui->twLend_8->setItem(0,9,new QTableWidgetItem("磨粒尺寸"));
    ui->twLend_8->setItem(0,10,new QTableWidgetItem("磨粒周长"));
    ui->twLend_8->setItem(0,11,new QTableWidgetItem("磨粒形状"));
    ui->twLend_8->setItem(0,12,new QTableWidgetItem("磨粒颜色"));
    ui->twLend_8->setItem(0,13,new QTableWidgetItem("磨粒表面纹理类型"));
    ui->twLend_8->setItem(0,14,new QTableWidgetItem("磨粒磨损类型"));
    ui->twLend_8->setItem(0,15,new QTableWidgetItem("磨粒损伤部位"));
    ui->twLend_8->setItem(0,16,new QTableWidgetItem("磨粒磨损机理"));
    ui->twLend_8->setItem(0,17,new QTableWidgetItem("磨粒反映故障信息"));
    ui->twLend_8->setItem(0,18,new QTableWidgetItem("磨粒典型性"));

    QSqlDatabase db;//创建一个SQL数据库实例
    int i1=1;
    int i2=1;
    int i3=1;
    int i4=1;
    int i5=1;
    int i6=1;
    int i7=1;
    int i8=1;
    int j=0;
    QString id1="0";
    QString id2="0";
    QString id3="0";
    QString id4="0";
    QString id5="0";
    QString id6="0";
    QString id7="0";
    QString id8="0";
    int arrayid3[1000];
    int arrayid4[1000];
    int arrayid5[1000];
    int arrayid6[1000];
    int arrayid7[1000];
    int arrayid8[1000];
    if(!createConnection(db))//调用connection.h头文件中定义的createConnection函数连接数据库
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                                      qApp->tr("Unable to establish a database connection."),
                                      QMessageBox::Cancel);
    }
    QSqlQuery query;
    //QString SQL1="select equipmentinfo.*,movepartinfo.* from equipmentinfo,movepartinfo where equipmentinfo.planeid=movepartinfo.planeid and equipmentinfo.planeid=?";
    QString SQL1="select equipmentinfo.*,movepartinfo.*,movepartrepairinfo.*,oilsampleinfo.*,oilanalyzeinfo.*,ferrographyinfo.*,ferrographypicinfo.*,abrasivemarkinfo.* from equipmentinfo,movepartinfo,movepartrepairinfo,oilsampleinfo,oilanalyzeinfo,ferrographyinfo,ferrographypicinfo,abrasivemarkinfo where equipmentinfo.planeid=movepartinfo.planeid and movepartinfo.movepartid=movepartrepairinfo.movepartid and movepartinfo.movepartid=oilsampleinfo.monitorpartid and oilsampleinfo.oilsampleid=oilanalyzeinfo.oilsampleid and oilsampleinfo.oilsampleid=ferrographyinfo.oilsampleid and ferrographyinfo.ferrographysheetid=ferrographypicinfo.ferrographysheetid and ferrographypicinfo.ferrographypicid=abrasivemarkinfo.ferrographypicid and ";
    if(ui->checkBox->isChecked())
    {
        SQL1=SQL1+"equipmentinfo.planeid=?";
        j++;
    }
    if(ui->checkBox_2->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"equipmentinfo.planetype=?";
        }
        else
        {
            SQL1=SQL1+" and equipmentinfo.planetype=?";
        }
        j++;

    }
    if(ui->checkBox_3->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"equipmentinfo.departid=?";
        }
        else
        {
            SQL1=SQL1+" and equipmentinfo.departid=?";
        }
        j++;

    }
    if(ui->checkBox_4->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"equipmentinfo.runhour=?";
        }
        else
        {
            SQL1=SQL1+" and equipmentinfo.runhour=?";
        }
        j++;

    }
    if(ui->checkBox_5->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"equipmentinfo.runstage=?";
        }
        else
        {
            SQL1=SQL1+" and equipmentinfo.runstage=?";
        }
        j++;

    }
    if(ui->checkBox_6->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"equipmentinfo.repairtime=?";
        }
        else
        {
            SQL1=SQL1+" and equipmentinfo.repairtime=?";
        }
        j++;

    }
/////////////movepartinfo
    if(ui->checkBox_7->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"movepartinfo.movepartid=?";
        }
        else
        {
            SQL1=SQL1+" and movepartinfo.movepartid=?";
        }
        j++;

    }

    if(ui->checkBox_8->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"movepartinfo.movepartname=?";
        }
        else
        {
            SQL1=SQL1+" and movepartinfo.movepartname=?";
        }
        j++;

    }
    if(ui->checkBox_9->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"movepartinfo.moveparttype=?";
        }
        else
        {
            SQL1=SQL1+" and movepartinfo.moveparttype=?";
        }
        j++;

    }
    if(ui->checkBox_10->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"movepartinfo.runhour=?";
        }
        else
        {
            SQL1=SQL1+" and movepartinfo.runhour=?";
        }
        j++;

    }
    if(ui->checkBox_11->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"movepartinfo.startdate=?";
        }
        else
        {
            SQL1=SQL1+" and movepartinfo.startdate=?";
        }
        j++;

    }
    if(ui->checkBox_12->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"movepartinfo.enddate=?";
        }
        else
        {
            SQL1=SQL1+" and movepartinfo.enddate=?";
        }
        j++;

    }
//////////////////movepartrepairinfo
    if(ui->checkBox_13->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"movepartrepairinfo.movepartrepairid=?";
        }
        else
        {
            SQL1=SQL1+" and movepartrepairinfo.movepartrepairid=?";
        }
        j++;

    }
    if(ui->checkBox_14->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"movepartrepairinfo.repairdate=?";
        }
        else
        {
            SQL1=SQL1+" and movepartrepairinfo.repairdate=?";
        }
        j++;

    }
    if(ui->checkBox_15->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"movepartrepairinfo.repairtime=?";
        }
        else
        {
            SQL1=SQL1+" and movepartrepairinfo.repairtime=?";
        }
        j++;

    }
    if(ui->checkBox_16->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"movepartrepairinfo.repairdepart=?";
        }
        else
        {
            SQL1=SQL1+" and movepartrepairinfo.repairdepart=?";
        }
        j++;

    }
/////////////////oilsampleinfo
    if(ui->checkBox_17->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.oilsampleid=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.oilsampleid=?";
        }
        j++;

    }
    if(ui->checkBox_18->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.sampledepartid=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.sampledepartid=?";
        }
        j++;

    }
    if(ui->checkBox_19->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.planetype=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.planetype=?";
        }
        j++;

    }
    if(ui->checkBox_20->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.planeid=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.planeid=?";
        }
        j++;

    }
    if(ui->checkBox_21->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.monitorpartname=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.monitorpartname=?";
        }
        j++;

    }
    if(ui->checkBox_22->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.monitorpartid=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.monitorpartid=?";
        }
        j++;

    }
    if(ui->checkBox_23->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.sampleid=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.sampleid=?";
        }
        j++;

    }
    if(ui->checkBox_24->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.oilworktime=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.oilworktime=?";
        }
        j++;

    }
    if(ui->checkBox_25->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.oiladdition=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.oiladdition=?";
        }
        j++;

    }
    if(ui->checkBox_26->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.sampledepartname=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.sampledepartname=?";
        }
        j++;

    }
    if(ui->checkBox_27->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.samplestuff=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.samplestuff=?";
        }
        j++;

    }
    if(ui->checkBox_28->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.sampledate=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.sampledate=?";
        }
        j++;

    }
    if(ui->checkBox_29->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.sampletime=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.sampletime=?";
        }
        j++;

    }
    if(ui->checkBox_30->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.samplesituation=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.samplesituation=?";
        }
        j++;

    }
    if(ui->checkBox_31->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.samplemethod=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.samplemethod=?";
        }
        j++;

    }
    if(ui->checkBox_32->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.samplevolume=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.samplevolume=?";
        }
        j++;

    }
    if(ui->checkBox_33->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.sendstuff=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.sendstuff=?";
        }
        j++;

    }
    if(ui->checkBox_34->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.senddate=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.senddate=?";
        }
        j++;

    }
    if(ui->checkBox_35->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilsampleinfo.sendtime=?";
        }
        else
        {
            SQL1=SQL1+" and oilsampleinfo.sendtime=?";
        }
        j++;

    }
/////////////oilanalyzeinfo
    if(ui->checkBox_36->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.oilsampleid=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.oilsampleid=?";
        }
        j++;

    }
    if(ui->checkBox_37->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.analyzedepartname=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.analyzedepartname=?";
        }
        j++;

    }
    if(ui->checkBox_38->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.senddepart=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.senddepart=?";
        }
        j++;

    }
    if(ui->checkBox_39->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.sendstuff=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.sendstuff=?";
        }
        j++;

    }
    if(ui->checkBox_40->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.receivedate=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.receivedate=?";
        }
        j++;

    }
    if(ui->checkBox_41->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.receivestuff=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.receivestuff=?";
        }
        j++;

    }
    if(ui->checkBox_42->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.contaminationanalyzemethod=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.contaminationanalyzemethod=?";
        }
        j++;

    }
    if(ui->checkBox_44->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.contaminationanalyzestuff=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.contaminationanalyzestuff=?";
        }
        j++;

    }
    if(ui->checkBox_45->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.contaminationanalyzedate=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.contaminationanalyzedate=?";
        }
        j++;

    }
    if(ui->checkBox_46->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.contaminationanalyzeequipment=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.contaminationanalyzeequipment=?";
        }
        j++;

    }
    if(ui->checkBox_47->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.contaminationanalyzereportid=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.contaminationanalyzereportid=?";
        }
        j++;

    }
    if(ui->checkBox_48->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.spectroscopymethod=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.spectroscopymethod=?";
        }
        j++;

    }
    if(ui->checkBox_49->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.spectroscopystuff=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.spectroscopystuff=?";
        }
        j++;

    }
    if(ui->checkBox_50->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.spectroscopydate=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.spectroscopydate=?";
        }
        j++;

    }
    if(ui->checkBox_51->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.spectroscopyequipment=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.spectroscopyequipment=?";
        }
        j++;

    }
    if(ui->checkBox_52->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.spectroscopyreportid=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.spectroscopyreportid=?";
        }
        j++;

    }
    if(ui->checkBox_53->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.ferrographymethod=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.ferrographymethod=?";
        }
        j++;

    }
    if(ui->checkBox_54->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.ferrographystuff=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.ferrographystuff=?";
        }
        j++;

    }
    if(ui->checkBox_55->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.ferrographydate=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.ferrographydate=?";
        }
        j++;

    }
    if(ui->checkBox_56->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.ferrographyequipment=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.ferrographyequipment=?";
        }
        j++;

    }
    if(ui->checkBox_57->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.ferrographyreportid=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.ferrographyreportid=?";
        }
        j++;

    }
    if(ui->checkBox_58->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.physicochemicalmethod=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.physicochemicalmethod=?";
        }
        j++;

    }
    if(ui->checkBox_59->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.physicochemicalstuff=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.physicochemicalstuff=?";
        }
        j++;

    }
    if(ui->checkBox_60->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.physicochemicaldate=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.physicochemicaldate=?";
        }
        j++;

    }
    if(ui->checkBox_61->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.physicochemicalequipment=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.physicochemicalequipment=?";
        }
        j++;

    }
    if(ui->checkBox_62->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"oilanalyzeinfo.physicochemicalreportid=?";
        }
        else
        {
            SQL1=SQL1+" and oilanalyzeinfo.physicochemicalreportid=?";
        }
        j++;

    }
/////////////ferrographyinfo
    if(ui->checkBox_43->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographyinfo.ferrographysheetid=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographyinfo.ferrographysheetid=?";
        }
        j++;

    }
    if(ui->checkBox_63->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographyinfo.ferrographyreportid=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographyinfo.ferrographyreportid=?";
        }
        j++;

    }
    if(ui->checkBox_64->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographyinfo.oilsampleid=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographyinfo.oilsampleid=?";
        }
        j++;

    }
    if(ui->checkBox_65->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographyinfo.ferrographyanalyzertype=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographyinfo.ferrographyanalyzertype=?";
        }
        j++;

    }
    if(ui->checkBox_66->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographyinfo.ferrographymakeoilconsumption=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographyinfo.ferrographymakeoilconsumption=?";
        }
        j++;

    }
    if(ui->checkBox_67->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographyinfo.ferrographymakemethod=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographyinfo.ferrographymakemethod=?";
        }
        j++;

    }
    if(ui->checkBox_68->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographyinfo.ferrographymakestuff=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographyinfo.ferrographymakestuff=?";
        }
        j++;

    }
//////////////ferrographypicinfo
    if(ui->checkBox_69->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.ferrographypicid=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.ferrographypicid=?";
        }
        j++;

    }
    if(ui->checkBox_70->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.ferrographysheetid=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.ferrographysheetid=?";
        }
        j++;

    }
    if(ui->checkBox_71->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.ferrographyreportid=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.ferrographyreportid=?";
        }
        j++;

    }
    if(ui->checkBox_72->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.microscopictype=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.microscopictype=?";
        }
        j++;

    }
    if(ui->checkBox_73->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.imageacquisitiontype=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.imageacquisitiontype=?";
        }
        j++;

    }
    if(ui->checkBox_74->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.lightsourcetype=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.lightsourcetype=?";
        }
        j++;

    }
    if(ui->checkBox_75->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.magnification=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.magnification=?";
        }
        j++;

    }
    if(ui->checkBox_76->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.imageacquisitionarea=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.imageacquisitionarea=?";
        }
        j++;

    }
    if(ui->checkBox_77->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.imageacquisitionstuff=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.imageacquisitionstuff=?";
        }
        j++;

    }
    if(ui->checkBox_78->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.ferrographypicpath=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.ferrographypicpath=?";
        }
        j++;

    }
    if(ui->checkBox_79->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.imagerecognitioninfoanalysis=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.imagerecognitioninfoanalysis=?";
        }
        j++;

    }
    if(ui->checkBox_80->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"ferrographypicinfo.imagesymbol=?";
        }
        else
        {
            SQL1=SQL1+" and ferrographypicinfo.imagesymbol=?";
        }
        j++;

    }
//////////abrasivemarkinfo

    if(ui->checkBox_81->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasiveid=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasiveid=?";
        }
        j++;

    }
    if(ui->checkBox_82->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.ferrographypicid=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.ferrographypicid=?";
        }
        j++;

    }
    if(ui->checkBox_83->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.ferrographysheetid=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.ferrographysheetid=?";
        }
        j++;

    }
    if(ui->checkBox_84->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.ferrographyreportid=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.ferrographyreportid=?";
        }
        j++;

    }
    if(ui->checkBox_85->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasivemarkstuff=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasivemarkstuff=?";
        }
        j++;

    }
    if(ui->checkBox_86->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasivepicpath=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasivepicpath=?";
        }
        j++;

    }
    if(ui->checkBox_87->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasivematerial=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasivematerial=?";
        }
        j++;

    }
    if(ui->checkBox_88->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasiveposition=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasiveposition=?";
        }
        j++;

    }
    if(ui->checkBox_89->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasivesize=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasivesize=?";
        }
        j++;

    }
    if(ui->checkBox_90->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasivesperimeter=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasivesperimeter=?";
        }
        j++;

    }
    if(ui->checkBox_91->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasiveshape=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasiveshape=?";
        }
        j++;

    }
    if(ui->checkBox_92->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasivecolor=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasivecolor=?";
        }
        j++;

    }
    if(ui->checkBox_93->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasivesurfacetexturetype=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasivesurfacetexturetype=?";
        }
        j++;

    }
    if(ui->checkBox_94->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasiveweartype=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasiveweartype=?";
        }
        j++;

    }
    if(ui->checkBox_95->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasivedamagetype=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasivedamagetype=?";
        }
        j++;

    }
    if(ui->checkBox_96->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasivemechanismtype=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasivemechanismtype=?";
        }
        j++;

    }
    if(ui->checkBox_97->isChecked())
    {
        if(j==0)
        {
            SQL1=SQL1+"abrasivemarkinfo.abrasivetypical=?";
        }
        else
        {
            SQL1=SQL1+" and abrasivemarkinfo.abrasivetypical=?";
        }
        j++;

    }

    query.prepare(SQL1);//从Tuser表中获取全部用户数据
    //query.addBindValue("LH04-567");
    if(ui->checkBox->isChecked())
    {
        query.addBindValue(ui->lineEdit->text().trimmed());//获取uid的值作为key值
    }
    if(ui->checkBox_2->isChecked())
    {
        query.addBindValue(ui->lineEdit_2->text().trimmed());
    }
    if(ui->checkBox_3->isChecked())
    {
        query.addBindValue(ui->lineEdit_3->text().trimmed());
    }
    if(ui->checkBox_4->isChecked())
    {
        query.addBindValue(ui->lineEdit_4->text().trimmed());
    }
    if(ui->checkBox_5->isChecked())
    {
        query.addBindValue(ui->lineEdit_5->text().trimmed());
    }
    if(ui->checkBox_6->isChecked())
    {
        query.addBindValue(ui->lineEdit_6->text().trimmed());
    }
//////////////////movepartinfo
    if(ui->checkBox_7->isChecked())
    {
        query.addBindValue(ui->lineEdit_7->text().trimmed());
    }
    if(ui->checkBox_8->isChecked())
    {
        query.addBindValue(ui->lineEdit_8->text().trimmed());
    }
    if(ui->checkBox_9->isChecked())
    {
        query.addBindValue(ui->lineEdit_9->text().trimmed());
    }
    if(ui->checkBox_10->isChecked())
    {
        query.addBindValue(ui->lineEdit_10->text().trimmed());
    }
    if(ui->checkBox_11->isChecked())
    {
        query.addBindValue(ui->lineEdit_11->text().trimmed());
    }
    if(ui->checkBox_12->isChecked())
    {
        query.addBindValue(ui->lineEdit_12->text().trimmed());
    }
/////////////movepartrepairinfo
    if(ui->checkBox_13->isChecked())
    {
        query.addBindValue(ui->lineEdit_13->text().trimmed());
    }
    if(ui->checkBox_14->isChecked())
    {
        query.addBindValue(ui->lineEdit_14->text().trimmed());
    }
    if(ui->checkBox_15->isChecked())
    {
        query.addBindValue(ui->lineEdit_15->text().trimmed());
    }
    if(ui->checkBox_16->isChecked())
    {
        query.addBindValue(ui->lineEdit_16->text().trimmed());
    }
///////////////oilsampleinfo
    if(ui->checkBox_17->isChecked())
    {
        query.addBindValue(ui->lineEdit_17->text().trimmed());
    }
    if(ui->checkBox_18->isChecked())
    {
        query.addBindValue(ui->lineEdit_18->text().trimmed());
    }
    if(ui->checkBox_19->isChecked())
    {
        query.addBindValue(ui->lineEdit_19->text().trimmed());
    }
    if(ui->checkBox_20->isChecked())
    {
        query.addBindValue(ui->lineEdit_20->text().trimmed());
    }
    if(ui->checkBox_21->isChecked())
    {
        query.addBindValue(ui->lineEdit_21->text().trimmed());
    }
    if(ui->checkBox_22->isChecked())
    {
        query.addBindValue(ui->lineEdit_22->text().trimmed());
    }
    if(ui->checkBox_23->isChecked())
    {
        query.addBindValue(ui->lineEdit_23->text().trimmed());
    }
    if(ui->checkBox_24->isChecked())
    {
        query.addBindValue(ui->lineEdit_24->text().trimmed());
    }
    if(ui->checkBox_25->isChecked())
    {
        query.addBindValue(ui->lineEdit_25->text().trimmed());
    }
    if(ui->checkBox_26->isChecked())
    {
        query.addBindValue(ui->lineEdit_26->text().trimmed());
    }
    if(ui->checkBox_27->isChecked())
    {
        query.addBindValue(ui->lineEdit_27->text().trimmed());
    }
    if(ui->checkBox_28->isChecked())
    {
        query.addBindValue(ui->lineEdit_28->text().trimmed());
    }
    if(ui->checkBox_29->isChecked())
    {
        query.addBindValue(ui->lineEdit_29->text().trimmed());
    }
    if(ui->checkBox_30->isChecked())
    {
        query.addBindValue(ui->lineEdit_30->text().trimmed());
    }
    if(ui->checkBox_31->isChecked())
    {
        query.addBindValue(ui->lineEdit_31->text().trimmed());
    }
    if(ui->checkBox_32->isChecked())
    {
        query.addBindValue(ui->lineEdit_32->text().trimmed());
    }
    if(ui->checkBox_33->isChecked())
    {
        query.addBindValue(ui->lineEdit_33->text().trimmed());
    }
    if(ui->checkBox_34->isChecked())
    {
        query.addBindValue(ui->lineEdit_34->text().trimmed());
    }
    if(ui->checkBox_35->isChecked())
    {
        query.addBindValue(ui->lineEdit_35->text().trimmed());
    }
/////////////////oilanalyzeinfo
    if(ui->checkBox_36->isChecked())
    {
        query.addBindValue(ui->lineEdit_36->text().trimmed());
    }
    if(ui->checkBox_37->isChecked())
    {
        query.addBindValue(ui->lineEdit_37->text().trimmed());
    }
    if(ui->checkBox_38->isChecked())
    {
        query.addBindValue(ui->lineEdit_38->text().trimmed());
    }
    if(ui->checkBox_39->isChecked())
    {
        query.addBindValue(ui->lineEdit_39->text().trimmed());
    }
    if(ui->checkBox_40->isChecked())
    {
        query.addBindValue(ui->lineEdit_40->text().trimmed());
    }
    if(ui->checkBox_41->isChecked())
    {
        query.addBindValue(ui->lineEdit_41->text().trimmed());
    }
    if(ui->checkBox_42->isChecked())
    {
        query.addBindValue(ui->lineEdit_42->text().trimmed());
    }
    if(ui->checkBox_44->isChecked())
    {
        query.addBindValue(ui->lineEdit_44->text().trimmed());
    }
    if(ui->checkBox_45->isChecked())
    {
        query.addBindValue(ui->lineEdit_45->text().trimmed());
    }
    if(ui->checkBox_46->isChecked())
    {
        query.addBindValue(ui->lineEdit_46->text().trimmed());
    }
    if(ui->checkBox_47->isChecked())
    {
        query.addBindValue(ui->lineEdit_47->text().trimmed());
    }
    if(ui->checkBox_48->isChecked())
    {
        query.addBindValue(ui->lineEdit_48->text().trimmed());
    }
    if(ui->checkBox_49->isChecked())
    {
        query.addBindValue(ui->lineEdit_49->text().trimmed());
    }
    if(ui->checkBox_50->isChecked())
    {
        query.addBindValue(ui->lineEdit_50->text().trimmed());
    }
    if(ui->checkBox_51->isChecked())
    {
        query.addBindValue(ui->lineEdit_51->text().trimmed());
    }
    if(ui->checkBox_52->isChecked())
    {
        query.addBindValue(ui->lineEdit_52->text().trimmed());
    }
    if(ui->checkBox_53->isChecked())
    {
        query.addBindValue(ui->lineEdit_53->text().trimmed());
    }
    if(ui->checkBox_54->isChecked())
    {
        query.addBindValue(ui->lineEdit_54->text().trimmed());
    }
    if(ui->checkBox_55->isChecked())
    {
        query.addBindValue(ui->lineEdit_55->text().trimmed());
    }
    if(ui->checkBox_56->isChecked())
    {
        query.addBindValue(ui->lineEdit_56->text().trimmed());
    }
    if(ui->checkBox_57->isChecked())
    {
        query.addBindValue(ui->lineEdit_57->text().trimmed());
    }
    if(ui->checkBox_58->isChecked())
    {
        query.addBindValue(ui->lineEdit_58->text().trimmed());
    }
    if(ui->checkBox_59->isChecked())
    {
        query.addBindValue(ui->lineEdit_59->text().trimmed());
    }
    if(ui->checkBox_60->isChecked())
    {
        query.addBindValue(ui->lineEdit_60->text().trimmed());
    }
    if(ui->checkBox_61->isChecked())
    {
        query.addBindValue(ui->lineEdit_61->text().trimmed());
    }
    if(ui->checkBox_62->isChecked())
    {
        query.addBindValue(ui->lineEdit_62->text().trimmed());
    }
/////////////ferrographyinfo
    if(ui->checkBox_43->isChecked())
    {
        query.addBindValue(ui->lineEdit_43->text().trimmed());
    }
    if(ui->checkBox_63->isChecked())
    {
        query.addBindValue(ui->lineEdit_63->text().trimmed());
    }
    if(ui->checkBox_64->isChecked())
    {
        query.addBindValue(ui->lineEdit_64->text().trimmed());
    }
    if(ui->checkBox_65->isChecked())
    {
        query.addBindValue(ui->lineEdit_65->text().trimmed());
    }
    if(ui->checkBox_66->isChecked())
    {
        query.addBindValue(ui->lineEdit_66->text().trimmed());
    }
    if(ui->checkBox_67->isChecked())
    {
        query.addBindValue(ui->lineEdit_67->text().trimmed());
    }
    if(ui->checkBox_68->isChecked())
    {
        query.addBindValue(ui->lineEdit_68->text().trimmed());
    }
//////////////ferrographypicinfo
    if(ui->checkBox_69->isChecked())
    {
        query.addBindValue(ui->lineEdit_69->text().trimmed());
    }
    if(ui->checkBox_70->isChecked())
    {
        query.addBindValue(ui->lineEdit_70->text().trimmed());
    }
    if(ui->checkBox_71->isChecked())
    {
        query.addBindValue(ui->lineEdit_71->text().trimmed());
    }
    if(ui->checkBox_72->isChecked())
    {
        query.addBindValue(ui->lineEdit_72->text().trimmed());
    }
    if(ui->checkBox_73->isChecked())
    {
        query.addBindValue(ui->lineEdit_73->text().trimmed());
    }
    if(ui->checkBox_74->isChecked())
    {
        query.addBindValue(ui->lineEdit_74->text().trimmed());
    }
    if(ui->checkBox_75->isChecked())
    {
        query.addBindValue(ui->lineEdit_75->text().trimmed());
    }
    if(ui->checkBox_76->isChecked())
    {
        query.addBindValue(ui->lineEdit_76->text().trimmed());
    }
    if(ui->checkBox_77->isChecked())
    {
        query.addBindValue(ui->lineEdit_77->text().trimmed());
    }
    if(ui->checkBox_78->isChecked())
    {
        query.addBindValue(ui->lineEdit_78->text().trimmed());
    }
    if(ui->checkBox_79->isChecked())
    {
        query.addBindValue(ui->lineEdit_79->text().trimmed());
    }
    if(ui->checkBox_80->isChecked())
    {
        query.addBindValue(ui->lineEdit_80->text().trimmed());
    }
//////////abrasivemarkinfo
    if(ui->checkBox_81->isChecked())
    {
        query.addBindValue(ui->lineEdit_81->text().trimmed());
    }
    if(ui->checkBox_82->isChecked())
    {
        query.addBindValue(ui->lineEdit_82->text().trimmed());
    }
    if(ui->checkBox_83->isChecked())
    {
        query.addBindValue(ui->lineEdit_83->text().trimmed());
    }
    if(ui->checkBox_84->isChecked())
    {
        query.addBindValue(ui->lineEdit_84->text().trimmed());
    }
    if(ui->checkBox_85->isChecked())
    {
        query.addBindValue(ui->lineEdit_85->text().trimmed());
    }
    if(ui->checkBox_86->isChecked())
    {
        query.addBindValue(ui->lineEdit_86->text().trimmed());
    }
    if(ui->checkBox_87->isChecked())
    {
        query.addBindValue(ui->lineEdit_87->text().trimmed());
    }
    if(ui->checkBox_88->isChecked())
    {
        query.addBindValue(ui->lineEdit_88->text().trimmed());
    }
    if(ui->checkBox_89->isChecked())
    {
        query.addBindValue(ui->lineEdit_89->text().trimmed());
    }
    if(ui->checkBox_90->isChecked())
    {
        query.addBindValue(ui->lineEdit_90->text().trimmed());
    }
    if(ui->checkBox_91->isChecked())
    {
        query.addBindValue(ui->lineEdit_91->text().trimmed());
    }
    if(ui->checkBox_92->isChecked())
    {
        query.addBindValue(ui->lineEdit_92->text().trimmed());
    }
    if(ui->checkBox_93->isChecked())
    {
        query.addBindValue(ui->lineEdit_93->text().trimmed());
    }
    if(ui->checkBox_94->isChecked())
    {
        query.addBindValue(ui->lineEdit_94->text().trimmed());
    }
    if(ui->checkBox_95->isChecked())
    {
        query.addBindValue(ui->lineEdit_95->text().trimmed());
    }
    if(ui->checkBox_96->isChecked())
    {
        query.addBindValue(ui->lineEdit_96->text().trimmed());
    }
    if(ui->checkBox_97->isChecked())
    {
        query.addBindValue(ui->lineEdit_97->text().trimmed());
    }


    query.exec();

    while(query.next())
    {

            if(query.value(0).toString()!=id1)
            {

                ui->twLend->setItem(i1,0,new QTableWidgetItem(query.value(0).toString()));
                ui->twLend->setItem(i1,1,new QTableWidgetItem(query.value(1).toString()));
                ui->twLend->setItem(i1,2,new QTableWidgetItem(query.value(2).toString()));
                ui->twLend->setItem(i1,3,new QTableWidgetItem(query.value(3).toString()));
                ui->twLend->setItem(i1,4,new QTableWidgetItem(query.value(4).toString()));
                ui->twLend->setItem(i1,5,new QTableWidgetItem(query.value(5).toString()));
                ui->twLend->setItem(i1,6,new QTableWidgetItem(query.value(6).toString()));
                id1=query.value(0).toString();
                i1++;
            }

            if(query.value(7).toString()!=id2)
            {

                ui->twLend_2->setItem(i2,0,new QTableWidgetItem(query.value(7).toString()));
                ui->twLend_2->setItem(i2,1,new QTableWidgetItem(query.value(8).toString()));
                ui->twLend_2->setItem(i2,2,new QTableWidgetItem(query.value(9).toString()));
                ui->twLend_2->setItem(i2,3,new QTableWidgetItem(query.value(10).toString()));
                ui->twLend_2->setItem(i2,4,new QTableWidgetItem(query.value(11).toString()));
                ui->twLend_2->setItem(i2,5,new QTableWidgetItem(query.value(12).toString()));
                ui->twLend_2->setItem(i2,6,new QTableWidgetItem(query.value(13).toString()));
                ui->twLend_2->setItem(i2,7,new QTableWidgetItem(query.value(14).toString()));
                ui->twLend_2->setItem(i2,8,new QTableWidgetItem(query.value(15).toString()));
                ui->twLend_2->setItem(i2,9,new QTableWidgetItem(query.value(16).toString()));
                id2=query.value(7).toString();
                i2++;
            }
            if(query.value(17).toString()!=id3)
            {
                int flag=0;
                for(int k=0;k<i3;k++)
                {
                    if(arrayid3[k]==query.value(17).toString().toInt())
                    {
                        flag=1;
                    }
                }
                if(flag==0)
                {
                ui->twLend_3->setItem(i3,0,new QTableWidgetItem(query.value(17).toString()));
                ui->twLend_3->setItem(i3,1,new QTableWidgetItem(query.value(18).toString()));
                ui->twLend_3->setItem(i3,2,new QTableWidgetItem(query.value(19).toString()));
                ui->twLend_3->setItem(i3,3,new QTableWidgetItem(query.value(20).toString()));
                ui->twLend_3->setItem(i3,4,new QTableWidgetItem(query.value(21).toString()));
                ui->twLend_3->setItem(i3,5,new QTableWidgetItem(query.value(22).toString()));
                ui->twLend_3->setItem(i3,6,new QTableWidgetItem(query.value(23).toString()));
                ui->twLend_3->setItem(i3,7,new QTableWidgetItem(query.value(24).toString()));
                ui->twLend_3->setItem(i3,8,new QTableWidgetItem(query.value(25).toString()));
                ui->twLend_3->setItem(i3,9,new QTableWidgetItem(query.value(26).toString()));
                id3=query.value(17).toString();
                arrayid3[i3]=id3.toInt();
                i3++;
                }
            }
            if(query.value(27).toString()!=id4)
            {
                int flag=0;
                for(int k=0;k<i4;k++)
                {
                    if(arrayid4[k]==query.value(27).toString().toInt())
                    {
                        flag=1;
                    }
                }
                if(flag==0)
                {
                ui->twLend_4->setItem(i4,0,new QTableWidgetItem(query.value(27).toString()));
                ui->twLend_4->setItem(i4,1,new QTableWidgetItem(query.value(28).toString()));
                ui->twLend_4->setItem(i4,2,new QTableWidgetItem(query.value(29).toString()));
                ui->twLend_4->setItem(i4,3,new QTableWidgetItem(query.value(30).toString()));
                ui->twLend_4->setItem(i4,4,new QTableWidgetItem(query.value(31).toString()));
                ui->twLend_4->setItem(i4,5,new QTableWidgetItem(query.value(32).toString()));
                ui->twLend_4->setItem(i4,6,new QTableWidgetItem(query.value(33).toString()));
                ui->twLend_4->setItem(i4,7,new QTableWidgetItem(query.value(34).toString()));
                ui->twLend_4->setItem(i4,8,new QTableWidgetItem(query.value(35).toString()));
                ui->twLend_4->setItem(i4,9,new QTableWidgetItem(query.value(36).toString()));
                ui->twLend_4->setItem(i4,10,new QTableWidgetItem(query.value(37).toString()));
                ui->twLend_4->setItem(i4,11,new QTableWidgetItem(query.value(38).toString()));
                ui->twLend_4->setItem(i4,12,new QTableWidgetItem(query.value(39).toString()));
                ui->twLend_4->setItem(i4,13,new QTableWidgetItem(query.value(40).toString()));
                ui->twLend_4->setItem(i4,14,new QTableWidgetItem(query.value(41).toString()));
                ui->twLend_4->setItem(i4,15,new QTableWidgetItem(query.value(42).toString()));
                ui->twLend_4->setItem(i4,16,new QTableWidgetItem(query.value(43).toString()));
                ui->twLend_4->setItem(i4,17,new QTableWidgetItem(query.value(44).toString()));
                ui->twLend_4->setItem(i4,18,new QTableWidgetItem(query.value(45).toString()));
                ui->twLend_4->setItem(i4,19,new QTableWidgetItem(query.value(46).toString()));
                ui->twLend_4->setItem(i4,20,new QTableWidgetItem(query.value(47).toString()));
                ui->twLend_4->setItem(i4,21,new QTableWidgetItem(query.value(48).toString()));
                id4=query.value(27).toString();
                arrayid4[i4]=id4.toInt();
                i4++;
                }
            }

            if(query.value(49).toString()!=id5)
            {
                int flag=0;
                for(int k=0;k<i5;k++)
                {
                    if(arrayid5[k]==query.value(49).toString().toInt())
                    {
                        flag=1;
                    }
                }
                if(flag==0)
                {
                ui->twLend_5->setItem(i5,0,new QTableWidgetItem(query.value(49).toString()));
                ui->twLend_5->setItem(i5,1,new QTableWidgetItem(query.value(50).toString()));
                ui->twLend_5->setItem(i5,2,new QTableWidgetItem(query.value(51).toString()));
                ui->twLend_5->setItem(i5,3,new QTableWidgetItem(query.value(52).toString()));
                ui->twLend_5->setItem(i5,4,new QTableWidgetItem(query.value(53).toString()));
                ui->twLend_5->setItem(i5,5,new QTableWidgetItem(query.value(54).toString()));
                ui->twLend_5->setItem(i5,6,new QTableWidgetItem(query.value(55).toString()));
                ui->twLend_5->setItem(i5,7,new QTableWidgetItem(query.value(56).toString()));
                ui->twLend_5->setItem(i5,8,new QTableWidgetItem(query.value(57).toString()));
                ui->twLend_5->setItem(i5,9,new QTableWidgetItem(query.value(58).toString()));
                ui->twLend_5->setItem(i5,10,new QTableWidgetItem(query.value(59).toString()));
                ui->twLend_5->setItem(i5,11,new QTableWidgetItem(query.value(60).toString()));
                ui->twLend_5->setItem(i5,12,new QTableWidgetItem(query.value(61).toString()));
                ui->twLend_5->setItem(i5,13,new QTableWidgetItem(query.value(62).toString()));
                ui->twLend_5->setItem(i5,14,new QTableWidgetItem(query.value(63).toString()));
                ui->twLend_5->setItem(i5,15,new QTableWidgetItem(query.value(64).toString()));
                ui->twLend_5->setItem(i5,16,new QTableWidgetItem(query.value(65).toString()));
                ui->twLend_5->setItem(i5,17,new QTableWidgetItem(query.value(66).toString()));
                ui->twLend_5->setItem(i5,18,new QTableWidgetItem(query.value(67).toString()));
                ui->twLend_5->setItem(i5,19,new QTableWidgetItem(query.value(68).toString()));
                ui->twLend_5->setItem(i5,20,new QTableWidgetItem(query.value(69).toString()));
                ui->twLend_5->setItem(i5,21,new QTableWidgetItem(query.value(70).toString()));
                ui->twLend_5->setItem(i5,22,new QTableWidgetItem(query.value(71).toString()));
                ui->twLend_5->setItem(i5,23,new QTableWidgetItem(query.value(72).toString()));
                ui->twLend_5->setItem(i5,24,new QTableWidgetItem(query.value(73).toString()));
                ui->twLend_5->setItem(i5,25,new QTableWidgetItem(query.value(74).toString()));
                ui->twLend_5->setItem(i5,26,new QTableWidgetItem(query.value(75).toString()));
                ui->twLend_5->setItem(i5,27,new QTableWidgetItem(query.value(76).toString()));
                //ui->twLend_5->setItem(i5,28,new QTableWidgetItem(query.value(77).toString()));

                id5=query.value(49).toString();
                arrayid5[i5]=id5.toInt();
                i5++;
                }
            }
            if(query.value(77).toString()!=id6)
            {
                int flag=0;
                for(int k=0;k<i6;k++)
                {
                    if(arrayid6[k]==query.value(77).toString().toInt())
                    {
                        flag=1;
                    }
                }
                if(flag==0)
                {
                ui->twLend_6->setItem(i6,0,new QTableWidgetItem(query.value(77).toString()));
                ui->twLend_6->setItem(i6,1,new QTableWidgetItem(query.value(78).toString()));
                ui->twLend_6->setItem(i6,2,new QTableWidgetItem(query.value(79).toString()));
                ui->twLend_6->setItem(i6,3,new QTableWidgetItem(query.value(80).toString()));
                ui->twLend_6->setItem(i6,4,new QTableWidgetItem(query.value(81).toString()));
                ui->twLend_6->setItem(i6,5,new QTableWidgetItem(query.value(82).toString()));
                ui->twLend_6->setItem(i6,6,new QTableWidgetItem(query.value(83).toString()));
                ui->twLend_6->setItem(i6,7,new QTableWidgetItem(query.value(84).toString()));

                id6=query.value(77).toString();
                arrayid6[i6]=id6.toInt();
                i6++;
                }
            }
            if(query.value(85).toString()!=id7)
            {
                int flag=0;
                for(int k=0;k<i7;k++)
                {
                    if(arrayid7[k]==query.value(85).toString().toInt())
                    {
                        flag=1;
                    }
                }
                if(flag==0)
                {
                ui->twLend_7->setItem(i7,0,new QTableWidgetItem(query.value(85).toString()));
                ui->twLend_7->setItem(i7,1,new QTableWidgetItem(query.value(86).toString()));
                ui->twLend_7->setItem(i7,2,new QTableWidgetItem(query.value(87).toString()));
                ui->twLend_7->setItem(i7,3,new QTableWidgetItem(query.value(88).toString()));
                ui->twLend_7->setItem(i7,4,new QTableWidgetItem(query.value(89).toString()));
                ui->twLend_7->setItem(i7,5,new QTableWidgetItem(query.value(90).toString()));
                ui->twLend_7->setItem(i7,6,new QTableWidgetItem(query.value(91).toString()));
                ui->twLend_7->setItem(i7,7,new QTableWidgetItem(query.value(92).toString()));
                ui->twLend_7->setItem(i7,8,new QTableWidgetItem(query.value(93).toString()));
                ui->twLend_7->setItem(i7,9,new QTableWidgetItem(query.value(94).toString()));
                ui->twLend_7->setItem(i7,10,new QTableWidgetItem(query.value(95).toString()));
                ui->twLend_7->setItem(i7,11,new QTableWidgetItem(query.value(96).toString()));
                ui->twLend_7->setItem(i7,12,new QTableWidgetItem(query.value(97).toString()));
                id7=query.value(85).toString();
                arrayid7[i7]=id7.toInt();
                i7++;
                }
            }
            if(query.value(98).toString()!=id8)
            {
                int flag=0;
                for(int k=0;k<i8;k++)
                {
                    if(arrayid8[k]==query.value(98).toString().toInt())
                    {
                        flag=1;
                    }
                }
                if(flag==0)
                {
                ui->twLend_8->setItem(i8,0,new QTableWidgetItem(query.value(98).toString()));
                ui->twLend_8->setItem(i8,1,new QTableWidgetItem(query.value(99).toString()));
                ui->twLend_8->setItem(i8,2,new QTableWidgetItem(query.value(100).toString()));
                ui->twLend_8->setItem(i8,3,new QTableWidgetItem(query.value(101).toString()));
                ui->twLend_8->setItem(i8,4,new QTableWidgetItem(query.value(102).toString()));
                ui->twLend_8->setItem(i8,5,new QTableWidgetItem(query.value(103).toString()));
                ui->twLend_8->setItem(i8,6,new QTableWidgetItem(query.value(104).toString()));
                ui->twLend_8->setItem(i8,7,new QTableWidgetItem(query.value(105).toString()));
                ui->twLend_8->setItem(i8,8,new QTableWidgetItem(query.value(106).toString()));
                ui->twLend_8->setItem(i8,9,new QTableWidgetItem(query.value(107).toString()));
                ui->twLend_8->setItem(i8,10,new QTableWidgetItem(query.value(108).toString()));
                ui->twLend_8->setItem(i8,11,new QTableWidgetItem(query.value(109).toString()));
                ui->twLend_8->setItem(i8,12,new QTableWidgetItem(query.value(110).toString()));
                ui->twLend_8->setItem(i8,13,new QTableWidgetItem(query.value(111).toString()));
                ui->twLend_8->setItem(i8,14,new QTableWidgetItem(query.value(112).toString()));
                ui->twLend_8->setItem(i8,15,new QTableWidgetItem(query.value(113).toString()));
                ui->twLend_8->setItem(i8,16,new QTableWidgetItem(query.value(114).toString()));
                ui->twLend_8->setItem(i8,17,new QTableWidgetItem(query.value(115).toString()));
                ui->twLend_8->setItem(i8,18,new QTableWidgetItem(query.value(116).toString()));
                id8=query.value(98).toString();
                arrayid8[i8]=id8.toInt();
                i8++;
                }
            }


        //i++;
    }
    //QString SQL2="select equipmentinfo.*,movepartinfo.* from equipmentinfo,movepartinfo where equipmentinfo.planeid=movepartinfo.planeid and equipmentinfo.planeid=?";


    db.close();
}

void Searchdata::on_addtobutton_clicked()//点击登录按钮
{
     uid++;
     QSqlDatabase db1;//创建一个SQL数据库实例
     int j=0;
     QString arraysql[10];
     QString SQL2="select equipmentinfo.*,movepartinfo.*,movepartrepairinfo.*,oilsampleinfo.*,oilanalyzeinfo.* from equipmentinfo,movepartinfo,movepartrepairinfo,oilsampleinfo,oilanalyzeinfo where equipmentinfo.planeid=movepartinfo.planeid and movepartinfo.movepartid=movepartrepairinfo.movepartid and movepartinfo.movepartid=oilsampleinfo.monitorpartid and oilsampleinfo.oilsampleid=oilanalyzeinfo.oilsampleid and ";
     if(ui->checkBox->isChecked())
     {
         SQL2=SQL2+"equipmentinfo.planeid=?";
         arraysql[j]="机号";
         j++;
     }
     if(ui->checkBox_2->isChecked())
     {
         if(j==0)
         {
             SQL2=SQL2+"equipmentinfo.planetype=?";
         }
         else
         {
             SQL2=SQL2+" and equipmentinfo.planetype=?";
         }
         arraysql[j]="机型";
         j++;

     }
     if(ui->checkBox_3->isChecked())
     {
         if(j==0)
         {
             SQL2=SQL2+"equipmentinfo.departid=?";
         }
         else
         {
             SQL2=SQL2+" and equipmentinfo.departid=?";
         }
         arraysql[j]="单位编号";
         j++;

     }
     if(ui->checkBox_4->isChecked())
     {
         if(j==0)
         {
             SQL2=SQL2+"equipmentinfo.runhour=?";
         }
         else
         {
             SQL2=SQL2+" and equipmentinfo.runhour=?";
         }
         arraysql[j]="飞行小时数";
         j++;

     }
     if(ui->checkBox_5->isChecked())
     {
         if(j==0)
         {
             SQL2=SQL2+"equipmentinfo.runstage=?";
         }
         else
         {
             SQL2=SQL2+" and equipmentinfo.runstage=?";
         }
         arraysql[j]="运行阶段";
         j++;

     }
     if(ui->checkBox_6->isChecked())
     {
         if(j==0)
         {
             SQL2=SQL2+"equipmentinfo.repairtime=?";
         }
         else
         {
             SQL2=SQL2+" and equipmentinfo.repairtime=?";
         }
         arraysql[j]="维修次数";
         j++;

     }
     /////////////movepartinfo
         if(ui->checkBox_7->isChecked())
         {
             if(j==0)
             {
                 SQL2=SQL2+"movepartinfo.movepartid=?";
             }
             else
             {
                 SQL2=SQL2+" and movepartinfo.movepartid=?";
             }
             arraysql[j]="动部件编号";
             j++;

         }

         if(ui->checkBox_8->isChecked())
         {
             if(j==0)
             {
                 SQL2=SQL2+"movepartinfo.movepartname=?";
             }
             else
             {
                 SQL2=SQL2+" and movepartinfo.movepartname=?";
             }
             arraysql[j]="动部件名称";
             j++;

         }
         if(ui->checkBox_9->isChecked())
         {
             if(j==0)
             {
                 SQL2=SQL2+"movepartinfo.moveparttype=?";
             }
             else
             {
                 SQL2=SQL2+" and movepartinfo.moveparttype=?";
             }
             arraysql[j]="动部件型号";
             j++;

         }
         if(ui->checkBox_10->isChecked())
         {
             if(j==0)
             {
                 SQL2=SQL2+"movepartinfo.runhour=?";
             }
             else
             {
                 SQL2=SQL2+" and movepartinfo.runhour=?";
             }
             arraysql[j]="运行时数";
             j++;

         }
         if(ui->checkBox_11->isChecked())
         {
             if(j==0)
             {
                 SQL2=SQL2+"movepartinfo.startdate=?";
             }
             else
             {
                 SQL2=SQL2+" and movepartinfo.startdate=?";
             }
             arraysql[j]="动部件起始日期";
             j++;

         }
         if(ui->checkBox_12->isChecked())
         {
             if(j==0)
             {
                 SQL2=SQL2+"movepartinfo.enddate=?";
             }
             else
             {
                 SQL2=SQL2+" and movepartinfo.enddate=?";
             }
             arraysql[j]="动部件终止日期";
             j++;

         }
         //////////////////movepartrepairinfo
             if(ui->checkBox_13->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"movepartrepairinfo.movepartrepairid=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and movepartrepairinfo.movepartrepairid=?";
                 }
                 arraysql[j]="动部件维修编号";
                 j++;

             }
             if(ui->checkBox_14->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"movepartrepairinfo.repairdate=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and movepartrepairinfo.repairdate=?";
                 }
                 arraysql[j]="维修日期";
                 j++;

             }
             if(ui->checkBox_15->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"movepartrepairinfo.repairtime=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and movepartrepairinfo.repairtime=?";
                 }
                 arraysql[j]="维修次数";
                 j++;

             }
             if(ui->checkBox_16->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"movepartrepairinfo.repairdepart=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and movepartrepairinfo.repairdepart=?";
                 }
                 arraysql[j]="维修执行单位";
                 j++;

             }
         /////////////////oilsampleinfo
             if(ui->checkBox_17->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.oilsampleid=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.oilsampleid=?";
                 }
                 j++;

             }
             if(ui->checkBox_18->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.sampledepartid=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.sampledepartid=?";
                 }
                 j++;

             }
             if(ui->checkBox_19->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.planetype=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.planetype=?";
                 }
                 j++;

             }
             if(ui->checkBox_20->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.planeid=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.planeid=?";
                 }
                 j++;

             }
             if(ui->checkBox_21->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.monitorpartname=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.monitorpartname=?";
                 }
                 j++;

             }
             if(ui->checkBox_22->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.monitorpartid=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.monitorpartid=?";
                 }
                 j++;

             }
             if(ui->checkBox_23->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.sampleid=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.sampleid=?";
                 }
                 j++;

             }
             if(ui->checkBox_24->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.oilworktime=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.oilworktime=?";
                 }
                 j++;

             }
             if(ui->checkBox_25->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.oiladdition=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.oiladdition=?";
                 }
                 j++;

             }
             if(ui->checkBox_26->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.sampledepartname=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.sampledepartname=?";
                 }
                 j++;

             }
             if(ui->checkBox_27->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.samplestuff=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.samplestuff=?";
                 }
                 j++;

             }
             if(ui->checkBox_28->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.sampledate=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.sampledate=?";
                 }
                 j++;

             }
             if(ui->checkBox_29->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.sampletime=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.sampletime=?";
                 }
                 j++;

             }
             if(ui->checkBox_30->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.samplesituation=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.samplesituation=?";
                 }
                 j++;

             }
             if(ui->checkBox_31->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.samplemethod=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.samplemethod=?";
                 }
                 j++;

             }
             if(ui->checkBox_32->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.samplevolume=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.samplevolume=?";
                 }
                 j++;

             }
             if(ui->checkBox_33->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.sendstuff=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.sendstuff=?";
                 }
                 j++;

             }
             if(ui->checkBox_34->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.senddate=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.senddate=?";
                 }
                 j++;

             }
             if(ui->checkBox_35->isChecked())
             {
                 if(j==0)
                 {
                     SQL2=SQL2+"oilsampleinfo.sendtime=?";
                 }
                 else
                 {
                     SQL2=SQL2+" and oilsampleinfo.sendtime=?";
                 }
                 j++;

             }
     if(!createConnection(db1))//调用connection.h头文件中定义的createConnection函数连接数据库
     {
         QMessageBox::critical(0, qApp->tr("Cannot open database"),
                                       qApp->tr("Unable to establish a database connection."),
                                       QMessageBox::Cancel);
     }

     QSqlQuery query1;
     query1.prepare("insert into addtosql(uid,sqlcommand,key1,key2,key3,key4,key5,key6,key7,key8,key9,key10)" "values(:uid,:sqlcommand,:key1,:key2,:key3,:key4,:key5,:key6,:key7,:key8,:key9,:key10)");//SQL语句，向数据库中插入记录
             query1.bindValue(":uid",uid);//绑定值
             query1.bindValue(":sqlcommand",SQL2);
             query1.bindValue(":key1",arraysql[0]);
             query1.bindValue(":key2",arraysql[1]);
             query1.bindValue(":key3",arraysql[2]);
             query1.bindValue(":key4",arraysql[3]);
             query1.bindValue(":key5",arraysql[4]);
             query1.bindValue(":key6",arraysql[5]);
             query1.bindValue(":key7",arraysql[6]);
             query1.bindValue(":key8",arraysql[7]);
             query1.bindValue(":key9",arraysql[8]);
             query1.bindValue(":key10",arraysql[9]);
             /*query1.bindValue(":key1","飞行小时数");
             query1.bindValue(":key2","飞行小时数");
             query1.bindValue(":key3","飞行小时数");
             query1.bindValue(":key4","飞行小时数");
             query1.bindValue(":key5","运行阶段");
                          query1.bindValue(":key6","维修次数");
                          query1.bindValue(":key7","222");
                          query1.bindValue(":key8","222");
                          query1.bindValue(":key9","222");
                          query1.bindValue(":key10","222");*/
             if(!query1.exec())//若访问数据库没有运行
                 QMessageBox::warning(this,tr("failed"),tr("insert error"),QMessageBox::Close);
             else
             {
                  QMessageBox::warning(this,tr("success"),tr("insert success"),QMessageBox::Close);
             }
     db1.close();


}


