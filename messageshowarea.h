#ifndef MESSAGESHOWAREA_H
#define MESSAGESHOWAREA_H

#include <QScrollArea>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "model/data.h"

// .h 文件中不宜进行 using namespace xxxx
using model::Message;

////////////////////////////////////////////////////////
/// 表示消息展示区
////////////////////////////////////////////////////////

class MessageShowArea : public QScrollArea
{
    Q_OBJECT
public:
    MessageShowArea();

    // 尾插
    void addMessage(bool isLeft, const Message& message);
    // 头插
    void addFrontMessage(bool isLeft, const Message& message);
    // 清空消息
    void clear();


private:
    QWidget* container;
};



////////////////////////////////////////////////////////
/// 表示一个消息元素
/// 这个里面要能同时支持 文本消息, 图片消息, 文件消息, 语音消息.
/// 当前先只考虑文本消息. 另外几个后续慢慢添加.
////////////////////////////////////////////////////////

class MessageItem : public QWidget {
    Q_OBJECT


public:
    // 此处的 isLeft 表示这个 Item 是否是一个 "左侧消息"
    MessageItem(bool isLeft);

    // 通过 工厂方法 创建 MessageItem 实例
    static MessageItem* makeMessageItem(bool isLeft, const Message& message);

    // 添加工厂函数
    static QWidget* makeTextMessageItem(bool isLeft, const QString& text);
    static QWidget* makeImageMessageItem(bool isLeft, const QString& fileId, const QByteArray& content);
    static QWidget* makeFileMessageItem(bool isLeft, const Message& message);
    static QWidget* makeSpeechMessageItem(bool isLeft, const Message& message);

private:
    bool isLeft;
};


////////////////////////////////////////////////////////
/// 创建类表示 "文本消息" 正文部分
/// 也让这个类表示 "文件消息"
////////////////////////////////////////////////////////

class MessageContentLabel : public QWidget {
    Q_OBJECT
public:
    // fileName 可以作为 text 的一部分, 传递进来. 不需要单独列一个参数
    MessageContentLabel(const QString& text, bool isLeft);

    void paintEvent(QPaintEvent* event) override;

private:
    QLabel* label;
    bool isLeft;

};








#endif // MESSAGESHOWAREA_H
