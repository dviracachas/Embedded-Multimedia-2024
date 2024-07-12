#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QCoreApplication>
#include <QtStateMachine/QStateMachine>
#include <QtStateMachine/QState>
#include <QtStateMachine/QFinalState>
#include <QDebug>
#include <QTimer>
#include "StateMachineController.h"
#include "processrunner.h"
#include <QInputDialog>
#include <iostream>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);



    //////////////////////////////////////////////////////////////////////////
    QStateMachine machine;
    ProcessRunner runner;

        QState *wwd_state = new QState();
        QState *stt_state = new QState();
        QState *gpt_state = new QState();
        QState *tts_state = new QState();
        QFinalState *final_state = new QFinalState();

        machine.addState(wwd_state);
        machine.addState(stt_state);
        machine.addState(gpt_state);
        machine.addState(tts_state);
        machine.addState(final_state);

        machine.setInitialState(wwd_state);

        StateMachineController controller(&machine);

        // Use the controller's signal to trigger transitions
        wwd_state->addTransition(&controller, &StateMachineController::keywordDetected, stt_state);
        stt_state->addTransition(&controller, &StateMachineController::transcriptionDone, gpt_state);

        QObject::connect(wwd_state, &QState::entered, [&runner, &controller]() {
            qDebug() << "Pola is sleeping zzz";
            runner.startProcess("/home/pi/Documents/HeyPolaProject/build-HeyPola-Qt_6_6_1_6_6_1-Debug/HeyPola");
        });
        QObject::connect(stt_state, &QState::entered, []() {
            qDebug() << "Hi Im Pola! How can I help you ?";
        });
        QObject::connect(gpt_state, &QState::entered, []() {
            qDebug() << "Entered GPT State. Type 'kw' to move to Transcription.";
        });
        QObject::connect(tts_state, &QState::entered, []() {
            qDebug() << "Entered Speech State. Type 'wwd' to move to wwd.";
        });
        QObject::connect(&machine, &QStateMachine::finished, []() {
            qDebug() << "Machine finished. Type 'quit' to exit.";
        });

        machine.start();

        QTimer timer;
        QObject::connect(&timer, &QTimer::timeout, [&controller, &runner]() {
            QString returnedOutput = runner.returnOutput();
            if (returnedOutput != "")
            {
                qDebug() << returnedOutput;
            }
            controller.handleUserInput(returnedOutput);
        });
        timer.start(100);  // Check every 100 ms

    //////////////////////////////////////////////////////////////////////////

    return app.exec();
}
