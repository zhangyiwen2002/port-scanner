#include <QOBJECT>
#include <QMainWindow>
#include <QStringList>
#include "threadipportscan.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    public slots:
        void slot_scan_progress(int);
        void slot_scan_online_ip(QString,int,QString);

    protected:
        void closeEvent(QCloseEvent *event);

    private slots:
        void on_pushButton_clicked();

    private:
        Ui::MainWindow *ui;
        ThreadIpPortScan *threadipportscan;


    signals:
        //给线程发送消息
        void sengMsgToThread(QStringList,int,int,int,int);
};
