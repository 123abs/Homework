#include <QCoreApplication>
#include<QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    int i,j,k;
    QVector<QString> name,number,c1,c2,title,t; //利用QVector
    QVector<QVector<QString>> student;
    title<<'\n'<<"姓名排序成绩单:"<<"课程1成绩排序成绩单:"<<"课程2成绩排序成绩单:";  //输入数据
    number<< "1403130209" << "1403140101" << "1403140102" << "1403140103" ;  
    name<<"鲁智深"<<"林冲"<<"宋江"<<"武松";              
    c1<<"80"<<"82"<<"76"<<"88";                   
    c2<<"72"<<"76"<<"85"<<"80";                   
    student<<number<<name<<c1<<c2;          //升维
    qDebug("原始成绩单:");
    qDebug()<<"学号"<<"\t\t"<<"姓名"<<"\t\t"<<"课程1"<<'\t'<<"课程2";
    for(int i=0;i<=3;i++)
        qDebug()<<number.at(i)<<"\t"
                <<name.at(i)<<"\t"
                <<c1.at(i)<<"\t"
                <<c2.at(i);
    for(int i =1;i<=3;i++)
    {
        qDebug()<<'\n'<<title.at(i);
        t=student.at(i);
        std::sort(t.begin(),t.end(),std::greater<QString>());
        qDebug()<<"学号"<<"\t\t"<<"姓名"<<"\t\t"<<"课程1"<<'\t'<<"课程2";
        for(int j=0;j<=3;j++)
        {
            for(k=0;k<=3;k++)
                if(t.at(j)==student.at(i).at(k)) break;           
            qDebug()<<student.at(0).at(k)<<'\t'
                    <<student.at(1).at(k)<<'\t'
                    <<student.at(2).at(k)<<'\t'
                    <<student.at(3).at(k)<<'\t';
        }
    }
    return a.exec();
}
