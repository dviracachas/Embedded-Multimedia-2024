#ifndef PROCESSRUNNER_H
#define PROCESSRUNNER_H

#include <QObject>
#include <QProcess>

class ProcessRunner : public QObject
{
    Q_OBJECT

public:
    ProcessRunner(QObject *parent = nullptr);

    void startProcess(const QString &exePath);
    QString returnOutput();

private slots:
    void readOutput();
    void readErrorOutput();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess process;
};

#endif // PROCESSRUNNER_H
