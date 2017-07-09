#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QComboBox>
#include <QGridLayout>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    memset((void*)&LoopQueue,0,sizeof(LoopQueue));

    ui->comboBox_ComChoose->installEventFilter(this);
    //com.installEventFilter(this);

    connect(&com,SIGNAL(readyRead()),this,SLOT(readSerial()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(dealData()));
}

Dialog::~Dialog()
{
    delete ui;
}

bool Dialog::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
       if (obj == ui->comboBox_ComChoose)
        {
           ui->comboBox_ComChoose->clear();
           QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
           for (const QSerialPortInfo &info : infos)
           {
               ui->comboBox_ComChoose->addItem(info.portName());
               qDebug() << info.description();
           }
       }
    }

    return QWidget::eventFilter(obj, event);
}


void Dialog::on_pushButton_openCom_clicked()
{
    if(!com.isOpen())
    {
        QString currentPortname = ui->comboBox_ComChoose->currentText();

        qDebug() << currentPortname;
        com.setPortName(ui->comboBox_ComChoose->currentText());
       // qDebug("%d",ui->comboBox_BaudRate->currentText().toInt());
        com.setBaudRate(ui->comboBox_BaudRate->currentText().toInt());

        int stopBit = ui->comboBox_stopBit->currentText().toInt();
        if(1.5 == stopBit)
        {
            com.setStopBits(QSerialPort::OneAndHalfStop);
        }
        else if(1 == stopBit)
        {
            com.setStopBits(QSerialPort::OneStop);
        }
        else if(2 == stopBit)
        {
            com.setStopBits(QSerialPort::TwoStop);
        }

        com.setDataBits((QSerialPort::DataBits)(ui->comboBox_dataBit->currentText().toInt()));

        int parity = ui->comboBox_oddevenCheck->currentIndex();
        if(0 == parity)
        {
            com.setParity(QSerialPort::NoParity);
        }
        else if(1 == parity)
        {
            com.setParity(QSerialPort::EvenParity);
        }
        else if(2 == parity)
        {
            com.setParity(QSerialPort::OddParity);
        }

        if(false==com.open(QIODevice::ReadWrite))
        {
            qDebug() << "串口打开失败";
        }
        else
        {
            ui->pushButton_openCom->setText("关闭串口");
            qDebug() << "串口打开成功";
        }
    }
    else
    {
        com.close();
        if(!com.isOpen())
        {
            ui->pushButton_openCom->setText("打开串口");
        }
    }

}


void Dialog::on_pushButton_clearRec_clicked()
{
    ui->textBrowser_RxWight->clear();
}

void Dialog::on_pushButton_clearSend_clicked()
{
    ui->textEdit_TxWight->clear();
}

void Dialog::on_pushButton_send_clicked()
{
    QString  buf = ui->textEdit_TxWight->toPlainText();
    qDebug() << "[发送]" + buf;
    com.write(buf.toStdString().c_str());
}

void Dialog::readSerial()
{
    if((LoopQueue.rear+1)%4096 == LoopQueue.front)
    {
        /*暂停接受*/
        //待完善
    }
    else
    {
       QByteArray buf = com.readAll();
       foreach( char temp , buf )
       {
           LoopQueue.buf[LoopQueue.rear] = temp;
           LoopQueue.rear = (LoopQueue.rear + 1) % 4096;
       }

    }
    if(sendDeal)
    {
        sendDeal = 0;
        timer.start(500);
    }
}

void Dialog::dealData()
{
    timer.stop();


    if((LoopQueue.rear+1)%4096 == LoopQueue.front)
    {
        sendDeal = 1;
    }
}
