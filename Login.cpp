#include "Login.h"
#include "ui_login.h"

#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QBitmap>
#include <QDesktopWidget>

#include "Global.h"
#include "Connection.h"
#include "imagecompletionui.h"
#include "Setting.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    customizeTitleBar();

    _moving = false;

    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());

    ui->_icon->resize(120, 60);
    QImage image;
    image.load(":/new/prefix1/icons/login_logo.png");
    ui->_icon->setPixmap(QPixmap::fromImage(image));
    ui->_icon->setScaledContents(true);

    ui->_editUsername->setPlaceholderText("用户名");
    ui->_editUsername->setStyleSheet("border: 2px solid #708090;height: 30px;");

    ui->_editPasswd->setPlaceholderText("密码");
    ui->_editPasswd->setStyleSheet("border: 2px solid #708090;height: 30px;");

    //ui->_login->setFocus();
    ui->_login->setIcon( Global::Awesome->icon(signin) );
    ui->_login->setStyleSheet("padding:5px 0;border:0px;background-color: #87CEEB;color:white;");

    ui->_cancel->setIcon( Global::Awesome->icon(remove_) );
    ui->_cancel->setStyleSheet("padding:5px 0;border:0px;background-color: #87CEEB;color:white;");

    ui->_login->setAutoDefault(false);
    ui->_cancel->setAutoDefault(false);

    QRect r = QApplication::desktop()->screenGeometry();
    this->move( r.center() - rect().center() );

    connect(ui->_login, SIGNAL(clicked()), this, SLOT(login()));
    connect(ui->_cancel, SIGNAL(clicked()), this, SLOT(close()));
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

    _settingButton->setStyleSheet("background-color:transparent;");
    _minButton->setStyleSheet("background-color:transparent;");
    _closeButton->setStyleSheet("background-color:transparent;");

    connect(_settingButton, SIGNAL(clicked()), this, SLOT(setting()));
    connect(_minButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(_closeButton, SIGNAL(clicked()), this, SLOT(close()));

    _label = new QLabel(this);
    _label->setText(tr("交互式磨粒图谱库构建系统"));

    setStyleSheet("QLabel{color:#CCCCCC;font-size:12px;font-weight:bold;} QToolButton{border:0px;}");
    _label->setStyleSheet("margin-left:6px;margin-top:5px;");

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(_label);
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
        QMessageBox::warning(this, tr(""), tr("用户名不能为空!"), QMessageBox::Close);
        return;
    }
    if(_passwd.isEmpty())
    {
        QMessageBox::warning(this, tr(""), tr("密码不能为空!"), QMessageBox::Close);
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

void Login::loadDesign(QString strDesign)
{
    // set stylesheet
    QFile file(QString("design/%1/Login/form.css").arg(strDesign));
    file.open(QFile::ReadOnly);
    this->ui->centralWidget->setStyleSheet(file.readAll());
}

void Login::setting()
{
    Setting* s = new Setting(this);
    s->show();
    this->setVisible(false);
}

void Login::showLogin()
{
    QRect r = QApplication::desktop()->screenGeometry();
    this->move( r.center() - rect().center() );
    this->setVisible(true);
}

void Login::paintEvent(QPaintEvent *event)
{
    QSize sz(width(), height());
    QBitmap objBitmap(sz);

    QPainter painter(&objBitmap);

    painter.fillRect(rect(),Qt::white);
    painter.setBrush(QColor(0,0,0));

    painter.drawRoundedRect(this->rect(),10,10);

    setMask(objBitmap);
}



