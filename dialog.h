#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_pushButton_openCom_clicked();

    void on_pushButton_clearRec_clicked();

    void on_pushButton_clearSend_clicked();

    void on_pushButton_send_clicked();

    void readSerial();

    void dealData();

//signals:
//    void sDealData(void);

private:
    Ui::Dialog *ui;

    QSerialPort com;

    struct{
        char buf[4096];
        int front;
        int rear;
    }LoopQueue;

    uchar sendDeal = 1;
    QTimer timer;



};

#endif // DIALOG_H
