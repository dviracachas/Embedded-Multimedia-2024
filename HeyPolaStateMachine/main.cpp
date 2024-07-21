#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCoreApplication>
#include <QtStateMachine/QStateMachine>
#include <QtStateMachine/QState>
#include <QtStateMachine/QFinalState>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include "StateMachineController.h"
#include "processrunner.h"
#include <QInputDialog>
#include <iostream>


int main(int argc, char *argv[])
{
//#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//#endif
    QGuiApplication app(argc, argv);

    QStateMachine machine;
    StateMachineController controller(&machine);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("controller", &controller);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
     /*
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    */
    engine.load(url);



    //////////////////////////////////////////////////////////////////////////
    ProcessRunner runner;

    QState *wwd_state = new QState();
    QState *rec_state = new QState();////new
    QState *stt_state = new QState();
    QState *gpt_state = new QState();
    QState *tts_state = new QState();
    QFinalState *final_state = new QFinalState();

    machine.addState(wwd_state);
    machine.addState(rec_state);////new
    machine.addState(stt_state);
    machine.addState(gpt_state);
    machine.addState(tts_state);
    machine.addState(final_state);

    machine.setInitialState(wwd_state);



    // Use the controller's signal to trigger transitions
    wwd_state->addTransition(&controller, &StateMachineController::keywordDetected, rec_state);////alterd Col85:94
    rec_state->addTransition(&controller, &StateMachineController::recordingDone, stt_state);////new
    stt_state->addTransition(&controller, &StateMachineController::transcriptionDone, gpt_state);
    stt_state->addTransition(&controller, &StateMachineController::stateError, stt_state);
    gpt_state->addTransition(&controller, &StateMachineController::llamaResponse, tts_state);
    tts_state->addTransition(&controller, &StateMachineController::textSound, wwd_state);

    QObject::connect(wwd_state, &QState::entered, [&runner, &controller]() {
        qDebug() << "Pola is sleeping zzz";
        runner.process.waitForFinished(-1);
        qDebug() << "start/Text2Speech finished";
        runner.startProcess("/home/pi/Documents/HeyPolaProject/build-HeyPola-Qt_6_6_1_6_6_1-Debug/HeyPola", false);////alterd
    });
    QObject::connect(rec_state, &QState::entered, [&runner,&controller]() {
        qDebug() << "Hi Im Pola! How can I help you ?";
        runner.process.waitForFinished(-1);
        qDebug() << "wake word finished.";
        runner.startProcess("/home/pi/Documents/fw_Record/fw_Record", false);////alterd
    });

    QObject::connect(stt_state, &QState::entered, [&runner,&controller]() {                         ////new
        qDebug() << "I heard you.";
        runner.process.waitForFinished(-1);
        qDebug() << "recording finished.";////new
        runner.startProcess("/home/pi/Documents/fw_Transcribe/transcribe.py", true);                ////new
    });                                                                                             ////new
    QObject::connect(gpt_state, &QState::entered, [&runner,&controller]() {
        qDebug() << "Alright, let me think about it.";
        runner.process.waitForFinished(-1);
        qDebug() << "transcription finished.";
        runner.startProcess("/home/pi/Documents/LlamaTest/qt_llama/qt_llama", false);
    });
    QObject::connect(tts_state, &QState::entered, [&runner,&controller]() {
        qDebug() << "Oh right, here is the answer.";
        runner.process.waitForFinished(-1);
        qDebug() << "ask llama finished.";
        runner.startProcess("/home/pi/Software/Embedded/TextToSpeech/build-TextToSpeech-Qt_6_6_1_6_6_1-Debug/TextToSpeech", false);
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
    timer.start(1);  // Check every 1 ms

    //////////////////////////////////////////////////////////////////////////

    return app.exec();
}
