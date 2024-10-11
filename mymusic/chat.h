#ifndef CHAT_H
#define CHAT_H
#include <QWidget>
#include<QDebug>
#include<QMouseEvent>
#include<QPoint>
#include<QFile>
#include<QPixmap>
#include<QPalette>
#include<QPainter>
#include<QTextStream>
#include<QList>
#include"comment.h"
class MainWindow;
namespace Ui {
class chat;
}
class chat : public QWidget
{
    Q_OBJECT

public:
    explicit chat(MainWindow *mainWidow,QWidget *parent = nullptr);
    ~chat();
    //边框拖动
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)  override;
    void updataBackground();
    void resizeEvent(QResizeEvent *event);
    void set_Formation();
    void send_singal();
private slots:
    void on_close_bt_clicked();

    void on_min_bt_clicked();

    void on_min_bt_2_clicked();

private:
    Ui::chat *ui;
    QPoint diff_pos;  // 鼠标和窗口的相对位移
    QPoint window_pos;
    QPoint mouse_pos;
    MainWindow *mainWidow;
};

#endif // CHAT_H
