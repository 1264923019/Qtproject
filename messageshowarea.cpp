#include "messageshowarea.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFontMetrics>
#include <QPainter>
#include <QPainterPath>
#include <QFileDialog>
#include <QTimer>
#include <QMenu>

#include "mainwidget.h"
#include "debug.h"


using namespace model;

////////////////////////////////////////////////////////
/// 表示消息展示区
////////////////////////////////////////////////////////

MessageShowArea::MessageShowArea() {

    // 1. 初始化基本属性
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setWidgetResizable(true);
    // 设置滚动条的样式
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 2px; background-color: rgb(240, 240, 240); }");
    this->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 0;}");
    this->setStyleSheet("QScrollArea { border: none; }");

    // 2. 创建 Container 这样的 widget, 作为包含内部元素的容器
    container = new QWidget();
    this->setWidget(container);

    // 3. 给 container 添加布局管理器
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    container->setLayout(layout);

// 添加 "构造测试数据" 逻辑.
#if TEST_UI

    for (int i = 1; i <= 30; ++i) {
        UserInfo userInfo;
        userInfo.userId = QString::number(1000 + i);
        userInfo.nickname = "张三" + QString::number(i);
        userInfo.description = "从今天开始认真敲代码";
        userInfo.avatar = QIcon(":/resource/image/defaultAvatar.png");
        Message message = Message::makeMessage(TEXT_TYPE, "", userInfo, (QString("这是一条测试消息") + QString::number(i)).toUtf8(), "");
        this->addMessage(true, message);
    }
    UserInfo userInfo;
    userInfo.userId = QString::number(1000);
    userInfo.nickname = "张三";
    userInfo.description = "从今天开始认真敲代码";
    userInfo.avatar = QIcon(":/resource/image/defaultAvatar.png");
    Message message = Message::makeMessage(TEXT_TYPE, "", userInfo, QString("这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息这是一条测试消息").toUtf8(), "");
    this->addMessage(false, message);
#endif




}

void MessageShowArea::addMessage(bool isLeft, const Message &message)
{
    // 构造 MessageItem, 添加到布局管理器中.
    MessageItem* messageItem = MessageItem::makeMessageItem(isLeft, message);
    container->layout()->addWidget(messageItem);
}

void MessageShowArea::addFrontMessage(bool isLeft, const Message &message)
{
    MessageItem* messageItem = MessageItem::makeMessageItem(isLeft, message);
    QVBoxLayout* layout = dynamic_cast<QVBoxLayout*>(container->layout());
    layout->insertWidget(0, messageItem);
}

void MessageShowArea::clear()
{
    // 遍历布局管理器, 删除里面的元素
    QLayout* layout = container->layout();
    for (int i = layout->count() - 1; i >= 0; --i) {
        QLayoutItem* item = layout->takeAt(i);
        if (item != nullptr && item->widget() != nullptr) {
            delete item->widget();
        }
    }
}

////////////////////////////////////////////////////////
/// 表示一个消息元素
////////////////////////////////////////////////////////

MessageItem::MessageItem(bool isLeft) : isLeft(isLeft)
{

}

MessageItem *MessageItem::makeMessageItem(bool isLeft, const Message &message)
{
    // 1. 创建对象和布局管理器
    MessageItem* messageItem = new MessageItem(isLeft);
    QGridLayout* layout = new QGridLayout();
    layout->setContentsMargins(30, 10, 40, 0);
    layout->setSpacing(10);
    // 这个 MessageItem 最低不能低于 100
    messageItem->setMinimumHeight(100);
    messageItem->setLayout(layout);

    // 2. 创建头像
    QPushButton* avatarBtn = new QPushButton();
    avatarBtn->setFixedSize(40, 40);
    avatarBtn->setIconSize(QSize(40, 40));
    avatarBtn->setIcon(message.sender.avatar);
    avatarBtn->setStyleSheet("QPushButton { border: none;}");
    if (isLeft) {
        layout->addWidget(avatarBtn, 0, 0, 2, 1, Qt::AlignTop | Qt::AlignLeft);
    } else {
        layout->addWidget(avatarBtn, 0, 1, 2, 1, Qt::AlignTop | Qt::AlignLeft);
    }

    // 3. 创建名字和时间
    QLabel* nameLabel = new QLabel();
    nameLabel->setText(message.sender.nickname + " | " + message.time);
    nameLabel->setAlignment(Qt::AlignBottom);
    nameLabel->setStyleSheet("QLabel { font-size: 12px; color: rgb(178, 178, 178); }");
    if (isLeft) {
        layout->addWidget(nameLabel, 0, 1);
    } else {
        layout->addWidget(nameLabel, 0, 0, Qt::AlignRight);
    }

    // 4. 创建消息体
    QWidget* contentWidget = nullptr;
    switch (message.messageType) {
    case TEXT_TYPE:
        contentWidget = makeTextMessageItem(isLeft, message.content);
        break;
    case IMAGE_TYPE:
        contentWidget = makeImageMessageItem(isLeft, message.fileId, message.content);
        break;
    case FILE_TYPE:
        contentWidget = makeFileMessageItem(isLeft, message);
        break;
    case SPEECH_TYPE:
        contentWidget = makeSpeechMessageItem(isLeft, message);
        break;
    default:
        LOG() << "错误的消息类型! messageType=" << message.messageType;
    }
    if (isLeft) {
        layout->addWidget(contentWidget, 1, 1);
    } else {
        layout->addWidget(contentWidget, 1, 0);
    }

    // // 5. 连接信号槽, 处理用户点击头像的操作
    // connect(avatarBtn, &QPushButton::clicked, messageItem, [=]() {
    //     MainWidget* mainWidget = MainWidget::getInstance();
    //     UserInfoWidget* userInfoWidget = new UserInfoWidget(message.sender, mainWidget);
    //     userInfoWidget->exec();
    // });

    // // 6. 当用户修改了昵称的时候, 同步修改此处的用户昵称.
    // if (!isLeft) {
    //     // 只是针对右侧消息(自己的消息) 做下列操作.
    //     DataCenter* dataCenter = DataCenter::getInstance();
    //     connect(dataCenter, &DataCenter::changeNicknameDone, messageItem, [=]() {
    //         nameLabel->setText(dataCenter->getMyself()->nickname + " | " + message.time);
    //     });

    //     connect(dataCenter, &DataCenter::changeAvatarDone, messageItem, [=]() {
    //         UserInfo* myself = dataCenter->getMyself();
    //         avatarBtn->setIcon(myself->avatar);
    //     });
    // }

    return messageItem;
}



QWidget *MessageItem::makeTextMessageItem(bool isLeft, const QString& text)
{
    MessageContentLabel* messageContentLabel = new MessageContentLabel(text, isLeft);
    return messageContentLabel;
}

QWidget *MessageItem::makeImageMessageItem(bool isLeft, const QString& fileId, const QByteArray& content)
{

    return nullptr;
}

QWidget *MessageItem::makeFileMessageItem(bool isLeft, const Message& message)
{

    return nullptr;
}

QWidget *MessageItem::makeSpeechMessageItem(bool isLeft, const Message& message)
{

    return nullptr;
}





////////////////////////////////////////////////////////
/// 创建类表示 "文本消息" 正文部分
/// 也能表示文件消息
////////////////////////////////////////////////////////
MessageContentLabel::MessageContentLabel(const QString &text, bool isLeft)
    : isLeft(isLeft)
{
    // 设置一下 SizePolicy
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(16);

    this->label = new QLabel(this);
    this->label->setText(text);
    this->label->setFont(font);
    this->label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    this->label->setWordWrap(true);    // 设置文本自动换行
    this->label->setStyleSheet("QLabel { padding: 0 10px; line-height: 1.2; background-color: transparent; }");


}

// 这个函数会该控件被显示的时候自动调用到.
void MessageContentLabel::paintEvent(QPaintEvent *event)
{
    (void) event;

    // 1. 获取到父元素的宽度
    QObject* object = this->parent();
    if (!object->isWidgetType()) {
        // 当前这个对象的父元素不是预期的 QWidget, 此时不需要进行任何后续的绘制操作.
        return;
    }
    QWidget* parent = dynamic_cast<QWidget*>(object);
    int width = parent->width() * 0.6;

    // 2. 计算当前文本, 如果是一行放置, 需要多宽.
    QFontMetrics metrics(this->label->font());
    int totalWidth = metrics.horizontalAdvance(this->label->text());

    // 3. 计算出此处的行数是多少 (40 表示左右各有 20px 的边距)
    int rows = (totalWidth / (width - 40)) + 1;
    if (rows == 1) {
        // 如果此时得到的行数就只有一行
        width = totalWidth + 40;
    }

    // 4. 根据行数, 计算得到高度. (20 表示上下各有 10px 的边距)
    int height = rows * (this->label->font().pixelSize() * 1.2 ) + 20;

    // 5. 绘制圆角矩形和箭头
    QPainter painter(this);
    QPainterPath path;
    // 设置 "抗锯齿"
    painter.setRenderHint(QPainter::Antialiasing);
    if (isLeft) {
        painter.setPen(QPen(QColor(255, 255, 255)));
        painter.setBrush(QColor(255, 255, 255));

        // 绘制圆角矩形
        painter.drawRoundedRect(10, 0, width, height, 10, 10);
        // 绘制箭头
        path.moveTo(10, 15);
        path.lineTo(0, 20);
        path.lineTo(10, 25);
        path.closeSubpath();   // 绘制的线形成闭合的多边形, 才能进行使用 Brush 填充颜色.
        painter.drawPath(path);  // 不要忘记真正的绘制操作

        this->label->setGeometry(10, 0, width, height);
    } else {
        painter.setPen(QPen(QColor(137, 217, 97)));
        painter.setBrush(QColor(137, 217, 97));

        // 圆角矩形左侧边的横坐标位置
        int leftPos = this->width() - width - 10; // 10 是用来容纳 箭头 的宽度
        // 圆角矩形右侧边的横坐标位置
        int rightPos = this->width() - 10;
        // 绘制圆角矩形
        painter.drawRoundedRect(leftPos, 0, width, height, 10, 10);
        // 绘制箭头
        path.moveTo(rightPos, 15);
        path.lineTo(rightPos + 10, 20);
        path.lineTo(rightPos, 25);
        path.closeSubpath();
        painter.drawPath(path);

        this->label->setGeometry(leftPos, 0, width, height);
    }

    // 6. 重新设置父元素的高度, 确保父元素足够高, 能够容纳下上述绘制的消息显示的区域
    //    注意高度要涵盖之前名字和时间的 label 的高度, 以及留点冗余空间.
    parent->setFixedHeight(height + 50);
}


