#ifndef THREADIPPORTSCAN_H
#define THREADIPPORTSCAN_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QStringList>
#include <ipthread.h>
#include <QVector>

class MainWindow;

class ThreadIpPortScan : public QThread
{
    Q_OBJECT
    public:
        ThreadIpPortScan();
        ~ThreadIpPortScan();

    private:
        QVector<IpThread*> ipthreads;//子线程序列
        int exit_num;//已经结束的子线程数
        bool isOpen;



    protected:
        void run() Q_DECL_OVERRIDE;

    public:
        void search_begin(void);    //开始搜寻
        void search_done(void);     //完成搜寻
        void search_stop(void);     //停止搜寻
        void init();

    public slots:
        //接收window的消息
        void recMegFromMain(QStringList,int,int,int,int);
        //接收成功的ip地址和端口号
        void rec_ip_connected(QString ip,int port,QString type);
        //接收已经结束的ip数
        void rec_over();

    signals:
        // 进度的信号，其中包括当前查找的在线系统数量
        void signal_scan_progress(int);
        //将成功的ip地址和端口号回传至主线程显示；
        void signal_online_ip(QString,int,QString);
        void over_all();
};

#endif // THREADIPPORTSCAN_H
