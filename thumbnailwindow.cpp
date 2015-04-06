#include "thumbnailwindow.h"
#include "ui_thumbnailwindow.h"

ThumbnailWindow::ThumbnailWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThumbnailWindow)
{
    ui->setupUi(this);
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(ItemClicked(QListWidgetItem*)));
    initListWidget();
}
ThumbnailWindow::ThumbnailWindow(QWidget *parent, QStringList picidList, QStringList picpathList):
    QDialog(parent),
    ferrographypicidList(picidList),
    ferrographypicpathList(picpathList),
    ui(new Ui::ThumbnailWindow)
{
    ui->setupUi(this);
    initListWidget();
    initIcons();
}

void ThumbnailWindow::initList(QStringList picidList, QStringList picpathList,QStringList abmpicidList,QStringList abmpicpathList)
{
    ferrographypicidList =  picidList;
    ferrographypicpathList = picpathList;
    abrasivepicpathList = abmpicpathList;
    abrasiveidList = abmpicidList;
    initIcons();
}

void ThumbnailWindow::initIcons()
{
    int idx = 0;
    if(ferrographypicpathList.isEmpty())
        return;
    foreach(QString path,ferrographypicpathList)
    {
        QPixmap pixmap(path);
        QListWidgetItem *pitem  = new QListWidgetItem(QIcon(pixmap.scaled(QSize(W_ICONSIZE,H_ICONSIZE))),ferrographypicidList.at(idx));
//        QListWidgetItem *pitem  = new QListWidgetItem(QIcon(pixmap.scaled(QSize(W_ICONSIZE,H_ICONSIZE))),tr("缩略图"));
        pitem->setSizeHint(QSize(W_ICONSIZE,H_ICONSIZE));
        ui->listWidget->insertItem(idx,pitem);
        idx++;
    }
}

void ThumbnailWindow::initListWidget()
{
    ui->listWidget->setIconSize(QSize(W_ICONSIZE,H_ICONSIZE));
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setMovement(QListView::Static);
    ui->listWidget->setSpacing(10);
}

ThumbnailWindow::~ThumbnailWindow()
{
    delete ui;
}

void ThumbnailWindow::ItemClicked(QListWidgetItem *item)
{
    if(!item)
        return;
    int nRowIdx = ui->listWidget->row(item);
    
    QMessageBox::warning(this,tr("提示"),ferrographypicidList.at(nRowIdx),QMessageBox::Close);
}
