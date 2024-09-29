#pragma once
#include<QString>
#include<QIcon>
#include<QUuid>
#include<QDateTime>
#include<QDebug>
#include<QFile>
#include<QFileInfo>
namespace model{

////////////////
/////工具函数/////
/// ////////////
///

//通过File找到文件代码地址，从而获取文件名
static inline QString getFileName(const QString&path){
    QFileInfo fileInfo(path);
    return fileInfo.fileName();
}

//封装一个“宏”打印日志
//#define TAG "[" <<__LINE__ << "]"  //__LINE__为表示代码行号的宏
                                     //__FILE__为表示代码位置的宏
//#define TAG QString("[%1：%2]").arg(QString(__FILE__),QString::number(__LINE__))
//格式化字符串【%1:%2】，用宏替换常数位置，file输出代码位置，line输出代码行号


#define TAG QString("[%1：%2]").arg(model::getFileName(__FILE__),QString::number(__LINE__))//getFileName为model命名空间，需要声明
#define LOG() qDebug().noquote()<<TAG//通过LOG调用TAG声明在什么文件的多少行
//通过.noquote取消QDebug声明时默认附加的 “”









static inline QString formatTime(int64_t timestamp){
    QDateTime dateTime=QDateTime::fromSecsSinceEpoch(timestamp);
    return dateTime.toString("MM-dd HH:mm:ss");//格式化时间
}

static inline int64_t getTime(){
    return QDateTime::currentSecsSinceEpoch();//得到秒级时间
}

//根据QByteArray转化为QIcon
static inline QIcon makeIcon(const QByteArray& byteArray){
    QPixmap pixmap;
    pixmap.loadFromData(byteArray);
    QIcon icon(pixmap);
    return icon;
}

//读写文件操作
//从指定文件,读取所有二进制内容转为一个QByteArray
static inline QByteArray loadFileToByteArray(const QString& path){
    QFile file(path);
    bool ok=file.open(QFile::ReadOnly);
    if(!ok){
        qDebug()<<"文件打开失败";
        return QByteArray();
    }
    QByteArray content=file.readAll();
    file.close();
    return content;
}
//
static inline void writeQByteArrayToFile(const QString& path,const QByteArray&content){
    QFile file(path);
    bool ok=file.open(QFile::ReadOnly);
    if(!ok){
        qDebug()<<"文件打开失败";
        return ;
    }
    file.write(content);
    file.flush();//刷新缓冲区
    file.close();
}





//////////////////////
//////////用户信息//////
///////////////////////
class UserInfo{
public:
    QString userId;     //用户id
    QString nickname;   //用户昵称
    QString description;//用户签名
    QString Phone;      //手机号码
    QIcon avatar;       //用户头像
};




////////////////////////
/////////消息信息////////
/// ////////////////////
enum MessageType{
    TEXT_TYPE,    //文本消息
    IMAGE_TYPE,   //图片消息
    FILE_TYPE,    //文件消息
    SPEECH_TYPE   //语言消息
};

class Message{
public:
    QString messageId;        //消息编号
    QString chatSessionId;     //消息会话编号
    QString time;             //消息时间
    MessageType messageType;  //消息类型
    UserInfo sender;          //发送者信息
    QByteArray content;       //消息正文
    QString fileId;           //文件身份标识
    QString fileName;         //文件名称


    //extraInfo在消息为文件时，作为文件名补充
    static Message makeMessage(MessageType messageType,const QString& chatSessionId,
                               const UserInfo&sender,const QByteArray& content,const QString& extraInfo){
        if(messageType==TEXT_TYPE){
            return makeTextMessage(chatSessionId,sender,content);
        }else if(messageType==IMAGE_TYPE){
            return makeImageMessage(chatSessionId,sender,content);
        }else if(messageType==FILE_TYPE){
             return makeFileMessage(chatSessionId,sender,content,extraInfo);
        }else if(messageType==SPEECH_TYPE){
             return makeSpeechMessage(chatSessionId,sender,content);
        }else{
            return Message();//触发未知消息类型
        }

    }


private:
    //生成唯一messageId方法
    static QString makeId(){
        return "M"+QUuid::createUuid().toString().sliced(25,12);
    }




    static Message makeTextMessage(const QString& chatSessionId,const UserInfo& sender,const QByteArray&content){
        Message message;
        //生成唯一性id
        message.messageId=makeId();
        //生成格式化时间
        message.time=formatTime(getTime());

        message.chatSessionId=chatSessionId;
        message.sender=sender;
        message.content=content;
        message.messageType=TEXT_TYPE;

        message.fileId="";
        message.fileName="";
        return message;
}

    static Message makeImageMessage(const QString& chatSessionId,const UserInfo&sender,const QByteArray&content){
        Message message;
        //生成唯一性id
        message.messageId=makeId();
        //生成格式化时间
        message.time=formatTime(getTime());

        message.chatSessionId=chatSessionId;
        message.sender=sender;
        message.content=content;
        message.messageType=IMAGE_TYPE;

        message.fileId="";
        message.fileName="";
        return message;
}

    static Message makeFileMessage(const QString& chatSessionId,const UserInfo&sender,const QByteArray&content,const QString&fileName){
        Message message;
        //生成唯一性id
        message.messageId=makeId();
        //生成格式化时间
        message.time=formatTime(getTime());

        message.chatSessionId=chatSessionId;
        message.sender=sender;
        message.content=content;
        message.messageType=FILE_TYPE;
        message.fileName="";

        message.fileId="";
        message.fileName=fileName;
        return message;
}

    static Message makeSpeechMessage(const QString& chatSessionId,const UserInfo&sender,const QByteArray&content){
        Message message;
        //生成唯一性id
        message.messageId=makeId();
        //生成格式化时间
        message.time=formatTime(getTime());

        message.chatSessionId=chatSessionId;
        message.sender=sender;
        message.content=content;
        message.messageType=SPEECH_TYPE;

        message.fileId="";
        message.fileName="";
        return message;
}

};


/////////////////////
//////////会话信息/////
/// ////////////////////
class ChatSessionInfo{
public:
    QString chatSessionId;       //会话编号
    QString chatSessionName;     //会话名称，对方名称或群名称
    Message lastMessage;         //最后的信息
    QIcon avater;                //会话头像，对方头像或群头像
    QString userId;              //会话对方编号，对方id或群id
};




}//end model
