#ifndef MYTIMELINE_H
#define MYTIMELINE_H
#include <QWidget>
#include <QPainter>
#include <qDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDateTime>
#include <QMap>
#include <QLabel>
#include "DateTimeUtil.h"
#include "TimeLineStyle.h"

class MyTimeLine : public QWidget
{
    Q_OBJECT

public:
    explicit MyTimeLine(QWidget *parent = nullptr);
    ~MyTimeLine();
    void SetDateTime(QDateTime dateTime);
    void InitializeUI();
    void SetTimeInterval(TimeInterval type);
    void SetTimeInterval(bool badd);
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);
signals:
    void TimeChangeSignal(QDateTime);
private:
    //计算时间轴的绘制线和时间文字信息
    void CalInterval(QVector<QLine> &lines, QVector<TextInfo_t>& textInfo, QVector<QRect>& VodRects);
    //计算时间轴偏移的比例，和实时的展示时间
    double CalMoveRatio(int movepix, QDateTime time, double moveRatio);
    //计算当前像素对应的文字信息
    QString CalTextInfo(int index, QTime midTime, int moveBlockSize);
    QString CalText(const int index, const QDateTime midDateTime, const int moveBlockSize);
    QVector<QRect> CalVodRects();
    QString CalMouseTime(QPoint p);
    //获取单个比例单位内，分成了几小块
    int GetBlockCount();
    int GetTimeRound();

private:
    TimeInterval m_IntervalType;    //本次时间间隔类型
    TimeInterval m_LastIntervalType;//上次时间间隔类型，关系到时间轴的更新频率
    int m_BeginX;                   //表示鼠标按下的位置
    int m_totalX;                   //表示累计的，每次鼠标位移位置
    int m_MoveX;                    //表示实时的鼠标位移
    QDateTime m_dateTime;           //初始的开始时间
    QDateTime m_MoveDateTime;       //时间轴实时的播放时间
    QDateTime m_LastMoveDateTime;   //时间轴上次实时绘制的播放时间，关系到时间轴的更新频率
    bool m_bMoving;                 //鼠标是否处于移动状态,避免时间轴和DateTimeEdit相互发信号
    QLabel *m_label;                //鼠标放置上去的时间信息
    int m_blockWidth;               //单个比例单位长度，px
    QVector<TimeInfo_t> m_VodVec;   //所有时间片
};
#endif // MYTIMELINE_H