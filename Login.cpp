#include "Login.h"
#include "ui_login.h"

#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QBitmap>
#include <QDesktopWidget>
#include <QDebug>

#include "Global.h"
#include "Connection.h"
#include "imagecompletionui.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    customizeTitleBar();

    _moving = false;

    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());

    ui->_icon->resize(180, 90);
    ui->_icon_2->resize(180, 90);

    QImage image;
    image.load(":/new/prefix1/icons/login_logo.png");
    ui->_icon->setPixmap(QPixmap::fromImage(image));
    ui->_icon->setScaledContents(true);

    QImage image2;
    image2.load(":/new/prefix1/icons/login_logo2.jpg");
    ui->_icon_2->setPixmap(QPixmap::fromImage(image2));
    ui->_icon_2->setScaledContents(true);

    ui->_login->setStyleSheet("background: #99cccc; color: black;");
    ui->_cancel->setStyleSheet("background: #999999; color: black;");
    connect(ui->_login, SIGNAL(clicked()), this, SLOT(login()));
    connect(ui->_cancel, SIGNAL(clicked()), this, SLOT(close()));

    ui->_login->setAttribute(Qt::WA_TranslucentBackground);
    ui->_cancel->setAttribute(Qt::WA_TranslucentBackground);
    ui->_labelMainTitle->setAttribute(Qt::WA_TranslucentBackground);
    ui->_labelUserName->setAttribute(Qt::WA_TranslucentBackground);
    ui->_labelPasswd->setAttribute(Qt::WA_TranslucentBackground);
    ui->_icon->setAttribute(Qt::WA_TranslucentBackground);
    ui->_icon_2->setAttribute(Qt::WA_TranslucentBackground);
    ui->centralStackedWidget->setAttribute(Qt::WA_TranslucentBackground);
    ui->centralStackedWidgetPage1->setAttribute(Qt::WA_TranslucentBackground);
    ui->centralStackedWidgetPage2->setAttribute(Qt::WA_TranslucentBackground);

    ui->label->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_2->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_3->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_4->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_5->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_6->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_7->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_8->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_9->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_10->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_11->setAttribute(Qt::WA_TranslucentBackground);
    ui->_labelStatus->setAttribute(Qt::WA_TranslucentBackground);
    ui->_comboBoxMask->setAttribute(Qt::WA_TranslucentBackground);
    ui->_comboBoxResult->setAttribute(Qt::WA_TranslucentBackground);

    ui->_buttonTest->setStyleSheet("background: #009966; color: black;");
    ui->_buttonSave->setStyleSheet("background: #99cccc; color: black;");
    ui->_buttonCancel2->setStyleSheet("background: #999999; color: black;");
    ui->_buttonCopyTo->setStyleSheet("background: #cccccc; color: black;");
    ui->_buttonResultTo->setStyleSheet("background: #cccccc; color: black;");
    ui->_buttonMaskTo->setStyleSheet("background: #cccccc; color: black;");

    ui->_comboBoxMask->addItems(QStringList() << "jpg" << "png" << "bmp");
    ui->_comboBoxResult->addItems(QStringList() << "jpg" << "png" << "bmp");


    QRect r = QApplication::desktop()->screenGeometry();
    this->move( r.center() - rect().center() );

    //this->setStyleSheet("background-image: url(:/new/prefix1/icons/login_bg.jpg);");


    ui->centralStackedWidget->setCurrentIndex(0);
}

Login::~Login()
{
    delete ui;
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    if(_moving)
    {
        this->move(event->globalPos() - _dpos);
    }
}

void Login::mousePressEvent(QMouseEvent *event)
{
    _moving = true;

    _pos = this->pos();
    _mousePos = event->globalPos();
    _dpos = _mousePos - _pos;
}

void Login::mouseReleaseEvent(QMouseEvent */*event*/)
{
    _moving = false;
}

void Login::customizeTitleBar()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);

    int width = this->width();
    _settingButton = new QToolButton(this);
    _minButton = new QToolButton(this);
    _closeButton = new QToolButton(this);

    _settingButton->setIcon(Global::Awesome->icon(gear));
    _minButton->setIcon(Global::Awesome->icon(minus));
    _closeButton->setIcon(Global::Awesome->icon(times));

    _settingButton->setGeometry(width-67,5,20,20);
    _minButton->setGeometry(width-46,5,20,20);
    _closeButton->setGeometry(width-25,5,20,20);

    _settingButton->setStyleSheet("background-color:rgba(0,0,0,0%)");
    _minButton->setStyleSheet("background-color:transparent;");
    _closeButton->setStyleSheet("background-color:transparent;");

    connect(_settingButton, SIGNAL(clicked()), this, SLOT(setting()));
    connect(_minButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(_closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(_settingButton);
    layout->addWidget(_minButton);
    layout->addWidget(_closeButton);

    layout->setSpacing(0);
    setLayout(layout);
}

void Login::login()
{
    _username = ui->_editUsername->text();
    _passwd   = ui->_editPasswd->text();

    if(_username.isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("用户名不能为空!"), QMessageBox::Close);
        return;
    }
    if(_passwd.isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("密码不能为空!"), QMessageBox::Close);
        return;
    }

    QSqlDatabase db;
    if(!createConnection(db))
    {
        QMessageBox::critical(0, qApp->tr("提示"),
                              qApp->tr("数据库连接失败!"),
                              QMessageBox::Cancel);
        return;
    }

    QString sql = "select * from user where name = ? and passwd = ?";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(0, _username);
    query.bindValue(1, _passwd);

    if(query.exec())
    {
        if(query.size() == 0)
        {
            QMessageBox::warning(this, tr("失败"), tr("登录失败,用户名或密码不正确!"), QMessageBox::Close);
        }
        else
        {
            if(query.next())
            {
                int no1 = query.record().indexOf("authority");
                _authority = query.value(no1).toString();

                QString connection = db.connectionName();
                db.close();
                QSqlDatabase::removeDatabase(connection);
                return getDataForMainform();
            }
        }
    }
    else  {
        QMessageBox::warning(this,tr("失败"),tr("查询用户信息失败"),QMessageBox::Close);
        return;
    }
}

void Login::getDataForMainform()
{
    Global::Authority = _authority;

    (new ImageCompletionUI)->show();
    this->deleteLater();
}

void Login::setting()
{
    ui->centralStackedWidget->setCurrentIndex(1);
    _settingButton->hide();

    loadDefaultConfigs();
}


void Login::loadDefaultConfigs()
{
    ui->_editIP->setText(Global::Hostname);
    ui->_editUsername_2->setText(Global::Username);
    ui->_editPasswd_2->setText(Global::Passwd);
    ui->_editDatabase->setText(Global::Database);

    ui->_editOriginalCopyTo->setText(Global::PathImage);
    ui->_editResultTo->setText(Global::PathResult);
    ui->_editMaskTo->setText(Global::PathMask);

    qDebug() << Global::ExtResult;
    int index = ui->_comboBoxResult->findText(Global::ExtResult);
    ui->_comboBoxResult->setCurrentIndex(index);
    index = ui->_comboBoxMask->findText(Global::ExtMask);
    ui->_comboBoxMask->setCurrentIndex(index);
}

void Login::on__buttonCancel2_clicked()
{
    ui->centralStackedWidget->setCurrentIndex(0);
    _settingButton->show();
}

void Login::on__buttonSave_clicked()
{
    Global::settings->setValue("MYSQL/databasename", ui->_editDatabase->text());
    Global::settings->setValue("MYSQL/hostname", ui->_editIP->text());
    Global::settings->setValue("MYSQL/username", ui->_editUsername_2->text());
    Global::settings->setValue("MYSQL/passwd", ui->_editPasswd_2->text());

    Global::settings->setValue("IMAGE/pathImage", ui->_editOriginalCopyTo->text());
    Global::settings->setValue("IMAGE/pathMask", ui->_editMaskTo->text());
    Global::settings->setValue("IMAGE/pathResult", ui->_editResultTo->text());
    Global::settings->setValue("IMAGE/extMask", ui->_comboBoxMask->currentText());
    Global::settings->setValue("IMAGE/extResult", ui->_comboBoxResult->currentText());

    ui->centralStackedWidget->setCurrentIndex(0);
    _settingButton->show();
}

void Login::on__buttonCopyTo_clicked()
{
    setText(tr("设置原图像拷贝文件夹"), ui->_editOriginalCopyTo);
}

void Login::on__buttonResultTo_clicked()
{
    setText(tr("设置标注结果保存文件夹"), ui->_editResultTo);
}

void Login::on__buttonMaskTo_clicked()
{
    setText(tr("设置掩码图像保存文件夹"), ui->_editMaskTo);
}

void Login::setText(QString str, QLineEdit* edit)
{
    QFileDialog dlg;
    dlg.setFileMode(QFileDialog::Directory);
    dlg.setOption(QFileDialog::ShowDirsOnly);
    dlg.setViewMode(QFileDialog::Detail);
    dlg.setWindowTitle(str);

    if(dlg.exec() == QDialog::Accepted)
    {
        QDir dir = dlg.selectedFiles()[0];
        edit->setText( dir.absolutePath());
    }
}

void Login::on__buttonTest_clicked()
{
    QSqlDatabase db;
    if(!createConnection(db))
    {
        ui->_labelStatus->setText(tr("连接失败!"));
    }
    else
    {
        ui->_labelStatus->setText(tr("连接成功!"));
    }
}
