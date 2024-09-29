#ifndef SELFINFOWIDGET_H
#define SELFINFOWIDGET_H

#include <QDialog>
#include <QWidget>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
class SelfInfoWidget : public QDialog
{
    Q_OBJECT
public:
    SelfInfoWidget(QWidget*parent);
private:
    QPushButton*avatarBtn;
    QLabel*idTag;
    QLabel*idLabel;

    QLabel*nameTag;
    QLabel*nameLabel;
    QLineEdit*nameEdit;
    QPushButton*nameModifyBtn;
    QPushButton*nameSubmitBtn;

    QLabel*descTag;
    QLabel*descLabel;
    QLineEdit*descEdit;
    QPushButton*descModifyBtn;
    QPushButton*descSubmitBtn;

    QLabel*phoneTag;
    QLabel*phoneLabel;
    QLineEdit*phoneEdit;
    QPushButton*phoneModifyBtn;
    QPushButton*phoneSubmitBtn;

    QLabel*verifyCodeTag;
    QLineEdit*verifyCodeEdit;
    QPushButton*getVerifyCodeBtn;
};

#endif // SELFINFOWIDGET_H
