#include "chat.h"
#include "ui_chat.h"
#include"mainwindow.h"
chat::chat(MainWindow *mainWidow,QWidget *parent):
    QWidget(parent),
    ui(new Ui::chat),mainWidow(mainWidow)
{
    ui->setupUi(this);
    // 移除边框
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    // 更新背景
    updataBackground();
//    select = new Select(mainWidow);
//    select->hide();
}

chat::~chat()
{
    delete ui;
}

void chat::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mouse_pos = event->globalPos();
        window_pos = this->pos();
        diff_pos = mouse_pos - window_pos;
        event->accept();
    }
}

void chat::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        QPoint pos = event->globalPos();
        this->move(pos - diff_pos);
        event->accept();
    }
}

void chat::on_close_bt_clicked()
{
   close();
}

void chat::on_min_bt_clicked()
{
    showMinimized();
}

void chat::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // 背景更新
    updataBackground();
}

void chat::set_Formation()//设置信息
{
    ui->username->setText(mainWidow->User->name);

}

void chat::send_singal()//发送信号
{
    Qmesg msg;
    msg.flag = 6; // 我的喜欢列表
//    user_text User(mainWidow->curNameid, "NULL");
//    memcpy(msg.data, &Song, sizeof(song_text));
//    mainWidow->clientSocket->write((char *)&msg, sizeof(Qmesg));
}

void chat::updataBackground()
{
    // 加载背景图片
    QPixmap background(":/tu/b1.png");

    // 检查图片是否成功加载
    if (background.isNull()) {
        qDebug() << "Failed to load background image: :/chat_lib/橙色.png";
        return;
    }

    // 确保图片不模糊
    background = background.scaled(ui->widget_2->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 创建一个带有弧度的矩形路径
    QPainterPath path;
    QRectF rect(0, 0, ui->widget_2->width(), ui->widget_2->height());
    qreal radius = 20; // 弧度半径
    path.addRoundedRect(rect, radius, radius);

    // 更新widget_2的背景绘制
    ui->widget_2->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(background));
    ui->widget_2->setPalette(palette);
}

void chat::on_min_bt_2_clicked()
{
    mainWidow->select->show();
    this->hide();
}

