#include "jakasetup.h"
#include "ui_jakasetup.h"

JakaSetup::JakaSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JakaSetup)
{
    ui->setupUi(this);
    Port = ui->PortLineEdit->text().toInt();
    IP = ui->IPLineEdit->text();
}

JakaSetup::~JakaSetup()
{
    delete ui;
}

void JakaSetup::on_Rect_clicked()
{
    ROIShape = 1;
}

void JakaSetup::on_Circ_clicked()
{
    ROIShape = 2;
}

void JakaSetup::on_Free_clicked()
{
    ROIShape = 3;
}


void JakaSetup::on_OKButton_clicked()
{
    ROIMode = ROIShape;
    Port = ui->PortLineEdit->text().toInt();
    IP = ui->IPLineEdit->text();    
    accept();
}

void JakaSetup::on_CancelButton_clicked()
{
    reject();
}
