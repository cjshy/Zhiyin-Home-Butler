#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 移除边框
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    // 更新背景
    updataBackground();
    //
    hoverframe_1 = new HoverFrame(this);
    hoverframe_1->hide();
    ui->pushButton->installEventFilter(this);
    ui->pushButton_2->installEventFilter(this);
    ui->pushButton_3->installEventFilter(this);
    slotOpenSerialPort();
    //
    set_Device();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->pushButton) { // 检查是否是特定的按钮
        if (event->type() == QEvent::Enter) {
            hoverframe_1->setGeometry(QCursor::pos().x(), QCursor::pos().y() - 50, 200, 100);
            if(deviceStatus["风扇"] == "关闭")
            {
                hoverframe_1->updateContent("风扇关闭", QPixmap(":/tu/风扇.png")); // 更新悬浮框内容
            }
            else
            {
                movie = new QMovie(":/tu/fs2.gif");
                hoverframe_1->updateContent("风扇打开", QPixmap(":/tu/风扇.png"),movie); // 更新悬浮框内容
            }
            hoverframe_1->show();
            return true; // 处理事件
        } else if (event->type() == QEvent::Leave) {
            hoverframe_1->hide();
            return true; // 处理事件
        }
    }
    else if(obj == ui->pushButton_2)
    {
        if (event->type() == QEvent::Enter) {
            hoverframe_1->setGeometry(QCursor::pos().x(), QCursor::pos().y() - 50, 200, 100);
            if(deviceStatus["电灯"] == "关闭")
            {
                hoverframe_1->updateContent("电灯关闭", QPixmap(":/tu/电灯泡.png")); // 更新悬浮框内容
            }
            else
            {
                hoverframe_1->updateContent("电灯打开", QPixmap(":/tu/电灯.png")); // 更新悬浮框内容
            }
            hoverframe_1->show();
            return true; // 处理事件
        } else if (event->type() == QEvent::Leave) {
            hoverframe_1->hide();
            return true; // 处理事件
        }
    }
    else if(obj == ui->pushButton_3)
    {
        if (event->type() == QEvent::Enter) {
            hoverframe_1->setGeometry(QCursor::pos().x(), QCursor::pos().y() - 50, 200, 100);
            if(deviceStatus["空调"] == "关闭")
            {
                 hoverframe_1->updateContent("空调关闭", QPixmap(":/tu/空调.png")); // 更新悬浮框内容
            }
            else
            {
                    movie = new QMovie(":/tu/kt.gif");
                 hoverframe_1->updateContent("空调打开", QPixmap(":/tu/空调.png"),movie); // 更新悬浮框内容
            }
            hoverframe_1->show();
            return true; // 处理事件
        } else if (event->type() == QEvent::Leave) {
            hoverframe_1->hide();
            return true; // 处理事件
        }
    }
    return QMainWindow::eventFilter(obj, event); // 否则继续默认处理
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mouse_pos = event->globalPos();
        window_pos = this->pos();
        diff_pos = mouse_pos - window_pos;
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        QPoint pos = event->globalPos();
        this->move(pos - diff_pos);
        event->accept();
    }
}

void MainWindow::slotLoadSerialPort()
{
    QStringList m_serialPortName;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) //获取有效的串口列表
    {
        m_serialPortName << info.portName();
        qWarning()<<"serialPortName:"<<info.portName();
    }
}

void MainWindow::slotCloseSerialPort()
{
    if (m_serialPort)
        m_serialPort->close();
}

void MainWindow::slotOpenSerialPort()
{
    m_serialPort = new QSerialPort(this);//新建一个窗口对象

    if(m_serialPort->isOpen())//判断对象是否已经打开，如果是就关掉
    {
        m_serialPort->clear();
        m_serialPort->close();
    }
    //设置串口名字
    m_serialPort->setPortName("COM1");
    //打开串口
    if (!m_serialPort->open(QIODevice::ReadWrite))//打开COM1串口，并且设置为读写状态
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
void MainWindow::slotReadData()
{
    // 读取串口的数据
    receivedData += m_serialPort->readAll();

    while (true) {
        int endIndex = receivedData.indexOf('\n'); // 查找换行符的位置
        if (endIndex == -1) {
            break; // 没有找到结束符，退出循环
        }

        // 提取完整命令
        QString command = receivedData.left(endIndex).trimmed(); // 处理前后空格
        receivedData.remove(0, endIndex + 1); // 移除已处理的命令

        // 处理接收到的命令
        qWarning() << "接收数据：" << command;

        // 创建一个命令到设备状态的映射
        static const QMap<QString, QString> commandToDevice = {
            {"打开风扇", "风扇"},
            {"关闭风扇", "风扇"},
            {"打开空调", "空调"},
            {"关闭空调", "空调"},
            {"打开电灯", "电灯"},
            {"关闭电灯", "电灯"}
        };

        // 检查命令是否在映射中
        if (commandToDevice.contains(command)) {
            QString device = commandToDevice[command]; // 获取设备名称
            QString action = command.startsWith("打开") ? "打开" : "关闭"; // 根据命令决定动作
            deviceStatus[device] = action; // 更新设备状态
            qDebug() << device << action; // 打印结果
        } else {
            qDebug() << "未知命令：" << command; // 处理未知命令
        }
    }
}



void MainWindow::slotSend(QString str)
{
    if (m_serialPort)
    {
        if (!m_serialPort->isOpen())
            qWarning()<<"请打开一个串口";

        QString info = str;
        QByteArray sendBuf;
        //把字符串转为字节
        sendBuf.append(info);// = info.toLocal8Bit();
        m_serialPort->write(sendBuf);
    }
}

void MainWindow::on_pushButton_close_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_min_clicked()
{
    this->showMinimized();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updataBackground();
}

void MainWindow::updataBackground()
{
    // 加载背景图片
    QPixmap background(":/tu/一居室平面图.jpeg");

    // 检查图片是否成功加载
    if (background.isNull()) {
        qDebug() << "Failed to load background image: :/tu/back.jpg";
        return;
    }

    // 确保图片不模糊
    background = background.scaled(ui->centralwidget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 创建一个白色背景的QPixmap
    QPixmap whiteBackground(background.size());
    whiteBackground.fill(Qt::white);

    // 将背景图片绘制到白色背景上
    QPainter painter(&whiteBackground);
    painter.drawPixmap(0, 0, background);

    // 创建一个带有弧度的矩形路径
    QPainterPath path;
    QRectF rect(0, 0, ui->centralwidget->width(), ui->centralwidget->height());
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
    ui->centralwidget->setPalette(palette);

    // 设置窗口背景透明
    ui->centralwidget->setAutoFillBackground(true);
}

void MainWindow::set_Device()
{
    // 初始化设备状态
        deviceStatus.insert("风扇", "关闭");
        deviceStatus.insert("空调", "关闭");
        deviceStatus.insert("电灯", "关闭");
}
