/********************************************************************************
** Form generated from reading UI file 'QvkMailDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QVKMAILDIALOG_H
#define UI_QVKMAILDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MailDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QRadioButton *mailRadioButton;
    QRadioButton *mailSelectedRadioButton;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *sendPushButton;
    QPushButton *cancelPushButton;

    void setupUi(QDialog *MailDialog)
    {
        if (MailDialog->objectName().isEmpty())
            MailDialog->setObjectName(QString::fromUtf8("MailDialog"));
        MailDialog->resize(410, 200);
        MailDialog->setWindowTitle(QString::fromUtf8("Dialog"));
        verticalLayout = new QVBoxLayout(MailDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(MailDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 0));
        label->setMaximumSize(QSize(150, 150));
        label->setText(QString::fromUtf8(""));
        label->setPixmap(QPixmap(QString::fromUtf8(":/pictures/sem_soc_net.png")));
        label->setScaledContents(true);

        horizontalLayout->addWidget(label);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(20, -1, 20, -1);
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        mailRadioButton = new QRadioButton(MailDialog);
        mailRadioButton->setObjectName(QString::fromUtf8("mailRadioButton"));
        mailRadioButton->setChecked(true);

        verticalLayout_2->addWidget(mailRadioButton);

        mailSelectedRadioButton = new QRadioButton(MailDialog);
        mailSelectedRadioButton->setObjectName(QString::fromUtf8("mailSelectedRadioButton"));

        verticalLayout_2->addWidget(mailSelectedRadioButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        sendPushButton = new QPushButton(MailDialog);
        sendPushButton->setObjectName(QString::fromUtf8("sendPushButton"));

        horizontalLayout_2->addWidget(sendPushButton);

        cancelPushButton = new QPushButton(MailDialog);
        cancelPushButton->setObjectName(QString::fromUtf8("cancelPushButton"));

        horizontalLayout_2->addWidget(cancelPushButton);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(MailDialog);

        QMetaObject::connectSlotsByName(MailDialog);
    } // setupUi

    void retranslateUi(QDialog *MailDialog)
    {
        mailRadioButton->setText(QApplication::translate("MailDialog", "Mail last Video", 0, QApplication::UnicodeUTF8));
        mailSelectedRadioButton->setText(QApplication::translate("MailDialog", "Mail one or more selected Video", 0, QApplication::UnicodeUTF8));
        sendPushButton->setText(QApplication::translate("MailDialog", "Send", 0, QApplication::UnicodeUTF8));
        cancelPushButton->setText(QApplication::translate("MailDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MailDialog);
    } // retranslateUi

};

namespace Ui {
    class MailDialog: public Ui_MailDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QVKMAILDIALOG_H
