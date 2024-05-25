#ifndef DBT_H
#define DBT_H
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "loginwindow.h" // Подключаем заголовочный файл LoginWindow



class DBT
{
    public:
        DBT();
        bool Add_agent(QString surname, QString name, QString patronymic, QString phone);
        bool Del_agent(int id);
        QSqlQuery AgTable();
        void Setup();

        bool AdminLogin(QString surname, QString name, QString patronymic);
        bool AgentLogin(QString surname, QString name, QString patronymic);

        bool EmpleeLogin(QString surname, QString name, QString patronymic);
        bool EmplerLogin(QString surname, QString name, QString patronymic);

        QSqlQuery EmployerTable(int employer_id);
        QSqlQuery Vacancies_exist(int employer_id, QString vacancyName);
        QSqlQuery Applications_move(int employeeId, QString vacancyName, int employer_id);
        QSqlQuery appDel(int app_id);
        QSqlQuery Vacancy_Add(QString vacancyName, int employerId);

        QSqlQuery addEmployer(QString employerName, QString employerSurname, QString employerPatronymic, QString employerINN, QString employerPhone);
        QSqlQuery addEmployee(QString employeeName, QString employeeSurname, QString employeePatronymic, QString employeeJobTime, QString employeePhone);
        QSqlQuery del_emplee(int id);
        QSqlQuery del_empler(int id);

        QSqlQuery pop_emplee();
        QSqlQuery pop_empler();



        QSqlQuery pop_acts();
        QSqlQuery setCommission(double commission, int actID);


        QSqlQuery chkApplication(int employeeId, int employerId, QString vacancyName);
        QSqlQuery addApplication(int employeeId, int employerId, QString vacancyName);


        QSqlQuery Emplee_split();

        QSqlQuery loadVacancy(int id);









    private:
};



#endif // DBT_H
