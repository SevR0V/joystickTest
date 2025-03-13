#ifndef GAMEPADWORKER_H
#define GAMEPADWORKER_H

#include <QObject>
#include <SDL3/SDL.h>
#include <QMap>

class GamepadWorker : public QObject {
    Q_OBJECT

public:
    explicit GamepadWorker(QObject *parent = nullptr);
    ~GamepadWorker();

    void setPrimaryDevice(const QString &deviceName);
    void setSecondaryDevice(const QString &deviceName);
    bool hasDeviceListChanged() const { return deviceListChanged; }
    void pollDevices();
    void updateDeviceList();
    void resetDeviceListChanged() { deviceListChanged = false; }

public slots:
    void stop(); // Сделали публичным слотом

signals:
    void primaryButtonPressed(int button);
    void primaryAxisMoved(int axis, Sint16 value);
    void secondaryButtonPressed(int button);
    void secondaryAxisMoved(int axis, Sint16 value);
    void deviceListUpdated(const QStringList &deviceNames);

private:
    void deactivateGamepad(SDL_Gamepad *&gamepad);

    SDL_Gamepad *primaryGamepad = nullptr;
    SDL_Gamepad *secondaryGamepad = nullptr;
    QString currentPrimaryName;
    QString currentSecondaryName;
    int lastNumGamepads = -1;
    bool deviceListChanged = false;
    QTimer *timer;
    bool running = true;
    QMap<QString, SDL_Gamepad*> gamepads;

    static const QString NO_DEVICE_NAME;
};

#endif // GAMEPADWORKER_H
