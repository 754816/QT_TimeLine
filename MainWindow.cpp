#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::InitUI()
{
    QDateTime datetime = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(datetime);
    ui->widget->SetDateTime(datetime);
}
void MainWindow::InitConnect()
{
    connect(ui->pushButton_ZoomIn, &QPushButton::clicked, this, [=](){
        ui->widget->SetTimeInterval(false);
    });
    connect(ui->pushButton_ZoomOut, &QPushButton::clicked, this, [=](){
        ui->widget->SetTimeInterval(true);
    });
    connect(ui->widget, &MyTimeLine::TimeChangeSignal, this, [=](QDateTime dateTime){
        if(ui->dateTimeEdit->dateTime() != dateTime)
            ui->dateTimeEdit->setDateTime(dateTime);
    });
    connect(ui->dateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, [=](QDateTime datetime){
        ui->widget->SetDateTime(datetime);
    });

    m_Timer = new QTimer(this);
    connect(m_Timer, &QTimer::timeout, this, [=](){
        QDateTime curDateTime = QDateTime::currentDateTime();
        ui->widget->SetDateTime(curDateTime);
    });
    m_Timer->start(500);
}
