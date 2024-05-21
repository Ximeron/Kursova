#include "employerwindow.h"
#include "ui_employerwindow.h"
#include "DBT.h"
#include "session.h"


EmployerWindow::EmployerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EmployerWindow)
{
    ui->setupUi(this);
    setupTableWidget();

    loadApplications();
}

EmployerWindow::~EmployerWindow()
{
    delete ui;
}

void EmployerWindow::setupTableWidget()
{
    // Настройка столбцов для tableWidgetApplications
    ui->employertableWidget->setColumnCount(3);
    ui->employertableWidget->setHorizontalHeaderLabels(QStringList() << "id" << "Соискатель" << "Вакансия");
}

void EmployerWindow::loadApplications()
{
    ui->employertableWidget->setRowCount(0);
    int employerId = Session::getInstance().getUserId();

    QSqlQuery application;
    application.prepare("SELECT a.id, e.id AS employee_id, (e.Фамилия || ' ' || e.Имя || ' ' || e.Отчество) AS employee_name, a.Должность "
                  "FROM Applications a "
                  "JOIN Employees e ON a.employee_id = e.id "
                  "WHERE a.employer_id = :employer_id");
    application.bindValue(":employer_id", employerId);

    if (application.exec()) {
        int row = 0;
        while (application.next()) {
            ui->employertableWidget->insertRow(row);
            ui->employertableWidget->setItem(row, 0, new QTableWidgetItem(application.value("id").toString())); // application_id
            ui->employertableWidget->setItem(row, 1, new QTableWidgetItem(application.value("employee_id").toString())); // employee_name
            ui->employertableWidget->setItem(row, 2, new QTableWidgetItem(application.value("Должность").toString())); // vacancy_name
            row++;
        }
    } else {
        qDebug() << "Ошибка загрузки заявок: " << application.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить заявки.");
    }
}


void EmployerWindow::on_addVacancyButton_clicked()
{
    QString vacancyName = ui->lineEdit->text().trimmed();

    if (vacancyName.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, введите название вакансии.");
        return;
    }

    // Получаем ID текущего работодателя из сессии
    int employerId = Session::getInstance().getUserId();

    // Добавление вакансии в базу данных
    QSqlQuery query;
    query.prepare("INSERT INTO Vacancies (vacancy_name, employer_id) VALUES (:vacancy_name, :employer_id)");
    query.bindValue(":vacancy_name", vacancyName);
    query.bindValue(":employer_id", employerId);

    if (query.exec()) {
        QMessageBox::information(this, "Успех", "Вакансия успешно добавлена.");
        ui->lineEdit->clear();
    } else {
        qDebug() << "Ошибка выполнения запроса: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить вакансию.");
    }
}



void EmployerWindow::on_moveToActsButton_clicked()
{
    int currentRow = ui->employertableWidget->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, выберите запись для перемещения.");
        return;
    }

    int applicationId = ui->employertableWidget->item(currentRow, 0)->text().toInt();
    int employeeId = ui->employertableWidget->item(currentRow, 1)->text().toInt();
    QString vacancyName = ui->employertableWidget->item(currentRow, 2)->text();
    int employer_id = Session::getInstance().getUserId();
    QSqlQuery query;
    query.prepare("INSERT INTO Acts (employer_id, employee_id, Должность) VALUES (:employer_id, :employee_id, :vacancy)");
    query.bindValue(":employee_id", employeeId);
    query.bindValue(":vacancy", vacancyName);
    query.bindValue(":employer_id", employer_id);

    if (query.exec()) {
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM Applications WHERE id = :id");
        deleteQuery.bindValue(":id", applicationId);

        if (deleteQuery.exec()) {
            ui->employertableWidget->removeRow(currentRow);
            QMessageBox::information(this, "Успех", "Запись успешно перемещена.");
        } else {
            qDebug() << "Ошибка удаления записи: " << deleteQuery.lastError().text();
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить запись из таблицы Applications.");
        }
    } else {
        qDebug() << "Ошибка перемещения записи: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось переместить запись в таблицу Acts.");
    }
}
