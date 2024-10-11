#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include<QMouseEvent>
#include<QPoint>
#include<QPixmap>
#include<QMap>
#include<QPalette>
#include<QPainter>
class MainWindow;
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(MainWindow *mainWidow,QWidget *parent = nullptr);
    ~Form();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void updataBackground();
private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_min_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Form *ui;
    QPoint diff_pos;  // 鼠标和窗口的相对位移
    QPoint window_pos;
    QPoint mouse_pos;
    MainWindow *mainWidow;
};

#endif // FORM_H
