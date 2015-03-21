#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>

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

    void    customizeTitleBar();

    QToolButton* _settingButton;
    QToolButton* _minButton;
    QToolButton* _closeButton;

    void loadDefaultConfigs();
    void setText(QString str, QLineEdit* edit);

public:
    void getDataForMainform();

private slots:
    void login();
    void setting();

    void on__buttonCancel2_clicked();
    void on__buttonSave_clicked();

    void on__buttonCopyTo_clicked();

    void on__buttonResultTo_clicked();

    void on__buttonMaskTo_clicked();

    void on__buttonTest_clicked();

signals:
    // signal emitted when login is clicked
    void acceptLogin( QString& username, QString& password );
};

#endif // LOGIN_H
