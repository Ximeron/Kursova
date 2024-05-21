#include "employeewindow.h"
#include "ui_employeewindow.h"
#include "DBT.h"
#include "session.h"

EmployeeWindow::EmployeeWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EmployeeWindow)
{
    ui->setupUi(this);
    loadEmployers();
    connect(ui->comboBoxEmployers, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EmployeeWindow::on_comboBoxEmployers_currentIndexChanged);

}

EmployeeWindow::~EmployeeWindow()
{
    delete ui;
}

void EmployeeWindow::loadEmployers()
{
    QSqlQuery query("SELECT id, Фамилия || ' ' || Имя || ' ' || Отчество AS ФИО FROM Employers");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        ui->comboBoxEmployers->addItem(name, id);
    }

    if (query.lastError().isValid()) {
        qDebug() << "Ошибка загрузки работодателей: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить работодателей.");
    }
}



void EmployeeWindow::loadVacancies(int employerId)
{
    ui->comboBoxVacancies->clear();

    QSqlQuery query;
    query.prepare("SELECT id, vacancy_name FROM Vacancies WHERE employer_id = :employer_id");
    query.bindValue(":employer_id", employerId);

    if (query.exec()) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString vacancyName = query.value(1).toString();
            ui->comboBoxVacancies->addItem(vacancyName, id);
        }
    } else {
        qDebug() << "Ошибка выполнения запроса: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить вакансии.");
    }
}

void EmployeeWindow::on_comboBoxEmployers_currentIndexChanged(int index)
{
    if (index == -1) {
        return;
    }

    int employerId = ui->comboBoxEmployers->currentData().toInt();
    loadVacancies(employerId);

}

void EmployeeWindow::on_applyButton_clicked()
{
    int employerId = ui->comboBoxEmployers->currentData().toInt();
    QString vacancyName = ui->comboBoxVacancies->currentText();
    int employeeId = Session::getInstance().getUserId();

    if (employerId == -1 || vacancyName.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, выберите работодателя и вакансию.");
        return;
    }

    // Проверка наличия записи в таблице Applications
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM Applications WHERE employee_id = :employee_id AND employer_id = :employer_id AND Должность = :vacancy_name");
    checkQuery.bindValue(":employee_id", employeeId);
    checkQuery.bindValue(":employer_id", employerId);
    checkQuery.bindValue(":vacancy_name", vacancyName);

    if (checkQuery.exec()) {
        if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this, "Внимание", "Вы уже подали заявку на эту вакансию.");
            return;
        }
    } else {
        qDebug() << "Ошибка выполнения запроса: " << checkQuery.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось проверить существование заявки.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Applications (employer_id, employee_id, Должность) VALUES (:employer_id, :employee_id, :vacancyName)");
    query.bindValue(":employee_id", employeeId);
    query.bindValue(":employer_id", employerId);
    query.bindValue(":vacancyName", vacancyName);

    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Заявка успешно подана.");
    } else {
        qDebug() << "Ошибка выполнения запроса: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось подать заявку.");
    }
}
