#include "DateTimeUtil.h"

QTime DateTimeUtil::sub(const QTime& time1, const QTime& time2)
{
    QDate date = QDate::currentDate();
    qint64 t = QDateTime(date, time1).toSecsSinceEpoch() - QDateTime(date, time2).toSecsSinceEpoch();
    if(t < 0) t = 0;
    QDateTime dt = QDateTime::fromSecsSinceEpoch(t);
    return dt.time();
}
QTime DateTimeUtil::sub(const QTime& time1, const qint64& seconds)
{
    QDate date = QDate::currentDate();
    qint64 t = QDateTime(date, time1).toSecsSinceEpoch() - seconds;
    if(t < 0) t = 0;
    QDateTime dt = QDateTime::fromSecsSinceEpoch(t);
    return dt.time();
}
QDate sub(const QDate& date1, const QDate& date2)
{
    QTime time = QTime::currentTime();
    qint64 t = QDateTime(date1, time).toSecsSinceEpoch() - QDateTime(date2, time).toSecsSinceEpoch();
    if(t < 0) t = 0;
    QDateTime dt = QDateTime::fromSecsSinceEpoch(t);
    return dt.date();
}
QDate sub(const QDate& date1, const qint64& seconds)
{
    QTime time = QTime::currentTime();  
    qint64 t = QDateTime(date1, time).toSecsSinceEpoch() - seconds;
    if(t < 0) t = 0;
    QDateTime dt = QDateTime::fromSecsSinceEpoch(t);
    return dt.date();
}
QDateTime DateTimeUtil::sub(const QDateTime& time1, const QDateTime& time2)
{
     qint64 t = time1.toSecsSinceEpoch() - time2.toSecsSinceEpoch();
     if(t < 0) t = 0;
     QDateTime dt = QDateTime::fromSecsSinceEpoch(t);
     return dt;
}
QDateTime DateTimeUtil::sub(const QDateTime& time1, const qint64& seconds)
{
    qint64 t = time1.toSecsSinceEpoch() - seconds;
    if(t < 0) t = 0;
    QDateTime dt = QDateTime::fromSecsSinceEpoch(t);
    return dt;
}

