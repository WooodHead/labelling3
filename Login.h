#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT
    
public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    
private:
    Ui::Login *ui;

private:
    QString _username;
    QString _passwd;
    QString _authority;
    QString _display_name;

    bool _moving;
    QPoint _pos;
    QPoint _dpos;
    QPoint _mousePos;

    void	mouseMoveEvent ( QMouseEvent * event );
    void	mousePressEvent ( QMouseEvent * event );
    void	mouseReleaseEvent ( QMouseEvent * event );

public:
    void getDataForMainform();

private slots:
    void login();
    void loadDesign(QString strDesign = "default");

signals:
    // signal emitted when login is clicked
    void acceptLogin( QString& username, QString& password );
};

#endif // LOGIN_H
