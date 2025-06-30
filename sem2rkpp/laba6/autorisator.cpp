#include "autorisator.h"
#include "ui_autorisator.h"
#include <QString>
#include <QMessageBox>
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
autorisator::autorisator(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::autorisator)
{
    ui->setupUi(this);
}

autorisator::~autorisator()
{
    delete ui;
}

void autorisator::on_pushButton_clicked()
{
    QString name = ui->username->toPlainText();
    QString password = ui->password->toPlainText();
    QRegularExpression digitRegex("\\d");
    if (!name.contains(digitRegex) && password.contains(digitRegex)) {
        QMessageBox::information(this, "Authorization", "You have successfully authorised");
        QFile file("C:/Users/HP/Documents/untitled5/users_history&data.txt");
        file.open(QIODeviceBase::WriteOnly);
        QTextStream out(&file);
        out<<"users data : "<<Qt::endl;
        if(ui->savename->isChecked()){
            out << "user name : " << name << "\n";}
        if(ui->savepassword->isChecked()){
            out << "user's password : " << password << "\n";}
            out<<"history of operations"<<"\n";
    }
    else {
        QMessageBox::critical(this, "Authorization", "Wrong input, autorisation failed. Please try again.");
    }
}


