#include "DBT.h"
#include "session.h"

DBT::DBT() {}
bool DBT::Add_agent(QString surname,QString name,QString patronymic,QString phone){
    QSqlQuery admin;
    admin.prepare("INSERT INTO Agents (Фамилия, Имя, Отчество, Телефон) "
                  "VALUES (:surname, :name, :patronymic, :phone)");
    admin.bindValue(":surname", surname);
    admin.bindValue(":name", name);
    admin.bindValue(":patronymic", patronymic);
    admin.bindValue(":phone", phone);
    if (admin.exec()) return true;
    else return false;
}
bool DBT::Del_agent(int id){
    QSqlQuery query;
    query.prepare("DELETE FROM Agents WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec()) return true;
    else return false;
}
QSqlQuery DBT::AgTable(){ //
    QSqlQuery query("SELECT id, Фамилия, Имя, Отчество, Телефон FROM Agents");
    return query;
}
void DBT::Setup(){
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS Admins ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "Фамилия TEXT UNIQUE, "
               "Имя TEXT UNIQUE, "
               "Отчество TEXT UNIQUE, "
               "Телефон TEXT"
               ")");
    query.exec("CREATE TABLE IF NOT EXISTS Agents ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "Фамилия TEXT UNIQUE, "
               "Имя TEXT UNIQUE, "
               "Отчество TEXT UNIQUE, "
               "Телефон TEXT"
               ")");
    query.exec("CREATE TABLE IF NOT EXISTS Employers ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "Фамилия TEXT UNIQUE, "
               "Имя TEXT UNIQUE, "
               "Отчество TEXT UNIQUE, "
               "ИНН TEXT UNIQUE, "
               "Телефон TEXT"
               ")");
    query.exec("CREATE TABLE IF NOT EXISTS Employees ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "Фамилия TEXT UNIQUE, "
               "Имя TEXT UNIQUE, "
               "Отчество TEXT UNIQUE, "
               "Телефон TEXT"
               "Специальность TEXT, "
               "Стаж INTEGER"
               ")");
    query.exec("CREATE TABLE IF NOT EXISTS Acts ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "employer_id INTEGER, "
               "employee_id INTEGER, "
               "Должность TEXT, "
               "Комиссионные REAL, "
               "agree_flag INTEGER,"
               "FOREIGN KEY(employer_id) REFERENCES Employers(id), "
               "FOREIGN KEY(employee_id) REFERENCES Employees(id)"
               ")");
    query.exec("CREATE TABLE IF NOT EXISTS Applications ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "employer_id INTEGER, "
               "employee_id INTEGER, "
               "Должность TEXT, "
               "FOREIGN KEY(employer_id) REFERENCES Employers(id), "
               "FOREIGN KEY(employee_id) REFERENCES Employees(id)"
               ")");
    query.exec("CREATE TABLE IF NOT EXISTS Vacancies ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "vacancy_name TEXT, "
               "employer_id INTEGER, "
               "FOREIGN KEY(employer_id) REFERENCES Employers(id)"
               ")");

    query.exec("CREATE TRIGGER IF NOT EXISTS prevent_delete_employer "
               "BEFORE DELETE ON Employers "
               "FOR EACH ROW "
               "BEGIN "
               "SELECT CASE "
               "WHEN ((SELECT COUNT(*) FROM Vacancies WHERE employer_id = OLD.id) > 0) "
               "OR ((SELECT COUNT(*) FROM Applications WHERE Должность IN (SELECT id FROM Vacancies WHERE employer_id = OLD.id)) > 0) "
               "OR ((SELECT COUNT(*) FROM Acts WHERE employer_id = OLD.id) > 0) "
               "THEN "
               "RAISE(ABORT, 'Cannot delete employer with existing dependent records') "
               "END; "
               "END;");

    query.exec("CREATE TRIGGER IF NOT EXISTS prevent_delete_employee "
               "BEFORE DELETE ON Employees "
               "FOR EACH ROW "
               "BEGIN "
               "SELECT CASE "
               "WHEN ((SELECT COUNT(*) FROM Applications WHERE employee_id = OLD.id) > 0) "
               "OR ((SELECT COUNT(*) FROM Acts WHERE employee_id = OLD.id) > 0) "
               "THEN "
               "RAISE(ABORT, 'Cannot delete employee with existing dependent records') "
               "END; "
               "END;");

    query.exec("CREATE TRIGGER IF NOT EXISTS prevent_delete_vacancy "
               "BEFORE DELETE ON Vacancies "
               "FOR EACH ROW "
               "BEGIN "
               "SELECT CASE "
               "WHEN ((SELECT COUNT(*) FROM Applications WHERE Должность = OLD.id) > 0) "
               "OR ((SELECT COUNT(*) FROM Acts WHERE Должность = OLD.vacancy_name AND employer_id = OLD.employer_id) > 0) "
               "THEN "
               "RAISE(ABORT, 'Cannot delete vacancy with existing dependent records') "
               "END; "
               "END;");
}
bool DBT::AdminLogin(QString surname,QString name,QString patronymic){

    QSqlQuery admin;
    admin.prepare("SELECT id FROM Admins WHERE Имя = :name AND Фамилия = :surname AND Отчество = :patronymic");
    admin.bindValue(":name", name);
    admin.bindValue(":surname", surname);
    admin.bindValue(":patronymic", patronymic);

    if (admin.exec() && admin.next()) {
        int userId = admin.value(0).toInt();
        Session::getInstance().setUserId(userId);

        return true;
    }else {
        qDebug() << "Admin query failed:" << admin.lastError().text();
        return false;
    }

}
bool DBT::AgentLogin(QString surname,QString name,QString patronymic){
    QSqlQuery agent;
    agent.prepare("SELECT id FROM Agents WHERE Имя = :name AND Фамилия = :surname AND Отчество = :patronymic");
    agent.bindValue(":name", name);
    agent.bindValue(":surname", surname);
    agent.bindValue(":patronymic", patronymic);

    if (agent.exec() && agent.next()) {
        int userId = agent.value(0).toInt();
        Session::getInstance().setUserId(userId);
        return true;
    }else {
        qDebug() << "Admin query failed:" << agent.lastError().text();
        return false;
    }

}
bool DBT::EmpleeLogin(QString surname,QString name,QString patronymic){
    QSqlQuery emplee;
    emplee.prepare("SELECT id FROM Employees WHERE Имя = :name AND Фамилия = :surname AND Отчество = :patronymic");
    emplee.bindValue(":name", name);
    emplee.bindValue(":surname", surname);
    emplee.bindValue(":patronymic", patronymic);

    if (emplee.exec() && emplee.next()) {
        int userId = emplee.value(0).toInt();
        Session::getInstance().setUserId(userId);
        return true;
    }else {
        qDebug() << "Admin query failed:" << emplee.lastError().text();
        return false;
    }

}bool DBT::EmplerLogin(QString surname,QString name,QString patronymic){
    QSqlQuery empler;
    empler.prepare("SELECT id FROM Employers WHERE Имя = :name AND Фамилия = :surname AND Отчество = :patronymic");
    empler.bindValue(":name", name);
    empler.bindValue(":surname", surname);
    empler.bindValue(":patronymic", patronymic);

    if (empler.exec() && empler.next()) {
        int userId = empler.value(0).toInt();
        Session::getInstance().setUserId(userId);
        return true;
    }else {
        qDebug() << "Admin query failed:" << empler.lastError().text();
        return false;
    }
}
