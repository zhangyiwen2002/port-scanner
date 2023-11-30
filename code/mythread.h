#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QObject>
#include <QString>
#include <QThread>
#include <QTcpSocket>
#include <QUdpSocket>

class Mythread : public QThread
{
    Q_OBJECT

    private:
        int id;//扫描的起始端口号
        int ip_id;//扫描的ip地址位于整个序列中的位置
    public:
        Mythread(int,int);
        void set(int id,int ip_id);


        //QThread的虚函数
        //线程处理函数
        //不能直接调用，通过start()间接调用
        void run();
    signals:
        void port_connected(QString,int,QString);
        void over();
        void progress(int);

};

#endif // MYTHREAD_H
