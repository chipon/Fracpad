#include "attrdialog.h"
#include "ui_attrdialog.h"

AttrDialog::AttrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttrDialog)
{
    ui->setupUi(this);
}

AttrDialog::~AttrDialog()
{
    delete ui;
}

QString AttrDialog::getValue()
{
    return value;
}

void AttrDialog::on_buttonBox_accepted()
{
    value=ui->lineEdit->text();
    accept();
}
