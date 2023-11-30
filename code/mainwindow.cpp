#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>

extern int l_int_scan_progress;
extern int m_i_onlinenum;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    threadipportscan=new ThreadIpPortScan();
    //接收子进程的数据
    connect(threadipportscan,&ThreadIpPortScan::signal_scan_progress,this,&MainWindow::slot_scan_progress);
    connect(threadipportscan,&ThreadIpPortScan::signal_online_ip,this,&MainWindow::slot_scan_online_ip);
    connect(threadipportscan,&ThreadIpPortScan::over_all,this,[&](){
       QMessageBox::information(this, "提示", "执行完毕!打开的端口数为"+QString::number(m_i_onlinenum));
    });
    //发数据给子线程
    connect(this, &MainWindow::sengMsgToThread, threadipportscan, &ThreadIpPortScan::recMegFromMain);
    qDebug() << "main="<<QThread::currentThreadId() << endl;
}

MainWindow::~MainWindow()
{
    delete ui;
    threadipportscan->search_stop();
    threadipportscan->wait();
    threadipportscan->deleteLater();
}



void MainWindow::slot_scan_progress(int value)
{
//    ui->progressBar->setValue(ui->progressBar->value()+1);
}

void MainWindow::slot_scan_online_ip(QString ip,int port,QString type)
{
    QString result=ip+" "+QString::number(port)+" "+type+"成功连接";
    ui->textEdit->append(result);
    qDebug()<<ip<<" "<<port;
}

void MainWindow::on_pushButton_clicked()
{
    ui->textEdit->clear();
//    ui->progressBar->setValue(0);
    threadipportscan->search_begin();
    QString list=ui->lineEdit->text();
    //获取IP序列
    QStringList ips=list.split(';');
    emit sengMsgToThread(ips,ui->startSpinBox_2->text().toInt(),ui->endSpinBox_2->text().toInt(),ui->ip_box->text().toInt(),ui->port_box->text().toInt());
    threadipportscan->init();
    m_i_onlinenum=0;
    threadipportscan->start();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(threadipportscan->isRunning())
    {
        threadipportscan->search_stop();
        threadipportscan->wait();
    }

    event->accept();
}
