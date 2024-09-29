#include "sessionfriendarea.h"


#include<QScrollBar>
#include<QVBoxLayout>
#include<QPushButton>
#include<QLabel>
#include<QStyleOption>
#include<QPainter>
#include"model/data.h"
#include"debug.h"

SessionFriendArea::SessionFriendArea(QWidget *parent)
    : QScrollArea{parent}
{
    //设置这个属性才能开启滚动效果
    this->setWidgetResizable(true);

    //滚动条样式
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width:2px;background-color:rgb(46,46,46);}");//垂直方向滚动条,宽度2像素，背景色
    this->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{height:0px;}");//水平方向滚动条，
    this->setStyleSheet("QWidegt{border:none;}");
    //创建widget
    container=new QWidget();
    container->setFixedWidth(310);//注意和中间框大小相同
    this->setWidget(container);

    //widget布局管理器
    QVBoxLayout* layout=new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0); //设置四边和元素直接距离为0
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);//向上对其
    container->setLayout(layout);

    //测试
    // for(int i=0;i<50;i++){
    //     QPushButton* b=new QPushButton();
    //     b->setText("an");
    //     layout->addWidget(b);
    // }


//构造临时数据进行调试
#if TEST_UI
    QIcon icon(":/resource/image/defaultAvatar.png");
    for(int i=0;i<30;i++){
        this->addItem(SessionItemType,QString::number(i),icon,"小明"+QString::number(i),"终末消息"+QString::number(1));
    }

#endif
}

//清除会话
void SessionFriendArea::clear()
{
    QLayout*layout=container->layout();
    //遍历布局管理器中所有元素，删除

    //因为删除时，数组下标也会修改，从前往后遍历修改i会跳过元素
    //从前往后遍历不需要修改i，始终只删除第0元素
    // for(int i=0;i<layout->count();){
    //     //takeAt移除下标，因为删除之后count总数也修改
    //     layout->takeAt(i);
    // }
    //从后往前遍历
    for(int i=layout->count()-1;i>=0;--i){//不可用size_t，因为size_t无符号整形恒>=0，无法停止循环
        //takeAt移除元素下标
        QLayoutItem* item=layout->takeAt(i);

        //释放内存，将移除出数组的widget内容释放，正常使用的时候，new出来的对象添加到布局管理器
        if(item->widget()){
            delete item->widget();
        }
    }
}


    //添加好友会话
void SessionFriendArea::addItem(ItemType itemType, const QString& id, const QIcon &avatar, const QString &name, const QString &text)
{
    SessionFriendItem* item = nullptr;
    if (itemType == SessionItemType) {
        item = new SessionItem(this, id, avatar, name, text);
    } else if (itemType == FriendItemType) {
        item = new FriendItem(this, id, avatar, name, text);
    } else if (itemType == ApplyItemType) {
        item = new ApplyItem(this, id, avatar, name);
    } else {
        LOG() << "错误的 ItemType! itemType=" << itemType;
        return;
    }
    container->layout()->addWidget(item);
}


void SessionFriendArea::clickItem(int index)
{
    if(index<0||index>=container->layout()->count()){
        LOG()<<"点击元素超出下标！index="<<index;
        return;
    }
    QLayoutItem*layoutItem=container->layout()->itemAt(index);
    if(layoutItem==nullptr||layoutItem->widget()==nullptr){
        LOG()<<"指定元素不存在！index="<<index;
        return;
    }
    SessionFriendItem*item=dynamic_cast<SessionFriendItem*>(layoutItem->widget());
    item->select();
}

SessionFriendItem::SessionFriendItem(QWidget *owner, const QIcon &avatar, const QString &name, const QString &text)
    :owner(owner)
{
    this->setFixedHeight(70);
    this->setStyleSheet("QWidget{background-color:rgb(231,231,231);}");
    //创建网格布局管理器
    QGridLayout*layout=new QGridLayout();
    layout->setContentsMargins(20,0,0,0);
    //layout->setSpacing(0);
    layout->setHorizontalSpacing(10);
    layout->setVerticalSpacing(0);
    this->setLayout(layout);
    //创建头像
    QPushButton*avatarBtn=new QPushButton();
    avatarBtn->setFixedSize(50,50);
    avatarBtn->setIconSize(QSize(50,50));
    avatarBtn->setIcon(avatar);
    avatarBtn->setStyleSheet("QPushutton{border:none;}");//无边框效果
    avatarBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);//Fixed固定尺寸横竖大小不变

    //创建名字
    QLabel* nameLabel=new QLabel();
    nameLabel->setText(name);
    nameLabel->setStyleSheet("QLabel{font-size:18px;font-weight：600；}");
    nameLabel->setFixedHeight(35);
    nameLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);//Expending，横向扩充把空间填满

    //创建消息文本
   messageLabel=new QLabel();
    messageLabel->setText(text);
    messageLabel->setFixedHeight(35);
    messageLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    //头像处于0，0位置，占据2行2列
    layout->addWidget(avatarBtn,0,0,2,2);
    //名字处于0行2列，占据1行1列
    layout->addWidget(nameLabel,0,2,1,8);
    //消息预览处于1行2列，占据1行1列
    layout->addWidget(messageLabel,1,2,1,8);

}

//widget自定义子类背景颜色需要手动确定继承父类背景色
void SessionFriendItem::paintEvent(QPaintEvent *event)
{
    (void)event;
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SessionFriendItem::mousePressEvent(QMouseEvent *event)
{
    //点击时修改背景色
    (void)event;
    select();
}

void SessionFriendItem::enterEvent(QEnterEvent *event)
{
    (void)event;
    if(this->selected){
        return;
    }
    this->setStyleSheet("QWidget{background-color:rgb(215,215,215);}");
}

void SessionFriendItem::leaveEvent(QEvent *event)
{
    (void)event;
    // 当前这个 item 是选中状态, 则背景色不受到该逻辑影响
    if (this->selected) {
        return;
    }
    // 还原背景色
    this->setStyleSheet("QWidget { background-color: rgb(231, 231, 231);}");
}

void SessionFriendItem::select()
{
    //鼠标点击时，获取所有兄弟元素
    const QObjectList children=this->parentWidget()->children();
    for(QObject* child:children){
        if(!child->isWidgetType()){
            //判断是否是widget
            continue;
        }
        //是widget则把child转化为SessionFriendItem
        SessionFriendItem*item=dynamic_cast<SessionFriendItem*>(child);
        if(item->selected){
            item->selected=false;
            item->setStyleSheet("QWidget{background-color:rgb(231,231,231);}");
        }
    }
    //点击时修改背景颜色
    this->setStyleSheet("QWidget{background-color:rgb(21,21,21);}");
    this->selected=true;
    //调用active
    this->active();
}

void SessionFriendItem::active()
{

}













///////////////////////
/// 会话Item的实现
/// //////////////////
SessionItem::SessionItem(QWidget *owner, const QString &chatSessionId,
                         const QIcon &avatar, const QString &name, const QString &lastMessage)
    :SessionFriendItem(owner,avatar,name,lastMessage),chatSessionId(chatSessionId)
{
//点击之后，加载历史消息记录
}

void SessionItem::active()
{

}


///////////////////////
/// 好友Item的实现
/// //////////////////

FriendItem::FriendItem(QWidget *owner, const QString &userId,
                       const QIcon &avatar, const QString &name, const QString &description)
    :SessionFriendItem(owner,avatar,name,description),userId(userId)

{
//点击之后，激活对应的会话列表元素
}

void FriendItem::active()
{

}

//////////////////
/// 好友申请Item的实现
/// //////////////
ApplyItem::ApplyItem(QWidget *owner, const QString &userId,
                     const QIcon &avatar, const QString &name)
    :SessionFriendItem(owner,avatar,name,""),userId(userId)
{
    //移除父类messageLabel
    QGridLayout* layout = dynamic_cast<QGridLayout*>(this->layout());
    layout->removeWidget(messageLabel);
    // 要记得释放内存, 否则会内存泄露.
    delete messageLabel;

    //设置 同意；拒绝 按钮
    QPushButton* acceptBtn=new QPushButton();
    acceptBtn->setText("同意");
    QPushButton* rejectBtn=new QPushButton();
    rejectBtn->setText("拒绝");

    layout->addWidget(acceptBtn,1,2,1,1);
    layout->addWidget(rejectBtn,1,3,1,1);

}

void ApplyItem::active()
{
    //无事发生
}
