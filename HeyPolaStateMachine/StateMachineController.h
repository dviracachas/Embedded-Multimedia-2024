#ifndef STATEMACHINECONTROLLER_H
#define STATEMACHINECONTROLLER_H

#include <QObject>
#include <QtStateMachine/QStateMachine>
#include <processrunner.h>
class StateMachineController : public QObject
{
    Q_OBJECT

public:
    explicit StateMachineController(QStateMachine* machine, QObject* parent = nullptr);

public slots:
    void handleUserInput(const QString& input);

signals:
    void keywordDetected();
    void recordingDone();
    void transcriptionDone(const QString text_t);
    void llamaResponse(const QString text_r);
    void textSound();
    void stateError();
    void replaceText(const QString newText);
private:
    QStateMachine* m_machine;
};

#endif // STATEMACHINECONTROLLER_H
