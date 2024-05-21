#ifndef DBT_H
#define DBT_H
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>


class DBT
{
public:
    DBT();
    bool Add_agent(QString surname,QString name,QString patronymic,QString phone);
    bool Del_agent(int id);
    QSqlQuery AgTable();
    void Setup();

    bool AdminLogin(QString surname,QString name,QString patronymic);
    bool AgentLogin(QString surname,QString name,QString patronymic);
    bool EmpleeLogin(QString surname,QString name,QString patronymic);
    bool EmplerLogin(QString surname,QString name,QString patronymic);


private:
};



#endif // DBT_H
