#include "agentwindow.h"
#include "ui_agentwindow.h"
#include "verificationdialog.h"
#include <QMainWindow>
#include "DBT.h"
DBT agentDB;
AgentWindow::AgentWindow(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::AgentWindow)
{
    ui->setupUi(this);
    ui->employerNameLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));
    ui->employerSurnameLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));
    ui->employerPatronymicLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));
    ui->employerINNLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]+")));
    ui->employerPhoneLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]+")));

    ui->employeeNameLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));
    ui->employeeSurnameLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));
    ui->employeePatronymicLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));
    ui->employeeJobTimeLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]+")));
    ui->employeePhoneLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]+")));

    ui->empleeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->emplerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    populateEmplsTable();
    loadActs();


}

AgentWindow::~AgentWindow()
{
    delete ui;

}


void AgentWindow::on_addEmployerButton_clicked()
{
    QString employerName = ui->employerNameLineEdit->text();
    QString employerSurname = ui->employerSurnameLineEdit->text();
    QString employerPatronymic = ui->employerPatronymicLineEdit->text();
    QString employerINN = ui->employerINNLineEdit->text();
    QString employerPhone = ui->employerPhoneLineEdit->text();



    if (employerName.isEmpty() || employerSurname.isEmpty() || employerPatronymic.isEmpty() || employerINN.isEmpty() || employerPhone.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля");
        return;
    }

    QSqlQuery query = agentDB.addEmployer(employerName, employerSurname, employerPatronymic, employerINN, employerPhone);

    if (query.exec())
    {
        QMessageBox::information(this, "Успех", "Работодатель успешно добавлен");
        ui->employerNameLineEdit->clear();
        ui->employerSurnameLineEdit->clear();
        ui->employerPatronymicLineEdit->clear();
        ui->employerINNLineEdit->clear();
        ui->employerPhoneLineEdit->clear();
        populateEmplsTable();

    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить работодателя");
    }
}

void AgentWindow::on_addEmployeeButton_clicked()
{
    QString employeeName = ui->employeeNameLineEdit->text();
    QString employeeSurname = ui->employeeSurnameLineEdit->text();
    QString employeePatronymic = ui->employeePatronymicLineEdit->text();
    QString employeeJobTime = ui->employeeJobTimeLineEdit->text();
    QString employeePhone = ui->employeePhoneLineEdit->text();

    if (employeeName.isEmpty() || employeeSurname.isEmpty() || employeePatronymic.isEmpty() || employeeJobTime.isEmpty() || employeePhone.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля");
        return;
    }

    QSqlQuery query = agentDB.addEmployee(employeeName, employeeSurname, employeePatronymic, employeeJobTime, employeePhone);

    if (query.exec())
    {
        QMessageBox::information(this, "Успех", "Соискатель успешно добавлен");
        ui->employeeNameLineEdit->clear();
        ui->employeeSurnameLineEdit->clear();
        ui->employeePatronymicLineEdit->clear();
        ui->employeeJobTimeLineEdit->clear();
        ui->employeePhoneLineEdit->clear();
        populateEmplsTable();

    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить соискателя");
    }
}

void AgentWindow::on_refreshEmpls_clicked()
{
    populateEmplsTable(); // Обновить таблицу
}

void AgentWindow::on_deleteEmplee_clicked()
{
    // Получаем выбранный элемент
    QTableWidgetItem* item = ui->empleeTable->currentItem();

    if (!item)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите запись для удаления");
        return;
    }

    int row = item->row(); // Получаем индекс строки
    int id = ui->empleeTable->item(row, 0)->text().toInt(); // Получаем ID агента

    // Выполняем запрос к базе данных для удаления агента
    QSqlQuery emplee_query = agentDB.del_emplee(id);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", "Вы уверены, что хотите удалить запись?",
            QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (emplee_query.exec())
        {
            populateEmplsTable();
            QMessageBox::information(this, "Успех", "Запись успешно удалена");
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить запись");
        }
    }

}

void AgentWindow::on_deleteEmpler_clicked()
{
    // Получаем выбранный элемент
    QTableWidgetItem* item = ui->emplerTable->currentItem();

    if (!item)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите запись для удаления");
        return;
    }


    int row = item->row(); // Получаем индекс строки
    int id = ui->emplerTable->item(row, 0)->text().toInt(); // Получаем ID агента

    // Выполняем запрос к базе данных для удаления агента
    QSqlQuery empler_query = agentDB.del_empler(id);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", "Вы уверены, что хотите удалить запись?",
            QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (empler_query.exec())
        {
            populateEmplsTable();
            QMessageBox::information(this, "Успех", "Запись успешно удалена");
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить запись");
        }
    }

}

void AgentWindow::populateEmplsTable()
{
    // Очищаем таблицу
    ui->empleeTable->clearContents();
    ui->empleeTable->setRowCount(0);

    ui->emplerTable->clearContents();
    ui->emplerTable->setRowCount(0);

    // Выполняем запрос к базе данных для получения данных агентов
    QSqlQuery emplee = agentDB.pop_emplee();
    QSqlQuery empler = agentDB.pop_empler();

    // Заполняем таблицу данными из запроса
    int row1 = 0;

    while (emplee.next())
    {
        ui->empleeTable->insertRow(row1);
        ui->empleeTable->setItem(row1, 0, new QTableWidgetItem(emplee.value(0).toString())); // ID
        ui->empleeTable->setItem(row1, 1, new QTableWidgetItem(emplee.value(1).toString())); // Фамилия
        ui->empleeTable->setItem(row1, 2, new QTableWidgetItem(emplee.value(2).toString())); // Имя
        ui->empleeTable->setItem(row1, 3, new QTableWidgetItem(emplee.value(3).toString())); // Отчество
        ui->empleeTable->setItem(row1, 4, new QTableWidgetItem(emplee.value(4).toString())); // Стаж
        ui->empleeTable->setItem(row1, 5, new QTableWidgetItem(emplee.value(5).toString())); // Телефон
        row1++;
    }

    int row2 = 0;

    while (empler.next())
    {
        ui->emplerTable->insertRow(row2);
        ui->emplerTable->setItem(row2, 0, new QTableWidgetItem(empler.value(0).toString())); // ID
        ui->emplerTable->setItem(row2, 1, new QTableWidgetItem(empler.value(1).toString())); // Фамилия
        ui->emplerTable->setItem(row2, 2, new QTableWidgetItem(empler.value(2).toString())); // Имя
        ui->emplerTable->setItem(row2, 3, new QTableWidgetItem(empler.value(3).toString())); // Отчество
        ui->emplerTable->setItem(row2, 4, new QTableWidgetItem(empler.value(5).toString())); // ИНН
        ui->emplerTable->setItem(row2, 5, new QTableWidgetItem(empler.value(6).toString())); // Телефон
        row2++;
    }
}


void AgentWindow::loadActs()
{
    ui->tableWidgetActs->clearContents();
    ui->tableWidgetActs->setRowCount(0);
    ui->tableWidgetActs->setColumnCount(6);
    ui->tableWidgetActs->setHorizontalHeaderLabels(QStringList() << "id" << "Работодатель" << "Соискатель" << "Должность" << "Комиссионные" << "Подтверждено?");
    QSqlQuery query = agentDB.pop_acts();

    if (query.exec())
    {
        int row = 0;

        while (query.next())
        {
            ui->tableWidgetActs->insertRow(row);
            ui->tableWidgetActs->setItem(row, 0, new QTableWidgetItem(query.value("id").toString())); // act_id
            ui->tableWidgetActs->setItem(row, 1, new QTableWidgetItem(query.value("employer_id").toString())); // employee_id
            ui->tableWidgetActs->setItem(row, 2, new QTableWidgetItem(query.value("employee_id").toString())); // vacancy_name
            ui->tableWidgetActs->setItem(row, 3, new QTableWidgetItem(query.value("Должность").toString())); // commission
            ui->tableWidgetActs->setItem(row, 4, new QTableWidgetItem(query.value("Комиссионные").toString())); // commission
            ui->tableWidgetActs->setItem(row, 5, new QTableWidgetItem(query.value("agree_flag").toBool() ? "Да" : "Нет")); // agree_flag
            row++;
        }
    }
    else
    {
        qDebug() << "Ошибка загрузки актов: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить акты.");
    }
}

void AgentWindow::on_verifyActButton_clicked()
{
    int currentRow = ui->tableWidgetActs->currentRow();

    if (currentRow == -1)
    {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, выберите акт для верификации.");
        return;
    }

    bool ok;
    int actId = ui->tableWidgetActs->item(currentRow, 0)->text().toInt(&ok);

    if (!ok)
    {
        QMessageBox::critical(this, "Ошибка", "Ошибка преобразования ID акта.");
        return;
    }

    VerificationDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        double commission = dialog.getCommission();

        QSqlQuery query = agentDB.setCommission(commission, actId);

        if (commission > 0)
        {
            if (query.exec())
            {
                QMessageBox::information(this, "Успех", "Акт успешно верифицирован.");
                loadActs();
            }
            else
            {
                qDebug() << "Ошибка верификации акта: " << query.lastError().text();
                QMessageBox::critical(this, "Ошибка", "Не удалось верифицировать акт.");
            }
        }
    }
}

void AgentWindow::on_exitButton_clicked()
{
    // Создаем объект LoginWindow
    LoginWindow* loginWindow = new LoginWindow();
    // Показываем окно LoginWindow
    loginWindow->show();
    // Закрываем текущее окно
    this->close();
}
