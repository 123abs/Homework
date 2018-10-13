#include <QCoreApplication>
#include<QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //use QList to store students' score
    int j;
    QList<QString> namelist,studentnumber,course1score,course2score,printtitle,templist;
    printtitle<<"姓名排序成绩单:"<<"课程1成绩排序成绩单:"<<"课程2成绩排序成绩单:";  //输出表头
    studentnumber<< "1403130209" << "1403140101" << "1403140102" << "1403140103" ;  //学号列表
    namelist<<"鲁智深"<<"林冲"<<"宋江"<<"武松";              //namelist
    course1score<<"80"<<"82"<<"76"<<"88";                   //score1list
    course2score<<"72"<<"76"<<"85"<<"80";                   //score2list
    // generate all QLists to totallist
    QList< QList<QString> > totallist;
    totallist<<studentnumber<<namelist<<course1score<<course2score;
    //打印原始成绩单
    qDebug("原始成绩单:");
    qDebug()<<"学号"<<"\t\t"<<"姓名"<<"\t\t"<<"课程1"<<'\t'<<"课程2";
    for(int i=0;i<studentnumber.size();i++)
        qDebug()<<studentnumber.at(i)<<"\t"<<namelist.at(i)<<"\t"
               <<course1score.at(i)<<"\t"<<course2score.at(i);
    for(int i =1;i<=3;i++)
    {
        templist=totallist.at(i);
        std::sort(templist.begin(),templist.end(),std::greater<QString>());
        qDebug()<<'\n'<<printtitle.at(i-1);
        qDebug()<<"学号"<<"\t\t"<<"姓名"<<"\t\t"<<"课程1"<<'\t'<<"课程2";
        for(int k=0;k<templist.size();k++)
        {
            for(j=0;j<totallist.at(i).size();j++)
                if(templist.at(k)==totallist.at(i).at(j)) break;            //寻找匹配项
            qDebug()<<totallist.at(0).at(j)<<'\t'<<totallist.at(1).at(j)<<'\t'
                      <<totallist.at(2).at(j)<<'\t'<<totallist.at(3).at(j)<<'\t';
        }
    }
    return a.exec();
}
