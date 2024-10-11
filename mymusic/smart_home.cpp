#include "smart_home.h"
#include "ui_smart_home.h"
#include"mainwindow.h"
#include<QDebug>
Smart_home::Smart_home(MainWindow *mainWidow,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Smart_home),mainWidow(mainWidow)
{
    ui->setupUi(this);
    // 移除边框
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //更新背景
    updataBackground();
    //初始化设备信息
    set_Device();
    slotOpenSerialPort();
    //按钮美化
    QFile file(":/word.qss");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString str = in.readAll();
    file.close();
    this->setStyleSheet(str);

}

Smart_home::~Smart_home()
{
    delete ui;
}

void Smart_home::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mouse_pos = event->globalPos();
        window_pos = this->pos();
        diff_pos = mouse_pos - window_pos;
        event->accept();
    }
}

void Smart_home::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        QPoint pos = event->globalPos();
        this->move(pos - diff_pos);
        event->accept();
    }
}

void Smart_home::slotLoadSerialPort()
{
    QStringList m_serialPortName;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) //获取有效的串口列表
    {
        m_serialPortName << info.portName();
        qWarning()<<"serialPortName:"<<info.portName();
    }
}

void Smart_home::slotCloseSerialPort()
{
    if (m_serialPort)
        m_serialPort->close();
}

void Smart_home::slotOpenSerialPort()
{
    m_serialPort = new QSerialPort(this);//新建一个窗口对象

    if(m_serialPort->isOpen())//判断对象是否已经打开，如果是就关掉
    {
        m_serialPort->clear();
        m_serialPort->close();
    }
    //设置串口名字
    m_serialPort->setPortName("COM2");
    //打开串口
    if (!m_serialPort->open(QIODevice::ReadWrite))//打开COM2串口，并且设置为读写状态
    {
        return;
    }

    m_serialPort->setBaudRate(9600);//波特率
    m_serialPort->setDataBits(QSerialPort::Data8);//数据位
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);//数据流控制（暂时不管）

    m_serialPort->setParity(QSerialPort::NoParity);//奇偶校验位
    m_serialPort->setStopBits(QSerialPort::OneStop);//停止位


    //把串口的可读信号和槽函数连接
    connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(slotReadData()));
}

void Smart_home::slotReadData()
{
    QString data = m_serialPort->readAll();
    qWarning()<<"接收数据："<<data;
}

void Smart_home::slotSend(QString str)
{
    if (m_serialPort)
    {
        if (!m_serialPort->isOpen())
            qWarning()<<"请打开一个串口";

        QString info = str;
        QByteArray sendBuf;
        //把字符串转为字节
        sendBuf.append(info);// = info.toLocal8Bit();
        m_serialPort->write(sendBuf+"\n");

        updateDeviceStatus(str);
        setDevicePixmp();
    }
}

void Smart_home::on_pushButton_close_clicked()
{
    mainWidow->form->show();
    this->close();
}

void Smart_home::on_pushButton_min_clicked()
{
    this->showMinimized();
}


void Smart_home::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updataBackground();
}

void Smart_home::updataBackground()
{
    // 加载背景图片
    QPixmap background(":/tu/b2.png");

    // 检查图片是否成功加载
    if (background.isNull()) {
        qDebug() << "Failed to load background image: :/tu/back.jpg";
        return;
    }

    // 确保图片不模糊
    background = background.scaled(ui->widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 创建一个白色背景的QPixmap
    QPixmap whiteBackground(background.size());
    whiteBackground.fill(Qt::white);

    // 将背景图片绘制到白色背景上
    QPainter painter(&whiteBackground);
    painter.drawPixmap(0, 0, background);

    // 创建一个带有弧度的矩形路径
    QPainterPath path;
    QRectF rect(0, 0, ui->widget->width(), ui->widget->height());
    qreal radius = 20; // 弧度半径
    path.addRoundedRect(rect, radius, radius);

    // 创建一个带有弧度的背景图片
    QPixmap roundedBackground(background.size());
    roundedBackground.fill(Qt::transparent);
    QPainter roundedPainter(&roundedBackground);
    roundedPainter.setRenderHint(QPainter::Antialiasing);
    roundedPainter.setClipPath(path);
    roundedPainter.drawPixmap(0, 0, whiteBackground);

    // 设置调色板
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(roundedBackground));
    ui->widget->setPalette(palette);

    // 设置窗口背景透明
    ui->widget->setAutoFillBackground(true);
}


void Smart_home::on_pushButton_2_clicked()
{
    qDebug()<<"关闭风扇";
    slotSend("关闭风扇");
}

void Smart_home::on_pushButton_clicked()
{
    qDebug()<<"打开风扇";
    slotSend("打开风扇");
}

void Smart_home::on_pushButton_3_clicked()
{
    qDebug()<<"打开空调";
    slotSend("打开空调");
}

void Smart_home::on_pushButton_4_clicked()
{
    qDebug()<<"关闭空调";
    slotSend("关闭空调");
}

void Smart_home::on_pushButton_5_clicked()
{
    qDebug()<<"打开电灯";
    slotSend("打开电灯");
}

void Smart_home::on_pushButton_6_clicked()
{
    qDebug()<<"关闭电灯";
    slotSend("关闭电灯");
}

void Smart_home::set_Device()
{
    // 初始化设备状态
    deviceStatus.insert("风扇", "关闭");
    deviceStatus.insert("空调", "关闭");
    deviceStatus.insert("电灯", "关闭");
}

void Smart_home::updateDeviceStatus(const QString &command) // 更新设备状态
{
    static const QMap<QString, QString> commandToDevice = {
        {"打开风扇", "风扇"},
        {"关闭风扇", "风扇"},
        {"打开空调", "空调"},
        {"关闭空调", "空调"},
        {"打开电灯", "电灯"},
        {"关闭电灯", "电灯"}
    };

    if (commandToDevice.contains(command)) {
        QString device = commandToDevice[command];
        QString action = command.startsWith("打开") ? "打开" : "关闭"; // 根据命令决定动作
        deviceStatus[device] = action; // 更新设备状态
        qDebug() << device << action; // 打印结果
    } else {
        qDebug() << "未知命令：" << command; // 处理未知命令
    }
    setDevicePixmp();
}

void Smart_home::setDevicePixmp() // 更新图标
{
    // 更新风扇图标
    if (deviceStatus["风扇"] == "打开") {
        ui->label->setStyleSheet("border-image: url(:/chat_lib/风扇_2.png);");
    } else {
        ui->label->setStyleSheet("border-image: url(:/chat_lib/风扇.png);");
    }

    // 更新空调图标
    if (deviceStatus["空调"] == "打开") {
        ui->label_2->setStyleSheet("border-image: url(:/chat_lib/制热.png);");
    } else {
        ui->label_2->setStyleSheet("border-image: url(:/chat_lib/空调.png);");
    }

    // 更新电灯图标
    if (deviceStatus["电灯"] == "打开") {
        ui->label_3->setStyleSheet("border-image: url(:/chat_lib/电灯.png);");
    } else {
        ui->label_3->setStyleSheet("border-image: url(:/chat_lib/电灯泡.png);");
    }
}


void Smart_home::on_pushButton_7_clicked() // 关闭所有设备
{
    // 关闭所有设备的指令
    slotSend("关闭风扇");

    slotSend("关闭空调");
    slotSend("关闭电灯");

    // 更新设备状态
    deviceStatus["风扇"] = "关闭";
    deviceStatus["空调"] = "关闭";
    deviceStatus["电灯"] = "关闭";

    // 更新界面图标
    setDevicePixmp();

    qDebug() << "所有设备已关闭";
}
