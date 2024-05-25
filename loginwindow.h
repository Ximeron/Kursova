#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui
{
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QDialog
{
        Q_OBJECT

    public:
        LoginWindow(QWidget* parent = nullptr);
        ~LoginWindow();

    private slots:
        void on_loginButton_clicked();

    private:
        Ui::LoginWindow* ui;
        QSqlDatabase db;

};
#endif // LOGINWINDOW_H
