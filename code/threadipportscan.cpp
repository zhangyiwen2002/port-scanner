#include "threadipportscan.h"
#include "mythread.h"
#include <QDebug>
#include <QTcpSocket>
#include <QMutex>
#include <QVector>

//锁 设为全局变量
QMutex g_mutex;
//要搜索的ip列表和端口范围
QStringList Ips;
int sPort;
int ePort;
//执行暂停和停止
bool m_b_paused = true;
bool m_b_stop = false;
//打开的ip
int  m_i_onlinenum = 0;
//搜索ip的进程数
int ip_num;
//搜索端口的进程数
int port_num;

QStringList m_qstringlist_onlineip;
//执行进度
int l_int_scan_progress = 0;

ThreadIpPortScan::ThreadIpPortScan()
{
    this->exit_num=0;
    for(int j=0;j<1000;j++){
        IpThread *ipthread=new IpThread(j);
        ipthreads.append(ipthread);
        connect(ipthreads[j],SIGNAL(ip_connected(QString,int,QString)),this,SLOT(rec_ip_connected(QString,int,QString)),Qt::DirectConnection);
        connect(ipthreads[j],&IpThread::over,this,&ThreadIpPortScan::rec_over,Qt::DirectConnection);
        connect(ipthreads[j],&IpThread::progress,this,[=](int l_int_scan_progress){
            qDebug()<<"l_int_scan_progress="<<l_int_scan_progress;
             emit signal_scan_progress(l_int_scan_progress);
        },Qt::DirectConnection);
    }
}

ThreadIpPortScan::~ThreadIpPortScan()
{
    this->search_stop();
    this->wait();
    this->deleteLater();
}

void ThreadIpPortScan::rec_ip_connected(QString ip,int port,QString type){
    emit signal_online_ip(ip,port,type);
}

void ThreadIpPortScan::rec_over(){
    this->exit_num++;
    if(this->exit_num==ip_num){
        qDebug()<<"已完成"<<m_i_onlinenum;
        emit over_all();
    }
}

void ThreadIpPortScan::run()
{
    while (!m_b_stop)
    {
        if(!m_b_paused)
        {

//            ipthreads.clear();

            qDebug() << "thread="<<QThread::currentThreadId() << endl;
            qDebug()<<ipthreads.length();
            m_i_onlinenum = 0;
            m_qstringlist_onlineip.clear();

            //线程任务
            for(int j=0;j<ip_num;j++){
                ipthreads[j]->start();
            }
            //子线程全部打开，发送信号
            search_done();
            qDebug() << QThread::currentThreadId() << endl;

        }

    }

}

void ThreadIpPortScan::recMegFromMain(QStringList ips, int sport, int eport, int num,int num1){
    Ips=ips;
    sPort=sport;
    ePort=eport;
    //线程数量不能超过待扫描的数量
    if(ips.length()>=num){
        ip_num=num;
    }else{
        ip_num=ips.length();
    }
    if(eport-sport+1>=num1){
        port_num=num1;
    }else{
        port_num=eport-sport+1;
    }

}

void ThreadIpPortScan::init(){
    this->exit_num=0;
    for(int i=0;i<1000;i++){
        ipthreads[i]->init();
    }
}


void ThreadIpPortScan::search_begin()
{
    m_b_paused = false;
}

void ThreadIpPortScan::search_done()
{
    m_b_paused = true;	//扫描完成后，将自动暂停，等待下一次启动
//    m_b_stop = true;
}

void ThreadIpPortScan::search_stop()
{
    m_b_stop = true;		//停止循环，退出线程
}
