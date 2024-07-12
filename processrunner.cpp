#include "processrunner.h"
#include <QDebug>
#include <QCoreApplication>  // Include this header to use QCoreApplication

ProcessRunner::ProcessRunner(QObject *parent) : QObject(parent)
{
    //connect(&process, &QProcess::readyReadStandardOutput, this, &ProcessRunner::readOutput);
    //connect(&process, &QProcess::readyReadStandardError, this, &ProcessRunner::readErrorOutput);
    //connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &ProcessRunner::processFinished);
}

void ProcessRunner::startProcess(const QString &exePath)
{
    process.start(exePath);
    if (!process.waitForStarted()) {
        qDebug() << "Failed to start process";
    }

}

void ProcessRunner::readOutput()
{
    QString output = process.readAllStandardOutput();

    qDebug() << "Output:" << output;
}

QString ProcessRunner::returnOutput ()
{
    QString output = process.readAllStandardOutput();
    return output;
}

void ProcessRunner::readErrorOutput()
{
    QString errorOutput = process.readAllStandardError();
    qDebug() << "Error Output:" << errorOutput;
}

void ProcessRunner::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus)
    qDebug() << "Process finished with exit code:" << exitCode;
    //QCoreApplication::quit();
}

