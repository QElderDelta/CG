#pragma once

#include <limits>

#include <QDebug>
#include <QDockWidget>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollBar>
#include <QSlider>
#include <QVBoxLayout>
#include <QtMath>
#include <QWheelEvent>

#include "inputbox.h"
#include "view.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent* event);
private:
    const double DEFAULT_SCALE = 50;

    double currentXScale_ = DEFAULT_SCALE;
    double currentYScale_ = DEFAULT_SCALE;
    int parameterValue_ = 0;
    int numberOfPoints_ = 1000;
    int rotationAngle_ = 0;

    QGraphicsScene* scene;
    QPoint sceneCenter{0, 0};
    QPoint currentCenter{0, 0};
    QVector<QGraphicsItem*> points;
    View* view;

    void addGridText(QGraphicsScene* scene, int cord, bool isXAxis, const QFont& font);
    void drawPlot();
    int  getNumberOfDigits(int number) const;
    void resizePlot();
    void movePlot();
private slots:
    void approxChanged(int numberOfPoints);
    void changeScale(bool zoomIn);
    void changeCenter(int dx, int dy);
    void mouseRotate(int angle);
    void parameterChanged(int paramter);
    void rotationAngleChanged(double angle);
    void xChanged(int dx);
    void yChanged(int dy);
    void xScaleChanged(double xScale);
    void yScaleChanged(double yScale);
signals:
    void changeRotationBox(double angle);
    void changeXBox(int dx);
    void changeYBox(int dy);
    void changeXScaleBox(double scale);
    void changeYScaleBox(double scale);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
