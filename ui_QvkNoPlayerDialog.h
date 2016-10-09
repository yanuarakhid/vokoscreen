/********************************************************************************
** Form generated from reading UI file 'QvkNoPlayerDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QVKNOPLAYERDIALOG_H
#define UI_QVKNOPLAYERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_NoPlayerDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NoPlayerDialog)
    {
        if (NoPlayerDialog->objectName().isEmpty())
            NoPlayerDialog->setObjectName(QStringLiteral("NoPlayerDialog"));
        NoPlayerDialog->resize(298, 123);
        NoPlayerDialog->setWindowTitle(QStringLiteral("Dialog"));
        verticalLayout_2 = new QVBoxLayout(NoPlayerDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(NoPlayerDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(100, 75));
        label->setPixmap(QPixmap(QString::fromUtf8(":/pictures/novideoplayer.png")));
        label->setScaledContents(true);

        horizontalLayout->addWidget(label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_2 = new QLabel(NoPlayerDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(NoPlayerDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(NoPlayerDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), NoPlayerDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), NoPlayerDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(NoPlayerDialog);
    } // setupUi

    void retranslateUi(QDialog *NoPlayerDialog)
    {
        label->setText(QString());
        label_2->setText(QApplication::translate("NoPlayerDialog", "Please install a videoplayer", 0));
        Q_UNUSED(NoPlayerDialog);
    } // retranslateUi

};

namespace Ui {
    class NoPlayerDialog: public Ui_NoPlayerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QVKNOPLAYERDIALOG_H
