#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "dialog.h"
#include "QList"
#include "QFileDialog"
#include <QStringList>


#include "QSqlQuery"
#include "QPalette"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize (800, 600);       //设置主窗体的最小尺寸
    setWindowTitle(tr("实验四 - 基本用户界面实验 "));

    //创建db(用来连接数据库的)对象
    db =QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("test");

    //创建menu
    QMenuBar* pMenuBar = ui->menuBar;   // 菜单栏
        //　新建一个菜单A
        QMenu* pMenuA = new QMenu("录入成绩");    // q则为Alt方式的快捷键
        // 新建一个Action，然后加入到菜单A中
        QAction* pActionOne = new QAction("单个录入");
        QAction* pActionTwo = new QAction("批量录入");

        //提示信息
        pActionOne->setStatusTip(tr("录入一个学生成绩"));
        pActionTwo->setStatusTip(tr("录入一批学生成绩"));

        pMenuA->addAction(pActionOne);
        connect(pActionOne,&QAction::triggered,
                this,&MainWindow::on_ActionOne_triggered);
        pMenuA->addAction(pActionTwo);
        connect(pActionTwo,&QAction::triggered,
                this,&MainWindow::on_ActionTwo_triggered);
        //　将菜单A再添加到MenuBar中
        pMenuBar->addMenu(pMenuA);


        /*// 如上述，此菜单即添加了图标
        QMenu* pMenuB = new QMenu("青哲");
        pMenuB->setIcon(QIcon(QPixmap(":/img/2.png")));
        QAction* pActionB = new QAction(QIcon(QPixmap(":/img/3.png")), "子菜单");
        pActionB->setStatusTip(tr("子菜单的提示信息"));
        pMenuB->addAction(pActionB);
        pMenuBar->addMenu(pMenuB);*/

}

MainWindow::~MainWindow()
{
    delete ui;
}

//QSqlDatabase MainWindow::get_sql()
//{
//    return  db;
//}

void MainWindow::on_Start_pushButton_clicked()
{
    if(db.isOpen())
        db.close();
    if(!db.open()){
        QMessageBox::critical(this,tr("错误"),tr("无法连接数据库！\n请检查数据库连接配置。"));
        return ;
    }else{
        QString str;
        if(ui->Num_radioButton->isChecked())
            str=QString("where sNumber='%1'").arg(ui->InPut_lineEdit->text());
        else if(ui->Name_radioButton->isChecked())
            str=QString("where sName='%1'").arg(ui->InPut_lineEdit->text());
        else if(ui->All_radioButton->isChecked())
            str=" ";
        QString queryStr=QString ("SELECT * FROM t_stud_info %1;").arg(str);
        //qDebug()<<queryStr;
        queryStudentInfo(queryStr);

        queryStr=QString("select t_stud_info.sName as '姓名',t_courses.courseName as'课程名称',"
                         " t_stud_course_info.scScores as'成绩', t_courses.courseCredit as'学分' "
                         " from"
                         " (t_stud_course_info inner join t_courses on t_stud_course_info.scCourseID=t_courses.courseID)"
                         " inner join t_stud_info on t_stud_info.sNumber=t_stud_course_info.scNumber "
                         " %1"
                         " order by t_stud_course_info.scNumber"
                         ";").arg(str);
        //qDebug()<<queryStr;
        QStringList queryResult=queryStudentScore(queryStr);


        //int i=0;   for(;i<(sizeof(Scores) / sizeof(Scores[0]));i++)     qDebug()<<Scores[i];
        //qDebug()<<Scores[2].toDouble();


        calculatePoints(queryResult);

        updateInfoWindow(queryResult);

        queryResult.clear();

        db.close();

        // 1403150225   闫志鹏
    }
}

void MainWindow::queryStudentInfo(QString queryStr)
{
    //qDebug()<<"123";
    QSqlQuery query;
    query.exec(queryStr);
    while(query.next()){
        //0 num ; 1 name ; 2 major ; 3 class
        //qDebug()<<query.value(0).toString()<<query.value(1).toString()<<query.value(2).toString()
        //        <<query.value(3).toString();
        ui->Out_lineEdit_Num->setText(query.value(0).toString());
        ui->Out_lineEdit_Name_2->setText(query.value(1).toString());
        ui->Out_lineEdit_Major->setText(query.value(2).toString());
        ui->Out_lineEdit_Class->setText(query.value(3).toString());

    }
}
QStringList MainWindow::queryStudentScore(QString queryStr)
{
    QSqlQuery query;
    query.exec(queryStr);
    QStringList queryResult;
    int i=0;

    while(query.next()){
        /*queryResult<<query.value(0).toString()
                  <<query.value(1).toString()
                 <<query.value(2).toString()
                <<query.value(3).toString();*/

        //课程名称
        Course.append(query.value(1).toString());
        queryResult.append(query.value(1).toString()); //append() 相当于 <<  在后面添加。

        //成绩
        Scores.append(query.value(2).toString());
        queryResult.append(query.value(2).toString());

        //学分
        Credit.append(query.value(3).toString());
        queryResult.append(query.value(3).toString());
        i++;

    }
    //qDebug()<<queryResult;

    return queryResult;
}
void MainWindow::updateInfoWindow(QStringList)
{
    ui->Out_textEdit->clear();

    //Html表格输出QString name=setText(query.value(1).toString());
    QString name = ui->Out_lineEdit_Name_2->text();
    QString str;
    QString Str_start="<table border='1' cellpadding='10' >"
                      "<tr>  <td align='center' colspan='5'> "+name+"</td>"
                      "<tr>  <td>编号</td>    <td>课程名称</td>    <td>成绩</td>     <td>学分</td>     <td>绩点</td>";
    QString str_end="<tr>  <td align='center' colspan='5'> 学期绩点"+zongjidian+"</td>" "</table>";

    for(int i=0 ;i<Course.count();i++){
        QString Str_mid=QString("<tr>  <td>"+QString::number(i+1)+"</td>    <td>"+Course.at(i)+"</td>    <td>"+Scores.at(i)+"</td>     <td>"+Credit.at(i)+"</td>       <td>"+jidian.at(i)+"</td>  </tr>");
         str.append(Str_mid);
    }

    Str_start.append(str);
    Str_start.append(str_end);

//    QString str =//"<h4 align='center'>"+name+"</h4>"
//                 "<table border='1' cellpadding='10' >"
//                 "<tr>  <td align='center' colspan='5'> "+name+"</td>"
//                 "<tr>  <td>编号</td>    <td>课程名称</td>    <td>成绩</td>     <td>学分</td>     <td>绩点</td>"
//                 "<tr>  <td>1</td>    <td>"+Course[0]+"</td>    <td>"+Scores[0]+"</td>     <td>"+Credit[0]+"</td>       <td>"+jidian[0]+"</td>  </tr>"
//                 "<tr>  <td>2</td>    <td>"+Course[1]+"</td>    <td>"+Scores[1]+"</td>     <td>"+Credit[1]+"</td>       <td>"+jidian[1]+"</td>"
//                 "<tr>  <td>3</td>    <td>"+Course[2]+"</td>    <td>"+Scores[2]+"</td>     <td>"+Credit[2]+"</td>       <td>"+jidian[2]+"</td>"
//                 "<tr>  <td>4</td>    <td>"+Course[3]+"</td>    <td>"+Scores[3]+"</td>     <td>"+Credit[3]+"</td>       <td>"+jidian[3]+"</td>"
//                 "<tr>  <td>5</td>    <td>"+Course[4]+"</td>    <td>"+Scores[4]+"</td>     <td>"+Credit[4]+"</td>       <td>"+jidian[4]+"</td>"
//                 "<tr>  <td>6</td>    <td>"+Course[5]+"</td>    <td>"+Scores[5]+"</td>     <td>"+Credit[5]+"</td>       <td>"+jidian[5]+"</td>"
//                 "<tr>  <td>7</td>    <td>"+Course[6]+"</td>    <td>"+Scores[6]+"</td>     <td>"+Credit[6]+"</td>       <td>"+jidian[6]+"</td>"
//                 "<tr>  <td align='center' colspan='5'> 学期绩点"+zongjidian+"</td>"
//                 "</table>";

    ui->Out_textEdit->insertHtml(Str_start);

    //清零
    Course.clear();
    Scores.clear();
    jidian.clear();
    Credit.clear();

}
void MainWindow::calculatePoints(QStringList)
{
    int i=0;
    //qDebug()<<"qqq";

    for( ;i<Course.count();i++){
        if(Scores.at(i)=="优秀"||Scores.at(i).toInt()>=90)  jidian<<("4.0");
        else if((Scores.at(i).toInt()>=85&&Scores.at(i).toInt()<90)||Scores.at(i)=="良好")  jidian<<("3.7");
        else if((Scores.at(i).toInt()>=80&&Scores.at(i).toInt()<85)||Scores.at(i)=="良好")  jidian<<("3.3");
        else if((Scores.at(i).toInt()>=75&&Scores.at(i).toInt()<89)||Scores.at(i)=="中等")  jidian<<("3.0");
        else if((Scores.at(i).toInt()>=70&&Scores.at(i).toInt()<75)||Scores.at(i)=="中等")  jidian<<("2.7");
        else if((Scores.at(i).toInt()>=68&&Scores.at(i).toInt()<70)||Scores.at(i)=="及格")  jidian<<("2.3");
        else if((Scores.at(i).toInt()>=60&&Scores.at(i).toInt()<68)||Scores.at(i)=="及格")  jidian<<("2.0");
        else if(Scores.at(i)=="不及格"||Scores.at(i).toInt()<60)  jidian<<("0");
        else if(Scores.at(i)=="缓考")  jidian<<("缓考");
        else
            jidian<<("0");
    }

    double Allxuefen=0;
    for(i=0;i<Course.count();i++){
        Allxuefen=Allxuefen + Credit.at(i).toDouble();
    }


    double jidian1=0;
    for(i=0;i<Course.count();i++){
        jidian1=jidian1+jidian.at(i).toDouble()*Credit.at(i).toDouble()/Allxuefen;
    }
    zongjidian = QString("%1").arg(jidian1);


}

void MainWindow::on_ActionOne_triggered()        //界面连接
{
    Dialog *w=new Dialog(this);
    w->show();
    dialog = new Dialog(this);
    dialog->getsql(db);
}

void MainWindow::on_ActionTwo_triggered()
{
    QString fName;
    fName=QFileDialog::getOpenFileName(this,"选择文件","D:\\Qt","");
    QFile f(fName);
    // 成功打开数据文件，则由文件中读取
    if(f.open(QIODevice::ReadOnly|QIODevice::Text)){
        QStringList dataList;
        qDebug().noquote()<<QString("数据由文件%1导入...").arg(fName);
        QTextStream stream (&f);
        while(!stream.atEnd())
            dataList<<stream.readLine();
        qDebug()<<dataList.at(2);
        parseData(dataList.join("-").simplified());
    }
    else{
        qDebug()<<"打开错误";
    }

}

void MainWindow::parseData(const QString sourceText)
{
    //qDebug()<<"加油！";
    QStringList dataList = sourceText.split("-");

    dataList.removeFirst();                  // 第一条数据是表头，删除
    //qDebug()<<"加油！";
    for(int i=0;i<dataList.count();i++){
        QString everyData=dataList.at(i);
        //qDebug()<<"加油！";
        QStringList EveryData_StringList=everyData.split("，");
        //qDebug()<<EveryData_StringList.at(1);
        infoData(EveryData_StringList);
    }
}

void MainWindow::infoData(QStringList sourcedata){

    QString StudentID=sourcedata.at(1);
    QString Source_Embed=sourcedata.at(5);
    QString Source_Microwave=sourcedata.at(6);
    QString Source_Communication=sourcedata.at(7);
    QString Source_English=sourcedata.at(8);


    if(db.isOpen())
        db.close();
    if(!db.open()){
        QMessageBox::critical(this,tr("错误"),tr("无法连接数据库！\n请检查数据库连接配置。"));
        return ;
    }else{
        QString InsertCourse_Embed= QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
                                         "values ('%1','14010271','%4','2017-2018','1')").arg(StudentID).arg(Source_Embed);
        InsertStudentInfo_MianWindow(InsertCourse_Embed);
        QString InsertCourse_Microwave= QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
                                         "values ('%1','14040421','%4','2017-2018','1')").arg(StudentID).arg(Source_Microwave);
        InsertStudentInfo_MianWindow(InsertCourse_Microwave);
        QString InsertCourse_Communication= QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
                                         "values ('%1','14040402','%4','2017-2018','1')").arg(StudentID).arg(Source_Communication);
        InsertStudentInfo_MianWindow(InsertCourse_Communication);
        QString InsertCourse_English= QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
                                         "values ('%1','14040781','%4','2017-2018','1')").arg(StudentID).arg(Source_English);
        InsertStudentInfo_MianWindow(InsertCourse_English);
        QMessageBox::about(NULL,"提示","数据插入成功");
        db.close();
    }

//    QString InsertCourse_Embed= QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
//                                     "values ('%1','14010271','%4','2017-2018','1')").arg(StudentID).arg(Source_Embed);
//    InsertStudentInfo_MianWindow(InsertCourse_Embed);
//    QString InsertCourse_Microwave= QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
//                                     "values ('%1','14040421','%4','2017-2018','1')").arg(StudentID).arg(Source_Microwave);
//    InsertStudentInfo_MianWindow(InsertCourse_Microwave);
//    QString InsertCourse_Communication= QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
//                                     "values ('%1','14040402','%4','2017-2018','1')").arg(StudentID).arg(Source_Communication);
//    InsertStudentInfo_MianWindow(InsertCourse_Communication);
//    QString InsertCourse_English= QString("insert into test.t_stud_course_info ( scNumber, scCourseID, scScores, scYear, scTerm) "
//                                     "values ('%1','14040781','%4','2017-2018','1')").arg(StudentID).arg(Source_English);
//    InsertStudentInfo_MianWindow(InsertCourse_English);

}

void MainWindow::InsertStudentInfo_MianWindow(QString sql){
     QSqlQuery insert(sql);
}
