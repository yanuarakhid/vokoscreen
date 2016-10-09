/********************************************************************************
** Form generated from reading UI file 'showclickDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWCLICKDIALOG_H
#define UI_SHOWCLICKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QPushButton *pushButton_gray;
    QPushButton *pushButton_cyan;
    QPushButton *pushButton_white;
    QPushButton *pushButton_green;
    QPushButton *pushButton_darkCyan;
    QPushButton *pushButton_darkYellow;
    QPushButton *pushButton_darkGray;
    QPushButton *pushButton_darkBlue;
    QPushButton *pushButton_magenta;
    QPushButton *pushButton_black;
    QPushButton *pushButton_darkRed;
    QPushButton *pushButton_darkGreen;
    QPushButton *pushButton_yellow;
    QPushButton *pushButton_darkMagenta;
    QPushButton *pushButton_red;
    QPushButton *pushButton_blue;
    QHBoxLayout *horizontalLayout_5;
    QGraphicsView *graphicsView;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_6;
    QSlider *horizontalSliderCircle;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QSlider *horizontalSliderOpacity;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QSlider *horizontalSliderShowtime;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QCheckBox *checkBoxRadiant;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(526, 520);
        Dialog->setMinimumSize(QSize(0, 520));
        horizontalLayout = new QHBoxLayout(Dialog);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        frame = new QFrame(Dialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        label->setEnabled(true);
        label->setMaximumSize(QSize(80, 80));
        label->setFrameShape(QFrame::NoFrame);
        label->setPixmap(QPixmap(QString::fromUtf8(":/pictures/farbeimer-gruen.png")));
        label->setScaledContents(true);

        verticalLayout_3->addWidget(label);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMaximumSize(QSize(80, 80));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/pictures/farbeimer-gelb.png")));
        label_3->setScaledContents(true);

        verticalLayout_3->addWidget(label_3);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(80, 80));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/pictures/farbeimer-rot.png")));
        label_2->setScaledContents(true);

        verticalLayout_3->addWidget(label_2);


        verticalLayout_2->addWidget(frame);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer_5 = new QSpacerItem(15, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton_gray = new QPushButton(Dialog);
        pushButton_gray->setObjectName(QStringLiteral("pushButton_gray"));

        gridLayout->addWidget(pushButton_gray, 3, 2, 1, 1);

        pushButton_cyan = new QPushButton(Dialog);
        pushButton_cyan->setObjectName(QStringLiteral("pushButton_cyan"));

        gridLayout->addWidget(pushButton_cyan, 0, 2, 1, 1);

        pushButton_white = new QPushButton(Dialog);
        pushButton_white->setObjectName(QStringLiteral("pushButton_white"));

        gridLayout->addWidget(pushButton_white, 0, 0, 1, 1);

        pushButton_green = new QPushButton(Dialog);
        pushButton_green->setObjectName(QStringLiteral("pushButton_green"));

        gridLayout->addWidget(pushButton_green, 2, 0, 1, 1);

        pushButton_darkCyan = new QPushButton(Dialog);
        pushButton_darkCyan->setObjectName(QStringLiteral("pushButton_darkCyan"));

        gridLayout->addWidget(pushButton_darkCyan, 0, 3, 1, 1);

        pushButton_darkYellow = new QPushButton(Dialog);
        pushButton_darkYellow->setObjectName(QStringLiteral("pushButton_darkYellow"));

        gridLayout->addWidget(pushButton_darkYellow, 2, 3, 1, 1);

        pushButton_darkGray = new QPushButton(Dialog);
        pushButton_darkGray->setObjectName(QStringLiteral("pushButton_darkGray"));

        gridLayout->addWidget(pushButton_darkGray, 3, 3, 1, 1);

        pushButton_darkBlue = new QPushButton(Dialog);
        pushButton_darkBlue->setObjectName(QStringLiteral("pushButton_darkBlue"));

        gridLayout->addWidget(pushButton_darkBlue, 3, 1, 1, 1);

        pushButton_magenta = new QPushButton(Dialog);
        pushButton_magenta->setObjectName(QStringLiteral("pushButton_magenta"));

        gridLayout->addWidget(pushButton_magenta, 1, 2, 1, 1);

        pushButton_black = new QPushButton(Dialog);
        pushButton_black->setObjectName(QStringLiteral("pushButton_black"));
        pushButton_black->setAutoFillBackground(false);

        gridLayout->addWidget(pushButton_black, 0, 1, 1, 1);

        pushButton_darkRed = new QPushButton(Dialog);
        pushButton_darkRed->setObjectName(QStringLiteral("pushButton_darkRed"));

        gridLayout->addWidget(pushButton_darkRed, 1, 1, 1, 1);

        pushButton_darkGreen = new QPushButton(Dialog);
        pushButton_darkGreen->setObjectName(QStringLiteral("pushButton_darkGreen"));

        gridLayout->addWidget(pushButton_darkGreen, 2, 1, 1, 1);

        pushButton_yellow = new QPushButton(Dialog);
        pushButton_yellow->setObjectName(QStringLiteral("pushButton_yellow"));

        gridLayout->addWidget(pushButton_yellow, 2, 2, 1, 1);

        pushButton_darkMagenta = new QPushButton(Dialog);
        pushButton_darkMagenta->setObjectName(QStringLiteral("pushButton_darkMagenta"));

        gridLayout->addWidget(pushButton_darkMagenta, 1, 3, 1, 1);

        pushButton_red = new QPushButton(Dialog);
        pushButton_red->setObjectName(QStringLiteral("pushButton_red"));

        gridLayout->addWidget(pushButton_red, 1, 0, 1, 1);

        pushButton_blue = new QPushButton(Dialog);
        pushButton_blue->setObjectName(QStringLiteral("pushButton_blue"));

        gridLayout->addWidget(pushButton_blue, 3, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        graphicsView = new QGraphicsView(Dialog);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setMinimumSize(QSize(0, 110));

        horizontalLayout_5->addWidget(graphicsView);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(5, -1, -1, -1);
        label_6 = new QLabel(Dialog);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMaximumSize(QSize(34, 13));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/pictures/cursor.png")));
        label_6->setScaledContents(true);

        horizontalLayout_4->addWidget(label_6);

        horizontalSliderCircle = new QSlider(Dialog);
        horizontalSliderCircle->setObjectName(QStringLiteral("horizontalSliderCircle"));
        horizontalSliderCircle->setMinimum(40);
        horizontalSliderCircle->setMaximum(110);
        horizontalSliderCircle->setValue(70);
        horizontalSliderCircle->setSliderPosition(70);
        horizontalSliderCircle->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(horizontalSliderCircle);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(5, -1, -1, -1);
        label_7 = new QLabel(Dialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMaximumSize(QSize(34, 13));
        label_7->setPixmap(QPixmap(QString::fromUtf8(":/pictures/transparenz.png")));
        label_7->setScaledContents(true);

        horizontalLayout_8->addWidget(label_7);

        horizontalSliderOpacity = new QSlider(Dialog);
        horizontalSliderOpacity->setObjectName(QStringLiteral("horizontalSliderOpacity"));
        horizontalSliderOpacity->setMinimum(1);
        horizontalSliderOpacity->setMaximum(100);
        horizontalSliderOpacity->setSliderPosition(50);
        horizontalSliderOpacity->setOrientation(Qt::Horizontal);

        horizontalLayout_8->addWidget(horizontalSliderOpacity);


        verticalLayout->addLayout(horizontalLayout_8);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(15);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(5, 0, 0, -1);
        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMaximumSize(QSize(25, 35));
        label_4->setSizeIncrement(QSize(0, 0));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/pictures/sandglass.png")));
        label_4->setScaledContents(true);
        label_4->setMargin(0);

        horizontalLayout_3->addWidget(label_4);

        horizontalSliderShowtime = new QSlider(Dialog);
        horizontalSliderShowtime->setObjectName(QStringLiteral("horizontalSliderShowtime"));
        horizontalSliderShowtime->setMinimum(1);
        horizontalSliderShowtime->setMaximum(20);
        horizontalSliderShowtime->setValue(5);
        horizontalSliderShowtime->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(horizontalSliderShowtime);


        gridLayout_2->addLayout(horizontalLayout_3, 9, 0, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_5 = new QLabel(Dialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setMaximumSize(QSize(40, 40));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/pictures/radiant.png")));
        label_5->setScaledContents(true);

        horizontalLayout_6->addWidget(label_5);

        checkBoxRadiant = new QCheckBox(Dialog);
        checkBoxRadiant->setObjectName(QStringLiteral("checkBoxRadiant"));

        horizontalLayout_6->addWidget(checkBoxRadiant);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_6);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::RestoreDefaults);

        horizontalLayout_2->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Show Click Settings", 0));
        label->setText(QString());
        label_3->setText(QString());
        label_2->setText(QString());
        pushButton_gray->setText(QString());
        pushButton_cyan->setText(QString());
        pushButton_white->setText(QString());
        pushButton_green->setText(QString());
        pushButton_darkCyan->setText(QString());
        pushButton_darkYellow->setText(QString());
        pushButton_darkGray->setText(QString());
        pushButton_darkBlue->setText(QString());
        pushButton_magenta->setText(QString());
        pushButton_black->setText(QString());
        pushButton_darkRed->setText(QString());
        pushButton_darkGreen->setText(QString());
        pushButton_yellow->setText(QString());
        pushButton_darkMagenta->setText(QString());
        pushButton_red->setText(QString());
        pushButton_blue->setText(QString());
        label_6->setText(QString());
        label_7->setText(QString());
        label_4->setText(QString());
        label_5->setText(QString());
        checkBoxRadiant->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWCLICKDIALOG_H
