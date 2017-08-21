/********************************************************************************
** Form generated from reading UI file 'QvkMagnifierDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QVKMAGNIFIERDIALOG_H
#define UI_QVKMAGNIFIERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MagnifierDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QRadioButton *radioButton1;
    QRadioButton *radioButton2;
    QRadioButton *radioButton3;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *MagnifierDialog)
    {
        if (MagnifierDialog->objectName().isEmpty())
            MagnifierDialog->setObjectName(QStringLiteral("MagnifierDialog"));
        MagnifierDialog->resize(295, 198);
        MagnifierDialog->setWindowTitle(QStringLiteral("Dialog"));
        MagnifierDialog->setWindowFilePath(QStringLiteral(""));
        verticalLayout_2 = new QVBoxLayout(MagnifierDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(MagnifierDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(130, 150));
        label_2->setText(QStringLiteral(""));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/pictures/magnifier.png")));
        label_2->setScaledContents(true);
        label_2->setWordWrap(false);

        horizontalLayout->addWidget(label_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(30, -1, 30, -1);
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        radioButton1 = new QRadioButton(MagnifierDialog);
        radioButton1->setObjectName(QStringLiteral("radioButton1"));
        radioButton1->setText(QStringLiteral("200 x 200"));

        verticalLayout->addWidget(radioButton1);

        radioButton2 = new QRadioButton(MagnifierDialog);
        radioButton2->setObjectName(QStringLiteral("radioButton2"));
        radioButton2->setText(QStringLiteral("400 x 200"));
        radioButton2->setChecked(true);

        verticalLayout->addWidget(radioButton2);

        radioButton3 = new QRadioButton(MagnifierDialog);
        radioButton3->setObjectName(QStringLiteral("radioButton3"));
        radioButton3->setText(QStringLiteral("600 x 200"));

        verticalLayout->addWidget(radioButton3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(MagnifierDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(MagnifierDialog);

        QMetaObject::connectSlotsByName(MagnifierDialog);
    } // setupUi

    void retranslateUi(QDialog *MagnifierDialog)
    {
        Q_UNUSED(MagnifierDialog);
    } // retranslateUi

};

namespace Ui {
    class MagnifierDialog: public Ui_MagnifierDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QVKMAGNIFIERDIALOG_H
