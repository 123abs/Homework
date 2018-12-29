#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <dialog.h>
#include <QString>

namespace Ui {
class MainWindow;
}
class GetSql;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void calculatePoints(QStringList);
    void queryStudentInfo(QString);
    void updateInfoWindow(QStringList);

    void parseData(const QString);
    void infoData(QStringList);

    QSqlDatabase get_sql();
    QSqlDatabase db;

    QStringList queryStudentScore(QString);
    GetSql* getSQL_dialog;

private slots:
    void on_Start_pushButton_clicked();

    void on_ActionOne_triggered();
    void on_ActionTwo_triggered();
    void InsertStudentInfo_MianWindow(QString);

private:
    Ui::MainWindow *ui;
    Dialog *dialog;


    //QString Course[10],Scores[10],Credit[10],jidian[10];
    QStringList Course,Scores,Credit,jidian;
    QString zongjidian;
};

#endif // MAINWINDOW_H
