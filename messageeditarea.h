#ifndef MESSAGEEDITAREA_H
#define MESSAGEEDITAREA_H

#include <QWidget>
#include<QPushButton>
#include<QPlainTextEdit>
class MessageEditArea : public QWidget
{
    Q_OBJECT
public:
    explicit MessageEditArea(QWidget *parent = nullptr);

private:
    QPushButton*sendImageBtn;
    QPushButton*sendFileBtn;
    QPushButton*sendSpeechBtn;
    QPushButton*showHistoryBtn;
    QPlainTextEdit*textEdit;
    QPushButton*sendTextBtn;
signals:
};

#endif // MESSAGEEDITAREA_H
