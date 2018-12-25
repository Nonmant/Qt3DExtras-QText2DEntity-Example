#include <QApplication>
#include <QGuiApplication>

#include <Qt3DExtras>

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    auto *view = new Qt3DExtras::Qt3DWindow;
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    auto *container = QWidget::createWindowContainer(view);
    auto screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);
    auto *widget = new QWidget;
    auto *hLayout = new QHBoxLayout(widget);
    hLayout->addWidget(container, 1);
    auto *input = new Qt3DInput::QInputAspect;
    view->registerAspect(input);

    // Root entity
    auto *rootEntity = new Qt3DCore::QEntity();

    // Camera
    auto *cameraEntity = view->camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/7.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 10.0f, 20.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    //light
    auto *lightEntity = new Qt3DCore::QEntity(rootEntity);
    auto *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    auto *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    // For camera controls
    auto *camController = new Qt3DExtras::QOrbitCameraController (rootEntity);
    camController->setCamera(cameraEntity);
    // Set root object of the scene
    view->setRootEntity(rootEntity);


    auto *text2dTransform = new Qt3DCore::QTransform;
    text2dTransform->setScale(0.125f);
    text2dTransform->setTranslation(QVector3D(-5, 0, 5));

    auto *text2d = new Qt3DExtras::QText2DEntity(rootEntity);
    text2d->setFont(QFont("monospace"));
    text2d->setHeight(20);
    text2d->setWidth(100);
    text2d->setText("A");
    text2d->setColor(Qt::yellow);
    text2d->addComponent(text2dTransform);

    widget->show();
    widget->resize(1200, 800);

    return a.exec();
}
