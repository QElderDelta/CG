#ifndef MAINWINDOWz_H
#define MAINWINDOW_H

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
#include <QSlider>
#include <QVBoxLayout>
#include <QtMath>

#include "inputbox.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent* event);
private:
    const static int DEFAULT_SCALE = 50;

    int currentXScale_ = DEFAULT_SCALE;
    int currentYScale_ = DEFAULT_SCALE;
    int parameterValue_ = 0;
    int numberOfPoints_ = 1000;
    int rotationAngle_ = 0;

    QGraphicsView* view;
    QGraphicsScene* scene;
    QVector<QGraphicsItem*> points;

    void addGridText(QGraphicsScene* scene, int cord, bool isXAxis, QFont font);
    void drawPlot();
    int  getNumberOfDigits(int number) const;
    void resizePlot();
private slots:
    void approxChanged(int numberOfPoints);
    void parameterChanged(int paramter);
    void rotationAngleChanged(double angle);
    void xScaleChanged(int xScale);
    void yScaleChanged(int yScale);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
