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
    ui->widget->setStyleSheet(QString("QWidget{background-color:rgb(160,160,160)}"));
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
        ui->dateTimeEdit->setDateTime(dateTime);
    });
    connect(ui->dateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, [=](QDateTime datetime){
        ui->widget->SetDateTime(datetime);
    });
}
