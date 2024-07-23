#include "MyTimeLine.h"

MyTimeLine::MyTimeLine(QWidget *parent) :
    QWidget(parent),
    m_IntervalType(_OneMin),
    m_BeginX(0),
    m_totalX(0),
    m_MoveX(0),
    m_blockWidth(0),
    m_bMoving(false)
{
    InitializeUI();
}

MyTimeLine::~MyTimeLine()
{
}
void MyTimeLine::InitializeUI()
{
    m_dateTime = TimeLineStyle::beginDateTime;
    m_MoveDateTime = TimeLineStyle::beginDateTime;
    m_label = new QLabel(this);
    m_label->setText(m_MoveDateTime.time().toString("hh:mm:ss"));
    m_label->setHidden(true);
    m_label->resize(60, 20);
    this->setMouseTracking(true);
}
void MyTimeLine::SetTimeInterval(TimeInterval type)
{
    m_IntervalType = type;
}
void MyTimeLine::SetTimeInterval(bool badd)
{
    QList<TimeInterval> list = {_OneMin, _FiveMin, _TenMin, _ThirtyMin, _OneHour, _TwoHour};
    int index = list.indexOf(m_IntervalType);
    if(0 < index && index < list.size() - 1)
    {
        if(badd) index++;
        else index--;
    }
    else if(0 == index && badd)
    {   //放大坐标轴
        index++;
    }
    else if(index == list.size() - 1 && !badd)
    {   //缩小坐标轴
        index--;
    }
    m_IntervalType = list.at(index);
    SetDateTime(m_MoveDateTime);
    //触发paintEvent进行绘制
    update();
}
void MyTimeLine::SetDateTime(QDateTime dateTime)
{
    if(m_bMoving) return;
    m_dateTime = dateTime;
    m_BeginX = 0;
    m_totalX = 0;
    m_MoveX = 0;
    //触发paintEvent进行绘制
    update();
}
void MyTimeLine::paintEvent(QPaintEvent *event)
{
    QVector<QLine> lines;
    QVector<TextInfo_t> textInfo;
    CalInterval(lines, textInfo);
    //STEP 1 ：先绘制间隔
    QPen pen(TimeLineStyle::green);
    QPainter painter(this);
    painter.setPen(pen);
    painter.drawLines(lines);
    //最终绘制文本时，向左偏移23个像素，让文字居中显示
    for(auto iter : textInfo)
    {
        iter.point.setX(iter.point.x() - 23);
        painter.drawText(iter.point, iter.text);
    }
    //STEP 2 ：再绘制中间红线，设置为>2，界面显示更加合理一些
    pen.setWidth(1);
    pen.setColor(TimeLineStyle::red);
    painter.setPen(pen);
    painter.drawLine(*lines.begin());
}
void MyTimeLine::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
    {
        SetTimeInterval(false);
    }
    else
    {
        SetTimeInterval(true);
    }
    m_label->setHidden(true);
}
void MyTimeLine::enterEvent(QEvent *)
{
    m_label->setHidden(false);
}
void MyTimeLine::leaveEvent(QEvent *)
{
    m_label->setHidden(true);
}
void MyTimeLine::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_bMoving = true;
        m_BeginX = event->pos().x();
    }
}
void MyTimeLine::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int curMoveX = event->pos().x() - m_BeginX;
        m_MoveX = m_totalX + curMoveX;
        update();
    }
    //鼠标在拖动时间轴时，不更新label信息,只移动label
    QPoint mousePos = event->pos();
    m_label->setHidden(false);
    m_label->move(mousePos.x() + 3, this->size().height() - 15);
    if(!m_bMoving)
    {
        QString timestr = CalMouseTime(mousePos);
        m_label->setText(timestr);
    }
}
void MyTimeLine::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_bMoving = false;
        m_totalX = m_totalX +  event->pos().x() - m_BeginX;
        m_MoveX = m_totalX;
    }
}
void MyTimeLine::CalInterval(QVector<QLine> &lines, QVector<TextInfo_t>& textInfo)
{
    //lines允许超出size的坐标，x < 0 || x > size().width()
    //STEP 1: 添加红线。如果是奇数长度，则红线在偏左，右边比左边大1px，偶数则相等；
    int TimeLineHeight = this->size().height();
    int TimeLineWidth = this->size().width();
    int mid = this->size().width() / 2;
    QLine RedLine(mid, 0, mid, TimeLineHeight - 2);
    lines.push_back(RedLine);

    //STEP 2: 按块划分间隔。添加白线，当间隔为1hour时，默认分为12块，每块中间再分6格,
    int BlockCount = GetBlockCount();
    m_blockWidth = (TimeLineWidth - TimeLineStyle::MarginSize * 2) / TimeLineStyle::TimeCount;
    int blockWidthMod = (TimeLineWidth - TimeLineStyle::MarginSize * 2) % TimeLineStyle::TimeCount;
    if(0 == m_blockWidth)
    {
        //打印分组失败，当前长度不满足分为TimeCount组
        return;
    }
    //划分成12块后，对移除的像素并入到两边MarginSize中
    int LeftMargin = 0;
    int RightMargin = 0;
    if(0 == blockWidthMod % 2)
    {
        LeftMargin = TimeLineStyle::MarginSize + blockWidthMod / 2;
        RightMargin = TimeLineStyle::MarginSize + blockWidthMod / 2;
    }
    else if(0 == (blockWidthMod - 1) % 2)
    {
        LeftMargin = TimeLineStyle::MarginSize + blockWidthMod / 2;
        RightMargin = TimeLineStyle::MarginSize + blockWidthMod / 2 + 1;
    }

    //STEP 3: 计算综合偏移量，正负和时间增大效果相反，后续可优化4舍5入精度，提高取模结果合理性
    double moveRatio = CalMoveRatio(m_MoveX, m_dateTime, m_blockWidth);
    int MovePixMod = (int)((double)moveRatio * (double)m_blockWidth) % m_blockWidth;

    //STEP 4:添加长竖线, 为了展示效果合理性，需要添加13+2根长竖线，+2表示余出左右两边，以应对拖动
    for(int i = -1; i <= TimeLineStyle::TimeCount + 1; i++)
    {   //第i根长竖线w = 左边Margin + i*单个块长度 - 像素偏移量
        int LongW = LeftMargin + i * m_blockWidth - MovePixMod;
        lines.push_back(QLine(LongW, TimeLineHeight - TimeLineStyle::LongLine, LongW, TimeLineHeight));

        //STEP 5: 添加短竖线，需要添加4/5根竖线
        for(int j = 1; j < BlockCount; j++)
        {
            int ShortH = TimeLineHeight - TimeLineStyle::ShortLine;
            double dj = (double)j;
            //第j根短竖线w = 长竖线w + j/BlockCount * 块长度
            int ShortW = LongW + dj / (double)BlockCount * (double)m_blockWidth;
            //当为5根短竖线时，第3根稍微加长显示
            if(BlockCount / 2 == j && 0 == BlockCount % 2) ShortH = TimeLineHeight - TimeLineStyle::MidLine;
            lines.push_back(QLine(ShortW, ShortH, ShortW, TimeLineHeight));
        }

        //STEP 6: 生成文本
        TextInfo_t info;
        info.point = QPoint(LongW, TimeLineStyle::TextHeight);
        info.text = CalText(i, m_dateTime, moveRatio);
        textInfo.push_back(info);
    }
}

double MyTimeLine::CalMoveRatio(int movepix, QDateTime datetime, double moveRatio)
{
    int interval = m_IntervalType;
    //t1:鼠标累计的偏移像素; t2:非整点时间的偏移位置
    double t1, t2;
    if(0 == moveRatio) t1 = 0;
    else t1 = (double)movepix / moveRatio;
    int ss = 0;
    if(_OneHour == m_IntervalType)
    {
        ss = datetime.time().minute() * 60 + datetime.time().second();
    }
    if(_TwoHour == m_IntervalType)
    {
        ss = datetime.time().hour() % 2 * 3600 + datetime.time().minute() * 60 + datetime.time().second();
    }
    if(_OneMin == m_IntervalType)
    {
        ss = datetime.time().second();
    }
    if(_FiveMin == m_IntervalType)
    {
        int s1 = datetime.time().minute() * 60 + datetime.time().second();
        ss = s1 % _FiveMin;
    }
    if(_TenMin == m_IntervalType)
    {
        int s1 = datetime.time().minute() * 60 + datetime.time().second();
        ss = s1 % _TenMin;
    }
    if(_ThirtyMin == m_IntervalType)
    {
        int s1 = datetime.time().minute() * 60 + datetime.time().second();
        ss = s1 % _ThirtyMin;
    }
    t2 = (double)ss / (double)interval;
    //t > 0,表示实际时间增大，鼠标是执行了向左拖动，t < 0，时间减小，鼠标向右拖
    double t = t2 - t1;
    //计算经过鼠标偏移后，实时显示的时间，及时发送，避免信号内容覆盖
    m_MoveDateTime = DateTimeUtil::sub(datetime, t1 * interval);
    emit TimeChangeSignal(m_MoveDateTime);
    return t;
}
QString MyTimeLine::CalText(const int index, const QDateTime midDateTime, const int moveBlockSize)
{
    QTime time(0,0,0);
    //STEP 1: 将时间按照间隔进行取模处理,例如间隔为1h，12:34:56处理后为12:00:00, 间隔30分钟，处理后12:30:00
    QDateTime IntegerTime = midDateTime;

    if(_OneHour == m_IntervalType)
    {
        IntegerTime.setTime(QTime(midDateTime.time().hour(), 0, 0));
    }
    if(_TwoHour == m_IntervalType)
    {            //提醒：这里是做了按2舍去归整计算，不可以省略
        int hh = midDateTime.time().hour() / 2 * 2;
        IntegerTime.setTime(QTime(hh, 0, 0));
    }
    if(_OneMin == m_IntervalType)
    {
        IntegerTime.setTime(QTime(midDateTime.time().hour(), midDateTime.time().minute(), 0));
    }
    if(_FiveMin == m_IntervalType)
    {
        int mm = midDateTime.time().minute() / 5 * 5;
        IntegerTime.setTime(QTime(midDateTime.time().hour(), mm, 0));
    }
    if(_TenMin == m_IntervalType)
    {
        int mm = midDateTime.time().minute() / 10 * 10;
        IntegerTime.setTime(QTime(midDateTime.time().hour(), mm, 0));
    }
    if(_ThirtyMin == m_IntervalType)
    {
        int mm = midDateTime.time().minute() / 30 * 30;
        IntegerTime.setTime(QTime(midDateTime.time().hour(), mm, 0));
    }

    //STEP 2: 根据中间的时间和Interval，计算出最左边的时间
    QDateTime LeftTime = DateTimeUtil::sub(IntegerTime, (TimeLineStyle::TimeCount / 2) * m_IntervalType);

    //STEP 3: 根据偏移量，更新最左边的时间
    QDateTime MoveLeftTime = DateTimeUtil::sub(LeftTime, -1 * moveBlockSize * m_IntervalType);

    //STEP 4: 根据index，计算i个间隔后的时间
    time = DateTimeUtil::sub(MoveLeftTime, -1 * index * m_IntervalType).time();
    //qDebug() << "  midTime" << midDateTime.time().toString("hh:mm:ss")
    //         << "  IntegerTime" << IntegerTime.time().toString("hh:mm:ss")
    //         << "  LeftTime" << LeftTime.time().toString("hh:mm:ss")
    //         << "  MoveLeftTime" << MoveLeftTime.time().toString("hh:mm:ss")
    //         << "  time" << time.toString("hh:mm:ss");
    return time.toString("hh:mm:ss");
}
QString MyTimeLine::CalMouseTime(QPoint p)
{
    int width = this->size().width() / 2;
    double ratio =  ((double)p.x() - (double)width) / (double)m_blockWidth;
    QDateTime time = DateTimeUtil::sub(m_MoveDateTime, -1 * ratio * m_IntervalType);
    return time.toString("hh:mm:ss");
}
int MyTimeLine::GetBlockCount()
{   //BlockCount不能为0
    if(_OneMin  == m_IntervalType || _ThirtyMin  == m_IntervalType || _OneHour  == m_IntervalType || _TwoHour  == m_IntervalType)
        return 6;
    else if(_FiveMin == m_IntervalType || _TenMin == m_IntervalType) return 5;
    else return 1;
}
int MyTimeLine::GetTimeRound()
{   //BlockCount不能为0
    if(_OneMin  == m_IntervalType || _FiveMin == m_IntervalType || _TenMin == m_IntervalType || _ThirtyMin  == m_IntervalType)
        return 60;
    else if( _OneHour  == m_IntervalType || _TwoHour  == m_IntervalType) return 24;
    return 24;
}