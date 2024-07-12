#ifndef STATEMACHINECONTROLLER_H
#define STATEMACHINECONTROLLER_H

#include <QObject>
#include <QtStateMachine/QStateMachine>

class StateMachineController : public QObject
{
    Q_OBJECT

public:
    explicit StateMachineController(QStateMachine* machine, QObject* parent = nullptr);

public slots:
    void handleUserInput(const QString& input);

signals:
    void keywordDetected();
    void transcriptionDone();

private:
    QStateMachine* m_machine;
};

#endif // STATEMACHINECONTROLLER_H
