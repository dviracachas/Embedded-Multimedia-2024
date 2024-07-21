#include "StateMachineController.h"
#include <QCoreApplication>
#include <QDebug>
#include <processrunner.h>
#include <fstream>
#include <QFile>
QString ask_read(const QString &transcribe_txt) {
    QFile file(transcribe_txt);
    if(!file.exists()){
        qDebug() << ".txt doesnt exist"<<transcribe_txt;
    }
    QString line;
    QString total_text;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
            total_text.append(line);
        }
    }
    file.close();
    return total_text;
}

StateMachineController::StateMachineController(QStateMachine* machine, QObject* parent)
    : QObject(parent), m_machine(machine) {}

void StateMachineController::handleUserInput(const QString& input) {
    if (input == "kw") {
        emit keywordDetected();
    } else if (input == "rec") {
        emit recordingDone();
    } else if (input == "tr") {  
        QString transcribed_from_txt = ask_read("/mnt/ramdisk/transcription.txt");
        if(transcribed_from_txt == "error"){
            emit stateError();
            qDebug() <<"Error in Transcribe";
        }else{
            emit transcriptionDone(transcribed_from_txt);
            //emit replaceText(transcribed_from_txt);
        }
    }else if (input == "ask"){    
        QString response_from_txt = ask_read("/mnt/ramdisk/response.txt");
        emit llamaResponse(response_from_txt);
        //emit replaceText(response_from_txt);
    }  else if (input == "talk"){
        emit textSound();
    } else if (input == "quit") {
        m_machine->stop();
        qApp->quit();
    } else {
        //
    }
}
