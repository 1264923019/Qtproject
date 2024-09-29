#include "messageeditarea.h"

#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QScrollBar>
MessageEditArea::MessageEditArea(QWidget *parent)
    : QWidget{parent}
{
    //设置窗口
    this->setFixedHeight(200);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //创建垂直布局管理器
    QVBoxLayout*vlayout=new QVBoxLayout();
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0,0,0,0);
    this->setLayout(vlayout);
    //创建水平布局管理器
    QHBoxLayout*hlayout=new QHBoxLayout();
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(10,0,0,0);
    hlayout->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    vlayout->addLayout(hlayout);

    //把四个按钮，创建并水平添加到布局中
    QString btnStyle="QPushButton{background-color: rgb(245, 245, 245); } QPushButton:pressed { background-color: rgb(220, 220, 220); }";
    QSize btnSize(35,35);
    QSize iconSize(25,25);

    sendImageBtn=new QPushButton();
    sendImageBtn->setFixedSize(btnSize);
    sendImageBtn->setIconSize(iconSize);
    sendImageBtn->setIcon(QIcon(":/resource/image/image.png"));
    sendImageBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendImageBtn);

    sendFileBtn=new QPushButton();
    sendFileBtn->setFixedSize(btnSize);
    sendFileBtn->setIconSize(iconSize);
    sendFileBtn->setIcon(QIcon(":/resource/image/file.png"));
    sendFileBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendFileBtn);

    sendSpeechBtn=new QPushButton();
    sendSpeechBtn->setFixedSize(btnSize);
    sendSpeechBtn->setIconSize(iconSize);
    sendSpeechBtn->setIcon(QIcon(":/resource/image/sound.png"));
    sendSpeechBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(sendSpeechBtn);

    showHistoryBtn=new QPushButton();
    showHistoryBtn->setFixedSize(btnSize);
    showHistoryBtn->setIconSize(iconSize);
    showHistoryBtn->setIcon(QIcon(":/resource/image/history.png"));
    showHistoryBtn->setStyleSheet(btnStyle);
    hlayout->addWidget(showHistoryBtn);


    //添加文本编辑行
    textEdit=new QPlainTextEdit();
    textEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    textEdit->setStyleSheet("QPlanTextEdit{border:none;blackground-color:transparent;font-size:14px;padding:10px");
    textEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width:2px;background-color:rgb(45,45,45);}");
    vlayout->addWidget(textEdit);


     // 添加文本发送按钮
       sendTextBtn=new QPushButton();
       sendTextBtn->setText("发送");
       sendTextBtn->setFixedSize(120,40);
       QString style="QPushButton{font-size:16px;color:rgb(7,173,97);border:none;background-color:rgb(233,233,233)";
       style+="QPushButton:hover{background-color:rgb(210,210,210);}";
       style+="QPushButton:pressed{background-color:rgb(100,100,100);}";
       sendTextBtn->setStyleSheet(style);
       vlayout->addWidget(sendTextBtn,0,Qt::AlignRight|Qt::AlignVCenter);

}
