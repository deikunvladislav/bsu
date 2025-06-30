#ifndef AUTORISATOR_H
#define AUTORISATOR_H

#include <QDialog>

namespace Ui {
class autorisator;
}

class autorisator : public QDialog
{
    Q_OBJECT

public:
    explicit autorisator(QWidget *parent = nullptr);
    ~autorisator();

private slots:
    void on_pushButton_clicked();

private:
    Ui::autorisator *ui;
};

#endif // AUTORISATOR_H
