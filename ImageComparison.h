#ifndef IMAGECOMPARISON_H
#define IMAGECOMPARISON_H

#include <QDialog>
#include <QImage>
#include <QDebug>

namespace Ui {
class ImageComparison;
}

class ImageComparison : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImageComparison(QWidget *parent = 0);
    ~ImageComparison();
    
private: // members
    Ui::ImageComparison *ui;

    QByteArray _old;
    QByteArray _new;

public: // methods
    void setData(QByteArray old_, QByteArray new_);

signals:
    void toChangeId();
};

#endif // IMAGECOMPARISON_H
