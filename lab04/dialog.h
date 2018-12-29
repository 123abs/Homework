#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    QStringList queryStudentScore(QString);
    void queryStudentInfo(QString);
    void initComboCourse();
    void getsql(QSqlDatabase getdb);



private slots:
    void on_ok_Button_clicked();

    void on_check_pushButton_clicked();

    void InsertStudentInfo(QString);

private:
    Ui::Dialog *ui;
    QSqlDatabase db_two;

    QString Course,Number;
    QString InsertCourse;


};

#endif // DIALOG_H
