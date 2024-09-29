#ifndef SESSIONFRIENDAREA_H
#define SESSIONFRIENDAREA_H

#include <QWidget>
#include<QScrollArea>
#include<QLabel>


///////////////////
// 滚动区域中Item的类型
// /////////////////

enum ItemType{
    SessionItemType,
    FriendItemType,
    ApplyItemType
};



///////////////////
////////滚动区域实现
/// /////////////
class SessionFriendArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit SessionFriendArea(QWidget *parent = nullptr);

    //清空区域内Item
    void clear();

    //添加Item,通过itemType类型不同，添加不同id
    void addItem(ItemType itemType,const QString&id,const QIcon&avatar,const QString& name,const QString&text);

    //选中某个指定Item,通过index下标选择
    void clickItem(int index);

private:
    QWidget*container;
    //后续往container内部layout中添加元素，能触发QScrollArea滚动效果

signals:
};



///////////////////
// 滚动区域Item实现
// /////////////////
class SessionFriendItem:public QWidget{
    Q_OBJECT//让支持信号槽的宏

public:
    SessionFriendItem(QWidget* owner,const QIcon& avatar,const QString& name,const QString& text);

    //使手动创建子类背景色符合父类
    void paintEvent(QPaintEvent*event)override;


    //鼠标点击时改变颜色
    void mousePressEvent(QMouseEvent*event)override;

    void enterEvent(QEnterEvent*event)override;
    void leaveEvent(QEvent*event)override;

    void select();

    //函数期望实现Item被点击之后的逻辑
   virtual  void active();

private:
    QWidget*owner;
    //owner指向上述SessionFriendArea
    bool selected=false;
    //用来表示当前Item是否被选中
protected:

    QLabel*messageLabel;


};






///////////////////////
/// 会话Item的实现
/// //////////////////

class SessionItem:public SessionFriendItem{
    Q_OBJECT

public:
    SessionItem(QWidget*owner,const QString&chatSessionId,const QIcon&avatar,
                const QString&name,const QString &lastMessage);

    void active()override;

private:
    //会话ID
    QString chatSessionId;
    // 最后一条消息的文本预览
    QString text;
};



///////////////////////
/// 好友Item的实现
/// //////////////////


class FriendItem:public SessionFriendItem{
    Q_OBJECT


public:
    FriendItem(QWidget*owner,const QString&userId,const QIcon&avatar,
               const QString&name,const QString&description);
    void active()override;
private:
    //好友ID
    QString userId;


};


//////////////////
/// 好友申请Item的实现
/// //////////////

class ApplyItem:public SessionFriendItem{
    Q_OBJECT


public:
    ApplyItem(QWidget*owner,const QString&userId,const QIcon&avatar,
              const QString&name);
    void active()override;
private:
    //申请人Id
    QString userId;
};





#endif // SESSIONFRIENDAREA_H
