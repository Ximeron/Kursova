#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "adminwindow.h"
#include "agentwindow.h"
#include "employerwindow.h"
#include "employeewindow.h"
#include <QMessageBox>
#include "DBT.h"
#include "session.h"
DBT loginDB;


LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->nameLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));
    ui->surnameLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));
    ui->patronymicLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Office.db");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database";
        throw std::runtime_error("Error: Unable to open database");
    } else {
        qDebug() << "Database opened successfully";
    }
    loginDB.Setup();

}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::on_loginButton_clicked()
{
    QString name = ui->nameLineEdit->text();
    QString surname = ui->surnameLineEdit->text();
    QString patronymic = ui->patronymicLineEdit->text();
    if (loginDB.AdminLogin(surname,name,patronymic)) {
        AdminWindow *adminWindow = new AdminWindow();
        adminWindow->show();
        this->close();
        return;
    }
    if (loginDB.AgentLogin(surname,name,patronymic)) {
        AgentWindow *agentWindow = new AgentWindow();
        agentWindow->show();
        this->close();
        return;
    }
    if (loginDB.EmpleeLogin(surname,name,patronymic)) {
        EmployeeWindow *employeeWindow = new EmployeeWindow();
        employeeWindow->show();
        this->close();
        return;
    }
    if (loginDB.EmplerLogin(surname,name,patronymic)) {
        EmployerWindow *employerWindow = new EmployerWindow();
        employerWindow->show();
        this->close();
        return;
    }
    QMessageBox::warning(this, "Ошибка", "Неверное имя пользователя или пароль");
}
