#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QFile>

//定义命名空间SK，将文本文件划分为不同的列
namespace SK {
enum SortKind{
    col01    =   0x00000001<<0,         //!< 第1列
    col02    =   0x00000001<<1,         //!< 第2列
    col03    =   0x00000001<<2,         //!< 第3列
    col04    =   0x00000001<<3,         //!< 第4列
    col05    =   0x00000001<<4,         //!< 第5列
    col06    =   0x00000001<<5,         //!< 第6列
    col07    =   0x00000001<<6,         //!< 第7列
    col08    =   0x00000001<<7,         //!< 第8列
    col09    =   0x00000001<<8,         //!< 第9列
    col10    =   0x00000001<<9,         //!< 第10列
    col11    =   0x00000001<<10,        //!< 第11列
    col12    =   0x00000001<<11,        //!< 第12列
    col13    =   0x00000001<<12,        //!< 第13列
    col14    =   0x00000001<<13,        //!< 第14列
    col15    =   0x00000001<<14,        //!< 第15列
    col16    =   0x00000001<<15,        //!< 第16列
    col17    =   0x00000001<<16,        //!< 第17列
    col18    =   0x00000001<<17,        //!< 第18列
    col19    =   0x00000001<<18,        //!< 第19列
    col20    =   0x00000001<<19,        //!< 第20列
    col21    =   0x00000001<<20,        //!< 第21列
    col22    =   0x00000001<<21,        //!< 第22列
    col23    =   0x00000001<<22,        //!< 第23列
    col24    =   0x00000001<<23,        //!< 第24列
    col25    =   0x00000001<<24,        //!< 第25列
    col26    =   0x00000001<<25,        //!< 第26列
    col27    =   0x00000001<<26,        //!< 第27列
    col28    =   0x00000001<<27,        //!< 第28列
    col29    =   0x00000001<<28,        //!< 第29列
    col30    =   0x00000001<<29,        //!< 第30列
    col31    =   0x00000001<<30,        //!< 第31列
    col32    =   0x00000001<<31,        //!< 第32列
};
}

//信息存储结构体
typedef struct
{
    QStringList stud;  //补全结构定义
} studData;




QDebug operator << (QDebug d, const studData &data)
{
  // 运算符重载函数，直接输出studData结构
    for(int i=0;i<data.stud.size();i++)
    d.noquote().nospace()<<QString(data.stud.at(i))<<"\t" ;
    return d;
}


// 比较类，用于std::sort第三个参数
class myCmp
{
public:
    myCmp(int selectedColumn) { this->currentColumn = selectedColumn; }
    bool operator() (const studData& d1,const studData& d2) ;
private:
    int currentColumn;
};



bool myCmp::operator()(const studData &d1,const  studData &d2)
{
    bool result = false;
    quint32 sortedColumn = 0x00000001<<currentColumn;
    switch (sortedColumn)
    {
        default: result=(d1.stud.at(currentColumn+1)>=d2.stud.at(currentColumn+1));break;
    }
    return result;
}


//执行类
class ScoreSorter
{
public:
    ScoreSorter(QString dataFile);

    QList<studData > sdata;
    QString datafile;

    studData listtitle;     //data标题
    studData data;          //data数据

    void readFile();            //将文本载入内存
    void doSort();              //排序
    void showdata(quint8 currentColumn);

};


ScoreSorter::ScoreSorter(QString dataFile)
{
    this->datafile = dataFile;
}


//读取文件
void ScoreSorter::readFile()
{
    QFile file(this->datafile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<QString("打开 %1 失败").arg(this->datafile);
    }

    QString titile(file.readLine()); //读取整行成字符串返回
    this->listtitle.stud = titile.split(" ", QString::SkipEmptyParts);   //按空格分隔

    if((this->listtitle.stud).last() == "\n")
        this->listtitle.stud.removeLast();


    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString str(line);


        data.stud = str.split(" ", QString::SkipEmptyParts);

        if((data.stud).last() == "\n")
            data.stud.removeLast();

        if(data.stud.size()==0) continue;
            this->sdata.append(data);  //添加数据到sdata

    }
}



//排序函数
void ScoreSorter::doSort()
{
    for(int i=1; i<this->listtitle.stud.size(); i++)
    {
        myCmp mycmp(i-1);    //初始化排序规则对象
        std::sort(this->sdata.begin() , this->sdata.end() , mycmp );  //排序

        this->showdata(i+1); //当前排序规则下的data写入文件
    }
}



void ScoreSorter::showdata(quint8 currentColumn)
{
    QFile file("sorted_"+this->datafile);

    file.open(QIODevice::ReadWrite | QIODevice::Append);

    QTextStream stream(&file);
    stream.setCodec("UTF-8");  //编码方式
    stream<<QString("排序后输出，当前排序第 ")<<currentColumn <<QString(" 列：")<<"\r\n";

    //输出表头
    for(int j=0;j<this->listtitle.stud.size();j++)
    {
        stream<<"   "<<this->listtitle.stud.at(j);
    }
        stream<<"\r\n";

    for(int i=0;i<this->sdata.size();i++)            //输出内容
    {
        for(int j=0;j<this->listtitle.stud.size();j++)
        stream<<this->sdata.at(i).stud.at(j)<<"\t";
        stream<<"\r\n";
    }


    stream<<"------------------------------------------------------------------"<<"\r\n\r\n";
    file.close();
}



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString datafile = "data.txt";

    // 如果排序后文件已存在，则删除之
    QFile f("sorted_"+datafile);
    if (f.exists()){
        f.remove();
    }

    ScoreSorter s(datafile);
    s.readFile();
    s.doSort();
    return 0;
}
