#include "mainwidget.h"
#include "./ui_mainwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "sessionfriendarea.h"
#include"selfinfowidget.h"
#include "debug.h"

MainWidget* MainWidget::instance=nullptr;

MainWidget* MainWidget::getInstance(){
    if(instance==nullptr){
        //不传参数，以桌面为父窗口
        //由于此处为整个程序的主窗口，所以可以
        instance=new MainWidget();
    }
    return instance;
}




MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("我的聊天室");
    this->setWindowIcon(QIcon(":/resource/image/logo.png"));


    //初始化主窗口布局样式
    initMainWindow();
    initLeftWindow();
    initMidWindow();
    initRightWindow();

    //信号槽初始化
    initSignalSlot();

}

MainWidget::~MainWidget()
{
    delete ui;
}

//初始化窗口布局
void MainWidget::initMainWindow()
{
    QHBoxLayout* layout=new QHBoxLayout();
    layout->setSpacing(0);//设置元素之间间距，0为紧挨着
    layout->setContentsMargins(0,0,0,0);//设置四个边距距离
    this->setLayout(layout);

    windowLeft=new QWidget();
    windowMid=new QWidget();
    windowRight=new QWidget();
//设置宽，左中宽度不变，右边最小宽度，可变大
    windowLeft->setFixedWidth(70);
    windowMid->setFixedWidth(310);
    windowRight->setMinimumWidth(600);
//设置颜色
    windowLeft->setStyleSheet("QWidget{background-color:rgb(46,46,46);}");
    windowMid->setStyleSheet("QWidget{background-color:rgb(200,200,200);}");
    windowRight->setStyleSheet("QWidget{background-color:rgb(240,240,240);}");

    //添加设置
    layout->addWidget(windowLeft);
    layout->addWidget(windowMid);
    layout->addWidget(windowRight);

}
//初始化左边框布局
void MainWidget::initLeftWindow()
{
    QVBoxLayout*layout=new QVBoxLayout();

    layout->setSpacing(20);//设置按钮间距
    layout->setContentsMargins(0,50,0,0);//左上右下四边距离

    windowLeft->setLayout(layout);

    userAvatar=new QPushButton();
    userAvatar->setFixedSize(45,45);
    userAvatar->setIconSize(QSize(45,45));
    userAvatar->setIcon(QIcon(":/resource/image/defaultAvatar.png"));
    userAvatar->setStyleSheet("QPushButton{background-color:transparent;}");//使按钮按下效果不明显
    layout->addWidget(userAvatar,1,Qt::AlignTop|Qt::AlignHCenter);//使图标区中，靠上对其


    sessionTabBtn=new QPushButton();
    sessionTabBtn->setFixedSize(45,45);
    sessionTabBtn->setIconSize(QSize(45,45));
    sessionTabBtn->setIcon(QIcon(":/resource/image/session_active.png"));
    sessionTabBtn->setStyleSheet("QPushButton{background-color:transparent;}");
    layout->addWidget(sessionTabBtn,1,Qt::AlignTop|Qt::AlignHCenter);

    friendTabBtn=new QPushButton();
    friendTabBtn->setFixedSize(45,45);
    friendTabBtn->setIconSize(QSize(45,45));
    friendTabBtn->setIcon(QIcon(":/resource/image/friend_inactive.png"));
    friendTabBtn->setStyleSheet("QPushButton{background-color:transparent;}");
    layout->addWidget(friendTabBtn,1,Qt::AlignTop|Qt::AlignHCenter);

    applyTabBtn=new QPushButton();
    applyTabBtn->setFixedSize(45,45);
    applyTabBtn->setIconSize(QSize(45,45));
    applyTabBtn->setIcon(QIcon(":/resource/image/apply_inactive.png"));
    applyTabBtn->setStyleSheet("QPushButton{background-color:transparent;}");
    layout->addWidget(applyTabBtn,1,Qt::AlignTop|Qt::AlignHCenter);

    layout->addStretch(20);//在下开辟空间，把图标挤到上边

}


//初始化中间框布局
void MainWidget::initMidWindow()
{
    QGridLayout* layout = new QGridLayout();
    // 距离上方有 20px 的距离, 另外三个方向都不要边距
    layout->setContentsMargins(0, 20, 0, 0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(10);
    windowMid->setLayout(layout);

    searchEdit = new QLineEdit();
    searchEdit->setFixedHeight(30);
    searchEdit->setPlaceholderText("搜索");
    searchEdit->setStyleSheet("QLineEdit { border-radius: 5px; background-color: rgb(226, 226, 226); padding-left: 5px;}");

    addFriendBtn = new QPushButton();
    addFriendBtn->setFixedSize(30, 30);
    addFriendBtn->setIcon(QIcon(":/resource/image/cross.png"));
    QString style = "QPushButton { border-radius: 5px; background-color: rgb(226, 226, 226); }";
    style += " QPushButton:pressed { background-color: rgb(240, 240, 240); }";
    addFriendBtn->setStyleSheet(style);

    sessionFriendArea = new SessionFriendArea();

    // 为了更灵活的控制边距, 只影响搜索框按钮这一行, 不影响下方列表这一行
    // 创建空白的 widget 填充到布局管理器中.
    QWidget* spacer1 = new QWidget();
    spacer1->setFixedWidth(10);
    QWidget* spacer2 = new QWidget();
    spacer2->setFixedWidth(10);
    QWidget* spacer3 = new QWidget();
    spacer3->setFixedWidth(10);

    layout->addWidget(spacer1, 0, 0);
    layout->addWidget(searchEdit, 0, 1);
    layout->addWidget(spacer2, 0, 2);
    layout->addWidget(addFriendBtn, 0, 3);
    layout->addWidget(spacer3, 0, 4);
    layout->addWidget(sessionFriendArea, 1, 0, 1, 5);

}

//初始化右边窗口
void MainWidget::initRightWindow()
{
    //创建布局管理器
    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setAlignment(Qt::AlignTop);
    windowRight->setLayout(vlayout);

    //创建上方区域
    QWidget* titleWidget = new QWidget();
    titleWidget->setFixedHeight(62);
    titleWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleWidget->setObjectName("titleWidget");
    titleWidget->setStyleSheet("#titleWidget { border-bottom: 1px solid rgb(230, 230, 230); border-left: 1px solid rgb(230, 230, 230); }");
    vlayout->addWidget(titleWidget);

    //给标题栏, 添加标题 label 和 一个按钮
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->setSpacing(0);
    // 使标题的 label 和 按钮距离左右两侧的边界, 有点间距.
    hlayout->setContentsMargins(10, 0, 10, 0);
    titleWidget->setLayout(hlayout);

    QLabel* sessionTitleLabel = new QLabel();
    sessionTitleLabel->setStyleSheet("QLabel { font-size: 22px; border-bottom: 1px solid rgb(230, 230, 230);}");

#if TEST_UI
    // 为了测试界面临时增加的. 实际这里的内容, 应该是使用从服务器获取的数据来设置.
    sessionTitleLabel->setText("这是会话标题");
#endif

    hlayout->addWidget(sessionTitleLabel);

   QPushButton* extraBtn = new QPushButton();
    extraBtn->setFixedSize(30, 30);
    extraBtn->setIconSize(QSize(30, 30));
    extraBtn->setIcon(QIcon(":/resource/image/more.png"));
    extraBtn->setStyleSheet("QPushButton { border:none; background-color: rgb(245, 245, 245); } QPushButton:pressed { background-color: rgb(220, 220, 220); }");
    hlayout->addWidget(extraBtn);

    //添加消息展示区
    messageShowArea=new MessageShowArea();
    vlayout->addWidget(messageShowArea);
    //添加消息编辑区
    messageEditArea=new MessageEditArea();
    vlayout->addWidget(messageEditArea);
}

void MainWidget::initSignalSlot()
{
    /////////////////
    /// 连接信号槽
    /// ///////////

    connect(sessionTabBtn,&QPushButton::clicked,this,&MainWidget::switchTabToSession);
    connect(friendTabBtn,&QPushButton::clicked,this,&MainWidget::switchTabToFriend);
    connect(applyTabBtn,&QPushButton::clicked,this,&MainWidget::switchTabToApply);

    ///////////////////////
//////////点击自己头像//////
/////////////////////
    connect(userAvatar,&QPushButton::clicked,this,[=](){
        SelfInfoWidget*selfInfoWidget=new SelfInfoWidget(this);
        selfInfoWidget->exec();
        //selfInfowidget->show();)
    });

}

void MainWidget::switchTabToSession()
{
    activeTab=SESSION_LIST;
    sessionTabBtn->setIcon(QIcon(":/resource/image/session_active.png"));
    friendTabBtn->setIcon(QIcon(":/resource/image/friend_inactive.png"));
    applyTabBtn->setIcon(QIcon(":/resource/image/apply_inactive.png"));

    this->loadSessionList();
}
void MainWidget::switchTabToFriend()
{
    activeTab=FRIEND_LIST;
    sessionTabBtn->setIcon(QIcon(":/resource/image/session_inactive.png"));
    friendTabBtn->setIcon(QIcon(":/resource/image/friend_active.png"));
    applyTabBtn->setIcon(QIcon(":/resource/image/apply_inactive.png"));

    this->loadFriendList();
}

void MainWidget::switchTabToApply()
{
    activeTab=APPLY_LIST;
    sessionTabBtn->setIcon(QIcon(":/resource/image/session_inactive.png"));
    friendTabBtn->setIcon(QIcon(":/resource/image/friend_inactive.png"));
    applyTabBtn->setIcon(QIcon(":/resource/image/apply_active.png"));

    this->loadApplyList();
}


////加载会话列表
void MainWidget::loadSessionList()
{

}

////加载好友列表
void MainWidget::loadFriendList()
{

}

////加载申请列表
void MainWidget::loadApplyList()
{

}
