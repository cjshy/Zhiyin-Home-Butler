#include "form.h"
#include "ui_form.h"
#include"mainwindow.h"
Form::Form(MainWindow *mainWidow,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form),mainWidow(mainWidow)
{
    ui->setupUi(this);
    // 移除边框
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //更新背景
    updataBackground();
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_close_clicked()
{
    this->close();
}

void Form::on_pushButton_min_clicked()
{
    this->showMinimized();
}

void Form::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mouse_pos = event->globalPos();
        window_pos = this->pos();
        diff_pos = mouse_pos - window_pos;
        event->accept();
    }
}

void Form::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        QPoint pos = event->globalPos();
        this->move(pos - diff_pos);
        event->accept();
    }
}

void Form::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updataBackground();
}

void Form::updataBackground()
{
    // 加载背景图片
    QPixmap background(":/chat_lib/bg.jpg");

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

void Form::on_pushButton_clicked()//智能家居
{
    mainWidow->home->show();
    this->hide();
}

void Form::on_pushButton_2_clicked()
{
    mainWidow->select->show();
    this->hide();
}
