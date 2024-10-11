#include "sqltext.h"

sqltext::sqltext()
{
    //判断数据库是否已连接上
    if(QSqlDatabase::contains("qt_sql_default_connection")==true)
    {
        //已存在则直接返回数据库对象，,qt_sql_default_connection代表当前数据库连接
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        //数据库不存在。则建立数据库连接
        database = QSqlDatabase::addDatabase("QSQLITE");
        //设置数据库名字
        database.setDatabaseName("Mydatabase");
    }
}
//打开数据库
bool sqltext::openDb()
{
    if(!database.open())
    {
        qDebug()<<"Error: connection with database failed";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
    return true;
}
//关闭数据库
void sqltext::closeDb()
{
    database.close();
}
//执行函数
void sqltext::runsql(QString sql)
{
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(sql))
    {
        qDebug()<<sqlQuery.lastError();
    }
    else
    {
        qDebug()<<"run succeed";
    }
}
//查询函数
void sqltext::searchsql(QSqlQuery &sqlQuery, QString sql)
{
    if(!sqlQuery.exec(sql))
    {
        qDebug()<<sqlQuery.lastError();
    }
    else
    {
        qDebug()<<"run succeed";
    }
}

bool sqltext::isTableExists(const QString &tablename)
{
    QSqlQuery query;
    query.exec(QString("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'").arg(tablename));
    return query.next();
}

char *sqltext::getUsernameById(const char *userId)
{
    QSqlQuery query;
    if (query.exec(QString("select name from user where id ='%1'").arg(userId)))
    {
        if (query.next())
        {
            QString username = query.value(0).toString();
            qDebug() << "找到的用户名: " << username;
            return strdup(username.toStdString().c_str()); // 使用 strdup 分配内存并返回 char*
        }
        else
        {
            qDebug() << "未找到匹配的用户ID: " << userId;
            return nullptr; // 如果未找到匹配的username，返回 nullptr
        }
    }
    else
    {
        qDebug() << "SQL 查询执行失败: " << query.lastError().text();
        return nullptr;
    }
}
