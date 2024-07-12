#include "StateMachineController.h"
#include <QCoreApplication>
#include <QDebug>

StateMachineController::StateMachineController(QStateMachine* machine, QObject* parent)
    : QObject(parent), m_machine(machine) {}

void StateMachineController::handleUserInput(const QString& input) {
    if (input == "kw\n") {
        emit keywordDetected();
    } else if (input == "tr") {
        emit transcriptionDone();
    } else if (input == "quit") {
        m_machine->stop();
        qApp->quit();
    } else {
        //
    }
}
