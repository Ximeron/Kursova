#include "employeewindow.h"
#include "ui_employeewindow.h"
#include "DBT.h"
#include "session.h"
DBT empleeDB;
EmployeeWindow::EmployeeWindow(QWidget* parent)
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
    QSqlQuery split = empleeDB.Emplee_split();

    while (split.next())
    {
        int id = split.value(0).toInt();
        QString name = split.value(1).toString();
        ui->comboBoxEmployers->addItem(name, id);
    }

    if (split.lastError().isValid())
    {
        qDebug() << "Ошибка загрузки работодателей: " << split.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить работодателей.");
    }
}



void EmployeeWindow::loadVacancies(int employerId)
{
    ui->comboBoxVacancies->clear();

    QSqlQuery query = empleeDB.loadVacancy(employerId);

    if (query.exec())
    {
        while (query.next())
        {
            int id = query.value(0).toInt();
            QString vacancyName = query.value(1).toString();
            ui->comboBoxVacancies->addItem(vacancyName, id);
        }
    }
    else
    {
        qDebug() << "Ошибка выполнения запроса: " << query.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить вакансии.");
    }
}

void EmployeeWindow::on_comboBoxEmployers_currentIndexChanged(int index)
{
    if (index == -1)
    {
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

    if (employerId == -1 || vacancyName.isEmpty())
    {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, выберите работодателя и вакансию.");
        return;
    }

    // Проверка наличия записи в таблице Applications
    QSqlQuery checkQuery = empleeDB.chkApplication(employeeId, employerId, vacancyName);

    if (checkQuery.exec())
    {
        if (checkQuery.next() && checkQuery.value(0).toInt() > 0)
        {
            QMessageBox::warning(this, "Внимание", "Вы уже подали заявку на эту вакансию.");
            return;
        }
    }
    else
    {
        qDebug() << "Ошибка выполнения запроса: " << checkQuery.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось проверить существование заявки.");
        return;
    }

    QSqlQuery addApp = empleeDB.addApplication(employeeId, employerId, vacancyName);

    if (addApp.exec())
    {
        QMessageBox::information(this, "Успех", "Заявка успешно подана.");
    }
    else
    {
        qDebug() << "Ошибка выполнения запроса: " << addApp.lastError().text();
        QMessageBox::critical(this, "Ошибка", "Не удалось подать заявку.");
    }
}
void EmployeeWindow::on_exitButton_clicked()
{
    // Создаем объект LoginWindow
    LoginWindow* loginWindow = new LoginWindow();
    // Показываем окно LoginWindow
    loginWindow->show();
    // Закрываем текущее окно
    this->close();
}
