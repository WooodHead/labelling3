#ifndef IMAGEPROPERTIES_H
#define IMAGEPROPERTIES_H

#include <QDialog>

namespace Ui {
class ImageProperties;
}

class ImageProperties : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImageProperties(QWidget *parent = 0);
    ~ImageProperties();
    
private:
    Ui::ImageProperties *ui;
};

#endif // IMAGEPROPERTIES_H
