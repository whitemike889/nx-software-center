#include "snapdclientkauthwrapper.h"

#include <QDebug>
#include <KAuth>

#include <Snapd/Client>

SnapdClientKAuthWrapper::SnapdClientKAuthWrapper(QObject * parent) : QObject(parent)
{

}

KAuth::ExecuteJob *SnapdClientKAuthWrapper::disable(const QString &snap)
{
    KAuth::Action disableAction("org.nomad.softwarecenter.disable");
    disableAction.setTimeout(10 * 60 * 1000); // 10 min timeout
    disableAction.setHelperId("org.nomad.softwarecenter");
    disableAction.addArgument("snap", snap);

    KAuth::ExecuteJob *job = disableAction.execute();
    connect(job, &KAuth::ExecuteJob::result, [=] (KJob *kjob) {
        auto job = qobject_cast<KAuth::ExecuteJob *>(kjob);
        qDebug() << QString("Disable snap %1 finished, errors ? ").arg(snap) << job->errorString() << job->error();
    });

    return job;
}

KAuth::ExecuteJob *SnapdClientKAuthWrapper::enable(const QString &snap)
{
    KAuth::Action action("org.nomad.softwarecenter.enable");
    action.setTimeout(10 * 60 * 1000); // 10 min timeout
    action.setHelperId("org.nomad.softwarecenter");
    action.addArgument("snap", snap);

    KAuth::ExecuteJob *job = action.execute();
    connect(job, &KAuth::ExecuteJob::result, [=] (KJob *kjob) {
        auto job = qobject_cast<KAuth::ExecuteJob *>(kjob);
        qDebug() << QString("Enable snap %1 finished, errors ? ").arg(snap) << job->errorString() << job->error();
    });

    return job;
}

KAuth::ExecuteJob *SnapdClientKAuthWrapper::remove(const QString &snap)
{
    KAuth::Action action("org.nomad.softwarecenter.remove");
    action.setTimeout(10 * 60 * 1000); // 10 min timeout
    action.setHelperId("org.nomad.softwarecenter");
    action.addArgument("snap", snap);

    KAuth::ExecuteJob *job = action.execute();
    connect(job, &KAuth::ExecuteJob::result, [=] (KJob *kjob) {
        auto job = qobject_cast<KAuth::ExecuteJob *>(kjob);
        qDebug() << QString("Remove snap %1 finished, errors ? ").arg(snap) << job->errorString() << job->error();
    });


    return job;
}

KAuth::ExecuteJob *SnapdClientKAuthWrapper::install(const QString &snap, const QString &channel)
{
    KAuth::Action action("org.nomad.softwarecenter.install");
    action.setHelperId("org.nomad.softwarecenter");
    action.addArgument("snap", snap);
    action.addArgument("channel", channel);
    action.setTimeout(10 * 60 * 1000); // 1h timeout

//    qDebug() << snap << channel;
    KAuth::ExecuteJob *job = action.execute();
    connect(job, &KAuth::ExecuteJob::result, [=] (KJob *kjob) {
        auto job = qobject_cast<KAuth::ExecuteJob *>(kjob);
        qDebug() << QString("Install snap %1 finished, errors ? ").arg(snap) << job->errorString() << job->error();
    });

    return job;
}

KAuth::ExecuteJob *SnapdClientKAuthWrapper::refresh(const QString &snap, const QString &channel)
{
    KAuth::Action action("org.nomad.softwarecenter.refresh");
    action.setTimeout(60 * 60 * 1000); // 1h timeout
    action.setHelperId("org.nomad.softwarecenter");
    action.addArgument("snap", snap);
    action.addArgument("channel", channel);

    KAuth::ExecuteJob *job = action.execute();
    connect(job, &KAuth::ExecuteJob::result, [=] (KJob *kjob) {
        auto job = qobject_cast<KAuth::ExecuteJob *>(kjob);
        qDebug() << QString("Refresh snap %1 finished, errors ? ").arg(snap) << job->errorString() << job->error();
    });

    return job;
}

