#include "employerwindow.h"
#include "ui_employerwindow.h"
#include "DBT.h"
#include "session.h"
#include <QSqlTableModel>
DBT EmplerDB;


EmployerWindow::EmployerWindow(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::EmployerWindow),
    model(nullptr)
{
    ui->setupUi(this);
    setupTableWidget();
    ui->employertableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Яa-zA-Z ]+")));
    ui->tabWidget->setCurrentIndex(0);
    db = QSqlDatabase::database();
    vacLoad();

}

EmployerWindow::~EmployerWindow()
{
    delete ui;
    if (model) {
        delete model;
    }
}

void EmployerWindow::vacLoad(){
    model = new QSqlTableModel(this, db);
    model->setTable("Vacancies");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    int employerId = Session::getInstance().getUserId();

    QString filter = QString("employer_id = %1").arg(employerId);
    model->setFilter(filter);
    model->select();

    model->setHeaderData(1, Qt::Horizontal, tr("Вакансия"));
    ui->vacancyView->setModel(model);

    // Скрытие всех столбцов, кроме "vacancy_name"
    for (int i = 0; i < model->columnCount(); ++i) {
        if (i != 1) {  // Скрываем все столбцы, кроме второго (vacancy_name)
            ui->vacancyView->setColumnHidden(i, true);
        }
    }

    ui->vacancyView->resizeColumnsToContents();
    ui->vacancyView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->vacancyView->setSelectionMode(QAbstractItemView::SingleSelection);
}

void EmployerWindow::on_deleteVacancyButton_clicked()
{
    QModelIndexList selectedRows = ui->vacancyView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Selection Error", "Please select a vacancy to delete.");
        return;
    }

    int row = selectedRows.first().row();
    int id = model->data(model->index(row, 0)).toInt(); // Предполагается, что столбец 0 содержит ID вакансии

    QSqlQuery query;
    query.prepare("DELETE FROM Vacancies WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to delete vacancy: " + query.lastError().text());
        return;
    }

    model->removeRow(row);
    model->select(); // Перезагрузка данных
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

    QSqlQuery application = EmplerDB.EmployerTable(employerId);

    if (application.exec())
    {
        int row = 0;

        while (application.next())
        {
            ui->employertableWidget->insertRow(row);
            ui->employertableWidget->setItem(row, 0, new QTableWidgetItem(application.value("id").toString())); // application_id
            ui->employertableWidget->setItem(row, 1, new QTableWidgetItem(application.value("employee_id").toString())); // employee_name
            ui->employertableWidget->setItem(row, 2, new QTableWidgetItem(application.value("Должность").toString())); // vacancy_name
            row++;
        }
    }
    else
    {
        qDebug() << "Ошибка загрузки заявок: " << application.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить заявки.");
    }
}

bool EmployerWindow::vacancyExists(const QString& vacancyName, int employerId)
{

    QSqlQuery vac_exist = EmplerDB.Vacancies_exist(employerId, vacancyName);

    if (!vac_exist.exec())
    {
        qDebug() << "Database Error: " << vac_exist.lastError().text();
        return false; // Или выбросить исключение, если это критично
    }

    if (vac_exist.next())
    {
        return vac_exist.value(0).toInt() > 0;
    }

    return false;
}

void EmployerWindow::on_addVacancyButton_clicked()
{
    QString vacancyName = ui->lineEdit->text().trimmed();

    if (vacancyName.isEmpty())
    {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, введите название вакансии.");
        return;
    }

    // Получаем ID текущего работодателя из сессии
    int employerId = Session::getInstance().getUserId();


    if (vacancyExists(vacancyName, employerId))
    {
        QMessageBox::warning(this, "Duplicate Error", "Данная вакансия уже существует.");
        return;
    }

    // Добавление вакансии в базу данных
    QSqlQuery query = EmplerDB.Vacancy_Add(vacancyName, employerId);

    if (query.exec())
    {
        QMessageBox::information(this, "Успех", "Вакансия успешно добавлена.");
        ui->lineEdit->clear();
    }
    else
    {
        qDebug() << "Ошибка выполнения запроса: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить вакансию.");
    }
    model->select(); // Перезагрузка данных

}



void EmployerWindow::on_moveToActsButton_clicked()
{
    int currentRow = ui->employertableWidget->currentRow();

    if (currentRow == -1)
    {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, выберите запись для перемещения.");
        return;
    }

    int applicationId = ui->employertableWidget->item(currentRow, 0)->text().toInt();
    int employeeId = ui->employertableWidget->item(currentRow, 1)->text().toInt();
    QString vacancyName = ui->employertableWidget->item(currentRow, 2)->text();
    int employer_id = Session::getInstance().getUserId();

    QSqlQuery query = EmplerDB.Applications_move(employeeId, vacancyName, employer_id);

    if (query.exec())
    {
        QSqlQuery deleteQuery = EmplerDB.appDel(applicationId);

        if (deleteQuery.exec())
        {
            ui->employertableWidget->removeRow(currentRow);
            QMessageBox::information(this, "Успех", "Запись успешно перемещена.");
        }
        else
        {
            qDebug() << "Ошибка удаления записи: " << deleteQuery.lastError().text();
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить запись из таблицы Applications.");
        }
    }
    else
    {
        qDebug() << "Ошибка перемещения записи: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось переместить запись в таблицу Acts.");
    }

    loadApplications();
}

void EmployerWindow::on_tabWidget_tabBarClicked(int index)
{
    if (index == 2)
    {
        // Создаем объект LoginWindow
        LoginWindow* loginWindow = new LoginWindow();
        // Показываем окно LoginWindow
        loginWindow->show();
        // Закрываем текущее окно
        this->close();
    }
}

