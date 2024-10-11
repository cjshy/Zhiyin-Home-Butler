#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QMouseEvent>
#include<QPoint>
#include<QPixmap>
#include<QMap>
#include<QPalette>
#include<QPainter>
#include<QMovie>
#include"HoverFrame.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void resizeEvent(QResizeEvent *event);
    void updataBackground();
    void set_Device();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void slotLoadSerialPort();
    void slotCloseSerialPort();
    void slotOpenSerialPort();
    void slotReadData();
    void slotSend(QString str);
    void on_pushButton_close_clicked();
    void on_pushButton_min_clicked();

private:
    Ui::MainWindow *ui;
    QPoint diff_pos;  // 鼠标和窗口的相对位移
    QPoint window_pos;
    QPoint mouse_pos;
    QSerialPort *m_serialPort;
    HoverFrame *hoverframe_1;
    QMap<QString, QString> deviceStatus; // 存放设备状态的容器
    QMovie *movie;
    QString receivedData; // 用于存储接收到的所有数据
};


#endif // MAINWINDOW_H
