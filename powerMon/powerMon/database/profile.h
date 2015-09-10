/**
* @file profile.h
* @brief 数据库操作类定义，该类定义对数据库进行操作的处理
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
#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>
#include <QString>
#include <QVariant>

#define profile (Profile::instance())

class QSqlDatabase;

class Profile : public QObject
{
Q_OBJECT
public:
    explicit Profile(QObject *parent = 0);
    Profile(const QString &filename, QObject *parent = 0);
    ~Profile();

    QSqlDatabase profileDatabase(bool open = true) const;
    QSqlDatabase qcDatabase(bool open = true) const;
    QSqlDatabase dataDatabase(bool open = true) const;

    bool openDatabase(const QString &path);
    bool openQcDatabase(const QString &path);
    bool openDataDatabase(const QString &path);

    void setValue(const QString &key, const QVariant &value , bool save = true);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
    template<class T> T value(const QString &key) { return  value(key).value<T>(); }
    int counts() const;
    QVariant value2(const QString &key, const QString &defaultKey, const QVariant &defaultValue = QVariant());

    void remove(const QString &key);
    QStringList keys() const;

    bool openProfileFlag() const { return m_errProfile; }
    bool openDataFlag() const { return m_errData; }
    bool openQcFlag() const { return m_errQc; }

    static Profile &instance();

signals:

public slots:

private:
    struct Data;
    Data *d;

    bool m_errProfile;
    bool m_errData;
    bool m_errQc;

    void init(const QString &filename);
};

#endif // PROFILE_H
