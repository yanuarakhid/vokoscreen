#ifndef QvkWebcam_H
#define QvkWebcam_H
#include <QDialog>
#include <QProcess>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>

class QvkWebcam : public QDialog
{ 
Q_OBJECT
public:
  QvkWebcam( QCheckBox *widget );
  virtual ~QvkWebcam();
  
  QStringList deviceList;
  
  QCheckBox *myWidget;
  
  QProcess *ProcessCammera1;
  QProcess *ProcessCammera2;
  QProcess *ProcessCammera3;
  
  QLabel *label1;
  QLabel *label2;
  QLabel *label3;
  
  QCheckBox *webcam1QCheckBox;  
  QCheckBox *webcam2QCheckBox;  
  QCheckBox *webcam3QCheckBox;
  
  QComboBox *webcam1QComboBox;
  QComboBox *webcam2QComboBox;
  QComboBox *webcam3QComboBox;

public slots:
  void startWebcam();
  void startWebcam1();
  void startWebcam2();
  void startWebcam3();

  void stopWebcam();
 
  void showWebcamDialog();
  void myfileSystemWatcher( const QString & path );
  QStringList resolution( QString device);

  void stateChanged1( QProcess::ProcessState newState );
  void stateChanged2( QProcess::ProcessState newState );
  void stateChanged3( QProcess::ProcessState newState );
  
  void reStartWebcam1( int );
  void reStartWebcam2( int );
  void reStartWebcam3( int );
  
protected:  


  
private:

  
};


#endif
