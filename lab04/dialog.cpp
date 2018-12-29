#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("登记成绩 "));

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::getsql(QSqlDatabase getdb)
{
    db_two=getdb;
    if(db_two.isOpen())
            db_two.close();
        if(!db_two.open()){
            QMessageBox::critical(this,tr("错误"),tr("无法连接数据库！\n请检查数据库连接配置。"));
            return ;
        }
}

void Dialog::on_check_pushButton_clicked()
{
//    if(db_two.isOpen())
//        db_two.close();
//    if(!db_two.open()){
//        QMessageBox::critical(this,tr("错误"),tr("无法连接数据库！\n请检查数据库连接配置。"));
//        return ;
//    }else{

        QString str;
        if(ui->In_Name_lineEdit->text().isEmpty())
            str=QString("where sNumber='%1'").arg(ui->In_ID_lineEdit->text());

        else if(ui->In_ID_lineEdit->text().isEmpty())
            str=QString("where sName='%1'").arg(ui->In_Name_lineEdit->text());

        QString queryStr=QString ("SELECT * FROM t_stud_info %1;").arg(str);
        //qDebug()<<queryStr;
        queryStudentInfo(queryStr);//查询学生信息
        initComboCourse();
//    }
}

void Dialog::initComboCourse()
{

    ui->Select_CourseName_Combobox->clear();
    ui->Select_CourseName_Combobox->insertItem(0,"选择科目");
    ui->Select_CourseName_Combobox->insertItem(1,"嵌入式系统设计");
    ui->Select_CourseName_Combobox->insertItem(2,"微波技术与天线");
    ui->Select_CourseName_Combobox->insertItem(3,"通信原理");
    ui->Select_CourseName_Combobox->insertItem(4,"科技英语");
}


void Dialog::queryStudentInfo(QString queryStr)
{
    //qDebug()<<"123";
    QSqlQuery query;
    query.exec(queryStr);
    while(query.next()){
        //0 num ; 1 name ; 2 major ; 3 class
        ui->In_ID_lineEdit->setText(query.value(0).toString());
        ui->In_Name_lineEdit->setText(query.value(1).toString());
    }
}

void Dialog::on_ok_Button_clicked()
{
//    if(db_two.isOpen())
//            db_two.close();
//        if(!db_two.open()){
//            QMessageBox::critical(this,tr("错误"),tr("无法连接数据库! \n请检查数据库连接配置"));
//            return;
//        }else{

            Number=QString("%1").arg(ui->In_ID_lineEdit->text());
            //qDebug()<<Number;

            //成绩
            Course=QString("%1").arg(ui->Scorse_LineEdit->text());
            qDebug()<<Course;

            if(ui->Select_CourseName_Combobox->currentIndex()==1){
                QString InsertCourse = QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
                                                 "values ('%1','14010271','%4','2017-2018','1')").arg(Number).arg(Course);
                qDebug()<<InsertCourse;
                InsertStudentInfo(InsertCourse);
            }
            else if(ui->Select_CourseName_Combobox->currentText()=="微波技术与天线"){
                QString InsertCourse = QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
                                                 "values ('%1','14040421','%4','2017-2018','1')").arg(Number).arg(Course);
                qDebug()<<InsertCourse;
                InsertStudentInfo(InsertCourse);
            }
            else if(ui->Select_CourseName_Combobox->currentText()=="通信原理"){
                QString InsertCourse = QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
                                                 "values ('%1','14040402','%4','2017-2018','1')").arg(Number).arg(Course);
                qDebug()<<InsertCourse;
                InsertStudentInfo(InsertCourse);
            }
            else if(ui->Select_CourseName_Combobox->currentText()=="科技英语"){
                QString InsertCourse = QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
                                                 "values ('%1','14040781','%4','2017-2018','1')").arg(Number).arg(Course);
                qDebug()<<InsertCourse;
                InsertStudentInfo(InsertCourse);
            }

            QMessageBox::about(NULL,"提示","数据插入成功");
//        }
}

void Dialog::InsertStudentInfo(QString sql){
     QSqlQuery insert(sql);
}


