
#include "QvkLog.h" 

#include <QFile>
#include <QTextStream>

#include <stdio.h>
#include <stdlib.h>

QvkLog::QvkLog()
{
}


QvkLog::~QvkLog()
{
}


void QvkLog::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt;

    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf( stderr, "%s \n", localMsg.constData() );
        txt += localMsg.constData();
        break;
    case QtWarningMsg:
        fprintf( stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        txt += localMsg.constData();
        break;
    case QtCriticalMsg:
        fprintf( stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        txt += localMsg.constData();
        break;
    case QtFatalMsg:
        fprintf( stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        txt += localMsg.constData();
        abort();
    }
/*
    QFile outFile( "vokoscreen.log" );
    outFile.open( QIODevice::WriteOnly | QIODevice::Append );

    QTextStream textStream( &outFile );
    textStream << txt << "\n";
    outFile.flush();
    outFile.close();
*/
    //emit newLogText( txt );
}
