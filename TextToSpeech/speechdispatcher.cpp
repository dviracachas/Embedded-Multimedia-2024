#include "speechdispatcher.h"
#include <iostream>
SpeechDispatcher::SpeechDispatcher(QObject *parent)
    : QObject{parent}
{



}

void SpeechDispatcher::start(QString text)
{
    this->splitText(text);
    this->speak();

}

void SpeechDispatcher::speak()
{
    QStringList argu;
    for (int i = 0; i < arguments.size(); i += 1)
    {
        argu <<"-t" <<"female1" << arguments[i];
        this->sayLine(argu);
        argu.clear();
        // std::cout << i << std::endl;
    }
    // std::cout << "Before Quiet()" << std::endl;
    this->quiet();

}


void SpeechDispatcher::quiet()
{
    std::cout << "talk";
    mProcess.kill();
    exit(0);
}

void SpeechDispatcher::splitText(QString text)
{
    QString tmp(text);
    QStringList list;
    list.clear();
    QString space =" ";

    while (!tmp.isEmpty()) {
        int nextSpace = tmp.indexOf(space,stringSize-1);
        int prevSpace = tmp.lastIndexOf(space,stringSize-1);
        int nextDistance = nextSpace - (stringSize-1);
        int prevDistance = (stringSize-1) - prevSpace;

        if (tmp.size()<=stringSize){
            list.append(tmp.left(stringSize));
            tmp.remove(0, stringSize);
        }else if (abs(nextDistance) < abs(prevDistance)){
            list.append(tmp.left(nextSpace+1));
            tmp.remove(0, nextSpace+1);
        }else if(abs(nextDistance) >= abs(prevDistance)){
            list.append(tmp.left(prevSpace+1));
            tmp.remove(0, prevSpace+1);
        }
    }
    arguments = list;

}

void SpeechDispatcher::sayLine(QStringList textlines)
{

    mProcess.start(command, textlines);
    mProcess.waitForFinished();                       //Waits for execution to complete
    QThread::sleep(delay);
}
