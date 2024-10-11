#ifndef SONG_WORD_H
#define SONG_WORD_H

#include <QWidget>
#include<QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include<QMap>
#include<QFile>
#include<QLabel>
#include<QTextStream>
#include"globalplayer.h"
class MainWindow;
namespace Ui {
class song_word;
}

class song_word : public QWidget
{
    Q_OBJECT

public:
    explicit song_word(MainWindow *mainWidow,QWidget *parent = nullptr);
    ~song_word();
    QNetworkAccessManager *manager;
    void parseJson(QByteArray jsonByte);//json字符串转换
    void setupLyricsLabels();
    void updateLabel(QLabel* label, const QString& text);
    void clearLabels();
private slots:
    void replyFinished(QNetworkReply *reply);
    void onPositionChanged(qint64 position);
private:
    Ui::song_word *ui;
    MainWindow *mainwindow;
    QMediaPlaylist * playlist;
    QMediaPlayer * player;
    QMap<int,QString> lrcMap;

};

#endif // SONG_WORD_H
