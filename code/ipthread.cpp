#include "ipthread.h"
#include <QCoreApplication>

extern QStringList Ips;//需要扫描的ip地址序列
extern int sPort;//需要扫描的开始端口
extern int ePort;//需要扫描的结束端口
extern int port_num;//扫描端口的线程数量
extern int ip_num;//扫描IP的线程数量

IpThread::IpThread(int id)
{
    this->id=id;
    this->exit_num=0;
    thread_num=0;
    for(int j=0;j<1000;j++){
        Mythread *mythread=new Mythread(sPort+j,id);
        mythreads.append(mythread);
        connect(mythreads[j],&Mythread::port_connected,this,[=](QString ip,int port,QString type){
            emit ip_connected(ip,port,type);
        },Qt::DirectConnection);
        connect(mythreads[j],&Mythread::progress,this,[=](int l_int_scan_progress){
            emit progress(l_int_scan_progress);
        },Qt::DirectConnection);
        connect(mythreads[j],&Mythread::over,this,&IpThread::rec_over,Qt::DirectConnection);
    }
}


void IpThread::rec_over(){
    this->exit_num++;
    if(exit_num==thread_num*port_num){
        emit over();
    }
}


void IpThread::run(){
    int i=id;//扫描的IP地址在序列中的位置
    int cnt=0;
    for(int cnt=id;cnt<Ips.length();cnt+=ip_num){
        thread_num++;
    }
    qDebug()<<"thread_num="<<thread_num<<ip_num;
    while(i<Ips.length()){
        qDebug() << "IpThread="<<i<< endl;
        //线程任务
        for(int j=0;j<port_num;j++){
            mythreads[j+port_num*cnt]->set(sPort+j,i);
            mythreads[j+port_num*cnt]->start();
        }
        i+=ip_num;//扫描下一个IP地址
        cnt++;
    }

}

void IpThread::init(){
    exit_num=0;
    thread_num=0;
}

IpThread::~IpThread(){

}
