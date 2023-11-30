#include "mythread.h"
#include <QMutex>

//使用全局变量
extern QMutex g_mutex; //线程锁
extern int l_int_scan_progress; //扫描进度
extern int m_i_onlinenum ; //在线数量
extern int ePort; //结束端口号
extern QStringList Ips; //ip列表
extern int port_num; //扫描端口的线程数量

Mythread::Mythread(int id,int ip_id)
{
    this->id=id;
    this->ip_id=ip_id;

}

void Mythread::set(int id,int ip_id){
    this->id=id;
    this->ip_id=ip_id;
}



void Mythread::run(){
    int i=id;//扫描的端口号
    QString t_ip=Ips.at(ip_id);
    //建立TCP和UDP的socket对象
    QTcpSocket *t_socket=new QTcpSocket();
    QUdpSocket *udpSocket=new QUdpSocket();
    udpSocket->joinMulticastGroup(QHostAddress(t_ip));
    while(i<=ePort){
        qDebug()<<"port="<<i<<"Mythread="<<QThread::currentThreadId();
        t_socket->abort();
        t_socket->connectToHost(t_ip,i);		//TCP端口扫描
        if(t_socket->waitForConnected(500)){
            m_i_onlinenum++;
            qDebug()<<t_ip<<i<<"connect over";
            emit port_connected(t_ip,i,"TCP");
        }else if(udpSocket->bind(QHostAddress::AnyIPv4,i)){ //UDP端口扫描
            m_i_onlinenum++;
            qDebug()<<t_ip<<i<<"connect over";
            emit port_connected(t_ip,i,"UDP");
        }else{
            qDebug()<<"t_ip="<<t_ip<<" "<<i<<"not open"<<endl;
        }
        i+=port_num;//扫描下一个端口号
    }
    l_int_scan_progress+=int((i-id)/port_num);
    emit progress(l_int_scan_progress);//向父线程传送进度
    emit over();//向父线程完成信号
    qDebug()<<"isDone";

}

