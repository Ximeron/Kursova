#include "verificationdialog.h"
#include "ui_verificationdialog.h"
#include <QMessageBox>


VerificationDialog::VerificationDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::VerificationDialog),
    commission(0.0)


{
    ui->setupUi(this);
}

VerificationDialog::~VerificationDialog()
{
    delete ui;
}

double VerificationDialog::getCommission() const
{
    return commission;
}

void VerificationDialog::on_buttonBox_accepted()
{
    bool ok;
    double value = ui->lineEditCommission->text().toDouble(&ok);
    if (ok && value > 0) {
        commission = value;
        accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите допустимое значение для комиссионных.");
    }
}

void VerificationDialog::on_buttonBox_rejected()
{
    reject();
}
