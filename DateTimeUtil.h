#ifndef DATETIMEUTIL_H
#define DATETIMEUTIL_H
#include <QDateTime>
//将QTime转为时间戳计算时间差，适用于1970年后的日期计算，且时间差距不特别大
namespace DateTimeUtil
{
    QTime sub(const QTime& time1, const QTime& time2);
    QTime sub(const QTime& time1, const qint64& seconds);
    QDate sub(const QDate& date1, const QDate& date2);
    QDate sub(const QDate& date1, const qint64& seconds);
    QDateTime sub(const QDateTime& time1, const QDateTime& time2);
    QDateTime sub(const QDateTime& time1, const qint64& seconds);
}
#endif // DATETIMEUTIL_H
