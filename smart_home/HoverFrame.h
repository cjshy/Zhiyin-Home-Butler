#ifndef HOVERFRAME_H
#define HOVERFRAME_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include<QMovie>
#include <QGraphicsDropShadowEffect>
class HoverFrame : public QWidget {
public:
    HoverFrame(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

        // 创建布局
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(10, 10, 10, 10); // 内边距

        // 添加图片
        imageLabel = new QLabel(this);
        QPixmap pixmap(":/tu/关闭.png");  // 确保替换为你的图片路径
        imageLabel->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio)); // 设置图片大小

        // 添加文本标签
        textLabel = new QLabel("这是悬浮框", this);
        textLabel->setStyleSheet("color: black; font-size: 16px; padding-top: 10px;"); // 文本样式

        layout->addWidget(imageLabel, 0, Qt::AlignCenter); // 图片居中
        layout->addWidget(textLabel, 0, Qt::AlignCenter); // 文本居中

        setLayout(layout);

        // 设置背景和圆角
        setStyleSheet("background-color: rgb(255, 255, 255); border-radius: 10px;"); // 设置半透明背景和圆角

        // 添加阴影效果
        QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
        shadowEffect->setBlurRadius(10);
        shadowEffect->setXOffset(5);
        shadowEffect->setYOffset(5);
        shadowEffect->setColor(Qt::black);
        setGraphicsEffect(shadowEffect);
    }

    void updateContent(const QString &text, const QPixmap &pixmap, QMovie *movie = nullptr) {
        textLabel->setText(text); // 更新文本内容

        if (movie) { // 只检查 movie 是否有效
            if (movie->isValid()) { // 检查 QMovie 是否有效
                imageLabel->clear(); // 清除之前的内容
                imageLabel->setMovie(const_cast<QMovie*>(movie)); // 设置新的 movie
                movie->start(); // 开始播放动画
            }
        } else { // 如果没有提供 movie，更新静态图片
            imageLabel->clear(); // 清除动画的内容
            imageLabel->setPixmap(pixmap); // 更新图片内容
        }
    }


private:
    QLabel *imageLabel;
    QLabel *textLabel;
    QVBoxLayout *layout;
};

#endif // HOVERFRAME_H
