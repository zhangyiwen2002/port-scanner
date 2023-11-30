#ifndef IPTHREAD_H
#define IPTHREAD_H
#include "mythread.h"
#include <QStringList>



class IpThread : public QThread
{
    Q_OBJECT
    private:
        int id;//线程开始的id值
        int exit_num;//子线程中结束的线程数
        int thread_num;//该线程需要扫描的IP地址数
        QVector<Mythread*> mythreads;//子线程的集合
    public:
        IpThread(int);
        void init();
        ~IpThread();
    protected:
        void run() Q_DECL_OVERRIDE;
    public slots:
        //接收子进程结束信号
        void rec_over();
    signals:
        void ip_connected(QString,int,QString);//发送连接成功的IP地址，端口号和连接方式
        void over();//发送完成信号
        void progress(int);//发送进度信号
};

#endif // IPTHREAD_H
