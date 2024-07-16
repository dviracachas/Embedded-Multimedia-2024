#include <QCoreApplication>
#include <QRegularExpression>
#include "speechdispatcher.h"
#include <QDir>
#include <QTextStream>
#include <iostream>

const QString getStringFromFile(const QString& fileName) {

    QDir dir;
    QFile file(dir.absoluteFilePath(fileName));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to access text file";
        return nullptr;
    }

    QTextStream in(&file);
    QString text = in.readAll();

    if (text.isEmpty()) {
        qDebug() << "Text file is empty";
        return nullptr;
    }

    return text;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SpeechDispatcher s;

    static const QString TEXT = getStringFromFile("/mnt/ramdisk/response.txt");

    s.start(TEXT);

    return a.exec();
}
