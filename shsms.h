/*版权归属：陈梓青
 *文件名称：shsms.h
 *版本号：1.0
 *作者：陈梓青
 *完成日期：2022.11.1
*/

#ifndef SHSMS_H
#define SHSMS_H

#include <QMainWindow>
#include "List.h"
#include <QObject>
#include <QEvent>
#include <QTableWidget>


namespace Ui {
class SHSMS;
}

class SHSMS : public QMainWindow
{
    Q_OBJECT

public:
    explicit SHSMS(QWidget *parent = 0);
    void on_chkBoxTabEditable_clicked(bool checked);
    ~SHSMS();



private:
    List *list = new List;
    int curRow;
    void readExcel();//读取Excel
    void writeExcel(QTableWidget *table,QString title, bool saveAs = 1);//写入Excel

    //bool eventFilter (QObject *editor, QEvent *event);//过滤器 响应回车键

private:
    Ui::SHSMS *ui;
};

#endif // SHSMS_H
