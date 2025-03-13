#ifndef JOYSTICKTEST_H
#define JOYSTICKTEST_H

#include <QMainWindow>
#include <QThread>
#include "gamepadworker.h"
#include <SDL3/SDL.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class joystickTest;
}
QT_END_NAMESPACE

class JoystickTest : public QMainWindow
{
    Q_OBJECT

public:
    JoystickTest(QWidget *parent = nullptr);
    ~JoystickTest();
    bool isJoyListenerFinished;

signals:
    void primaryButtonPressed(int button);
    void primaryAxisMoved(int axis, Sint16 value);
    void secondaryButtonPressed(int button);
    void secondaryAxisMoved(int axis, Sint16 value);

private slots:
    // гуишные штучки
    void onLineEditLeftClicked(QString name);
    void onLineEditRightClicked(QString name);
    void startProgressCountdown();
    void updateProgress();
    // геймпаддные штучки
    void updateDeviceList(const QStringList &deviceNames);
    void onPrimaryDeviceChanged(int index);
    void onSecondaryDeviceChanged(int index);
    void checkForDeviceChanges();
    void onPrimaryButtonPressed(int button);
    void onPrimaryAxisMoved(int axis, Sint16 value);
    void onSecondaryButtonPressed(int button);
    void onSecondaryAxisMoved(int axis, Sint16 value);

public slots:
    void stopProgressCountdown();

private:
    Ui::joystickTest *ui;
    // гуишные штучки
    void replaceLineEdits(QWidget* widget);
    void replaceLineEditsInWidget(QWidget *widget, QWidget *mainWidget);
    QTimer *progressTimer;
    // геймпаддные штучки
    QString currentPrimaryName;
    QString currentSecondaryName;

    QThread *workerThread;
    GamepadWorker *worker;
};
#endif // JOYSTICKTEST_H
