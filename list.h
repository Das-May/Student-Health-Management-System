/*版权归属：陈梓青
 *文件名称：list.h
 *版本号：1.0
 *作者：陈梓青
 *完成日期：2022.11.1
*/

#ifndef LIST_H
#define LIST_H

#include <QObject>
#include <QTableWidget>

struct student//信息
{
    /*
    //string name;//①姓名
    //string id;//②学号
    //string born;//③出生日期
    //string sex;//④性别
    //string condition;//⑤身体情况
    //string other;//⑥备注
    */
    int index;//索引号
    QString inf[6];
    student* next;
};

class List : public QObject
{
    Q_OBJECT
public:
    explicit List(QObject *parent = nullptr);
    ~List();//析构函数

    student* getHeader();//获取头结点
    int getIndex();//获取最大行数

    void add(int index, int indexOfString, QString newInf);//结点创建（编辑）
    student* insert(int index, int indexOfString, QString newInf);//插入（添加）
    int search(QString id);//按学号查找
    student* nodeDelete(int index);//删除结点
    student* infDelete(int index, int indexOfString);//删除内容
    void mergeSort();//归并排序（含前处理）

    void read(QTableWidget *table);//读取数据 从表格控件中
    void write(QTableWidget *table);//写出数据 到表格控件中
    void display();


private:
    student* tail;
    student* ms(student* tail);//归并排序（核心处理算法）


signals:

public slots:
};

#endif // LIST_H
