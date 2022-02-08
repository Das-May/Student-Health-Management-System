/*版权归属：陈梓青
 *文件名称：main.cpp
 *版本号：1.0
 *作者：陈梓青
 *完成日期：2022.11.1
*/

#include "shsms.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SHSMS w;
    w.show();

    return a.exec();
}
