#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include<QPushButton>
#include <QLineEdit>


#include "sessionfriendarea.h"
#include"messageshowarea.h"
#include"messageeditarea.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

private:
    static MainWidget* instance;
    MainWidget(QWidget *parent=nullptr);

public:
    static MainWidget* getInstance();
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    SessionFriendArea* sessionFriendArea;
    //左中右窗口
    QWidget* windowLeft;
    QWidget* windowMid;
    QWidget* windowRight;

    //左边框
    QPushButton* userAvatar;//用户头像
    QPushButton* sessionTabBtn;//会话标签页按钮
    QPushButton* friendTabBtn;//好友标签页按钮
    QPushButton* applyTabBtn;//好友申请标签页按钮

    //中间框
    QLineEdit* searchEdit; // 用户搜索框
    QPushButton* addFriendBtn; // 添加好友按钮

    //右边框
    MessageShowArea*messageShowArea;//消息展示区
    MessageEditArea*messageEditArea;//消息编辑区


//左边框点击响应转换
    enum ActiveTab{
        SESSION_LIST,
        FRIEND_LIST,
        APPLY_LIST
    };
//默认为会话响应
    ActiveTab activeTab=SESSION_LIST;

    //初始化窗口布局
    void initMainWindow();//主窗口
    void initLeftWindow();//左
    void initMidWindow();//中
    void initRightWindow();//右

    //信号槽初始化
    void initSignalSlot();

    void switchTabToSession();//连接到会话
    void switchTabToFriend();//连接到好友
    void switchTabToApply();//连接到好友申请

    //加载对话列表
    void loadSessionList();
    void loadFriendList();
    void loadApplyList();



};
#endif // MAINWIDGET_H
