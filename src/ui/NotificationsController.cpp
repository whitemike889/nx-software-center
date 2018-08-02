#include <QDebug>
#include <QVariant>
#include <interactors/TaskMetadata.h>
#include "NotificationsController.h"

NotificationsController::NotificationsController(QObject *parent) :
    QObject(parent), worker(nullptr), tasksNotificationsEnabled(true)
{
    connect(&expirationTimer, &QTimer::timeout, this, &NotificationsController::notificationExpired);
}


void NotificationsController::handleTaskStarted(const QVariantMap &data) {
    if (tasksNotificationsEnabled && shouldBeNotified(data))
        notifyTaskDescription(data);
}

void NotificationsController::handleTaskCompleted(const QVariantMap &data) {
    if (tasksNotificationsEnabled && shouldBeNotified(data))
        notifyTaskDescription(data);
}

bool NotificationsController::shouldBeNotified(const QVariantMap &data) const {
    return data.value(TaskMetadata::KEY_TYPE) != TaskMetadata::VALUE_TYPE_UPDATE;
}

void NotificationsController::resetExpirationTimer()
{
    expirationTimer.stop();
    expirationTimer.start(3000);
}

void NotificationsController::notifyTaskDescription(const QVariantMap &data) {
    QString msg = data.value(TaskMetadata::KEY_DESCRIPTION).toString();
    int notificationType = MESSAGE;

    if (data.value(TaskMetadata::KEY_STATUS) == TaskMetadata::VALUE_STATUS_FAILED)
        notificationType = FAILURE;

    emit showNotificationRequest(notificationType, msg);

    resetExpirationTimer();
}


void NotificationsController::hideNotification() {
    expirationTimer.stop();
    emit notificationExpired();
}

void NotificationsController::setWorker(Worker *worker) {
    NotificationsController::worker = worker;

    if (NotificationsController::worker != nullptr)
        disconnect(NotificationsController::worker, nullptr, this, nullptr);

    NotificationsController::worker = worker;

    connect(worker, &Worker::taskStarted, this, &NotificationsController::handleTaskStarted);
    connect(worker, &Worker::taskCompleted, this, &NotificationsController::handleTaskCompleted);
}
