#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QMediaDevices>
#include <QBuffer>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QProcess>
#include <QElapsedTimer>
#include <iostream>
#include <fstream>



void ask_listen(const QString &question) {
        QProcess process;
        process.start("python3", QStringList() << "/home/pi/.HeyPolaProject/Embedded-Multimedia-2024/qt_llama/AskPola.py"<< question);
        //std::cout << "You asked: \"" << question.toStdString() << "\"" << std::endl;
        process.waitForFinished();
        QString output(process.readAllStandardOutput());
        std::cout << output.toStdString();
        exit(0);
        //qDebug() << output;
} 
void ask_read(const QString &transcribe_txt) {
    QFile file(transcribe_txt);
    if(!file.exists()){
        qDebug() << ".txt doesnt exist"<<transcribe_txt;
    }
    QString line;
    QString question;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
            question.append(line);
        }
    }
    file.close();
    //std::cout<< question.toStdString()<< std::endl;
    ask_listen(question);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString frage = "/mnt/ramdisk/transcription.txt";
    ask_read(frage);
    QCoreApplication::quit();
    return a.exec();
}
