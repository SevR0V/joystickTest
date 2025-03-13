#include "gamepadworker.h"
#include <QTimer>
#include <QDebug>

const QString GamepadWorker::NO_DEVICE_NAME = "No Device";

GamepadWorker::GamepadWorker(QObject *parent) : QObject(parent)
{
    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        qDebug() << "SDL3 Init failed in worker:" << SDL_GetError();
        return;
    }

    currentPrimaryName = NO_DEVICE_NAME;
    currentSecondaryName = NO_DEVICE_NAME;
    updateDeviceList();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GamepadWorker::pollDevices);
    timer->start(16);
}

GamepadWorker::~GamepadWorker()
{
    for (SDL_Gamepad *gamepad : gamepads.values()) {
        if (gamepad) SDL_CloseGamepad(gamepad);
    }
    SDL_Quit();
}

void GamepadWorker::setPrimaryDevice(const QString &deviceName)
{
    if (deviceName != NO_DEVICE_NAME) {
        deactivateGamepad(primaryGamepad);
        primaryGamepad = gamepads.value(deviceName, nullptr);
        if (primaryGamepad) {
            qDebug() << "Primary Gamepad opened:" << deviceName;
            currentPrimaryName = deviceName;
        } else {
            qDebug() << "Failed to set Primary Gamepad:" << deviceName << "not found";
            currentPrimaryName = NO_DEVICE_NAME;
        }
    } else {
        deactivateGamepad(primaryGamepad);
        currentPrimaryName = NO_DEVICE_NAME;
    }
}

void GamepadWorker::setSecondaryDevice(const QString &deviceName)
{
    if (deviceName != NO_DEVICE_NAME) {
        deactivateGamepad(secondaryGamepad);
        secondaryGamepad = gamepads.value(deviceName, nullptr);
        if (secondaryGamepad) {
            qDebug() << "Secondary Gamepad opened:" << deviceName;
            currentSecondaryName = deviceName;
        } else {
            qDebug() << "Failed to set Secondary Gamepad:" << deviceName << "not found";
            currentSecondaryName = NO_DEVICE_NAME;
        }
    } else {
        deactivateGamepad(secondaryGamepad);
        currentSecondaryName = NO_DEVICE_NAME;
    }
}

void GamepadWorker::stop()
{
    running = false;
    timer->stop();
    qDebug() << "GamepadWorker stopped";
}

void GamepadWorker::pollDevices()
{
    if (!running) return;

    SDL_UpdateGamepads();

    int numGamepads = 0;
    SDL_GetGamepads(&numGamepads);
    if (numGamepads != lastNumGamepads) {
        updateDeviceList();
    }

    if (primaryGamepad) {
        for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i) {
            if (SDL_GetGamepadButton(primaryGamepad, static_cast<SDL_GamepadButton>(i))) {
                emit primaryButtonPressed(i);
            }
        }
        for (int i = 0; i < SDL_GAMEPAD_AXIS_COUNT; ++i) {
            Sint16 axisValue = SDL_GetGamepadAxis(primaryGamepad, static_cast<SDL_GamepadAxis>(i));
            if (abs(axisValue) > 1000) {
                emit primaryAxisMoved(i, axisValue);
            }
        }
    }

    if (secondaryGamepad) {
        for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i) {
            if (SDL_GetGamepadButton(secondaryGamepad, static_cast<SDL_GamepadButton>(i))) {
                emit secondaryButtonPressed(i);
            }
        }
        for (int i = 0; i < SDL_GAMEPAD_AXIS_COUNT; ++i) {
            Sint16 axisValue = SDL_GetGamepadAxis(secondaryGamepad, static_cast<SDL_GamepadAxis>(i));
            if (abs(axisValue) > 1000) {
                emit secondaryAxisMoved(i, axisValue);
            }
        }
    }
}

void GamepadWorker::updateDeviceList()
{
    int numGamepads = 0;
    SDL_JoystickID *gamepadIds = SDL_GetGamepads(&numGamepads);
    if (numGamepads == lastNumGamepads && !deviceListChanged) {
        SDL_free(gamepadIds);
        return;
    }

    lastNumGamepads = numGamepads;
    deviceListChanged = true;

    QMap<QString, SDL_Gamepad*> newGamepads;
    for (const QString &name : gamepads.keys()) {
        if (name != currentPrimaryName && name != currentSecondaryName) {
            SDL_Gamepad *gamepad = gamepads[name];
            if (gamepad) SDL_CloseGamepad(gamepad);
        } else {
            newGamepads[name] = gamepads[name];
        }
    }
    gamepads = newGamepads;

    QStringList deviceNames;
    deviceNames << NO_DEVICE_NAME;

    if (gamepadIds && numGamepads > 0) {
        for (int i = 0; i < numGamepads; ++i) {
            SDL_Gamepad *tempGamepad = SDL_OpenGamepad(gamepadIds[i]);
            if (tempGamepad) {
                const char *name = SDL_GetGamepadName(tempGamepad);
                QString deviceName = name ? name : QString("Gamepad %1").arg(gamepadIds[i]);
                if (!gamepads.contains(deviceName)) {
                    gamepads[deviceName] = tempGamepad;
                } else {
                    SDL_CloseGamepad(tempGamepad);
                }
                deviceNames << deviceName;
            }
        }
        SDL_free(gamepadIds);
    }

    emit deviceListUpdated(deviceNames);
}

void GamepadWorker::deactivateGamepad(SDL_Gamepad *&gamepad)
{
    if (gamepad) {
        QString nameToRemove;
        for (const QString &name : gamepads.keys()) {
            if (gamepads[name] == gamepad && name != currentPrimaryName && name != currentSecondaryName) {
                nameToRemove = name;
                break;
            }
        }
        if (!nameToRemove.isEmpty()) {
            gamepads.remove(nameToRemove);
            SDL_CloseGamepad(gamepad);
        }
        gamepad = nullptr;
        qDebug() << "Gamepad deactivated";
    }
}
