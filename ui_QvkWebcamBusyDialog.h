/********************************************************************************
** Form generated from reading UI file 'QvkWebcamBusyDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QVKWEBCAMBUSYDIALOG_H
#define UI_QVKWEBCAMBUSYDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WebcamBusyDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *label_Titel_Busy;
    QHBoxLayout *horizontalLayout_Picture_and_Info;
    QFrame *frame_Picture;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_Picture;
    QLabel *label_Picture;
    QFormLayout *formLayout_Info;
    QLabel *label_Name;
    QLabel *label_Name_Value;
    QLabel *label_Device;
    QLabel *label_Device_Value;
    QLabel *label_UsedBy;
    QLabel *label_UsedBy_Value;
    QHBoxLayout *horizontalLayout_ButtonBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *WebcamBusyDialog)
    {
        if (WebcamBusyDialog->objectName().isEmpty())
            WebcamBusyDialog->setObjectName(QString::fromUtf8("WebcamBusyDialog"));
        WebcamBusyDialog->resize(451, 170);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(WebcamBusyDialog->sizePolicy().hasHeightForWidth());
        WebcamBusyDialog->setSizePolicy(sizePolicy);
        WebcamBusyDialog->setWindowTitle(QString::fromUtf8("vokoscreen webcam"));
        WebcamBusyDialog->setModal(true);
        verticalLayout_2 = new QVBoxLayout(WebcamBusyDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetMaximumSize);
        label_Titel_Busy = new QLabel(WebcamBusyDialog);
        label_Titel_Busy->setObjectName(QString::fromUtf8("label_Titel_Busy"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_Titel_Busy->setFont(font);
        label_Titel_Busy->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_Titel_Busy);

        horizontalLayout_Picture_and_Info = new QHBoxLayout();
        horizontalLayout_Picture_and_Info->setObjectName(QString::fromUtf8("horizontalLayout_Picture_and_Info"));
        frame_Picture = new QFrame(WebcamBusyDialog);
        frame_Picture->setObjectName(QString::fromUtf8("frame_Picture"));
        frame_Picture->setMinimumSize(QSize(100, 100));
        frame_Picture->setMaximumSize(QSize(100, 100));
        frame_Picture->setBaseSize(QSize(0, 0));
        frame_Picture->setFrameShape(QFrame::StyledPanel);
        frame_Picture->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_Picture);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_Picture = new QVBoxLayout();
        verticalLayout_Picture->setObjectName(QString::fromUtf8("verticalLayout_Picture"));
        label_Picture = new QLabel(frame_Picture);
        label_Picture->setObjectName(QString::fromUtf8("label_Picture"));
        label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/pictures/webcamBusy.png")));
        label_Picture->setScaledContents(true);

        verticalLayout_Picture->addWidget(label_Picture);


        horizontalLayout_5->addLayout(verticalLayout_Picture);


        horizontalLayout_Picture_and_Info->addWidget(frame_Picture);

        formLayout_Info = new QFormLayout();
        formLayout_Info->setObjectName(QString::fromUtf8("formLayout_Info"));
        formLayout_Info->setSizeConstraint(QLayout::SetDefaultConstraint);
        formLayout_Info->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        formLayout_Info->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_Info->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_Info->setVerticalSpacing(0);
        label_Name = new QLabel(WebcamBusyDialog);
        label_Name->setObjectName(QString::fromUtf8("label_Name"));
        label_Name->setMargin(6);

        formLayout_Info->setWidget(0, QFormLayout::LabelRole, label_Name);

        label_Name_Value = new QLabel(WebcamBusyDialog);
        label_Name_Value->setObjectName(QString::fromUtf8("label_Name_Value"));
        label_Name_Value->setMargin(6);

        formLayout_Info->setWidget(0, QFormLayout::FieldRole, label_Name_Value);

        label_Device = new QLabel(WebcamBusyDialog);
        label_Device->setObjectName(QString::fromUtf8("label_Device"));
        label_Device->setMargin(6);

        formLayout_Info->setWidget(1, QFormLayout::LabelRole, label_Device);

        label_Device_Value = new QLabel(WebcamBusyDialog);
        label_Device_Value->setObjectName(QString::fromUtf8("label_Device_Value"));
        label_Device_Value->setMargin(6);

        formLayout_Info->setWidget(1, QFormLayout::FieldRole, label_Device_Value);

        label_UsedBy = new QLabel(WebcamBusyDialog);
        label_UsedBy->setObjectName(QString::fromUtf8("label_UsedBy"));
        label_UsedBy->setMargin(6);

        formLayout_Info->setWidget(2, QFormLayout::LabelRole, label_UsedBy);

        label_UsedBy_Value = new QLabel(WebcamBusyDialog);
        label_UsedBy_Value->setObjectName(QString::fromUtf8("label_UsedBy_Value"));
        label_UsedBy_Value->setMargin(6);

        formLayout_Info->setWidget(2, QFormLayout::FieldRole, label_UsedBy_Value);


        horizontalLayout_Picture_and_Info->addLayout(formLayout_Info);


        verticalLayout_2->addLayout(horizontalLayout_Picture_and_Info);

        horizontalLayout_ButtonBox = new QHBoxLayout();
        horizontalLayout_ButtonBox->setObjectName(QString::fromUtf8("horizontalLayout_ButtonBox"));
        buttonBox = new QDialogButtonBox(WebcamBusyDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        horizontalLayout_ButtonBox->addWidget(buttonBox);


        verticalLayout_2->addLayout(horizontalLayout_ButtonBox);


        retranslateUi(WebcamBusyDialog);

        QMetaObject::connectSlotsByName(WebcamBusyDialog);
    } // setupUi

    void retranslateUi(QDialog *WebcamBusyDialog)
    {
        label_Titel_Busy->setText(QApplication::translate("WebcamBusyDialog", "Device is busy", 0, QApplication::UnicodeUTF8));
        label_Picture->setText(QString());
        label_Name->setText(QApplication::translate("WebcamBusyDialog", "Name", 0, QApplication::UnicodeUTF8));
        label_Name_Value->setText(QApplication::translate("WebcamBusyDialog", "Unknown", 0, QApplication::UnicodeUTF8));
        label_Device->setText(QApplication::translate("WebcamBusyDialog", "Device", 0, QApplication::UnicodeUTF8));
        label_Device_Value->setText(QApplication::translate("WebcamBusyDialog", "Unknown", 0, QApplication::UnicodeUTF8));
        label_UsedBy->setText(QApplication::translate("WebcamBusyDialog", "Used by", 0, QApplication::UnicodeUTF8));
        label_UsedBy_Value->setText(QApplication::translate("WebcamBusyDialog", "Unknown", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(WebcamBusyDialog);
    } // retranslateUi

};

namespace Ui {
    class WebcamBusyDialog: public Ui_WebcamBusyDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QVKWEBCAMBUSYDIALOG_H
