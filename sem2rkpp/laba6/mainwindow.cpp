#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "autorisator.h"
#include <QComboBox>
#include <QStringList>
#include <QString>
#include <QMessageBox>
#include <QMap>
#include <QFile>
#include <QTextStream>
QList <QString> history;
int k=1;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->currencies1->addItems({"USD", "EUR", "GBP", "JPY", "CNH", "RUB", "BYN"});
    ui->currencies2->addItems({"USD", "EUR", "GBP", "JPY", "CNH", "RUB", "BYN"});
    ui->currencies1->hide();
    ui->currencies2->hide();
    ui->curency1->hide();
    ui->curency2->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->title->hide();
    ui->curency2->isReadOnly();
    ui->history->hide();
    ui->savetofile->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSign_In_triggered()
{
    autorisator window;
    window.setModal(true);
    window.exec();
}
void MainWindow::on_actionInstruction_triggered()
{
    QMessageBox::information(this, "Instruction", "curency1- curency you want to convert. curency2-result of convertion");
}

void MainWindow::on_actionClose_APP_triggered()
{
    hide();
}


void MainWindow::on_actionConverte_triggered()
{
    QString cur1 =ui->currencies1->currentText();
    double cur1amount=(ui->curency1->toPlainText()).toDouble();
    QString cur2 =ui->currencies2->currentText();
    double cur2amount=(ui->curency2->toPlainText()).toDouble();
    if(cur1amount<=0){
        QMessageBox::critical(this, "EROR OF INPUT, 404", "YOUR INPUT IS WRONG, IF YOU HAVE NEGATIVE MONEY, GO TO BANK AND GET A LOAN");
    }
    QMap <QString, double> curses {{"USDtoEUR",0.8861}, {"USDtoGBP",0.7451},{"USDtoJPY",143.74},{"USDtoCNH",7.2},
    {"USDtoRUB",79.67},{"USDtoBYN",3.02},{"EURtoUSD",1.1287},{"EURtoGBP",0.8402},{"EURtoJPY",162.08},{"EURtoCNH",8.12},
    {"EURtoRUB",89.86},{"EURtoBYN",3.39},{"GBPtoUSD",1.3421},{"GBPtoEUR",1.1902},{"GBPtoJPY",193.03},{"GBPtoCNH",9.66},
    {"GBPtoRUB",106.92},{"GBPtoBYN",4.02},{"JPYtoUSD",0.006955},{"JPYtoGBP",0.005177},{"JPYtoEUR",0.006164},{"JPYtoCNH",0.05003},
    {"JPYtoRUB",0.5498},{"JPYtoBYN",0.0209},{"CNHtoUSD",0.1384},{"CNHtoGBP",0.1034},{"CNHtoJPY",19.98},{"CNHtoEUR",0.1227},
    {"CNHtoRUB",11.06},{"CNHtoBYN",0.4163},{"RUBtoUSD",0.01251},{"RUBtoGBP",0.009336},{"RUBtoJPY",1.81},{"RUBtoCNH",0.09},
    {"RUBtoEUR",0.01109},{"RUBtoBYN",0.0376},{"BYNtoUSD",0.3329},{"BYNtoGBP",0.2484},{"BYNtoJPY",47.76},{"BYNtoCNH",2.40},
    {"BYNtoRUB",26.59},{"BYNtoEUR",0.2947},{"USDtoUSD",1},{"EURtoEUR",1},{"GBPtoGBP",1},{"CNHtoCNH",1},{"JPYtoJPY",1},
    {"RUBtoRUB",1},{"BYNtoBYN",1}};
    QString key= cur1+"to"+cur2;
    cur2amount=cur1amount*curses.value(key);
    ui->curency2->setPlainText(QString::number(cur2amount));
    QString element= ui->curency1->toPlainText()+" "+cur1+ " was converted to "+ ui->curency2->toPlainText()+" "+cur2;
    history.push_back(element);
    ui->history->addItem(element);
}


void MainWindow::on_actionOpen_App_triggered()
{
    ui->title->show();
    ui->currencies1->show();
    ui->currencies2->show();
    ui->curency1->show();
    ui->curency2->show();
    ui->label_2->show();
    ui->label_3->show();
    ui->history->show();
    ui->savetofile->show();
    QMessageBox::information(this, "APP OPENED", "APP WAS SUCCESFULLY ALUNCHED, ENJOY BEST CURENCIES CONVERTOR INT THE WORLD :))");
}


void MainWindow::on_savetofile_clicked()
{
    QFile file("C:/Users/HP/Documents/untitled5/users_history&data.txt");
    file.open(QIODeviceBase::Append);
    QTextStream out(&file);
    for (int i=0; i<history.size();i++){
        out<<"Operation № "<< k<< " "<<history[i]<<"\n";
        k++;}
    history.clear();
}

