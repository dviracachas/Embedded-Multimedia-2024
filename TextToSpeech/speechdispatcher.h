#ifndef SPEECHDISPATCHER_H
#define SPEECHDISPATCHER_H

#include <QObject>
#include <QDebug>
#include <QSysInfo>
#include <QProcess>
#include <QThread>
#include <QRegularExpression>

class SpeechDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit SpeechDispatcher(QObject *parent = nullptr);

    QString text;
    void start(QString text);
    void speak();
    void quiet();

private:
    QProcess mProcess;
    const QString command ="spd-say";
    const int stringSize =75;
    const int delay =4.8;
    QStringList arguments;
    void splitText(QString text);
    void sayLine(QStringList textlines);


};

#endif // SPEECHDISPATCHER_H



