#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "loginwindow.h" // Подключаем заголовочный файл LoginWindow

#include "DBT.h"
DBT adminDB;


AdminWindow::AdminWindow(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    ui->nameLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));
    ui->surnameLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));
    ui->patronymicLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я]+")));
    ui->phoneLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]+")));
    ui->agentsViewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabWidget->setCurrentIndex(0);
    populateAgentsTable();
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::on_addAgentButton_clicked()
{
    // Получаем данные агента из полей ввода
    QString surname = ui->surnameLineEdit->text();
    QString name = ui->nameLineEdit->text();
    QString patronymic = ui->patronymicLineEdit->text();
    QString phone = ui->phoneLineEdit->text();


    // Проверяем, что все поля заполнены
    if (surname.isEmpty() || name.isEmpty() || patronymic.isEmpty() || phone.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все обязательные поля");
        return;
    }

    // Выполняем запрос к базе данных для добавления агента
    bool chk = adminDB.Add_agent(surname, name, patronymic, phone);

    if (chk)
    {
        QMessageBox::information(this, "Успех", "Агент успешно добавлен");
        ui->surnameLineEdit->clear();
        ui->nameLineEdit->clear();
        ui->patronymicLineEdit->clear();
        ui->phoneLineEdit->clear();
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить агента");
    }
}

void AdminWindow::on_refreshButton_clicked()
{
    populateAgentsTable(); // Обновить таблицу
}

void AdminWindow::on_deleteButton_clicked()
{
    // Получаем выбранный элемент
    QTableWidgetItem* item = ui->agentsViewTable->currentItem();

    if (!item)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите запись для удаления");
        return;
    }

    int row = item->row(); // Получаем индекс строки
    int id = ui->agentsViewTable->item(row, 0)->text().toInt(); // Получаем ID агента
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", "Вы уверены, что хотите удалить запись?",
            QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (adminDB.Del_agent(id))
        {
            populateAgentsTable();
            QMessageBox::information(this, "Успех", "Запись успешно удалена");
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить запись");
        }
    }
}


void AdminWindow::populateAgentsTable()
{

    // Очищаем таблицу
    ui->agentsViewTable->clearContents();
    ui->agentsViewTable->setRowCount(0);
    ui->agentsViewTable->setColumnCount(5);
    ui->agentsViewTable->setHorizontalHeaderLabels(QStringList() << "id" << "Фамилия" << "Имя" << "Отчество" << "Телефон");

    // Выполняем запрос к базе данных для получения данных агентов
    QSqlQuery query = adminDB.AgTable();
    // Заполняем таблицу данными из запроса
    int row = 0;

    while (query.next())
    {
        ui->agentsViewTable->insertRow(row);
        ui->agentsViewTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // ID
        ui->agentsViewTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // Фамилия
        ui->agentsViewTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // Имя
        ui->agentsViewTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString())); // Отчество
        ui->agentsViewTable->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // Телефон
        row++;
    }
}

void AdminWindow::on_tabWidget_currentChanged(int index)
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

