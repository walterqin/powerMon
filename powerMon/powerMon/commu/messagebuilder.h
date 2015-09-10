/**
* @file messagebuilder.h
* @brief 通讯协议构造类定义
* @author qgj
* @date 2015-06-13
* @ingroup commu
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-13    QinGenjian    create
*
*/

#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H

#include <QDataStream>

class MessageBuilder : public QDataStream
{
public:
    MessageBuilder(int address);
    MessageBuilder(const QString &path);

    bool seek(int pos);
    bool seekToData();
    int discardRoute();
    void reset();
    int pos() const;
    int size() const;
    int left() const;
    bool checkDataSize();
};

#endif // MESSAGEBUILDER_H
