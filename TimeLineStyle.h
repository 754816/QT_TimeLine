#ifndef TIMELINESTYLE_H
#define TIMELINESTYLE_H
#include <QPoint>
#include <QString>
#include <QDateTime>
#include <QColor>
#include <QList>
enum TimeInterval
{
    _OneMin = 60,             //间隔1分钟
    _FiveMin = 300,           //间隔5分钟
    _TenMin = 600,            //间隔10分钟
    _ThirtyMin = 1800,        //间隔30分钟
    _OneHour = 3600,          //间隔1小时
    _TwoHour = 7200           //间隔2小时
};
struct TextInfo_t
{
    QPoint point;
    QString text;
};
namespace TimeLineStyle
{
    const QDateTime beginDateTime = QDateTime::fromString("2024-08-23 12:34:56", "yyyy-MM-dd hh:mm:ss");        //不具备展示意义，只是给为了给dateTime赋一个初始值
    const int TimeCount = 12;                           //在时间轴上拆分成12块，展示12+1个文字标签，修改时不能为0
    const int ShortLine = 6;                            //短竖线，单位px
    const int MidLine = 9;                              //中竖线，单位px
    const int LongLine = 16;                            //长竖线，单位px
    const int MarginSize = 10;                          //两边预留范围，避免两头text被完全遮盖
    const int TextHeight = 11;                          //text显示高度
    const QColor red = QColor(220, 20, 20);             //红色
    const QColor green = QColor(150, 220, 100);          //绿色
}
#endif // TIMELINESTYLE_H
