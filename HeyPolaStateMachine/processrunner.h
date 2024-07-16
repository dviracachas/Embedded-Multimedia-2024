#ifndef PROCESSRUNNER_H
#define PROCESSRUNNER_H

#include <QObject>
#include <QProcess>

class ProcessRunner : public QObject
{
    Q_OBJECT

public:
    ProcessRunner(QObject *parent = nullptr);
    QProcess process;
    void startProcess(const QString &exePath, const bool isPython);
    QString returnOutput();

private slots:
    void readOutput();
    void readErrorOutput();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

};
#endif // PROCESSRUNNER_H
