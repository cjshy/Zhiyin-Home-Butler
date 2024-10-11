#include "song_word.h"
#include "ui_song_word.h"
#include"mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
song_word::song_word(MainWindow *mainwindow,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::song_word),mainwindow(mainwindow)
{
    ui->setupUi(this);
    //设置palyer
    playlist = GlobalPlayer::instance()->playlist();
    player = GlobalPlayer::instance()->player();
    player->setPlaylist(playlist);
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
    //设置QNetworkAccessManger
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

setupLyricsLabels();
}

song_word::~song_word()
{
    delete ui;
}

 void  song_word::replyFinished(QNetworkReply *reply)
{
     qDebug()<<"歌词网页响应已完成"<<endl;
     QVariant ret = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
     qDebug()<<"网页返回状态为"<<ret<<endl;
     if(ret == 200)//200说明页面访问成功
     {
         QByteArray data = reply->readAll();//读取所有的网络数据
         parseJson(data);
     }
 }

 void song_word::onPositionChanged(qint64 position)//歌词同步
 {
     // 判断歌词容器是否为空
     if (lrcMap.isEmpty()) {
         // 清空所有标签
         clearLabels();
         ui->label_4->setText("暂无当前歌曲歌词");
         return;
     }
         int pos = position/10;
         QMap<int, QString>::iterator iter = lrcMap.begin();
             while (iter != lrcMap.end())
             {
                 if(pos-50<=iter.key()&& pos+50>=iter.key())
                 {
                         int j=0;
                         if(iter != lrcMap.begin())
                         {
                             iter--;
                             ui->label_1->setText(iter.value());
                             j++;
                         }
                         if(iter != lrcMap.begin())
                         {
                             iter--;
                             ui->label_2->setText(iter.value());
                             j++;
                         }

                         if(iter != lrcMap.begin())
                         {
                             iter--;
                             ui->label_3->setText(iter.value());
                             j++;
                         }
                         for(;j>0;j--)
                         {
                             iter++;
                         }
                    //中间
                    ui->label_4->setText(iter.value());
                    iter++;
                    if(iter != lrcMap.end())
                    {
                        ui->label_5->setText(iter.value());
                    }
                    else
                    {
                        ui->label_5->setText("");
                        return;
                    }
                    iter++;
                    if(iter != lrcMap.end())
                    {
                        ui->label_6->setText(iter.value());
                    }
                    else
                    {
                        ui->label_6->setText("");
                        return;
                    }
                    iter++;
                    if(iter != lrcMap.end())
                    {
                        ui->label_7->setText(iter.value());
                    }
                    else
                    {
                        ui->label_7->setText("");
                        return;
                    }
                    iter++;
                    if(iter != lrcMap.end())
                    {
                        ui->label_8->setText(iter.value());
                    }
                    else
                    {
                        ui->label_8->setText("");
                        return;
                    }
                    iter++;
                    if(iter != lrcMap.end())
                    {
                        ui->label_9->setText(iter.value());
                    }
                    else
                    {
                        ui->label_9->setText("");
                        return;
                    }
                    iter++;
                    if(iter != lrcMap.end())
                    {
                        ui->label_10->setText(iter.value());
                    }
                    else
                    {
                        ui->label_10->setText("");
                        return;
                    }
                 }
                 iter++;
             }

 }

 void song_word::parseJson(QByteArray jsonByte)
 {
     lrcMap.clear();//清空歌词容器
   QJsonParseError json_error;
   QJsonDocument parse_doucment = QJsonDocument::fromJson(jsonByte,&json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject rootobj = parse_doucment.object();
            if(rootobj.contains("lrc"))
            {
                QJsonValue valuedata = rootobj.value("lrc");
                if(valuedata.isObject())
                {
                    QJsonObject valuedataObject = valuedata.toObject();
                    if(valuedataObject.contains("lyric"))
                    {
                        QJsonValue play_url_value = valuedataObject.value("lyric");
                        if(play_url_value.isString())
                        {
                            QString play_lrcStr = play_url_value.toString();
                            if(play_lrcStr !="")
                            {
                                QString s = play_lrcStr;
                                QStringList s1 = s.split("\n");
                                for(int i=0;i<s1.size()-1;i++)
                                {
                                    QString ss1 = s1.at(i);
                                    QRegExp ipRegExp = QRegExp("\\[\\d\\d\\S\\d\\d\\S\\d\\d\\]");
                                    bool match = ipRegExp.indexIn(ss1);
                                    if(match == false)
                                    {
                                        int s_1 = ss1.mid(1,2).toInt(); //分
                                        int s_2 = ss1.mid(4,2).toInt(); //秒
                                        int s_3 = ss1.mid(7,2).toInt(); //毫秒
                                        int s_count = (s_1*60+s_2)*100+s_3;
                                        int lrctime = s_count;
                                        QString lrcstr = ss1.mid(10);
                                        //用qmap来保存
                                        lrcMap.insert(lrctime,lrcstr);
                                        qDebug()<<"时间："<<lrctime<<"歌词："<<lrcstr;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {

            }
        }
    }
 }

 void song_word::setupLyricsLabels()//设置歌词样式
 {
     // 设置通用样式
     QFont font("Microsoft YaHei", 14); // 字体设置
     // 示例歌词标签的样式
     QLabel *labels[] = {
         ui->label_1,
         ui->label_2,
         ui->label_3,
         ui->label_4, // 当前歌词
         ui->label_5,
         ui->label_6,
         ui->label_7,
         ui->label_8,
         ui->label_9,
         ui->label_10
     };

     for (QLabel *label : labels) {
         label->setFont(font); // 设置字体
         label->setAlignment(Qt::AlignCenter); // 设置文本居中
     }

     // 设置当前歌词的特殊样式
     ui->label_4->setFont(QFont("Microsoft YaHei", 22, QFont::Bold)); // 设置更大的字体并加粗
     ui->label_4->setStyleSheet("color: #ffcc00; border: none; background-color: rgba(40, 40, 40, 0);"); // 设置当前歌词的颜色
 }

 void song_word::clearLabels() {
     ui->label_1->setText("");
     ui->label_2->setText("");
     ui->label_3->setText("");
     ui->label_4->setText("");
     ui->label_5->setText("");
     ui->label_6->setText("");
     ui->label_7->setText("");
     ui->label_8->setText("");
     ui->label_9->setText("");
     ui->label_10->setText("");
 }

