
#include "QvkWebcamBusyDialog.h"

QvkWebcamBusyDialog::QvkWebcamBusyDialog( QString webcamName, QString Device, QString Busyby )
    {
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth( this->sizePolicy().hasHeightForWidth());
        this->setSizePolicy(sizePolicy);
        setModal(true);
        verticalLayout_Dialog = new QVBoxLayout( this );
        verticalLayout_Dialog->setSizeConstraint(QLayout::SetMaximumSize);
        label_Titel_Busy = new QLabel( this );
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_Titel_Busy->setFont(font);
        label_Titel_Busy->setAlignment(Qt::AlignCenter);

        verticalLayout_Dialog->addWidget(label_Titel_Busy);

        horizontalLayout_Picture_and_Info = new QHBoxLayout();
        frame_Picture = new QFrame( this );
        frame_Picture->setMinimumSize(QSize(100, 100));
        frame_Picture->setMaximumSize(QSize(100, 100));
        frame_Picture->setBaseSize(QSize(0, 0));
        frame_Picture->setFrameShape(QFrame::StyledPanel);
        frame_Picture->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_Picture);
        verticalLayout_Picture = new QVBoxLayout();
        label_Picture = new QLabel(frame_Picture);
        label_Picture->setPixmap(QPixmap(QString::fromUtf8(":/pictures/webcamBusy.png")));
        label_Picture->setScaledContents(true);

        verticalLayout_Picture->addWidget(label_Picture);

        horizontalLayout_5->addLayout(verticalLayout_Picture);

        horizontalLayout_Picture_and_Info->addWidget(frame_Picture);

        formLayout_Info = new QFormLayout();
        formLayout_Info->setSizeConstraint(QLayout::SetDefaultConstraint);
        formLayout_Info->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        formLayout_Info->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_Info->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_Info->setVerticalSpacing(0);
        label_Name = new QLabel( this );
        label_Name->setMargin(6);

        formLayout_Info->setWidget(0, QFormLayout::LabelRole, label_Name);

        label_Name_Value = new QLabel( this );
        label_Name_Value->setMargin(6);

        formLayout_Info->setWidget(0, QFormLayout::FieldRole, label_Name_Value);

        label_Device = new QLabel( this );
        label_Device->setMargin(6);

        formLayout_Info->setWidget(1, QFormLayout::LabelRole, label_Device);

        label_Device_Value = new QLabel( this );
        label_Device_Value->setMargin(6);

        formLayout_Info->setWidget(1, QFormLayout::FieldRole, label_Device_Value);

        label_UsedBy = new QLabel( this );
        label_UsedBy->setMargin(6);

        formLayout_Info->setWidget(2, QFormLayout::LabelRole, label_UsedBy);

        label_UsedBy_Value = new QLabel( this );
        label_UsedBy_Value->setMargin(6);

        formLayout_Info->setWidget(2, QFormLayout::FieldRole, label_UsedBy_Value);

        horizontalLayout_Picture_and_Info->addLayout(formLayout_Info);

        verticalLayout_Dialog->addLayout(horizontalLayout_Picture_and_Info);

        horizontalLayout_ButtonBox = new QHBoxLayout();
        buttonBox = new QDialogButtonBox( this );
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
	connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );

        horizontalLayout_ButtonBox->addWidget(buttonBox);
        verticalLayout_Dialog->addLayout(horizontalLayout_ButtonBox);

        setWindowTitle( "vokoscreen webcam" );
        label_Titel_Busy->setText( tr( "Webcam is busy" ) );
	label_Name->setText( tr( "Name" ) );
        label_Name_Value->setText( webcamName );
	label_Device->setText( tr( "Device" ) );
        label_Device_Value->setText( Device );
	label_UsedBy->setText( tr( "Used by" ) );
        label_UsedBy_Value->setText( "<html><head/><body><p><span style=\" color:#ff0000;\">" + Busyby + "</span></p></body></html>" );
	show();
}

QvkWebcamBusyDialog::~QvkWebcamBusyDialog()
{
}
