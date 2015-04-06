#ifndef THUMBNAILWINDOW_H
#define THUMBNAILWINDOW_H

#include <QDialog>
#include <QStringList>
#include <QPixmap>
#include <QMessageBox>
#include <QListWidgetItem>

#include "Global.h"

const int W_ICONSIZE = 96; 
const int H_ICONSIZE = 96; 
const int H_ITEMSIZE = 110;

namespace Ui {
class ThumbnailWindow;
}

class ThumbnailWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit ThumbnailWindow(QWidget *parent = 0);
    ThumbnailWindow(QWidget *parent,QStringList picidList,QStringList picpathList);
    ~ThumbnailWindow();

    void initListWidget();
    void initIcons();
    void initList(QStringList picidList,QStringList picpathList,QStringList abmpicidList,QStringList abmpicpathList);
    
private slots:
    void ItemClicked(QListWidgetItem *item);
    
private:
    Ui::ThumbnailWindow *ui;
    
    QStringList ferrographypicidList;
    QStringList ferrographypicpathList;
    QStringList abrasivepicpathList;
    QStringList abrasiveidList;
};

#endif // THUMBNAILWINDOW_H
