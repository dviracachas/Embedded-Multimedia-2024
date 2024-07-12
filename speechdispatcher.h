#ifndef SPEECHDISPATCHER_H
#define SPEECHDISPATCHER_H

#include <QObject>
#include <QDebug>
#include <QSysInfo>
#include <QProcess>
#include <QThread>

class SpeechDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit SpeechDispatcher(QObject *parent = nullptr);

    QString text;



signals:


public:
    void start(QString text);
    void speak();
    void quiet();

private:
    void divideText(QString text);
    void sayLine(QStringList textlines);
    QProcess mProcess;
    QString command ="spd-say";
    const int stringSize = 75;
    const int delay = 5;
    QStringList arguments;



};

#endif // SPEECHDISPATCHER_H



