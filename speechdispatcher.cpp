#include "speechdispatcher.h"

SpeechDispatcher::SpeechDispatcher(QObject *parent)
    : QObject{parent}
{



}

void SpeechDispatcher::start(QString text)
{
    this->divideText(text);
    qInfo() << arguments;
    qInfo() << arguments.size();
    this->speak();

}

void SpeechDispatcher::speak()
{
    QStringList argu;
    for (int i = 0; i < arguments.size(); i += 1)
    {
        argu <<"-t" <<"female1" << arguments[i];
        qInfo() << argu;
        this->sayLine(argu);
        argu.clear();
    }
    this->quiet();


}


void SpeechDispatcher::quiet()
{
    mProcess.kill();
}

void SpeechDispatcher::divideText(QString text)
{
    QString tmp(text);
    QStringList list;
    list.clear();

    while (!tmp.isEmpty()) {
        list.append(tmp.left(stringSize));
        tmp.remove(0, stringSize);
    }
    arguments = list;

}

void SpeechDispatcher::sayLine(QStringList textlines)
{

    mProcess.start(command, textlines);
    mProcess.waitForFinished();                       //Waits for execution to complete
    QThread::sleep(delay);
}
