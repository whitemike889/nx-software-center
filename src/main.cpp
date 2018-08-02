#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QIcon>

#include <ui/UpgraderController.h>
#include <ui/NotificationsController.h>
#include <ui/DeployedApplicationsController.h>
#include <QtCore/QStandardPaths>

#include "entities/Cache.h"
#include "gateways/CacheSource.h"

#include "ui/SearchController.h"
#include "ui/TasksController.h"
#include "ui/DeployController.h"
#include "ui/RegistryController.h"
#include "ui/RunController.h"
#include "ui/RemoveController.h"
#include "ui/ApplicationViewController.h"

#define QML_MODULE_NAMESPACE "org.nxos.softwarecenter"
#define QML_MODULE_MAJOR_VERSION 1

ApplicationRepository *repository;
QNetworkAccessManager *networkAccessManager = nullptr;
Worker *worker = nullptr;
Deployer *deployer = nullptr;
Registry *registry = nullptr;
Cache *cache = nullptr;
Upgrader *upgrader = nullptr;
Remover *remover = nullptr;
DeployedApplicationsRegistry *deployedApplicationsRegistry = nullptr;

void registerQmlModules();

void initSoftwareCenterModules(QObject *parent);

Q_DECLARE_METATYPE(Application)

Q_DECLARE_METATYPE(QList<Application>)


void registerMetatypes();

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

#ifdef QT_DEBUG
    QQmlDebuggingEnabler enabler;
#endif

    QQmlApplicationEngine engine;

    app.setWindowIcon(QIcon::fromTheme("nx-software-center"));
    app.setApplicationDisplayName("Nomad Software Center");
    initSoftwareCenterModules(nullptr);
    registerQmlModules();
    registerMetatypes();

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

void registerMetatypes() {
    qRegisterMetaType<Application>("Application");
    qRegisterMetaType<QList<Application>>("ApplicationList");
}

void initSoftwareCenterModules(QObject *parent) {
    networkAccessManager = new QNetworkAccessManager(parent);

    worker = new Worker();

    repository = new ApplicationRepositoryRestClient("http://apps.nxos.org/api");

    registry = new Registry();
    QObject::connect(worker, &Worker::taskCompleted, registry, &Registry::handleTaskCompleted);

    CacheSource *cacheSource = new CacheSource(Cache::getApplicationsCachePath(), parent);

    deployedApplicationsRegistry = new DeployedApplicationsRegistry();
    auto cacheDirLocations = QStandardPaths::standardLocations(QStandardPaths::CacheLocation);
    deployedApplicationsRegistry->setCacheDir(cacheDirLocations.first());
    deployedApplicationsRegistry->setApplicationsDir(QDir::homePath() + "/Applications");

    deployer = new Deployer();
    deployer->setRepository(repository);
    deployer->setDeployedApplicationsRegistry(deployedApplicationsRegistry);

    remover = new Remover();
    remover->setRegistry(deployedApplicationsRegistry);

    cache = new Cache;
//    cache->setRepository(repository);
    QObject::connect(registry, &Registry::installedApplicationsChanged, cache,
                     &Cache::handleInstalledApplicationsChanged);

    upgrader = new Upgrader();
    upgrader->setDeployedApplicationsRegistry(deployedApplicationsRegistry);
    upgrader->setApplicationRepository(repository);
    upgrader->setDeployedApplicationsRegistry(deployedApplicationsRegistry);
    upgrader->setRemover(remover);
    upgrader->setDeployer(deployer);
    upgrader->lookUpForUpgrades();
}

static QObject *searchControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    SearchController *searchControler = new SearchController(repository);
    return searchControler;
}

static QObject *tasksControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    TasksController *taskController = new TasksController();
    taskController->setWorker(worker);
    return taskController;
}

static QObject *deployControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    DeployController *installControler = new DeployController(deployer);
    installControler->setWorker(worker);
    return installControler;
}

static QObject *removeControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    auto *removeController = new RemoveController();
    removeController->setWorker(worker);
    removeController->setRemover(remover);
    return removeController;
}

static QObject *registryControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    RegistryController *registryControler = new RegistryController(registry);
    return registryControler;
}

static QObject *upgraderControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    auto *upgraderController = new UpgraderController();
    upgraderController->setUpgrader(upgrader);
    upgraderController->setWorker(worker);

    return upgraderController;
}

static QObject *notificationsControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    auto *notificationsController = new NotificationsController();
    notificationsController->setWorker(worker);
    return notificationsController;
}

static QObject *runControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    auto runController = new RunController(nullptr);
    runController->setDeployedApplicationsRegistry(deployedApplicationsRegistry);
    return runController;
}

static QObject *applicationViewControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    auto applicationViewController = new ApplicationViewController();
    applicationViewController->setRegistry(registry);
    applicationViewController->setWorker(worker);
    applicationViewController->setRepository(repository);
    return applicationViewController;
}


static QObject *deployedApplicationsControllerSingletonProvider(QQmlEngine *, QJSEngine *) {
    auto deployedApplicationsController = new DeployedApplicationsController();
    deployedApplicationsController->setWorker(worker);
    deployedApplicationsController->setRegistry(deployedApplicationsRegistry);

    return deployedApplicationsController;
}

void registerQmlModules() {
    qmlRegisterSingletonType<SearchController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                               "SearchController",
                                               searchControllerSingletonProvider);

    qmlRegisterSingletonType<TasksController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                              "TasksController",
                                              tasksControllerSingletonProvider);

    qmlRegisterSingletonType<DeployController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                               "DeployController",
                                               deployControllerSingletonProvider);

    qmlRegisterSingletonType<DeployController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                               "RemoveController",
                                               removeControllerSingletonProvider);

    qmlRegisterSingletonType<RegistryController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                                 "RegistryController",
                                                 registryControllerSingletonProvider);

    qmlRegisterSingletonType<UpgraderController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                                 "UpgraderController",
                                                 upgraderControllerSingletonProvider);

    qmlRegisterSingletonType<NotificationsController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                                      "NotificationsController",
                                                      notificationsControllerSingletonProvider);

    qmlRegisterSingletonType<RunController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                            "RunController", runControllerSingletonProvider);

    qmlRegisterSingletonType<RunController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                            "ApplicationViewController", applicationViewControllerSingletonProvider);

    qmlRegisterSingletonType<DeployedApplicationsController>(QML_MODULE_NAMESPACE, QML_MODULE_MAJOR_VERSION, 0,
                                            "DeployedApplicationsController", deployedApplicationsControllerSingletonProvider);

}
