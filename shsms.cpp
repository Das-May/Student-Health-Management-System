/*版权归属：陈梓青
 *文件名称：shsms.cpp
 *版本号：1.0
 *作者：陈梓青
 *完成日期：2022.11.1
*/


#include "shsms.h"
#include "ui_shsms.h"
#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QAxObject>
#include <QKeyEvent>
#include <QVariant>
#include <QColor>
#include <QFont>
#include <QString>
#include <QByteArray>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/*Student Health Status Management System*/
/*学生健康情况管理系统*/

SHSMS::SHSMS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SHSMS)
{
    ui->setupUi(this);
    /////////窗口设置//////
    setWindowTitle("学生健康管理系统 1.0");
    setWindowIcon(QIcon(":/res/icon.png"));

    QStringList headerText;
    headerText<<"姓名"<<"学号"<<"出生日期"<<"性别"<<"身体情况"<<"备注";
    ui->tableWidget->setColumnCount(headerText.count());//列数与headerText的列数相等
    ui->tableWidget->setHorizontalHeaderLabels(headerText);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //行头自适应表格
    ui->tableWidget->horizontalHeader()->setFont(QFont("黑体", 12));//设置表头字体
    ui->tableWidget->setRowCount(10);



    ////////功能键设置////////
    //打开文件
    connect(ui->actionOpen,&QAction::triggered,[=](){
        this->readExcel();
    });

    //保存文件
    connect(ui->actionSave,&QAction::triggered,[=](){
        this->writeExcel(ui->tableWidget,"学生健康管理系统", false);
    });

    //保存为
    connect(ui->actionSaveAs,&QAction::triggered,this,[=](){
        this->writeExcel(ui->tableWidget,"学生健康管理系统", true);
    });

    //退出
    connect(ui->actionClose,&QAction::triggered,[=](){
        this->close();
    });

    //插入行
    connect(ui->btnInsert,&QPushButton::clicked,this,[=](){
        int curRow = ui->tableWidget->currentRow();//当前行数
        qDebug() << curRow;
        if(curRow >= 0)//如果用户有选择单元格
        {
            int curCol=ui->tableWidget->currentColumn();//当前列数
            ui->tableWidget->insertRow(curRow); //插入一行，但不会自动为单元格创建item
        }
        else//否则默认从最后一行开始添加行
        {
            curRow = ui->tableWidget->rowCount();
            qDebug() << curRow;
            ui->tableWidget->setRowCount(curRow + 1);
        }
        list->insert(curRow, 0, "");
        ui->tableWidget->setItem(curRow, 0, new QTableWidgetItem(""));
    });

    //删除整行
    connect(ui->btnDelete,&QPushButton::clicked,this,[=](){
        int curRow=ui->tableWidget->currentRow();//当前行数
        qDebug()<<"当前行数："<<curRow;
        list->read(ui->tableWidget);
        list->nodeDelete(curRow);
        list->write(ui->tableWidget);
    });


    //按学号排序
    connect(ui->btnSort,&QPushButton::clicked,this,[=](){
        list->read(ui->tableWidget);//读取信息
        qDebug()<<"读取信息";
        list->mergeSort();//排序
        qDebug()<<"排序";
        list->write(ui->tableWidget);//更新表格
        qDebug()<<"更新表格";
    });

    //按学号查找
    connect(ui->btnSearch,&QPushButton::clicked,[=](){
        bool ok;
        QString id = QInputDialog::getText(this, tr(""),tr("请输入要查找的学号"),QLineEdit::Normal,"",&ok);
        if(ok)
        {
            list->read(ui->tableWidget);//读取信息
            curRow = list->search(id);//强调特定表格
            if(curRow != -1)
            {
                qDebug()<<curRow;
                ui->tableWidget->item(curRow, 1)->setBackgroundColor(QColor("#d2cc72"));
            }
            else
            {
                QMessageBox msg(this);
                msg.setText("系统中无此数据！");// 设置提示框内容
                msg.setStandardButtons(QMessageBox::Ok);//对话框上包含的按钮
                msg.exec();//调用
            }

        }

    });
}

void SHSMS::on_chkBoxTabEditable_clicked(bool checked)
{ //设置编辑模式
    if (checked)
        //双击或获取焦点后单击，进入编辑状态
        ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    else
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //不允许编辑
}

//读取文件
void SHSMS::readExcel()
{
    QString filePath = QFileDialog::getOpenFileName(this,"open",
                                                "","execl(*.txt)");
    //指定父对象（this），“open”具体操作，打开，“../”默认，之后可以添加要打开文件的格式

    if(filePath != "")//用户选择文件后
    {
        std::ifstream read(filePath.toUtf8().data(), std::ios::in);
        if(!read)
        {
            qDebug() << "文件读取失败！";
            return;
        }
        string str;
        QString qstr;
        int i = 0, j = 0;
        while(getline(read, str))
        {
            qstr = QString(QString::fromLocal8Bit(str.c_str()));//有效防止乱码
            qDebug() << qstr << qstr.length();
            for(j = 0; j < 6; j++)
            {
                int position = qstr.indexOf("\t");
//                qDebug() << position;
                QString subStr = qstr.left(position);
//                qDebug() << subStr;
                ui->tableWidget->setRowCount(i + 1);
                ui->tableWidget->setItem(i,j,new QTableWidgetItem(subStr));
//                qDebug() << qstr.right(qstr.length() - position - 1);
                qstr = qstr.right(qstr.length() - position - 1);//丢弃已录入的字符串
            }
            i++;
        }
    }
}


//导出文件
void SHSMS::writeExcel(QTableWidget *table, QString title, bool saveAs)
{
    QString fileName;
    if(saveAs){
        fileName = QFileDialog::getSaveFileName(table, "保存",
                                                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                "Excel 文件(*.xls *.xlsx)");
    }
    else
    {
        fileName = "write.xls";
    }

    if (fileName != "")
    {
        QAxObject *excel = new QAxObject;
        if (excel->setControl("Excel.Application")) //连接Excel控件
        {
            excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
            excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
            QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
            workbooks->dynamicCall("Add");//新建一个工作簿
            QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
            QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);
            int i,j,colcount=table->columnCount();
            QAxObject *cell,*col;
            //标题行
            cell=worksheet->querySubObject("Cells(int,int)", 1, 1);
            cell->dynamicCall("SetValue(const QString&)", title);
            cell->querySubObject("Font")->setProperty("Size", 18);
            //调整行高
            worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);
            //合并标题行
            QString cellTitle;
            cellTitle.append("A1:");
            cellTitle.append(QChar(colcount - 1 + 'A'));
            cellTitle.append(QString::number(1));
            QAxObject *range = worksheet->querySubObject("Range(const QString&)", cellTitle);
            range->setProperty("WrapText", true);
            range->setProperty("MergeCells", true);
            range->setProperty("HorizontalAlignment", -4108);//xlCenter
            range->setProperty("VerticalAlignment", -4108);//xlCenter
            //列标题
            for(i=0;i<colcount;i++)
            {
                QString columnName;
                columnName.append(QChar(i  + 'A'));
                columnName.append(":");
                columnName.append(QChar(i + 'A'));
                col = worksheet->querySubObject("Columns(const QString&)", columnName);
                col->setProperty("ColumnWidth", table->columnWidth(i)/6);
                cell=worksheet->querySubObject("Cells(int,int)", 2, i+1);
                columnName=table->horizontalHeaderItem(i)->text();
                cell->dynamicCall("SetValue(const QString&)", columnName);
                cell->querySubObject("Font")->setProperty("Bold", true);
                cell->querySubObject("Interior")->setProperty("Color",QColor(191, 191, 191));
                cell->setProperty("HorizontalAlignment", -4108);//xlCenter
                cell->setProperty("VerticalAlignment", -4108);//xlCenter
            }
            //数据区
            for(i=0;i<table->rowCount();i++){
                for (j=0;j<colcount;j++)
                {
                    worksheet->querySubObject("Cells(int,int)", i+3, j+1)->dynamicCall("SetValue(const QString&)", table->item(i,j)?table->item(i,j)->text():"");
                }
            }
            //画框线
            QString lrange;
            lrange.append("A2:");
            lrange.append(colcount - 1 + 'A');
            lrange.append(QString::number(table->rowCount() + 2));
            range = worksheet->querySubObject("Range(const QString&)", lrange);
            range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
            range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
            //调整数据区行高
            QString rowsName;
            rowsName.append("2:");
            rowsName.append(QString::number(table->rowCount() + 2));
            range = worksheet->querySubObject("Range(const QString&)", rowsName);
            range->setProperty("RowHeight", 20);
            workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));//保存至fileName
            workbook->dynamicCall("Close()");//关闭工作簿
            excel->dynamicCall("Quit()");//关闭excel
            delete excel;
            excel=NULL;
            if(saveAs)
            {
                if (QMessageBox::question(NULL,"完成","文件已经导出，是否现在打开？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
                {
                    QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
                }
            }
            else
                QMessageBox::warning(NULL,"提示","保存成功！请前往c盘的文档文件夹查看write.xls。",QMessageBox::Yes);
        }
        else
        {
            QMessageBox::warning(NULL,"错误","未能创建 Excel 对象，请安装 Microsoft Excel。",QMessageBox::Apply);
        }
    }
}

SHSMS::~SHSMS()
{
    delete ui;
}
