#include "DBT.h"
#include "session.h"

DBT::DBT() {}
bool DBT::Add_agent(QString surname, QString name, QString patronymic, QString phone)
{
    QSqlQuery admin;
    admin.prepare("INSERT INTO Agents (Фамилия, Имя, Отчество, Телефон) "
            "VALUES (:surname, :name, :patronymic, :phone)");
    admin.bindValue(":surname", surname);
    admin.bindValue(":name", name);
    admin.bindValue(":patronymic", patronymic);
    admin.bindValue(":phone", phone);

    return admin.exec();
}
bool DBT::Del_agent(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Agents WHERE id = :id");
    query.bindValue(":id", id);

    return query.exec();

}
QSqlQuery DBT::AgTable()  //
{
    QSqlQuery query("SELECT id, Фамилия, Имя, Отчество, Телефон FROM Agents");
    return query;
}
void DBT::Setup()
{
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
            "RAISE(ABORT, 'Нельзя удалить работодателя, т.к. он участвует в системе и уже используется') "
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
            "RAISE(ABORT, 'Нельзя удалить соискателя, т.к. он участвует в системе и уже используется') "
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
            "RAISE(ABORT, 'Нельзя удалить вакансию которая участвует в системе и уже используется') "
            "END; "
            "END;");
}
bool DBT::AdminLogin(QString surname, QString name, QString patronymic)
{

    QSqlQuery admin;
    admin.prepare("SELECT id FROM Admins WHERE Имя = :name AND Фамилия = :surname AND Отчество = :patronymic");
    admin.bindValue(":name", name);
    admin.bindValue(":surname", surname);
    admin.bindValue(":patronymic", patronymic);

    if (admin.exec() && admin.next())
    {
        int userId = admin.value(0).toInt();
        Session::getInstance().setUserId(userId);

        return true;
    }
    else
    {
        qDebug() << "Admin query failed:" << admin.lastError().text();
        return false;
    }

}
bool DBT::AgentLogin(QString surname, QString name, QString patronymic)
{
    QSqlQuery agent;
    agent.prepare("SELECT id FROM Agents WHERE Имя = :name AND Фамилия = :surname AND Отчество = :patronymic");
    agent.bindValue(":name", name);
    agent.bindValue(":surname", surname);
    agent.bindValue(":patronymic", patronymic);

    if (agent.exec() && agent.next())
    {
        int userId = agent.value(0).toInt();
        Session::getInstance().setUserId(userId);
        return true;
    }
    else
    {
        qDebug() << "Agent query failed:" << agent.lastError().text();
        return false;
    }

}
bool DBT::EmpleeLogin(QString surname, QString name, QString patronymic)
{
    QSqlQuery emplee;
    emplee.prepare("SELECT id FROM Employees WHERE Имя = :name AND Фамилия = :surname AND Отчество = :patronymic");
    emplee.bindValue(":name", name);
    emplee.bindValue(":surname", surname);
    emplee.bindValue(":patronymic", patronymic);

    if (emplee.exec() && emplee.next())
    {
        int userId = emplee.value(0).toInt();
        Session::getInstance().setUserId(userId);
        return true;
    }
    else
    {
        qDebug() << "Employee query failed:" << emplee.lastError().text();
        return false;
    }

}
bool DBT::EmplerLogin(QString surname, QString name, QString patronymic)
{
    QSqlQuery empler;
    empler.prepare("SELECT id FROM Employers WHERE Имя = :name AND Фамилия = :surname AND Отчество = :patronymic");
    empler.bindValue(":name", name);
    empler.bindValue(":surname", surname);
    empler.bindValue(":patronymic", patronymic);

    if (empler.exec() && empler.next())
    {
        int userId = empler.value(0).toInt();
        Session::getInstance().setUserId(userId);
        return true;
    }
    else
    {
        qDebug() << "Employer query failed:" << empler.lastError().text();
        return false;
    }
}

QSqlQuery DBT::EmployerTable(int employerId)
{
    QSqlQuery application;
    application.prepare("SELECT a.id, e.id AS employee_id, (e.Фамилия || ' ' || e.Имя || ' ' || e.Отчество) AS employee_name, a.Должность "
            "FROM Applications a "
            "JOIN Employees e ON a.employee_id = e.id "
            "WHERE a.employer_id = :employer_id");
    application.bindValue(":employer_id", employerId);
    return application;
}

QSqlQuery DBT::Vacancies_exist(int employer_id, QString vacancyName)
{
    QSqlQuery vac_exist;
    vac_exist.prepare("SELECT COUNT(*) FROM Vacancies WHERE vacancy_name = :vacancy_name AND employer_id = :employer_id");
    vac_exist.bindValue(":vacancy_name", vacancyName);
    vac_exist.bindValue(":employer_id", employer_id);

    return vac_exist;
}

QSqlQuery DBT::Applications_move(int employeeId, QString vacancyName, int employer_id)
{
    QSqlQuery app;
    app.prepare("INSERT INTO Acts (employer_id, employee_id, Должность) VALUES (:employer_id, :employee_id, :vacancy)");
    app.bindValue(":employee_id", employeeId);
    app.bindValue(":vacancy", vacancyName);
    app.bindValue(":employer_id", employer_id);
    return app;
}

QSqlQuery DBT::Vacancy_Add(QString vacancyName, int employerId)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Vacancies (vacancy_name, employer_id) VALUES (:vacancy_name, :employer_id)");
    query.bindValue(":vacancy_name", vacancyName);
    query.bindValue(":employer_id", employerId);
    return query;
}

QSqlQuery DBT::appDel(int app_id)
{
    QSqlQuery appdel;
    appdel.prepare("DELETE FROM Applications WHERE id = :id");
    appdel.bindValue(":id", app_id);
    return appdel;
}

QSqlQuery DBT::addEmployer(QString employerName, QString employerSurname, QString employerPatronymic, QString employerINN, QString employerPhone)
{

    QSqlQuery query;
    query.prepare("INSERT INTO Employers (Фамилия, Имя, Отчество, ИНН, Телефон) VALUES (:employerSurname, :employerName, :employerPatronymic, :employerINN, :employerPhone)");
    query.bindValue(":employerName", employerName);
    query.bindValue(":employerSurname", employerSurname);
    query.bindValue(":employerPatronymic", employerPatronymic);
    query.bindValue(":employerINN", employerINN);
    query.bindValue(":employerPhone", employerPhone);
    return query;
}

QSqlQuery DBT::addEmployee(QString employeeName, QString employeeSurname, QString employeePatronymic, QString employeeJobTime, QString employeePhone)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Employees (Имя, Фамилия, Отчество, Стаж, Телефон) VALUES (:employeeName, :employeeSurname, :employeePatronymic, :employeeJobTime, :employeePhone)");
    query.bindValue(":employeeName", employeeName);
    query.bindValue(":employeeSurname", employeeSurname);
    query.bindValue(":employeePatronymic", employeePatronymic);
    query.bindValue(":employeeJobTime", employeeJobTime);
    query.bindValue(":employeePhone", employeePhone);
    return query;

}

QSqlQuery DBT::pop_emplee()
{
    QSqlQuery emplee("SELECT id, Фамилия, Имя, Отчество, Телефон, Стаж FROM Employees");
    return emplee;
}
QSqlQuery DBT::pop_empler()
{
    QSqlQuery empler("SELECT id, Фамилия, Имя, Отчество, ИНН, Телефон FROM Employers");
    return empler;
}
QSqlQuery DBT::del_emplee(int id)
{
    QSqlQuery emplee_query;
    emplee_query.prepare("DELETE FROM Employees WHERE id = :id");
    emplee_query.bindValue(":id", id);
    return emplee_query;
}
QSqlQuery DBT::del_empler(int id)
{
    QSqlQuery empler_query;
    empler_query.prepare("DELETE FROM Employers WHERE id = :id");
    empler_query.bindValue(":id", id);
    return empler_query;
}



QSqlQuery DBT::pop_acts()
{
    QSqlQuery acts("SELECT id, employer_id, employee_id, Должность, Комиссионные, agree_flag FROM Acts");
    return acts;
}
QSqlQuery DBT::setCommission(double commission, int actID)
{
    QSqlQuery com;
    com.prepare("UPDATE Acts SET Комиссионные = :commission, agree_flag = :agree_flag WHERE id = :id");
    com.bindValue(":commission", commission);
    com.bindValue(":agree_flag", 1); // Устанавливаем agree_flag в true
    com.bindValue(":id", actID);
    return com;
}

QSqlQuery DBT::Emplee_split()
{
    QSqlQuery split("SELECT id, Фамилия || ' ' || Имя || ' ' || Отчество AS ФИО FROM Employers");
    return split;
}


QSqlQuery DBT::loadVacancy(int employerId)
{
    QSqlQuery query;
    query.prepare("SELECT id, vacancy_name FROM Vacancies WHERE employer_id = :employer_id");
    query.bindValue(":employer_id", employerId);
    return query;
}

QSqlQuery DBT::chkApplication(int employeeId, int employerId, QString vacancyName)
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM Applications WHERE employee_id = :employee_id AND employer_id = :employer_id AND Должность = :vacancy_name");
    checkQuery.bindValue(":employee_id", employeeId);
    checkQuery.bindValue(":employer_id", employerId);
    checkQuery.bindValue(":vacancy_name", vacancyName);
    return checkQuery;
}



QSqlQuery DBT::addApplication(int employeeId, int employerId, QString vacancyName)
{
    QSqlQuery addApp;
    addApp.prepare("INSERT INTO Applications (employer_id, employee_id, Должность) VALUES (:employer_id, :employee_id, :vacancyName)");
    addApp.bindValue(":employee_id", employeeId);
    addApp.bindValue(":employer_id", employerId);
    addApp.bindValue(":vacancyName", vacancyName);
    return addApp;
}

QSqlQuery DBT::DelAct(int id)
{
    QSqlQuery DelAct;
    DelAct.prepare("DELETE FROM Acts WHERE id = :id");
    DelAct.bindValue(":id", id);

    return DelAct;
}
