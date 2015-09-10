/**
* @file profile.cpp
* @brief 数据库操作类实现，该类是单实例实现，静态初始化
* @author qgj
* @date 2015-06-04
* @ingroup configure
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-04    QinGenjian    create
*
*/

#include <QDebug>
#include <QHash>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDataStream>
#include <QByteArray>
#include <QReadWriteLock>
#include <QDir>

#include "configure/machineconfig.h"
//#include "assaycore/nxglobal.h"
//#include "message/messageBox.h"
#include "profile.h"

struct Profile::Data
{
    QString filename;
//    QHash<QString, QVariant> values;
    QMap<QString, QVariant> values;
    int counts;
    QReadWriteLock lock;
};

static const char *profileName = "profile";
static const char *qcName = "qc";
static const char *dataName = "data";

Q_GLOBAL_STATIC(Profile, profileInstance)   /**< 多线程环境下的单实例 */

/**
* @brief 获取实例对象
* @retval 静态对象
*/
Profile &Profile::instance()
{
    return *profileInstance();
}

/**
* @brief 构造函数
* @param 父指针
*/
Profile::Profile(QObject *parent)
    : QObject(parent)
    , m_errProfile(false)
    , m_errData(false)
    , m_errQc(false)
{
    QDir dir(MachineConfig::getDatabasePath());
//    QString path = dir.absoluteFilePath("../db/profile.db");
    dir.cdUp();
    init(dir.absoluteFilePath("db/profile.db"));
}

/**
* @brief 构造函数
* @param [In] 文件路径名
* @param [In] 父指针
*/
Profile::Profile(const QString &filename, QObject *parent)
        : QObject(parent)
{
    init(filename);
}

/**
* @brief 析构函数
*/
Profile::~Profile()
{
    delete d;
}

/**
* @brief 初始化
* @param [In] 文件路径名
*/
void Profile::init(const QString &filename)
{
    d = new Data;
    d->filename = filename;
    d->counts = 0;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", profileName);
    db.setDatabaseName(filename);
    if (!db.open())
    {
//        MessageBox::reportInfor(Info_Database_ConnectErr);
        m_errProfile = false;
    }
    else
    {
        // 加载所有数据
        QSqlQuery q(db);
        q.exec("select name, value, tip, editType from profile");
        while(q.next())
        {
            QStringList tmp;
            tmp << q.value(1).toString() << q.value(2).toString() << q.value(3).toString();
            d->values[q.value(0).toString()] = tmp;
//            qDebug() << q.value(0).toString() << ":" << tmp;
            d->counts++;
        }
        m_errProfile = true;
    }

    QSqlDatabase::addDatabase("QSQLITE", dataName);
    QSqlDatabase::addDatabase("QSQLITE", qcName);
    openDatabase(MachineConfig::getDatabasePath());
}

/**
* @brief 获取配置数据库连接
* @param [In] 打开标志，默认true
* @retval 数据库连接
*/
QSqlDatabase Profile::profileDatabase(bool open) const
{
    return QSqlDatabase::database(profileName, open);
}

/**
* @brief 获取质控数据库连接
* @param [In] 打开标志，默认true
* @retval 数据库连接
*/
QSqlDatabase Profile::qcDatabase(bool open) const
{
    return QSqlDatabase::database(qcName, open);
}

/**
* @brief 获取检测结果数据库连接
* @param [In] 打开标志，默认true
* @retval 数据库连接
*/
QSqlDatabase Profile::dataDatabase(bool open) const
{
    return QSqlDatabase::database(dataName, open);
}

/**
* @brief 打开系统数据库
* @param [In] 数据库文件路径
* @retval 是否成功
*/
bool Profile::openDatabase(const QString &path)
{
    QDir dir(path);
    QSqlDatabase db;

    dir.cdUp();

    db = dataDatabase(false);
    if (db.isOpen())
        db.close();
    QString dataFile = dir.absoluteFilePath("db/data.db");
    db.setDatabaseName(dataFile);
    if (!db.open())
    {
        m_errData = false;
//        MessageBox::reportInfor(Info_Database_ConnectErr);
    }
    else
        m_errData = true;

    db = qcDatabase(false);
    if (db.isOpen())
        db.close();
    QString qcFile = dir.absoluteFilePath("db/qc.db");
    db.setDatabaseName(qcFile);
    if (!db.open())
    {
        m_errQc = false;
//        MessageBox::reportInfor(Info_Database_ConnectErr);
    }
    else
        m_errQc = true;

    return true;
}

/**
* @brief 设置配置文件
* @param [In] 键值
* @param [In] 值
* @param [In] 保存标志
*/
void Profile::setValue(const QString &key, const QVariant &value, bool save)
{
    {
        QWriteLocker locker(&d->lock);
        if (value.isValid())
            d->values[key] = value;
        else
            d->values.remove(key);
    }

    if (save)
    {
        QSqlDatabase db = QSqlDatabase::database(profileName);
        QStringList colValue = value.toStringList();
        QString val = colValue.at(0);
        QString tip = colValue.at(1);
        QString editType = colValue.at(2);

        qDebug() << val << tip << editType;

        QSqlQuery query(db);
        if (value.isValid())
        {
            query.prepare("insert or replace into profile (name, value, tip, editType) values(?,?,?,?)");
            query.addBindValue(key);
            query.addBindValue(val);
            query.addBindValue(tip);
            query.addBindValue(editType);
        }
        else
        {
            query.prepare("delete from profile where name = ?");
            query.addBindValue(key);
        }
        query.exec();
    }
}

/**
* @brief 获取给定键值对应的值
* @param [In] 键值
* @param [In] 缺省值
* @retval 值
*/
QVariant Profile::value(const QString &key, const QVariant &defaultValue)
{
    QReadLocker locker(&d->lock);
    QVariant val = d->values.value(key);
    return val.isValid() ? val : defaultValue;
}

/**
* @brief 获取给定键值对应的值，键值非法时使用缺省键值
* @param [In] 键值
* @param [In] 缺省键值
* @param [In] 缺省值
* @retval 值
*/
QVariant Profile::value2(const QString &key, const QString &defaultKey, const QVariant &defaultValue)
{
    QVariant v = value(key);
    if (v.isValid())
        return v;
    else
        return value(defaultKey, defaultValue);
}

/**
* @brief 移走给定键值对应的键－值对
* @param [In] 键值
*/
void Profile::remove(const QString &key)
{
    {
        QWriteLocker locker(&d->lock);
        d->values.remove(key);
    }

    QSqlDatabase db = QSqlDatabase::database(profileName);
    QSqlQuery query(db);
    query.exec("delete from profile where name = '" + key + "'");
}

/**
* @brief 获取键值列表
* @retval 键值列表
*/
QStringList Profile::keys() const
{
    QWriteLocker locker(&d->lock);
    return d->values.keys();
}

/**
* @brief 获取配置参数个数
* @retval 配置参数个数
*/
int Profile::counts() const
{
    QWriteLocker locker(&d->lock);
    return d->counts;
}
