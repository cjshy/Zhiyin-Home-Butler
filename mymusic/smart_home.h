#ifndef SMART_HOME_H
#define SMART_HOME_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QMouseEvent>
#include<QPoint>
#include<QPixmap>
#include<QMap>
#include<QPalette>
#include<QPainter>
#include<QFile>
#include<QTextStream>
class MainWindow;
namespace Ui {
class Smart_home;
}

class Smart_home : public QWidget
{
    Q_OBJECT

public:
    explicit Smart_home(MainWindow *mainWidow,QWidget *parent = nullptr);
    ~Smart_home();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void resizeEvent(QResizeEvent *event);
    void updataBackground();
    void set_Device();
    void updateDeviceStatus(const QString &command);
    void setDevicePixmp();
private slots:
    void slotLoadSerialPort();
    void slotCloseSerialPort();
    void slotOpenSerialPort();
    void slotReadData();
    void slotSend(QString str);
    void on_pushButton_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_min_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Smart_home *ui;
    QPoint diff_pos;  // 鼠标和窗口的相对位移
    QPoint window_pos;
    QPoint mouse_pos;
    QSerialPort *m_serialPort;
    QMap<QString, QString> deviceStatus; // 存放设备状态的容器
    MainWindow *mainWidow;
};

#endif // SMART_HOME_H
