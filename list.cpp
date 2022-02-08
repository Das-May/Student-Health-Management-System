/*版权归属：陈梓青
 *文件名称：list.cpp
 *版本号：1.0
 *作者：陈梓青
 *完成日期：2022.11.1
*/

#include "list.h"
#include <QDebug>

//构造函数
List::List(QObject *parent) : QObject(parent)
{
    tail = new student;
    tail->index = -1;
    tail->inf[0] = "姓名";
    tail->inf[1] = "学号";
    tail->inf[2] = "出生日期";
    tail->inf[3] = "性别";
    tail->inf[4] = "身体情况";
    tail->inf[5] = "备注";
    tail->next = tail;
}

//获取头结点
student* List::getHeader()
{
    return tail->next;
}

//获取最大行数
int List::getIndex()
{
    return tail->index;
}


//结点创建或内容添加
void  List::add(int index, int indexOfString, QString newInf )
{
    //当此结点已创建，仅需修改内容
    if(index <= tail->index)
        {
            student* find = tail;//找寻结点
            while (find->index != index)
            {
                find = find->next;
            }
            find->inf[indexOfString] = newInf;//输入数据
        }
    //当此索引号下的结点未被创建时，创建结点并添加内容
    else
    {
        student* p = tail;//尾插法，记录初始位置
        student* head = tail->next;
        while (index > tail->index)
        {
            tail = new student;//新建结点
            tail->index = p->index + 1;//输入数据
            p->next = tail;
            if (tail->index == index)
            {
                tail->inf[indexOfString] = newInf;
                tail->next = head;//拼接新节点
                break;
            }
            p = p->next;
        }
    }
//    return tail;
}

//插入
student* List::insert(int index, int indexOfString, QString newInf)
{
    if (index > tail->index)//判断此时调用插入函数是否合理
        return tail;
    //找寻需要插入的位置的上一个结点
    student* find = tail;
    while (find->next->index != index)
    {
        find = find->next;
    }
    //新建结点
    student* p = new student;
    p->index = index;
    p->inf[indexOfString] = newInf;
    //链接结点
    p->next = find->next;
    find->next = p;
    //调整后续结点的索引号
    find = find->next->next;
    index++;
    while(find != tail->next)
    {
        find->index = index;
        index++;
        find = find->next;
    }
    return tail;
}

//按学号查找
/*
* 参数：指定学生的学号
* 返回值：对应的结点的索引号
*/
int List::search(QString id)
{
    student* find = tail;
    do
    {
        if (find->inf[1] == id)
            return find->index;
        find = find->next;
    } 	while (find != tail);
    return -1;
}

//删除结点（整行）
student* List::nodeDelete(int index)
{
    //查找索引号
    qDebug()<<"删除整行 行号："<<index;
    student* find = tail;
    while (find->next->index != index)
    {
        find = find->next;
        qDebug() << find->index << find->inf[0] <<
                    find->inf[1] << find->inf[2] <<
                    find->inf[3] << find->inf[4] << find->inf[5] << " ";
    }
    //删除
    student* p = find->next;
    if(p == tail)
        tail = find;
    find->next = p->next;
    delete p;

    //调整后续结点的索引号
    if(find != tail)
    {
        while (find != tail->next)
        {
            find = find->next;
            find->index = index;
            index++;
            qDebug() << find->index << find->inf[0] << find->inf[1]
                     << find->inf[2] << find->inf[3] << find->inf[4] << find->inf[5] << " ";
        }
    }


    return tail;
}

//删除内容
student* List::infDelete(int index, int indexOfString)
{
    //查找索引号
    student* find = tail;
    while (find->index != index)
    {
        find = find->next;
    }
    //删除内容 即将内容置为空
    find->inf[indexOfString] = "";
    return tail;
}

//归并排序
//注意：归并排序利用的是单向链表的头结点，而List类创建的是记录尾结点的单向链表
//故排序前需要先进行链表转换

//归并排序（核心处理算法）    传入的是单链表的头结点
student* List::ms(student* head)
{
    //先判断链表长度是否大于1，不满足条件时无需排序
    if (head!=NULL &&head->next!=NULL)
    {
        /*运用快慢指针，找到链表的中间结点*/
        student* fast = head->next;
        student* slow = head;
        while (fast != NULL && fast->next != NULL)
        {
            fast = fast->next->next;
            slow = slow->next;
        }//循环结束后，若链表中含5组数据（不包含头结点），则slow指向第2组；
        //若链表中含6组数据（不包含头结点），则slow指向第2组
        //即slow会指向中间元素的前一个

        /*将链表分成两部分进行分割*/
        student* p1 = ms(slow->next);//右链链头
        slow->next = NULL;//断链操作
        student* p2 = ms(head);//左链链头

        /*对两条子链进行归并*/
        student* p0 = new student;//p0是排序后的链表的头指针
        student* p = p0;
        //比较子链头部（对应子链的最小元素），较小链表头部链接到排好序的链表尾部，较小链表的头指针后移
        while (p1 != NULL && p2 != NULL)
        {
            if (p1->inf[1] < p2->inf[1]) {
                p->next = p1;
                p1 = p1->next;
            }
            else
            {
                p->next = p2;
                p2 = p2->next;
            }
            p = p->next;//p始终保持为排好序的链表的尾指针
        }

        /*只剩p1或p2时，直接链接到排好序的链表的后面*/
        if (p1 != NULL)
            p->next = p1;
        if (p2 != NULL)
            p->next = p2;

        /*释放头指针，返回链表的第一个结点的地址*/
        p = p0->next;
        delete p0;
        return p;
    }
    return head;//空链表无需排序
}

//归并排序（含前处理）
void List::mergeSort()
{
    /*转换链表形式  断链  成为单向链表*/
    student* head0 = tail->next;
    student* head1 = head0->next;
    tail->next = NULL;
    /*排序，获取新排序下的第一组信息的结点的地址*/
    head1 = ms(head1);
    head0->next = head1;///////我也不知道为什么不加这句话不行
    /*找寻尾结点*/
    int i = 0;
    while (head1->next != NULL)
    {
        head1->index = i;//因为链表已经重新排序，那么结点对应的索引号也应重新排序
        i++;
        head1 = head1->next;
    }
    head1->index = i;
    /*连接  成为循环链表*/
    tail = head1;
    tail->next = head0;

    this->display();
//    return tail;
}

//读取数据
void List::read(QTableWidget *table)
{
    qDebug()<<"链表读取表格";
    qDebug()<<table->rowCount();
    for(int i = 0; i < table->rowCount(); i++)
    {
        for(int j = 0; j < 6; j++)
        {
            if(table->item(i,j)!=NULL &&         //判断指向该cell的指针是否为空
                         table->item(i,j)->text()!=tr("") )  //判断该cell的text是否为空
            {
                    qDebug()<<table->item(i, j)->text();
                    this->add(i, j, table->item(i, j)->text());
            }
        }
    }
    this->display();
}

//在控制台 展示数据
void List::display()
{
    qDebug()<<"尾结点的索引号："<<tail->index;
    student* p = tail->next;
    int i = -1;
    while (i <= tail->index)
    {
        qDebug() << p->index << p->inf[0] << p->inf[1] << p->inf[2] << p->inf[3] << p->inf[4] << p->inf[5] << " ";
        p = p->next;
        i++;
    }
    qDebug() << '\n';
}

//写出数据
void List::write(QTableWidget *table)
{
    student *find = tail->next->next;
    table->setRowCount(tail->index + 10);
    int i = 0;
    for(; i <= tail->index; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            table->setItem(i, j, new QTableWidgetItem( find->inf[j] ) );
        }
        find = find->next;
    }
    for(;i<table->rowCount();i++)
    {
        for(int j = 0; j < 6; j++)
        {
            table->setItem(i, j, new QTableWidgetItem(""));
        }
    }
    display();

}

//析构函数
List::~List()
{
    if(tail != NULL)
    {
        student* p = tail->next;
        tail->next = NULL;
        tail = p;
        while (tail != NULL)
        {
            tail = tail->next;
            delete p;
            p = tail;
        }
    }
    delete tail;
}
